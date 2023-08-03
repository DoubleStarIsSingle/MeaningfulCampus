import argparse
import math
import torch
import torch.backends.cudnn as cudnn
import torch.utils.data


from assets.my_sequence_folders import SequenceFolder
from models import superpoint, triangulation, densedepth
from assets.utils import *



parser = argparse.ArgumentParser(description='Structure from Motion Learner training on KITTI and CityScapes Dataset',
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

parser.add_argument('--data', default='/root/autodl-tmp/my_portland_hotel_example/', type=str, metavar='DIR',
                    help='path to dataset')
parser.add_argument('--dataset-format', default='sequential', metavar='STR',
                    help='dataset format, stacked: sequential: sequential folders')
parser.add_argument('-j', '--workers', default=4, type=int, metavar='N',help='number of data loading workers')
parser.add_argument('-b', '--batch-size', default=16, type=int,metavar='N', help='mini-batch size')
parser.add_argument('--print-freq', default=200, type=int,metavar='N', help='print frequency')
parser.add_argument('--seed', default=1, type=int, help='seed for random functions, and network initialization')
parser.add_argument('--ttype2', default='/root/autodl-tmp/my_portland_hotel_example/scene_list.txt', type=str, help='Text file indicates input data')
parser.add_argument('--mindepth', type=float ,default=0.5, help='minimum depth')
parser.add_argument('--maxdepth', type=float ,default=5.0, help='maximum depth') #10.
parser.add_argument('--width', type=int ,default=320, help='image width')
parser.add_argument('--height', type=int ,default=240, help='image height')
parser.add_argument('--seq_length', default=3, type=int, help='length of sequence')
parser.add_argument('--seq_gap', default=1, type=int, help='gap between frames for ScanNet dataset')
parser.add_argument('--resume', type=bool ,default = True, help='Use pretrained network')
parser.add_argument('--pretrained', dest='pretrained', default='./assets/pretrained_checkpoint.pth.tar', metavar='PATH',
                    help='path to pre-trained model')
parser.add_argument('--do_confidence', type=bool ,default = True, help='confidence in triangulation')
parser.add_argument('--dist_orthogonal', type=int ,default = 1, help='offset distance in pixels')
parser.add_argument('--kernel_size', type=int ,default = 1, help='kernel size')
parser.add_argument('--out_length', type=int ,default = 100, help='output length of epipolar patch')
parser.add_argument('--depth_range', type=bool ,default = True, help='clamp using range of depth')
parser.add_argument('--num_kps', default=512, type=int, help='number of interest keypoints')
parser.add_argument('--model_type', type=str,default = 'resnet50', help='network backbone')
parser.add_argument('--align_corners', type=bool,default = False, help='align corners')
parser.add_argument('--descriptor_dim', type=int,default = 128, help='dimension of descriptor')
parser.add_argument('--detection_threshold', type=float,default = 0.0005, help='threshold for interest point detection')
parser.add_argument('--frac_superpoint', type=float,default = .5, help='fraction of interest points')
parser.add_argument('--nms_radius', type=int,default = 9, help='radius for nms')
parser.add_argument('--print_res', type=bool, default = True, help='print the detail test result or not, true for default.')
parser.add_argument('--save_res', type=bool, default = True, help='save the detail test result or not, true for default.')
parser.add_argument('--save_path', default='/root/autodl-tmp/deltas/metric_log.txt', type=str, metavar='DIR', help='path to save metic result')

n_iter = 0


def my_compute_errors(gt, pred, valid, print_res=True, save_res=False, save_path='/root/autodl-tmp/deltas/metric_log.txt'):
    
    """
    Evaluates depth metrics
    gt or tgt_depth_tiled.shape :  torch.Size([12, 240, 320])
    pred or output.shape :  torch.Size([12, 240, 320])
    valid or mask.shape :  torch.Size([12, 240, 320])
    """

    abs_diff, abs_real, rmse, rmse_log = 0,0,0,0
    batch_size = gt.size(0)

    abs_real_list = []
    abs_diff_list = []
    rmse_list = []
    rmse_log_list = []

    count = 0
    black_count = []

    for current_gt, current_pred, current_valid in zip(gt, pred, valid):
        valid_gt = current_gt[current_valid]
        valid_pred = current_pred[current_valid]

        if len(valid_gt) == 0:
            count += 1
            black_count.append(count)
            print('************ length of valid_gt of file index {} is zero...'.format(count))
            continue
        else:
            step_abs_real = torch.mean(torch.abs(valid_gt - valid_pred))
            step_abs_diff = torch.mean(torch.abs(valid_gt - valid_pred) / valid_gt)
            step_rmse = torch.mean((valid_gt - valid_pred)**2)
            step_rmse_log = torch.mean((torch.log(valid_gt) - torch.log(valid_pred))**2)

            abs_real += step_abs_real
            abs_diff += step_abs_diff
            rmse += step_rmse
            rmse_log += step_rmse_log
            
            if print_res:
                count += 1 
                print('------------------------metrics of file index {}'.format(count))
                print('abs_real : ', step_abs_real.item())
                print('abs_diff : ', step_abs_diff.item())
                print('    rmse : ', math.sqrt(step_rmse.item()))
                print('rmse_log : ', math.sqrt(step_rmse_log.item()))
            if save_res:
                abs_real_list.append(step_abs_real.item())
                abs_diff_list.append(step_abs_diff.item())
                rmse_list.append(math.sqrt(step_rmse.item()))
                rmse_log_list.append(math.sqrt(step_abs_diff.item()))

    if save_res:
        with open(save_path,"w") as f:
            target_index = 0
            for i in range(0, count):
                if i+1 in black_count:
                    continue
                f.write('------------------------metrics of file index {}\n'.format(i+1))
                f.write('abs_real : {}\n'.format(abs_real_list[target_index]))
                f.write('abs_diff : {}\n'.format(abs_diff_list[target_index]))
                f.write('    rmse : {}\n'.format(rmse_list[target_index]))
                f.write('rmse_log : {}\n'.format(rmse_log_list[target_index]))  
                target_index += 1
        f.close()

    return [abs_real/batch_size, abs_diff/batch_size, math.sqrt(rmse/batch_size), math.sqrt(rmse_log/batch_size)]


def main():
    global n_iter
    args = parser.parse_args()
    torch.manual_seed(args.seed)

    # Data loading code
    print("=> fetching scenes in '{}'".format(args.data))
    normalize = Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])

    test_transform = Compose([
        Scale(),
        ArrayToTensor(), 
        normalize])

    test_set = SequenceFolder(
        args.data,
        transform=test_transform,
        seed=args.seed,
        ttype=args.ttype2,
        sequence_length=args.seq_length,
        sequence_gap = args.seq_gap,
        height = args.height, 
        width = args.width,
    )

    print('{} samples found in {} valid scenes'.format(len(test_set), len(test_set.scenes)))


    test_loader = torch.utils.data.DataLoader(
        test_set, batch_size=args.batch_size, shuffle=False,
        num_workers=args.workers, pin_memory=True)

    # create model
    print("=> creating model")

    #step 1 using superpoint
    config_sp = {
        'top_k_keypoints': args.num_kps,
        'height': args.height, 
        'width': args.width,
        'align_corners': args.align_corners, 
        'detection_threshold':args.detection_threshold,
        'frac_superpoint':args.frac_superpoint,
        'nms_radius':args.nms_radius,
    }

    cudnn.benchmark = True

    supernet = superpoint.Superpoint(config_sp)
    supernet = supernet.cuda() if torch.cuda.is_available() else supernet

    #step 2 using differentiable triangulation
    config_tri = {
        'dist_ortogonal':args.dist_orthogonal,
        'kernel_size':args.kernel_size,
        'out_length':args.out_length,
        'depth_range': args.depth_range,
        'has_confidence':args.do_confidence,
        'align_corners': args.align_corners,
    }


    trinet = triangulation.TriangulationNet(config_tri)
    trinet = trinet.cuda() if torch.cuda.is_available() else trinet

    #step 3 using sparse-to-dense

    config_depth = {
        'min_depth':args.mindepth,
        'max_depth':args.maxdepth,  
        'input_shape' : (args.height,args.width,1),
    }

    depthnet = densedepth.SparsetoDenseNet(config_depth)
    depthnet = depthnet.cuda() if torch.cuda.is_available() else depthnet

    #load pre-trained weights

    if args.resume:
        if torch.cuda.is_available(): 
          weights = torch.load(args.pretrained)
        else:
          weights = torch.load(args.pretrained, map_location=torch.device('cpu'))
        supernet.load_state_dict(weights['state_dict'], strict = True)
        trinet.load_state_dict(weights['state_dict_tri'], strict = True)
        depthnet.load_state_dict(weights['state_dict_depth'], strict = True)
        if torch.cuda.is_available():
          depthnet = torch.nn.DataParallel(depthnet).cuda()
          supernet = torch.nn.DataParallel(supernet).cuda()
          trinet = torch.nn.DataParallel(trinet).cuda()
        

    errors_depth, error_names = validate_with_gt(args, test_loader, supernet, trinet, depthnet, test_set)

def validate_with_gt(args, val_loader, supernet, trinet, depthnet, val_set=None):

    error_names = ['abs_rel', 'abs_diff', 'rmse', 'rmse_log']
    errors_depth = AverageMeter(i=len(error_names))


    # switch to evaluate mode

    supernet.eval()
    trinet.eval()
    depthnet.eval()


    with torch.no_grad():

        for i, (tgt_img, ref_imgs, poses, intrinsics, tgt_depth, ref_depths) in enumerate(val_loader):

            print('====================================================')

            tgt_img_var = tgt_img 
            ref_imgs_var = ref_imgs
            img_var = make_symmetric(tgt_img_var, ref_imgs_var)

            ##Step 1: Detect and Describe Points
            data_sp = {'img': img_var,'process_tsp':'ts'} #t is detector, s is descriptor
            pred_sp = supernet(data_sp)
            print('\n')
            print('\n... ... stage-1 ... superpoint keypoints detection and description ... ...')
            print('batch_size : ', tgt_img_var.shape[0])
            print("pred_sp['keypoints'].shape : ", pred_sp['keypoints'].shape)
            print("pred_sp['descriptors'].shape : ", pred_sp['descriptors'].shape)
            print("len(pred_sp['scores_sparse']) : ", len(pred_sp['scores_sparse']))
            # print("pred_sp['scores_sparse'] : ", pred_sp['scores_sparse'])

    
            batch_sz = tgt_img_var.shape[0]  
            img_var = img_var[:batch_sz]
            
            ##Pose and intrinsics
            poses_var = [pose for pose in poses]
            intrinsics_var = intrinsics 
            seq_val = args.seq_length-1
            pose = torch.cat(poses_var,1) 
            pose = pose_square(pose)
       

            ##Depth
            tgt_depth_var = tgt_depth 
            ref_depths_var = [ref_depth for ref_depth in ref_depths]
            depth = tgt_depth_var
            depth_ref = torch.stack(ref_depths_var,1)
                    
            #Keypoints and descriptor logic 
            keypoints = pred_sp['keypoints'][:batch_sz]
            features =  pred_sp['features'][:batch_sz]
            skip_half = pred_sp['skip_half'][:batch_sz]
            skip_quarter = pred_sp['skip_quarter'][:batch_sz]
            skip_eight = pred_sp['skip_eight'][:batch_sz]
            skip_sixteenth = pred_sp['skip_sixteenth'][:batch_sz]
            scores = pred_sp['scores'][:batch_sz]
            desc = pred_sp['descriptors']
            desc_anc =desc[:batch_sz,:,:,:] 
            desc_view =desc[batch_sz:,:,:,:] 
            desc_view = reorder_desc(desc_view,batch_sz)  

            ## Step 2: Match & Triangulate Points
            data_sd= {'iter':n_iter, 'intrinsics':intrinsics_var,'pose':pose,'depth':depth, 'ref_depths': depth_ref,'scores':scores,
                'keypoints':keypoints,'descriptors':desc_anc,'descriptors_views':desc_view,'img_shape':tgt_img_var.shape, 'sequence_length':seq_val}
            pred_sd = trinet(data_sd)

            view_matches = pred_sd['multiview_matches'] 
            anchor_keypoints = pred_sd['keypoints']
            keypoints3d_gt = pred_sd['keypoints3d_gt']
            range_mask_view = pred_sd['range_kp']
            range_mask = torch.sum(range_mask_view,1) 


            d_shp = tgt_depth_var.shape
            keypoints_3d = pred_sd['keypoints_3d']
            kp3d_val = keypoints_3d[:,:,2].view(-1,1).t()
            kp3d_filter = (range_mask>0).view(-1,1).t()
            kp3d_filter = (kp3d_filter) & (kp3d_val>args.mindepth) & (kp3d_val<args.maxdepth)

            print('\n... ... stage-2 ... keypoints matching and triangulation and depth estimation ... ...')
            print("pred_sd['keypoints_3d'].shape : ", pred_sd['keypoints_3d'].shape)
            print("pred_sd['keypoints3d_gt'].shape : ", pred_sd['keypoints3d_gt'].shape)

            ## Step 3: Densify using Sparse-to-Dense 
            data_dd= {'anchor_keypoints':keypoints, 'keypoints_3d':keypoints_3d,'sequence_length':args.seq_length,'skip_sixteenth':skip_sixteenth,
                'range_mask':range_mask,'features':features,'skip_half':skip_half, 'skip_quarter':skip_quarter,'skip_eight':skip_eight}
            pred_dd = depthnet(data_dd)
            output = pred_dd['dense_depth']

            print('\n... ... stage-3 ... Sparse-to-Dense ... ...')
            print("pred_dd['dense_depth'].shape : ", pred_dd['dense_depth'].shape)


            # Calculate metrics
            tgt_depth_tiled = depth
            if output.is_cuda:
                tgt_depth_tiled = tgt_depth_tiled.to(output.device)

            mask = (tgt_depth_tiled <= args.maxdepth) & (tgt_depth_tiled >= args.mindepth) & (tgt_depth_tiled == tgt_depth_tiled)
            mask.detach_()
            output = output.squeeze(1)

            print('\n... ... calculating errors ... ...')
            print("tgt_depth_tiled.shape : ", tgt_depth_tiled.shape)
            print("output.shape : ", output.shape)
            print("mask.shape : ", mask.shape)

            errors_depth.update(my_compute_errors(tgt_depth_tiled, output, mask, args.print_res, args.save_res, args.save_path))

            print('\nAll Data: abs_real {:.4f} , abs_diff {:.4f} , rmse {:.4f} , rmse_log {:.4f}'.format(errors_depth.avg[0], errors_depth.avg[1], errors_depth.avg[2], errors_depth.avg[3]))
        
    return errors_depth.avg, error_names


if __name__ == '__main__':
    main()


