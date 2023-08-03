#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>

using namespace std;
using namespace cv;

/****************************************************
 * ��������ʾ�����ʹ��2D-2D������ƥ���������˶�
 * **************************************************/



void getFileNames(string path, vector<string>& files)
{
    //�ļ����
    //ע�⣺�ҷ�����Щ���´���˴���long���ͣ�ʵ�������лᱨ������쳣
    intptr_t hFile = 0;
    //�ļ���Ϣ
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //�����Ŀ¼,�ݹ����
            //�������,���ļ�����·������vector��
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

bool find_feature_matches(const Mat& img_1, const Mat& img_2,
    std::vector<KeyPoint>& keypoints_1,
    std::vector<KeyPoint>& keypoints_2,
    std::vector<DMatch>& matches,
    string output_path) {
    //-- ��ʼ��
    Mat descriptors_1, descriptors_2;
    double min_dist = 10000, max_dist = 0;
    // used in OpenCV3
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    //-- ��һ��:��� Oriented FAST �ǵ�λ��
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

 
    //-- �ڶ���:���ݽǵ�λ�ü��� BRIEF ������
    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    if (descriptors_1.total() * descriptors_2.total() == 0) {
        ofstream file;
        string fail_log_path = "E:\\opencv\\camera_pose\\fail_log.txt";
        file.open(fail_log_path, ios::out | ios::app);
        string img_name = output_path.substr(27, 7) + ".jpg";
        file << img_name << endl;
        file.close();
        cout << "bad image which need to be deleted " + img_name << endl;
        return false;
    }

    //-- ������:������ͼ���е�BRIEF�����ӽ���ƥ�䣬ʹ�� Hamming ����
    vector<DMatch> match;
    //BFMatcher matcher ( NORM_HAMMING );

    matcher->match(descriptors_1, descriptors_2, match);
    //-- ���Ĳ�:ƥ����ɸѡ
    //�ҳ�����ƥ��֮�����С�����������, ���������Ƶĺ�����Ƶ������֮��ľ���
    for (int i = 0; i < descriptors_1.rows; i++) {
        double dist = match[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    //��������֮��ľ��������������С����ʱ,����Ϊƥ������.����ʱ����С�����ǳ�С,����һ������ֵ30��Ϊ����.
    for (int i = 0; i < descriptors_1.rows; i++) {
        if (match[i].distance <= max(2 * min_dist, 30.0)) {
            matches.push_back(match[i]);
        }
    }
    return true;
}
// ��������ת�����һ������
Point2d pixel2cam(const Point2d& p, const Mat& K) {
    return Point2d
    (
        (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
        (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
    );
}

bool pose_estimation_2d2d(std::vector<KeyPoint> keypoints_1,
    std::vector<KeyPoint> keypoints_2,
    std::vector<DMatch> matches,
    Mat& R, Mat& t,
    string output_path) {
    // ����ڲ�,portland_hotel
    Mat K = (Mat_<double>(3, 3) << 570.342224, 0, 320.0, 0, 570.342224, 240.0, 0, 0, 1);

    //-- ��ƥ���ת��Ϊvector<Point2f>����ʽ
    vector<Point2f> points1;
    vector<Point2f> points2;

    for (int i = 0; i < (int)matches.size(); i++) {
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }

    //-- ���㱾�ʾ���
    Point2d principal_point(318.14, 255.52);  //�������, portland_hotel�궨ֵ
    double focal_length = 517.97;      //�������, portland_hotel�궨ֵ
    Mat essential_matrix;
    essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    // cout << "essential_matrix is " << endl << essential_matrix.total() << endl;
    if (essential_matrix.total() != 9) {
        ofstream file;
        string fail_log_path = "E:\\opencv\\camera_pose\\fail_log.txt";
        file.open(fail_log_path, ios::out | ios::app);
        string img_name = output_path.substr(27, 7) + ".jpg";
        file << img_name << endl;
        file.close();
        cout << "failed to calculate essential_matrix of " + img_name << endl;
        return false;
    }

    //-- �ӱ��ʾ����лָ���ת��ƽ����Ϣ.
    // �˺�������Opencv3���ṩ
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
    //cout << "R is " << endl << R << endl;
    // cout << "t is " << endl << t << endl;
    Mat bottom = (Mat_<double>(1, 4) << 0.0, 0.0, 0.0, 1.0);
    Mat Rt1, pose;
    hconcat(R, t, Rt1);
    vconcat(Rt1, bottom, pose);
    // cout << "Rt is " << endl << pose << endl;
    int nrow = pose.rows;
    int ncol = pose.cols;

    ofstream file;
    file.open(output_path, ios::out);
    // ����������ľ���Ϊ6λС��
    file << std::fixed << std::setprecision(6);
    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            double digit = pose.at<double>(i, j);
            file << digit;
            if (j != ncol - 1) {
                file << ' ';
            }
        }
        file << endl;
    }
    file.close();
    return true;
}

int main() {
    //-- ��ȡͼ��
    vector<string> fileNames;
    string root="E:\\opencv\\camera_pose\\image"; 	//�Լ�ѡ��Ŀ¼����
    string output = "E:\\opencv\\camera_pose\\pose\\";
    getFileNames(root, fileNames);
    int len = fileNames.size();
    // 4755
    int l_index = 8707, r_index = 8708;
    for (int i = 8708; i < len; i++) {
        if ((r_index - l_index) >= 20) {
            cout << "new scene should be set up!" << endl;
            break;
        }
        cout << fileNames[i] << endl;
        Mat img_1 = imread(fileNames[l_index], CV_LOAD_IMAGE_COLOR);
        Mat img_2 = imread(fileNames[r_index], CV_LOAD_IMAGE_COLOR);

        assert(img_1.data && img_2.data && "Can not load images!");
        string fail_log_path = "E:\\opencv\\camera_pose\\fail_log.txt";
        string pose_path = output + fileNames[i].substr(28, 7) + ".txt";

        vector<KeyPoint> keypoints_1, keypoints_2;
        vector<DMatch> matches;
        bool check = find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches, pose_path);
        if (!check) {
            break;
        }
        cout << "һ���ҵ���" << matches.size() << "��ƥ���" << endl;

        if (matches.size() < 5) {
            ofstream file;
            file.open(fail_log_path, ios::out | ios::app);
            string img_name = pose_path.substr(27, 7) + ".jpg";
            file << img_name << endl;
            file.close();
            cout << "matched keypoint pairs less than 5, failed to calculate essential_matrix of " + img_name << endl;
            r_index++;
            continue;
        }

        //-- ��������ͼ����˶�
        Mat R, t;
        bool pass = pose_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t, pose_path);
        cout << "+++++++++++++++++++++++++++++++++++++" << l_index << "++++++++++++++++++" << r_index << endl;
        if (pass) {
            l_index = r_index;
            r_index++;
        }
        else {
            r_index++;
        }
        cout << "+++++++++++++++++++++++++++++++++++++" << l_index << "++++++++++++++++++" << r_index << endl;
        //-- ��֤E=t^R*scale
        //Mat t_x =
        //    (Mat_<double>(3, 3) << 0, -t.at<double>(2, 0), t.at<double>(1, 0),
        //        t.at<double>(2, 0), 0, -t.at<double>(0, 0),
        //        -t.at<double>(1, 0), t.at<double>(0, 0), 0);

        //// cout << "t^R=" << endl << t_x * R << endl;

        ////-- ��֤�Լ�Լ��
        //Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
        //for (DMatch m : matches) {
        //    Point2d pt1 = pixel2cam(keypoints_1[m.queryIdx].pt, K);
        //    Mat y1 = (Mat_<double>(3, 1) << pt1.x, pt1.y, 1);
        //    Point2d pt2 = pixel2cam(keypoints_2[m.trainIdx].pt, K);
        //    Mat y2 = (Mat_<double>(3, 1) << pt2.x, pt2.y, 1);
        //    Mat d = y2.t() * t_x * R * y1;
        //    // cout << "epipolar constraint = " << d << endl;
        //}
    }
    return 0;
}