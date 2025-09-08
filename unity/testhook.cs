using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class testhook : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        /*AndroidJavaClass sys = new AndroidJavaClass("java.lang.System");
        sys.CallStatic("loadLibrary", "shadowhook");*/
      
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnGUI()
    {
        if (GUILayout.Button("OnHOOk",GUILayout.Height(200),GUILayout.Width(200)))
        {
            Debug.Log("Click on hook");
            doHookUnityGetBufferManagerGles();
        }
    }

    [DllImport("nativehook")]
    private static extern void doHookNative();
    [DllImport("nativehook")]
    private static extern void doHookUnityGetBufferManagerGles();

    
}
