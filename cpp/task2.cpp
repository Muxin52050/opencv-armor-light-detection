/*
 * 任务二：从图像中识别装甲板灯条并框选。
 * 测试图像：images/3.jpg
 *
 * 原理：灯条是画面里最亮的橙红色发光竖条，HSV 上满足
 *       H 偏红橙(0~30)、S 高、V 高。按面积和细长比过滤轮廓，
 *       排除地面反光等干扰。
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    string src = (argc > 1) ? argv[1] : "images/3.jpg";
    string dst = (argc > 2) ? argv[2] : "result/task2_result.jpg";

    Mat img = imread(src);
    if (img.empty()) {
        cerr << "无法读取图像: " << src << endl;
        return 1;
    }

    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    Mat mask;
    inRange(hsv, Scalar(0, 70, 170), Scalar(30, 255, 255), mask);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int count = 0;
    for (const auto& cnt : contours) {
        double area = contourArea(cnt);
        Rect r = boundingRect(cnt);
        // 灯条特征：有面积、竖直细长
        if (area > 15 && r.height > 25 && r.height > r.width * 1.5) {
            rectangle(img, r, Scalar(0, 255, 255), 2);   // 黄色框
            cout << "灯条" << (++count) << ": 左上角(" << r.x << "," << r.y
                 << ") 宽" << r.width << " 高" << r.height << endl;
        }
    }

    imwrite(dst, img);
    cout << "共识别到灯条: " << count << " 个" << endl;
    cout << "结果已保存: " << dst << endl;
    return 0;
}
