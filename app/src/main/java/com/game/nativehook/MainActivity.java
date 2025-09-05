package com.game.nativehook;

import androidx.appcompat.app.AppCompatActivity;
import com.bytedance.shadowhook.ShadowHook;

import android.os.Bundle;
import android.widget.TextView;

import com.game.nativehook.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'nativehook' library on application startup.
    static {
      //  System.loadLibrary("nativehook");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;

       // doHook();
    }

    /**
     * A native method that is implemented by the 'nativehook' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void doHook();
    public native void doUnHook();
}