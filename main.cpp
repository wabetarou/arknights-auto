#include <ApplicationServices/ApplicationServices.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <cctype>

using namespace std::chrono;
typedef struct CGPoint CGPoint;
// g++ -std=c++11 -framework CoreGraphics -framework CoreFoundation main.cpp `pkg-config --cflags opencv4` `pkg-config --libs opencv4`

///print out location in a nice way to std cout
void click(CGPoint);

int main(int argc, char **argv) {
    clock_t p_start = clock();
    auto start = std::chrono::system_clock::now();

    double maxVal;
    cv::Point maxLoc;
    cv::Mat result;
    cv::Mat screen;
    CGPoint location;

    cv::Mat img_auto;
    cv::Mat img_result;
    cv::Mat img_stage_decide;
    cv::Mat img_stage_start;
    cv::Mat img_stage_start2;
    cv::Mat img_result_member;

    double threshold = 0.999;
    int wait_time = 90;

    if (argc > 1) {
        if (strcmp(argv[1],"s") == 0) {
            wait_time = 900;
            img_result = cv::imread("../image/result_annihilation.png",0);
        } else {
            wait_time = atoi(argv[1]);
            img_result = cv::imread("../image/result.png",0);
        }
    } else {
        img_result = cv::imread("../image/result.png",0); 
    }


    img_auto = cv::imread("../image/auto.png",0);
    img_stage_decide = cv::imread("../image/stage_decide.png",0);
    img_stage_start = cv::imread("../image/stage_start.png",0);
    img_stage_start2 = cv::imread("../image/stage_start2.png",0);
    img_result_member = cv::imread("../image/result_member.png",0);

    printf("start\n");
    int flag_end = 0;
    int flag_click = 0;
    while (true) {
        system("screencapture -x ../image/screenshot.png");
        screen = cv::imread("../image/screenshot.png",0);

        // 自動指揮 -> ステージ決定
        cv::matchTemplate(screen, img_auto, result, cv::TM_CCORR_NORMED);
        cv::minMaxLoc(result, NULL, &maxVal , NULL, NULL);
        if (maxVal > threshold) {
            cv::matchTemplate(screen, img_stage_decide, result, cv::TM_CCORR_NORMED);
            cv::minMaxLoc(result, NULL, NULL , NULL, &maxLoc);
            location.x = maxLoc.x + img_stage_decide.cols/2.0;
            location.y = maxLoc.y + img_stage_decide.rows/2.0;
            click(location);
            std::cout << "clicked stage_decide\n";
            flag_click++;
        }
        // ステージスタート
        if (!flag_click) {
            cv::matchTemplate(screen, img_stage_start, result, cv::TM_CCORR_NORMED);
            cv::minMaxLoc(result, NULL, &maxVal , NULL, &maxLoc);
            if (maxVal > threshold) {
                location.x = maxLoc.x + img_stage_start.cols/2.0;
                location.y = maxLoc.y + img_stage_start.rows/2.0;
                click(location);
                std::cout << "clicked stage_start\n";
                flag_click++;
                sleep(wait_time);
            } else {
                cv::matchTemplate(screen, img_stage_start2, result, cv::TM_CCORR_NORMED);
                cv::minMaxLoc(result, NULL, &maxVal , NULL, &maxLoc);
                if (maxVal > threshold) {
                    location.x = maxLoc.x + img_stage_start2.cols/2.0;
                    location.y = maxLoc.y + img_stage_start2.rows/2.0;
                    click(location);
                    std::cout << "clicked stage_start\n";
                    flag_click++;
                    sleep(wait_time);
                }
            }
        }
        // ステージリザルト
        if (!flag_click) {
            cv::matchTemplate(screen, img_result, result, cv::TM_CCORR_NORMED);
            cv::minMaxLoc(result, NULL, &maxVal , NULL, &maxLoc);
            if (maxVal > threshold) {
                location.x = maxLoc.x + img_result.cols/2.0;
                location.y = maxLoc.y + img_result.rows/2.0;
                click(location);
                std::cout << "clicked result\n";
                flag_click++;
            } 
        }
        if (!flag_click) {
            cv::matchTemplate(screen, img_result_member, result, cv::TM_CCORR_NORMED);
            cv::minMaxLoc(result, NULL, &maxVal , NULL, &maxLoc);
            if (maxVal > threshold) {
                location.x = maxLoc.x + img_result_member.cols/2.0;
                location.y = maxLoc.y + img_result_member.rows/2.0;
                click(location);
                std::cout << "clicked result\n";
                flag_click++;
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