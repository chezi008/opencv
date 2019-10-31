//
// Created by chezi008 on 2019/10/25.
//

#include "include/MotionDetection.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <jni.h>
#include"omp.h"
#include "include/Log.h"

using namespace cv;
#define MOTION_THRESH 10


Mat frameDiff(Mat prevFrame, Mat curFrame, Mat nextFrame) {
    Mat diffFrames1, diffFrames2, output;
    // Compute absolute difference between current frame and the nextframe
    absdiff(nextFrame, curFrame, diffFrames1);
    // Compute absolute difference between current frame and the previous frame
    absdiff(curFrame, prevFrame, diffFrames2);
    // Bitwise "AnD" operation between the above two diff images
    bitwise_and(diffFrames1, diffFrames2, output);
    return output;
}

Mat preVideoFrame, curVideoFrame, nextVideoFrame;
Mat dist, blurDist, threDist, meanDist, stdDist;
void processFrame(Mat curFrame) {
    if (preVideoFrame.empty()) {
        curFrame.copyTo(preVideoFrame);
        LOGI("processFrame preVideoFrame:---->", "");
        return;
    }
    if (curVideoFrame.empty()) {
        curFrame.copyTo(curVideoFrame);
        LOGI("processFrame curVideoFrame:---->", "");
        return;
    }
    curFrame.copyTo(nextVideoFrame);

    char text[100] = {0};

    if (!preVideoFrame.empty() && !curVideoFrame.empty() && !nextVideoFrame.empty()) {
        LOGI("processFrame distMap:---->", "");
        //转换成灰度图
//        cvtColor(nextVideoFrame, frame1, COLOR_BGR2GRAY);
        dist =frameDiff(preVideoFrame,curVideoFrame, nextVideoFrame);

        //降噪 3x3内核,均值滤波操作
        blur(dist, blurDist, Size(3, 3));

        threshold(blurDist, threDist, 100, 255, THRESH_BINARY);
        //计算元素的均值和标准差
        meanStdDev(threDist, meanDist, stdDist);

        sprintf(text, "Standard Deviation - %d", stdDist.at<uchar>(0, 0));
        cv::putText(curFrame, text, Point(70, 70), FONT_HERSHEY_SIMPLEX,
                    1, (255, 0, 255), 1, LINE_AA);

        if (stdDist.at<uchar>(0, 0) > MOTION_THRESH) {
            printf("%d motion detected\r", stdDist.at<uchar>(0, 0));
            LOGI("%d motion detected\r", stdDist.at<uchar>(0, 0));
            fflush(stdout);
        }

//        blurDist.copyTo(curFrame);
        curVideoFrame.copyTo(preVideoFrame);
        nextVideoFrame.copyTo(curVideoFrame);
    } else {
        LOGI("processFrame empty:---->p:%d", preVideoFrame.empty());
    }

    extern "C"
    JNIEXPORT void JNICALL Java_com_chezi008_libopencv_motiondetection_DetectionMotionTracker_nativeProcessFrame
            (JNIEnv *jenv, jclass, jlong cur) {
        Mat *curVideoFrame = reinterpret_cast<Mat *>(cur);
        processFrame(*curVideoFrame);
    }
}

