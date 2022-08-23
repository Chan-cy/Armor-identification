#include "include/Armordetection.h"

#define MIN_LOW 48  //low brightness origin index
#define MAX_LOW 184  //low brightness ending index

#define MIN_HIGH 0   //high brightness origin index
#define MAX_HIGH 48  // high brightness ending index

#define FILE_ROOT_HIGH "../img/"
#define FILE_ROOT_LOW "../img_low/"

int main()
{
    Armordetection detector;
    String Root = FILE_ROOT_HIGH;
    for (int i = MIN_HIGH; i < MAX_HIGH ; ++i) {
        String file = Root + to_string(i) + ".jpg";
        detector.src = imread(file);
        detector.armordetection();
        imshow("detect", detector.src);
        waitKey(0);
    }
}
