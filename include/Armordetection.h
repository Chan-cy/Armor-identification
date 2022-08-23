#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui_c.h>
#include <iostream>
#include<vector>
#include "DNN_detect.h"

using namespace std;
using namespace cv;

#ifndef ARMOR_IDENTIFICATION_ARMORDETECTION_H
#define ARMOR_IDENTIFICATION_ARMORDETECTION_H

#define CENTER_Y 0.20  // The difference of center point's Y
#define CENTER_X 0.20  // The difference of center point's X
#define HEIGHT 0.20    // The difference of tow rectangle's height
#define ANGLE 0.20     // The difference of tow rectangle's angle
#define AREA 0.20      // The difference of tow rectangle's area
#define ADD(v, w, x, y, z)  ((v) + (w) + (x) + (y) + (z))
#define TOTAL_WEIGHT ADD(CENTER_Y, CENTER_X, HEIGHT, ANGLE, AREA)
#define NUM_DETECT_THRESHOLD 0.6
#define IS_NUM_DETECT true

class Armordetection
{
public:
    int match_num{};
    vector<RotatedRect>rect_match;
    vector<Point3f>level_end;

    Point2f points1[4];
    Point2f points2[4];
    Point2f currentCenter;

    Mat src;

    int pred_num{};
    double height{};

    void armordetection();
private:
    int index = 0;
    float maxlevel = 0;
    double area[2]{};

    Point p1, p2;

    Rect rect_test;
    RotatedRect rect, leftrect, rightrect;

    Mat GrayImg;
    Mat BinBrightImg, BinGrayImg;
    Mat final;
    float level{};

    void pre_treatment();
    void draw_rect();
    int match();

    static double max(double first, double second);
    static double min(double first, double second);
};
#endif //ARMOR_IDENTIFICATION_ARMORDETECTION_H
