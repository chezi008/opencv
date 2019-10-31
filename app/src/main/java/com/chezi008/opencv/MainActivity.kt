package com.chezi008.opencv

import androidx.appcompat.app.AppCompatActivity

import android.os.Bundle
import android.util.Log
import com.chezi008.libopencv.motiondetection.DetectionCallback
import com.chezi008.libopencv.motiondetection.DetectionMotionTracker
import kotlinx.android.synthetic.main.activity_main.*

import org.opencv.core.Mat

class MainActivity : AppCompatActivity() {
    private val TAG = javaClass.simpleName
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btnCallback.setOnClickListener {
            DetectionMotionTracker.nativeProcessFrame(123L, DetectionCallback {
                Log.d(TAG, "onCreate: jni callback------>")
            })
        }
    }
}
