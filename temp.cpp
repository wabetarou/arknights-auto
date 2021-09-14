#include <ApplicationServices/ApplicationServices.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <chrono>

using namespace std::chrono;
typedef struct CGPoint CGPoint;
// g++ -std=c++11 -framework CoreGraphics -framework CoreFoundation -o everyday.out everyday.cpp `pkg-config --cflags opencv4` `pkg-config --libs opencv4`

///print out location in a nice way to std cout
void click(CGPoint);
Boolean click_image(cv::Mat *,cv::Mat *,cv::Mat *);

int main(void) {
    clock_t p_start = clock();
    auto start = std::chrono::system_clock::now();

    double maxVal;
    cv::Point maxLoc;
    cv::Mat result;
    cv::Mat screen;
    CGPoint location;

    cv::Mat img_adopt;
    cv::Mat img_adopt_result;
    cv::Mat img_recruit;
    cv::Mat img_get;
    cv::Mat img_skip;
    cv::Mat img_bell_main;
    cv::Mat img_bell;
    cv::Mat img_trade;
    cv::Mat img_manufacture;
    cv::Mat img_trust;
    cv::Mat img_return;

    img_recruit = cv::imread("./image/recruit.png",0);
    img_get = cv::imread("./image/get.png",0); 
    img_adopt = cv::imread("./image/adopt.png",0);
    img_adopt_result = cv::imread("./image/adopt_result.png",0);
    img_skip = cv::imread("./image/skip.png",0);
    img_bell_main = cv::imread("./image/main_bell.png",0);
    img_bell = cv::imread("./image/bell.png",0);
    img_trust = cv::imread("./image/trust.png",0);
    img_manufacture = cv::imread("./image/manufacture.png",0);
    img_trust = cv::imread("./image/trust.png",0);
    img_return = cv::imread("./image/return.png",0);

    double threshold = 0.999;
    int wait_time = 90;

    printf("start\n");
    int state = 1;
    // [1:home, 2:recruit, 3:base, 4:mission]
    int flag_home = 1;
    int flag_end = 0;
    int flag_click = 0;
    int flag_bell = 0;
    int flag_recruit = 0;
    int flag_mission = 0;
    while (true) {
        system("screencapture -x ./image/screenshot.png");
        screen = cv::imread("./image/screenshot.png",0);
        // 物資獲得
        switch(state) {
            case 1:
                if (!flag_recruit) {
                    click_image(&screen, &img_recruit, &result) {
                        std::cout << "clicked get\n";
                        flag_click++; 
                    }
                }
        }
        if (flag_home) {

        }
        if (!flag_click) {
            if (click_image(&screen, &img_get, &result)) {
                std::cout << "clicked get\n";
                flag_click++;
            }
        }
        // 公開求人
        if (!flag_click) {
            if (click_image(&screen, &img_recruit, &result)) {
                std::cout << "clicked recruit\n";
                flag_click++;
            }
        }
        // 公開求人採用
        if (!flag_click) {
            if (click_image(&screen, &img_adopt, &result)) {
                std::cout << "clicked adopt\n";
                flag_click++;
            }
        }
        // 公開求人採用スキップ
        if (!flag_click) {
            if (click_image(&screen, &img_skip, &result)) {
                std::cout << "clicked skip\n";
                flag_click++;
            }
        }
        // 公開求人採用結果
        if (!flag_click) {
            if (click_image(&screen, &img_adopt_result, &result)) {
                std::cout << "clicked adopt_result\n";
                flag_click++;
            }
        }
        if (!(flag_bell == 5)) {
            if (flag_bell == 0 && click_image(&screen, &img_bell, &result)) {
                std::cout << "clicked bell_main\n";
                flag_bell = 1;
            }
            if (flag_bell == 1 && click_image(&screen, &img_trade, &result)) {
                std::cout << "clicked trade\n";
                flag_bell = 2;
            }
            if (flag_bell == 2 && click_image(&screen, &img_manufacture, &result)) {
                std::cout << "clicked manufacture\n";
                flag_bell = 3;
            }
            if (flag_bell == 3 && click_image(&screen, &img_trust, &result)) {
                std::cout << "clicked trust\n";
                flag_bell = 4;
            }
            if (flag_bell == 4 && click_image(&screen, &img_return, &result)) {
                std::cout << "clicked return\n";
                flag_bell = 5;
            }
        }
        if (!flag_click) {
            flag_end++;
            if (flag_end > 30) break;
        } else {
            flag_click = 0;
            flag_end = 0;
        }
        sleep(5);
        auto end = system_clock::now();
        auto dur = end - start;        // 要した時間を計算
        auto msec = duration_cast<milliseconds>(dur).count();
        // 要した時間をミリ秒（1/1000秒）に変換して表示
        std::cout << msec/1000.0;
        clock_t p_end = clock();
        std::cout << "(" << (double)(p_end - p_start) / CLOCKS_PER_SEC << ") 秒経過\n";
    }
    

}

void click(CGPoint location) {
    CGEventRef event = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, location, kCGMouseButtonLeft);  
    CGEventSetIntegerValueField(event, kCGMouseEventClickState, 1);  
    CGEventPost(kCGHIDEventTap, event);  
    CGEventSetType(event, kCGEventLeftMouseUp);  
    CGEventPost(kCGHIDEventTap, event);  
    CFRelease(event);    
}

Boolean click_image(cv::Mat *screen, cv::Mat *image, cv::Mat *result) {
    double maxVal;
    cv::Point maxLoc;
    CGPoint location;
    cv::matchTemplate(*screen, *image, *result, cv::TM_CCORR_NORMED);
    cv::minMaxLoc(*result, NULL, &maxVal , NULL, &maxLoc);
    if (maxVal > 0.999) {
        location.x = maxLoc.x + image->cols/2.0;
        location.y = maxLoc.y + image->rows/2.0;
        click(location);
        return true;
    } 
    return false;
}