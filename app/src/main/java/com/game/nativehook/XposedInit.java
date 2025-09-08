package com.game.nativehook;

import android.os.Handler;
import android.os.Looper;

import java.io.BufferedReader;
import java.io.FileReader;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.callbacks.XC_LoadPackage;
import de.robv.android.xposed.XposedBridge;

public class XposedInit implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(final XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {
        // 只在目标包里执行
      //  if (!lpparam.packageName.equals("com.DefaultCompany.TimeRewinderProject")) {
        if (!lpparam.packageName.equals("com.pwrd.projectt.laohu")) {
            return;
        }

        XposedBridge.log("MyXposedHook: Loaded" + lpparam.packageName);


        // 延迟几秒，等 Unity 初始化
        Handler handler = new Handler(Looper.getMainLooper());
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (isUnityLoaded()) {
                    XposedBridge.log("libunity.so detected, calling native hook...");
                    // 调用你的 JNI 方法
                    try {
                        System.loadLibrary("nativehook"); // 确保你的 so 已经加载
                        doHookDOAddUI(); // JNI 方法
                    } catch (Throwable t) {
                        XposedBridge.log("Native load error: " + t);
                    }
                } else {
                    XposedBridge.log("libunity.so not yet loaded, try again later...");
                }
            }
        }, 5000); // 延迟 5 秒
    }

    // 检查 /proc/self/maps 是否包含 libunity.so
    private boolean isUnityLoaded() {
        try (BufferedReader br = new BufferedReader(new FileReader("/proc/self/maps"))) {
            String line;
            while ((line = br.readLine()) != null) {
                if (line.contains("libil2cpp.so")) {
                    return true;
                }
            }
        } catch (Exception e) {
            XposedBridge.log("Error reading maps: " + e);
        }
        return false;
    }

    public native void doHookbyXposed();
    public native void doHookDOAddUI();
}
