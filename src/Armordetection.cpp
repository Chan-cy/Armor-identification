//
// Created by insppp on 22-8-22.
//

#include "../include/Armordetection.h"

void Armordetection::pre_treatment()
{

    // Color channel processing
    std::vector<Mat> channels;
    split(src, channels);  // Separate color channels

    // keep RED
    GrayImg = channels.at(2) - channels.at(0);

    // Threshold processing
    threshold(GrayImg, BinBrightImg, 55, 255, THRESH_BINARY);

    // Morphological operation (dilate)
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    dilate(BinBrightImg, BinBrightImg, element);

    // Closed operation
    morphologyEx(BinBrightImg, final, MORPH_CLOSE, element);

    //--------------------------Find rectangular frame---------------------------------//

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(final, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Filter rectangle
    for (int i = 0; i < hierarchy.size(); ++i)
    {
        rect = minAreaRect(contours.at(i));  // Get minimum area rectangle

        // Convert the rectangle of the second quadrant to the fourth quadrant
        if (rect.size.width > rect.size.height)
        {
            swap(rect.size.width, rect.size.height);
            rect.angle -= 90;
        }

        if (rect.size.width * rect.size.height > 300)  // Area filter
        {
            if (rect.size.width < rect.size.height / 2.5)
            {
                if ((rect.angle <= 45 && rect.angle>=0) || (rect.angle>-45 && rect.angle<=0))
                {
                    rect_match.push_back(rect);  //Put the matched rectangle into the array (rect_match)
                }
            }
        }
    }
    //Empty the array to prevent memory leakage
    contours.clear();
    hierarchy.clear();
}



int Armordetection::match()
{
    if (rect_match.size() < 2) // Less than two light bars cannot form an armor plate
    {
        return match_num = 0;
    }
    else
    {
        for (int j = 0; j < rect_match.size(); ++j)
        {
            for (int k = j + 1; k < rect_match.size(); ++k)  // Matching light strips in pairs
            {
                leftrect = rect_match[j];
                rightrect = rect_match[k];
                height = (leftrect.size.height + rightrect.size.height) / 2;  // Average height
                level = 0;


                // Match the y values of the centers of the left and right rectangles
                if (leftrect.center.y - rightrect.center.y >  height * 1.2)
                {
                    continue;
                }
                else if (leftrect.center.y == rightrect.center.y)
                {
                    level += CENTER_Y * 20;
                }
                else if (abs(leftrect.center.y - rightrect.center.y) < 0.4 * height)
                {
                    level += CENTER_Y * 16;
                }
                else if (abs(leftrect.center.y - rightrect.center.y) < 0.5 * height)
                {
                    level += CENTER_Y * 12;
                }
                else if (abs(leftrect.center.y - rightrect.center.y) < 0.7 * height)
                {
                    level += CENTER_Y * 8;
                }
                else if (abs(leftrect.center.y - rightrect.center.y) < 0.9 * height)
                {
                    level += CENTER_Y * 4;
                }

                // Match the x value of the center of the left and right rectangles
                if (abs(leftrect.center.x - rightrect.center.x) > (2.5 * height))
                {
                    continue;
                }
                else if (1.1 > (abs(leftrect.center.x - rightrect.center.x) / (2 * height)) > 0.9)
                {
                    level += CENTER_X * 20;
                }
                else if (0.9 > (abs(leftrect.center.x - rightrect.center.x) / (2 * height)) > 0.8)
                {
                    level += CENTER_X * 16;
                }
                else if (0.8 > (abs(leftrect.center.x - rightrect.center.x) / (2 * height)) > 0.7)
                {
                    level += CENTER_X * 12;
                }
                else if (0.7 > (abs(leftrect.center.x - rightrect.center.x) / (2 * height)) > 0.6)
                {
                    level += CENTER_X * 8;
                }
                else if (0.6 > (abs(leftrect.center.x - rightrect.center.x) / (2 * height)) > 0.5)
                {
                    level += CENTER_X * 4;
                }


                // Left and right rectangle angle matching
                if (abs(leftrect.angle - rightrect.angle) > 20)
                {
                    continue;
                }
                else if (leftrect.angle == rightrect.angle)
                {
                    level += ANGLE * 20;
                }
                else if (abs(leftrect.angle - rightrect.angle) < 4)
                {
                    level += ANGLE * 16;
                }
                else if (abs(leftrect.angle - rightrect.angle) < 8)
                {
                    level += ANGLE * 12;
                }
                else if (abs(leftrect.angle - rightrect.angle) < 12)
                {
                    level += ANGLE * 8;
                }
                else if (abs(leftrect.angle - rightrect.angle) < 16)
                {
                    level += ANGLE * 4;
                }

                // Left and right rectangle height matching
                if ((min(leftrect.size.height, rightrect.size.height) /
                     max(leftrect.size.height, rightrect.size.height)) == 1)
                {
                    level += HEIGHT * 20;
                }
                else if ((min(leftrect.size.height, rightrect.size.height) /
                          max(leftrect.size.height, rightrect.size.height)) > 0.95)
                {
                    level += HEIGHT * 16;
                }
                else if ((min(leftrect.size.height, rightrect.size.height) /
                          max(leftrect.size.height, rightrect.size.height)) > 0.9)
                {
                    level += HEIGHT * 12;
                }
                else if ((min(leftrect.size.height, rightrect.size.height) /
                          max(leftrect.size.height, rightrect.size.height)) > 0.85)
                {
                    level += HEIGHT * 8;
                }
                else if ((min(leftrect.size.height, rightrect.size.height) /
                          max(leftrect.size.height, rightrect.size.height)) > 0.8)
                {
                    level += HEIGHT * 4;
                }

                // Area matching of left and right rectangles
                area[0] = leftrect.size.width * leftrect.size.height;
                area[1] = rightrect.size.width * rightrect.size.height;
                if ((min(area[0], area[1])/max(area[0], area[1]) > 0.95))
                {
                    level += AREA * 20;
                }
                else if (min(area[0], area[1])/max(area[0], area[1]) >  0.85)
                {
                    level += AREA * 16;
                }
                else if (min(area[0], area[1]) / max(area[0], area[1]) > 0.75)
                {
                    level += AREA * 12;
                }
                else if (min(area[0], area[1]) / max(area[0], area[1]) > 0.65)
                {
                    level += AREA * 8;
                }
                else if (min(area[0], area[1]) / max(area[0], area[1]) > 0.55)
                {
                    level += AREA * 4;
                }
                //Ternary array, X is the left rectangle, y is the right rectangle
                if (level != 0)level_end.emplace_back(j, k, level);
            }
        }

        // compare level
        maxlevel = 0;
        if (level_end.empty()){
            return match_num = 0;
        }
        for (int C = 0; C < level_end.size(); ++C)
        {
            if (level_end[C].z > maxlevel)
            {
                maxlevel = level_end[C].z;
                index = C;
            }
        }


        if (maxlevel < TOTAL_WEIGHT * 20 * NUM_DETECT_THRESHOLD || IS_NUM_DETECT)
        {
            rect_match[static_cast<unsigned long>(level_end[index].y)].points(points2);
            rect_match[static_cast<unsigned long>(level_end[index].x)].points(points1);

            p1 = Point(static_cast<int>(points2[2].x), static_cast<int>(points2[2].y - height * 0.5));
            p2 = Point(static_cast<int>(points1[0].x), static_cast<int>(points1[0].y + height * 0.6 ));
            Rect roi_rect = Rect(p1, p2);
            Mat num_roi = src(roi_rect).clone();

            area[0] = rect_match[static_cast<unsigned long>(level_end[index].y)].size.height *
                      rect_match[static_cast<unsigned long>(level_end[index].y)].size.width;
            area[1] = rect_match[static_cast<unsigned long>(level_end[index].x)].size.height *
                      rect_match[static_cast<unsigned long>(level_end[index].x)].size.width;
//            cout << "area: " << min(area[0], area[1])/max(area[0], area[1]) << endl;


            if (min(area[0], area[1])/max(area[0], area[1]) < 0.40 && IS_NUM_DETECT)
            {
                resize(num_roi, num_roi,Size(0, 0), 1.8, 1);
            }
            pred_num = DNN_detect::dnn_detect(num_roi);
            if (IS_NUM_DETECT)
            {
                cout << pred_num << endl;
            }

            if (pred_num == 0)return match_num = 0;
            imshow("num_roi", num_roi);
        }
//        cout << "level: " << maxlevel << " persent:"<< maxlevel / (20 *  TOTAL_WEIGHT)<< endl;

        // Output center point coordinates
        currentCenter.x = (rect_match[static_cast<unsigned long>(level_end[index].x)].center.x +
                           rect_match[static_cast<unsigned long>(level_end[index].y)].center.x) / 2;
        currentCenter.y = (rect_match[static_cast<unsigned long>(level_end[index].x)].center.y +
                           rect_match[static_cast<unsigned long>(level_end[index].y)].center.y) / 2;
        return match_num = 1;
    }
}

double Armordetection::max(double first, double second)
{
    return first > second ? first : second;
}

double Armordetection::min(double first, double second)
{
    return first < second ? first : second;
}


void Armordetection::draw_rect()
{
    rect_match[static_cast<unsigned long>(level_end[index].y)].points(points2);
    rect_match[static_cast<unsigned long>(level_end[index].x)].points(points1);

    if (points2[0].x <= points1[0].x) {

        // Length of rectangle
        line(src, points1[2], points2[1], Scalar(0, 0, 255), 2, 8, 0);
        line(src, points1[3], points2[0], Scalar(0, 0, 255), 2, 8, 0);

        // Width of rectangle
        line(src, points1[2], points1[3], Scalar(0, 0, 255), 2, 8, 0);
        line(src, points2[1], points2[0], Scalar(0, 0, 255), 2, 8, 0);
    }
    else if (points2[0].x > points1[0].x) {

        //Length of rectangle
        line(src, points1[0], points2[3], Scalar(0, 0, 255), 2, 8, 0);
        line(src, points1[1], points2[2], Scalar(0, 0, 255), 2, 8, 0);

        // Width of rectangle
        line(src, points1[0], points1[1], Scalar(0, 0, 255), 2, 8, 0);
        line(src, points2[2], points2[3], Scalar(0, 0, 255), 2, 8, 0);
    }
}

void Armordetection::armordetection() {
    pre_treatment();
    match();
    if (match_num == 1) //If the function match() matches successfully, it returns 1
    {
        draw_rect();
    }
    rect_match.clear();
    level_end.clear();
}
