package com.chezi008.libopencv.motiondetection;

public class DetectionMotionTracker
{
    static {
        //注意这里为自己指定的.so文件，无lib前缀，亦无后缀
        System.loadLibrary("pqmotiondetection");
    }


    public static native void nativeProcessFrame(long frame,DetectionCallback callback);
}
