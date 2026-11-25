/*
 * 任务一：用黄色框出蓝色矩形区域，用绿色框出红色矩形区域。
 * 测试图像：images/2.png
 *
 * 原理：BGR 转 HSV 后，白背景饱和度 S≈0，蓝/红矩形 S 很高。
 *       用 inRange 按色调阈值分割，再找轮廓画最小外接矩形。
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

// 对某一颜色做阈值分割并画最小外接矩形，返回检测到的个数
int detectColorRects(const Mat& img, const Scalar& lower, const Scalar& upper,
                     const Scalar& boxColor)
{
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    Mat mask;
    inRange(hsv, lower, upper, mask);

    // 闭运算填补矩形内部细小缝隙
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int count = 0;
    for (const auto& cnt : contours) {
        if (contourArea(cnt) < 100) continue;   // 过滤噪声
        RotatedRect rect = minAreaRect(cnt);
        Point2f pts[4];
        rect.points(pts);
        for (int i = 0; i < 4; ++i) {
            line(img, pts[i], pts[(i + 1) % 4], boxColor, 2);
        }
        ++count;
    }
    return count;
}

int main(int argc, char** argv)
{
    string src = (argc > 1) ? argv[1] : "images/2.png";
    string dst = (argc > 2) ? argv[2] : "result/task1_result.png";

    Mat img = imread(src);
    if (img.empty()) {
        cerr << "无法读取图像: " << src << endl;
        return 1;
    }

    // HSV 中蓝色 H≈100~130；红色 H≈0~10（OpenCV 的 H 范围为 0~179）
    int blue = detectColorRects(img, Scalar(90, 80, 80), Scalar(130, 255, 255),
                                Scalar(0, 255, 255));   // 黄色框
    int red  = detectColorRects(img, Scalar(0, 80, 80), Scalar(12, 255, 255),
                                Scalar(0, 255, 0));     // 绿色框

    imwrite(dst, img);
    cout << "蓝色矩形: " << blue << " 个(黄色框), 红色矩形: " << red << " 个(绿色框)" << endl;
    cout << "结果已保存: " << dst << endl;
    return 0;
}
