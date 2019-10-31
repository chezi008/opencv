package com.chezi008.libopencv.motiondetection;

/**
 * - @description:
 * - @author:  chezi008/chezi008@qq.com
 * - @date:  2019/10/31 14:37
 */
public interface DetectionCallback {
    /**
     * 检测到运动
     */
    void motionDetection(int motionThresh);
}
