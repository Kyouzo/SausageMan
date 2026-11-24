#pragma once
void InitOffsets();
void ScreenAndTouchOffsets();

void weaponoffsets()
{
  /*
  public System.Int32 $vA(); // 0x5ADDE94 change to 9999 for unli ammo
  public System.Int32 $vA(); // 0x5ADDE94 // decides wheter weapon is rapid or single fire best for snipers
  public System.Single $PB(); // 0x5AE48A0 // RELOAD SPEED, change to 0.1 for fast reload
  public virtual System.Int32 Fire(System.Single holdOnPower, ref System.Boolean& isBulletCost); // 0x5AE68F0 set to 0 for no ammo cost
  public System.Int32 $HC(); // 0x5AE84A0 SET TO 0 NO NEED RELOAD + INFINITE AMMO
  ANOTHER IF TRUE NO RELOAD BUT ALSO NO AMMO public System.Boolean $lC(); // 0x5AE0D88

  */
}

void *mainthread(void *)
{
  ProcMap il2cppMap = waitForLib("libil2cpp.so");
  ProcMap unityMap = waitForLib("libunity.so");

  Il2cpp::Init();
  Il2cpp::EnsureAttached();
  sleep(3);
  LOGI("[4] Getting method offsets...");
  Offset();
  ScreenAndTouchOffsets();
  InitOffsets();
  if (xhook_register(OBFUSCATE("libunity\\.so$"),
                     OBFUSCATE("eglSwapBuffers"),
                     (void *)_eglSwapBuffers,
                     (void **)&orig_eglSwapBuffers) == 0)
  {
    xhook_refresh(0);
    LOGI("Graphics hook installed successfully");
  }
  else
  {
    LOGE("xhook_register failed for eglSwapBuffers");
  }

  started = true;
  return nullptr;
}
__attribute__((constructor)) void lib_main()
{
  std::thread(mainthread, nullptr).detach();
}
extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
  VM = vm;
  JNIEnv *env;
  if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
  {
    return JNI_ERR;
  }
  if (vm == nullptr)
  {
    return JNI_ERR;
  }

  jvm = vm;
  LOGI("JNI loaded successfully");
  return JNI_VERSION_1_6;
}
void InitOffsets()
{

  auto unityEngineImage = Il2cpp::GetImage("UnityEngine.dll");
  if (!unityEngineImage)
  {
    LOGE("Failed to get UnityEngine.dll image");
    return;
  }

  auto transformClass = unityEngineImage->getClass("UnityEngine.Transform");
  if (transformClass)
  {
    auto getPositionMethod = transformClass->getMethod("get_position", 0);
    if (getPositionMethod)
    {
      Class_Transform_get_position = getPositionMethod->getAbsAddress();
      LOGI("Transform.get_position found at: 0x%lX", Class_Transform_get_position);
    }
    else
    {
      LOGI("Failed to find Transform.get_position method");
    }
  }
  else
  {
    LOGI("Failed to find Transform class");
  }

  auto componentClass = unityEngineImage->getClass("UnityEngine.Component");
  if (componentClass)
  {
    auto getTransformMethod = componentClass->getMethod("get_transform", 0);
    if (getTransformMethod)
    {
      Class_Component_get_transform = getTransformMethod->getAbsAddress();
      LOGI("Component.get_transform found at: 0x%lX", Class_Component_get_transform);
    }
    else
    {
      LOGI("Failed to find Component.get_transform method");
    }
  }
  else
  {
    LOGI("Failed to find Component class");
  }

  auto cameraClass = unityEngineImage->getClass("UnityEngine.Camera");
  if (cameraClass)
  {
    auto getMainMethod = cameraClass->getMethod("get_main", 0);
    if (getMainMethod)
    {
      Class_Camera_get_main = getMainMethod->getAbsAddress();
      LOGI("Camera.get_main found at: 0x%lX", Class_Camera_get_main);
    }
    else
    {
      LOGI("Failed to find Camera.get_main method");
    }

    auto worldToScreenPointMethod = cameraClass->getMethod("WorldToScreenPoint", 1);
    if (worldToScreenPointMethod)
    {
      Class_Camera_WorldToScreenPoint = worldToScreenPointMethod->getAbsAddress();
      LOGI("Camera.WorldToScreenPoint found at: 0x%lX", Class_Camera_WorldToScreenPoint);
    }
    else
    {
      LOGI("Failed to find Camera.WorldToScreenPoint method");
    }
  }
  else
  {
    LOGI("Failed to find Camera class");
  }

  LOGI("All offsets Hooked");
}

void ScreenAndTouchOffsets()
{

  auto inputModuleImage = Il2cpp::GetImage("UnityEngine.InputLegacyModule.dll");
  if (!inputModuleImage)
  {
    inputModuleImage = Il2cpp::GetImage("UnityEngine.dll");
  }

  if (inputModuleImage)
  {
    auto inputClass = inputModuleImage->getClass("UnityEngine.Input");
    if (inputClass)
    {
      auto getTouchMethod = inputClass->getMethod("GetTouch", 1);
      if (getTouchMethod)
      {
        Input_GetTouch = getTouchMethod->getAbsAddress();
        LOGI("Input.GetTouch found at: 0x%lx", Input_GetTouch);
      }
      else
      {
        LOGI("Failed to find Input.GetTouch address");
      }

      auto getTouchSupportedMethod = inputClass->getMethod("get_touchSupported", 0);
      if (getTouchSupportedMethod)
      {
        Input_get_touchSupported = getTouchSupportedMethod->getAbsAddress();
        LOGI("Input.get_touchSupported found at: 0x%lx", Input_get_touchSupported);
      }
      else
      {
        LOGI("Failed to find Input.get_touchSupported address");
      }

      auto getTouchCountMethod = inputClass->getMethod("get_touchCount", 0);
      if (getTouchCountMethod)
      {
        Input_get_touchCount = getTouchCountMethod->getAbsAddress();
        LOGI("Input.get_touchCount found at: 0x%lx", Input_get_touchCount);
      }
      else
      {
        LOGI("Failed to find Input.get_touchCount address");
      }

      auto getMousePositionMethod = inputClass->getMethod("get_mousePosition", 0);
      if (getMousePositionMethod)
      {
        Input_get_mousePosition = getMousePositionMethod->getAbsAddress();
        LOGI("Input.get_mousePosition found at: 0x%lx", Input_get_mousePosition);
      }
      else
      {
        LOGI("Failed to find Input.get_mousePosition address");
      }
    }
    else
    {
      LOGI("Failed to find Input class");
    }
  }
  else
  {
    LOGE("Failed to get Input module image");
  }
  auto coreModuleImage = Il2cpp::GetImage("UnityEngine.CoreModule.dll");
  if (coreModuleImage)
  {
    auto screenClass = coreModuleImage->getClass("UnityEngine.Screen");
    if (screenClass)
    {
      auto getWidthMethod = screenClass->getMethod("get_width", 0);
      if (getWidthMethod)
      {
        Screen_get_width = getWidthMethod->getAbsAddress();
        LOGI("Screen.get_width found at: 0x%lx", Screen_get_width);
      }
      else
      {
        LOGI("Failed to find Screen.get_width address");
      }

      auto getHeightMethod = screenClass->getMethod("get_height", 0);
      if (getHeightMethod)
      {
        Screen_get_height = getHeightMethod->getAbsAddress();
        LOGI("Screen.get_height found at: 0x%lx", Screen_get_height);
      }
      else
      {
        LOGI("Failed to find Screen.get_height address");
      }

      auto getDpiMethod = screenClass->getMethod("get_dpi", 0);
      if (getDpiMethod)
      {
        Screen_get_dpi = getDpiMethod->getAbsAddress();
        LOGI("Screen.get_dpi found at: 0x%lx", Screen_get_dpi);
      }
      else
      {
        LOGI("Failed to find Screen.get_dpi address");
      }
    }
    else
    {
      LOGI("Failed to find Screen class");
    }
  }
  else
  {
    LOGE("Failed to get CoreModule image");
  }
}
