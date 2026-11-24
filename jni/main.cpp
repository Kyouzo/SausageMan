#define IMGUI_DEFINE_MATH_OPERATORS
#include <jni.h>
#include <pthread.h>
#include <android/input.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <sstream>
#include <array>
#include <unistd.h>
#include <sys/system_properties.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include "include/Logger.h"
#include "Oxorany/oxorany.h"
#include "include/xdl/xdl.h"
#include "dobby.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_android.h"
#include "src/ImGui/IconsFontAwesome7.h"
#include "icons/azxie.h"
#include "icons/azxiofont.h"
#include "src/ImGui/font_awesome.h"
#include "icons/Textures.h"
#include "include/IL2CppSDKGenerator/BasicStructs/Call_BasicStructs.h"
#include "include/IL2CppSDKGenerator/IL2Cpp/Call_IL2Cpp.h"

#include "Il2cpp/Il2cpp.h"
#include "Il2cpp/il2cpp-class.h"

#include "include/Hook/KittyMemory/MemoryPatch.h"
#include "src/obfuscate.h"
#include "src/Utils.h"
#include "src/Touch.h"
#include "xhook/xhook.h"
#include "AxiomRuntime/include/online.h"
ImFont *azxioicons = nullptr;
#include "src/customwidgets.h"

uintptr_t m_unity;
bool setup = false;
using namespace Il2cpp;
using namespace ImGui;
int32_t sWidth = 0;
int32_t sHeight = 0;
#define glWidth sWidth
#define glHeight sHeight
std::atomic<uintptr_t> m_akdelaywg{
    0};
static bool isMenuVisible = true;
static int tab = 0;
#include "Features/CallESP.h"

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
  eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

  if (glWidth > 0 && glHeight > 0)
  {
    if (sWidth == 0)
      sWidth = glWidth;
    if (sHeight == 0)
      sHeight = glHeight;
  }

  if (!setup)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    Styles();

    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");

    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = nullptr;

    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;

    if (sizeof(custom_font_data) > 0)
    {
      ImFontConfig cfg;
      cfg.FontDataOwnedByAtlas = false;
      io.Fonts->AddFontFromMemoryTTF((void *)azxio_font, sizeof(azxio_font), 20.0f, &cfg, io.Fonts->GetGlyphRangesChineseFull());

      if (Icons::DataSize > 0)
      {
        ImFontConfig icon_cfg;
        icon_cfg.FontDataOwnedByAtlas = false;
        icon_cfg.PixelSnapH = true;
        g_IconFont = io.Fonts->AddFontFromMemoryTTF((void *)Icons::RawData, Icons::DataSize, 24.0f, &icon_cfg, io.Fonts->GetGlyphRangesDefault());
      }
    }
    else
    {
      io.Fonts->AddFontDefault();
    }

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;

    static const ImWchar icons_ranges[] = {
        0xe005,
        0xf8ff,
        0};

    io.Fonts->AddFontFromMemoryTTF(
        fa_solid_900_ttf,
        fa_solid_900_ttf_len,
        10.0f,
        &icons_config,
        icons_ranges);

    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    cfg.OversampleH = 4;
    cfg.OversampleV = 4;
    cfg.PixelSnapH = true;
    cfg.MergeMode = false;

    azxioicons = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
        AzxioIcons,
        sizeof(AzxioIcons),
        25.0f,
        &cfg,
        ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

    setup = true;
  }

  ImGuiIO &io = ImGui::GetIO();
  ImGui_GetTouch(&io);
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
  ImGui::NewFrame();

  Azxie(ImGui::GetBackgroundDrawList());

  ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

  ImGui::Begin("Sausage Man - Kyouzo", nullptr, ImGuiWindowFlags_None);
  {
    ImVec2 avail = ImGui::GetContentRegionAvail();

    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float child_width = (avail.x - spacing) * 0.5f;
    float child_height = avail.y;

    ImGui::BeginChild("ChildOne", ImVec2(child_width, child_height), true);
    {
      if (var.isEspEnabled)
      {
        if (ImGui::Button("ESP ENABLED", ImVec2(-1, 40)))
        {
          var.isEspEnabled = false;
        }
      }
      else
      {
        if (ImGui::Button("ESP DISABLED", ImVec2(-1, 40)))
        {
          var.isEspEnabled = true;
        }
      }

      AnimatedToggle("Esp Line", &var.EspLine);
      AnimatedToggle("Esp Box", &var.EspBox);
      AnimatedToggle("Esp Bone", &var.EspBone);
      AnimatedToggle("Esp Health", &var.EspHealth);
      AnimatedToggle("Esp Name", &var.EspName);
      AnimatedToggle("Esp Count", &var.EspCount);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("ChildTwo", ImVec2(child_width, child_height), true);
    {
    }
    ImGui::EndChild();
  }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  if (clearMousePos)
  {
    io.MousePos = ImVec2(-1, -1);
    clearMousePos = false;
  }
  return orig_eglSwapBuffers(dpy, surface);
}
#include "AzxionsThreads.h"