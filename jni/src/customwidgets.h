#pragma once
extern ImFont *azxioicons;

float accent[4] = { 118 / 255.f, 119 / 255.f, 130 / 255.f, 1.f };

struct TabState
{
  float animProgress = 0.0f;
  int activeTab = 0;
  std::vector<float> tabTextAnimProgress;
};
struct ToggleState
{
  float animProgress = 0.0f;
  bool targetState = false;
};
float LerpFloat(float a, float b, float t)
{
  return a + (b - a) * t;
}
float LerpExponential(float current, float target, float deltaTime, float speed)
{
  return LerpFloat(current, target, 1.0f - expf(-speed * deltaTime));
}

float EaseOutCubic(float t) {
  t = std::clamp(t, 0.0f, 1.0f);
  return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
}
static std::unordered_map<std::string, ToggleState> g_toggleStates;
bool AnimatedToggle(const char *label, bool *value)
{
  ToggleState &toggleState = g_toggleStates[label];

  float speed = 0.12f;

  if (*value != toggleState.targetState)
  {
    toggleState.targetState = *value;
  }

  if (toggleState.targetState)
  {
    toggleState.animProgress = LerpFloat(toggleState.animProgress, 1.0f, speed);
  }
  else
  {
    toggleState.animProgress = LerpFloat(toggleState.animProgress, 0.0f, speed);
  }

  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 toggleSize = ImVec2(50.0f, 26.0f);
  ImRect toggleRect(cursorPos, ImVec2(cursorPos.x + toggleSize.x, cursorPos.y + toggleSize.y));

  ImDrawList *drawList = ImGui::GetWindowDrawList();

  ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
  if (toggleState.animProgress > 0.5f)
  {
    bgColor = ImVec4(0.4f, 0.8f, 0.4f, 0.8f);
  }
  drawList->AddRectFilled(toggleRect.Min, toggleRect.Max, ImGui::GetColorU32(bgColor), 13.0f);
  drawList->AddRect(toggleRect.Min, toggleRect.Max,
                    ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.8f)), 13.0f, 0, 1.5f);

  float circleX = LerpFloat(cursorPos.x + 4.0f, cursorPos.x + toggleSize.x - 22.0f, EaseOutCubic(toggleState.animProgress));
  drawList->AddCircleFilled(ImVec2(circleX + 9.0f, cursorPos.y + 13.0f), 9.0f,
                            ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.9f)));

  ImGui::Dummy(toggleSize);
  ImGui::SameLine();
  ImGui::Text(label);

  ImGui::SetCursorScreenPos(toggleRect.Min);
  ImGui::InvisibleButton(("##toggle_" + std::string(label)).c_str(), toggleSize);

  if (ImGui::IsItemClicked())
  {
    *value = !*value;
    return true;
  }

  return false;
}

bool CustomTabBar(const char *id, int *activeTab, const std::vector<const char *> &tabNames, TabState &tabState)
{
  ImDrawList *drawList = ImGui::GetWindowDrawList();
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  float availWidth = ImGui::GetContentRegionAvail().x;
  float tabWidth = availWidth / tabNames.size();

  if (tabState.tabTextAnimProgress.size() != tabNames.size())
  {
    tabState.tabTextAnimProgress.resize(tabNames.size(), 0.0f);
  }

  float deltaTime = ImGui::GetIO().DeltaTime;

  float animSpeed = 3.0f;

  tabState.animProgress = LerpExponential(tabState.animProgress, 1.0f, deltaTime, animSpeed);

  ImVec2 tabBarSize = ImVec2(availWidth, 45.0f);
  ImGui::Dummy(tabBarSize);
  ImGui::Spacing();

  drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + availWidth, cursorPos.y + 45.0f),
                          ImGui::GetColorU32(ImGuiCol_ChildBg), 6.0f);

  float targetTabPos = (float)*activeTab;
  float animatedTabPos = LerpExponential(tabState.animProgress == 0.0f ? *activeTab : tabState.animProgress * targetTabPos,
                                         targetTabPos, deltaTime, animSpeed);

  float indicatorX = cursorPos.x + animatedTabPos * tabWidth;
  ImVec2 indicatorMin = ImVec2(indicatorX, cursorPos.y);
  ImVec2 indicatorMax = ImVec2(indicatorX + tabWidth, cursorPos.y + 45.0f);

  ImVec4 activeIndicatorColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
  activeIndicatorColor.w *= 0.15f;
  drawList->AddRectFilled(indicatorMin, indicatorMax, ImGui::GetColorU32(activeIndicatorColor), 4.0f);

  for (size_t i = 0; i < tabNames.size(); i++)
  {
    ImVec2 tabMin = ImVec2(cursorPos.x + i * tabWidth, cursorPos.y);
    ImVec2 tabMax = ImVec2(tabMin.x + tabWidth, cursorPos.y + 45.0f);
    ImVec2 tabCenter = ImVec2((tabMin.x + tabMax.x) * 0.5f, (tabMin.y + tabMax.y) * 0.5f);

    bool isActive = (int)i == *activeTab;
    float hoverAlpha = ImGui::IsMouseHoveringRect(tabMin, tabMax) ? 0.1f : 0.0f;

    float targetAlpha = isActive ? 1.0f : 0.0f;
    tabState.tabTextAnimProgress[i] = LerpExponential(
        tabState.tabTextAnimProgress[i],
        targetAlpha,
        deltaTime,
        animSpeed);

    if (hoverAlpha > 0.0f && !isActive)
    {
      drawList->AddRectFilled(tabMin, tabMax, ImGui::GetColorU32(ImVec4(1, 1, 1, hoverAlpha)), 4.0f);
    }

    if (isActive)
    {
      float borderHeight = 3.0f;
      drawList->AddLine(ImVec2(tabMin.x, tabMax.y - borderHeight),
                        ImVec2(tabMax.x, tabMax.y - borderHeight),
                        ImGui::GetColorU32(ImVec4(1.0f, 0.4f, 0.4f, 0.8f)), borderHeight);
    }

    ImVec2 textSize = ImGui::CalcTextSize(tabNames[i]);

    ImVec2 textPos = ImVec2(
        tabCenter.x - textSize.x * 0.5f,
        tabCenter.y - textSize.y * 0.5f);

    ImGui::PushClipRect(tabMin, tabMax, true);

    ImVec4 activeColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    ImVec4 inactiveColor = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
    ImVec4 textColor = ImLerp(inactiveColor, activeColor, tabState.tabTextAnimProgress[i]);
    ImGui::PushFont(azxioicons);
    drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                      textPos,
                      ImGui::GetColorU32(textColor), tabNames[i]);
                      ImGui::PopFont();
    ImGui::PopClipRect();

    ImGui::SetCursorScreenPos(tabMin);
    ImGui::InvisibleButton(("##tab_" + std::string(tabNames[i])).c_str(), ImVec2(tabWidth, 45.0f));
    if (ImGui::IsItemClicked())
    {
      *activeTab = (int)i;
      tabState.animProgress = 0.0f;
    }
  }

  return true;
}

bool AnimatedToggle(const char *label, bool *value, ToggleState &toggleState)
{
  float speed = 0.12f;

  if (*value != toggleState.targetState)
  {
    toggleState.targetState = *value;
  }

  if (toggleState.targetState)
  {
    toggleState.animProgress = LerpFloat(toggleState.animProgress, 1.0f, speed);
  }
  else
  {
    toggleState.animProgress = LerpFloat(toggleState.animProgress, 0.0f, speed);
  }

  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 toggleSize = ImVec2(50.0f, 26.0f);
  ImRect toggleRect(cursorPos, ImVec2(cursorPos.x + toggleSize.x, cursorPos.y + toggleSize.y));

  ImDrawList *drawList = ImGui::GetWindowDrawList();

  ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
  if (toggleState.animProgress > 0.5f)
  {
    bgColor = ImVec4(0.4f, 0.8f, 0.4f, 0.8f);
  }
  drawList->AddRectFilled(toggleRect.Min, toggleRect.Max, ImGui::GetColorU32(bgColor), 13.0f);

  drawList->AddRect(toggleRect.Min, toggleRect.Max,
                    ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.8f)), 13.0f, 0, 1.5f);

  float circleX = LerpFloat(cursorPos.x + 4.0f, cursorPos.x + toggleSize.x - 22.0f, EaseOutCubic(toggleState.animProgress));
  ImVec2 circlePos = ImVec2(circleX, cursorPos.y + 3.0f);
  drawList->AddCircleFilled(ImVec2(circleX + 9.0f, cursorPos.y + 13.0f), 9.0f,
                            ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.9f)));

  ImGui::Dummy(toggleSize);
  ImGui::SameLine();
  ImGui::Text(label);

  ImGui::SetCursorScreenPos(toggleRect.Min);
  ImGui::InvisibleButton(("##toggle_" + std::string(label)).c_str(), toggleSize);

  if (ImGui::IsItemClicked())
  {
    *value = !*value;
    return true;
  }

  return false;
}
bool CustomSlider(const char *label, float *v, float v_min, float v_max, const char *format = "%.0f")
{
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;
  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  ImGuiID id = window->GetID(label);
  float w = 300.0f;
  float height_slider_track = 15.0f;
  float height_grabber = 21.0f;
  float text_spacing = 5.0f;
  ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
  char value_buf[64];
  snprintf(value_buf, sizeof(value_buf), format, *v);
  ImVec2 value_size = ImGui::CalcTextSize(value_buf, NULL, true);
  ImVec2 pos = window->DC.CursorPos;
  float total_h = label_size.y + text_spacing + height_grabber;
  ImRect total_bb(pos, ImVec2(pos.x + w, pos.y + total_h + 2.0f));
  ImRect slider_bb(
      ImVec2(pos.x, pos.y + label_size.y + text_spacing),
      ImVec2(pos.x + w, pos.y + label_size.y + text_spacing + height_slider_track));
  ImGui::ItemSize(total_bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(total_bb, id))
    return false;
  bool hovered, held;
  ImRect interact_bb = slider_bb;
  interact_bb.Expand(ImVec2(0.0f, 4.0f));
  bool pressed = ImGui::ButtonBehavior(interact_bb, id, &hovered, &held);
  if (held || pressed)
  {
    float t = (g.IO.MousePos.x - slider_bb.Min.x) / (slider_bb.Max.x - slider_bb.Min.x);
    t = ImClamp(t, 0.0f, 1.0f);
    *v = v_min + (v_max - v_min) * t;
    ImGui::MarkItemEdited(id);
  }
  static ImGuiStorage *animStorage = nullptr;
  if (!animStorage)
    animStorage = new ImGuiStorage();
  float anim_v = animStorage->GetFloat(id, *v);
  float lerp_speed = 18.0f * g.IO.DeltaTime;
  if (fabsf(anim_v - *v) > 0.01f)
    anim_v += (*v - anim_v) * lerp_speed;
  else
    anim_v = *v;
  animStorage->SetFloat(id, anim_v);
  ImGuiID hover_id = window->GetID((std::string(label) + "_hover").c_str());
  float anim_hover = animStorage->GetFloat(hover_id, 0.0f);
  float target_hover = (hovered || held) ? 1.0f : 0.0f;
  float hover_speed = 10.0f * g.IO.DeltaTime;
  anim_hover += (target_hover - anim_hover) * hover_speed;
  animStorage->SetFloat(hover_id, anim_hover);
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImU32 col_bg = ImGui::GetColorU32(ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
  ImU32 col_accent = IM_COL32(255, 214, 235, 255);
  ImU32 col_accent_dim = IM_COL32(255, 214, 235, 150);
  ImU32 col_text = ImGui::GetColorU32(ImVec4(0.80f, 0.80f, 0.80f, 1.00f));
  ImU32 col_text_val = IM_COL32(255, 255, 255, 255);
  draw_list->AddText(pos, col_text, label);
  draw_list->AddText(ImVec2(total_bb.Max.x - value_size.x, pos.y), col_text_val, value_buf);
  float fraction = (anim_v - v_min) / (v_max - v_min);
  fraction = ImClamp(fraction, 0.0f, 1.0f);
  float track_y = slider_bb.Min.y + (slider_bb.GetHeight() * 0.5f);
  draw_list->AddRectFilled(
      ImVec2(slider_bb.Min.x, track_y - height_slider_track * 0.5f),
      ImVec2(slider_bb.Max.x, track_y + height_slider_track * 0.5f),
      col_bg,
      height_slider_track * 0.5f);
  float current_grab_radius = (height_grabber * 0.5f) + (anim_hover * 1.5f);
  float grab_x = slider_bb.Min.x + (slider_bb.GetWidth() * fraction);
  grab_x = ImClamp(grab_x, slider_bb.Min.x + current_grab_radius, slider_bb.Max.x - current_grab_radius);
  if (fraction > 0.01f)
  {
    draw_list->AddRectFilled(
        ImVec2(slider_bb.Min.x, track_y - height_slider_track * 0.5f),
        ImVec2(grab_x, track_y + height_slider_track * 0.5f),
        col_accent,
        height_slider_track * 0.5f);
  }
  if (hovered || held)
  {
    draw_list->AddCircleFilled(ImVec2(grab_x, track_y), current_grab_radius + 3.0f, col_accent_dim);
  }
  draw_list->AddCircleFilled(ImVec2(grab_x, track_y), current_grab_radius, IM_COL32(255, 255, 255, 255));
  draw_list->AddCircleFilled(ImVec2(grab_x, track_y), current_grab_radius * 0.6f, col_accent);
  return (held || pressed);
}

void Styles()
{
  ImGuiStyle &style = ImGui::GetStyle();

  style.WindowRounding = 6.0f;
  style.ChildRounding = 6.0f;
  style.FrameRounding = 8.0f;
  style.PopupRounding = 0.0f;
  style.ScrollbarRounding = 5.0f;
  style.GrabRounding = 2.0f;
  style.TabRounding = 2.0f;

  style.WindowBorderSize = 0.8f;
  style.ChildBorderSize = 0.8f;
  style.PopupBorderSize = 0.8f;
  style.FrameBorderSize = 0.0f;
  style.TabBorderSize = 0.8f;

 // style.WindowPadding = ImVec2(8.0f, 8.0f);
 // style.FramePadding = ImVec2(6.0f, 4.0f);
 // style.ItemSpacing = ImVec2(6.0f, 4.0f);
  //!style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
  // style.CellPadding = ImVec2(4.0f, 2.0f);
//   style.TouchExtraPadding = ImVec2(0.0f, 0.0f);

  style.ScrollbarSize = 12.0f;
  style.GrabMinSize = 10.0f;

  ImVec4 *colors = style.Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.80f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.18f, 0.80f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 0.80f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
  colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
  colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
  colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f);
  colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
  colors[ImGuiCol_TabActive] = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 0.80f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 0.80f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 0.80f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.80f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.56f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.16f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
}

class CustomWidget
{
public:
  bool text_field(
      const char *label,
      const char *id,
      char *buf,
      size_t buf_size,
      const ImVec2 &size,
      const ImU32 &bg_color,
      float rounding,
      ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
  {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;

    ImGuiContext &g = *GImGui;
    ImGuiStyle &style = g.Style;
    const ImGuiID widget_id = window->GetID(id);

    ImVec2 frame_size = size;
    if (frame_size.x <= 0.0f)
      frame_size.x = ImGui::CalcItemWidth();
    if (frame_size.y <= 0.0f)
      frame_size.y = ImGui::GetFrameHeight();

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect frame_bb(pos, ImVec2(pos.x + frame_size.x, pos.y + frame_size.y));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max);

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, widget_id))
      return false;

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(
        frame_bb.Min,
        frame_bb.Max,
        ImGui::GetColorU32(bg_color),
        rounding);

    const bool hovered = ImGui::IsItemHovered();
    const bool active = ImGui::IsItemActive();

    ImVec4 border_color = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
    if (active)
      border_color = ImVec4(0.6f, 0.6f, 0.8f, 0.8f);
    else if (hovered)
      border_color = ImVec4(0.5f, 0.5f, 0.6f, 0.6f);

    draw_list->AddRect(
        frame_bb.Min,
        frame_bb.Max,
        ImGui::GetColorU32(border_color),
        rounding,
        0,
        1.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushItemWidth(frame_size.x);

    bool value_changed = ImGui::InputText(id, buf, buf_size, flags);

    ImGui::PopItemWidth();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
    if (label[0] != '\0' && label[0] != '#')
    {
      const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
      const ImVec2 label_pos = ImVec2(
          pos.x + style.FramePadding.x,
          pos.y - label_size.y - style.ItemSpacing.y);

      ImGui::RenderText(label_pos, label);
    }

    return value_changed;
  }
};
inline std::unique_ptr<CustomWidget> widget = std::make_unique<CustomWidget>();



namespace lonxzsy
{
    const ImVec4 Col_BgBase = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
    const ImVec4 Col_GlowTopLeft = ImVec4(0.28f, 0.12f, 0.48f, 1.00f);
    const ImVec4 Col_GlowBotRight = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);

    const ImVec4 Col_Panel = ImVec4(0.10f, 0.09f, 0.13f, 0.60f);
    const ImVec4 Col_Border = ImVec4(0.20f, 0.18f, 0.28f, 1.00f);
    const ImVec4 Col_Accent = ImVec4(0.68f, 0.40f, 0.98f, 1.00f);

    const ImVec4 Col_Text = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    const ImVec4 Col_TextDim = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    const ImVec4 Col_ComboBg = ImVec4(0.11f, 0.10f, 0.14f, 1.00f);

    struct AnimState { float Value; };
    static std::map<ImGuiID, AnimState> s_Anim;

    float ImLerp(float a, float b, float t) { return a + (b - a) * t; }

    ImU32 GetColor(ImVec4 col, float alpha = 1.0f) {
        ImVec4 c = col; c.w *= alpha;
        return ImGui::ColorConvertFloat4ToU32(c);
    }

    float GetAnim(ImGuiID id, float target, float speed) {
        auto& state = s_Anim[id];
        float dt = ImGui::GetIO().DeltaTime;
        float t = 1.0f - std::pow(0.5f, dt * speed);
        state.Value = ImLerp(state.Value, target, t);
        if (std::abs(target - state.Value) < 0.0005f) state.Value = target;
        return state.Value;
    }

    void DrawShadow(ImDrawList* dl, ImVec2 min, ImVec2 max, float rounding, float size, ImVec4 color, float alpha_mul = 1.0f) {
        dl->AddRect(min - ImVec2(1, 1), max + ImVec2(1, 1), GetColor(color, 0.15f * alpha_mul), rounding, 0, 1.5f);
        dl->AddRect(min - ImVec2(size / 2, size / 2), max + ImVec2(size / 2, size / 2), GetColor(color, 0.05f * alpha_mul), rounding + size / 2, 0, 3.0f);
    }

    void DrawBackground(ImDrawList* dl, ImVec2 pos, ImVec2 size, float rounding) {
        dl->AddRectFilled(pos, pos + size, GetColor(Col_BgBase), rounding);

        dl->AddRectFilledMultiColor(
            pos,
            pos + size,
            GetColor(Col_GlowTopLeft, 0.30f),
            GetColor(Col_BgBase, 0.0f),
            GetColor(Col_GlowBotRight, 0.20f),
            GetColor(Col_BgBase, 0.0f)
        );

        dl->AddRect(pos, pos + size, GetColor(Col_Border, 0.4f), rounding, 0, 1.0f);

        float sb_width = 75.0f;
        dl->AddLine(
            pos + ImVec2(sb_width, 20),
            pos + ImVec2(sb_width, size.y - 20),
            GetColor(Col_Border, 0.3f),
            1.0f
        );
    }

    bool Tab(const char* label, const char* icon, bool selected, ImFont* icon_font = nullptr) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImVec2(50, 50);
        ImRect bb(pos, pos + size);

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        float anim_select = GetAnim(id, selected ? 1.0f : 0.0f, 12.0f);
        float anim_hover = GetAnim(id + 1, hovered ? 1.0f : 0.0f, 15.0f);

        ImDrawList* dl = window->DrawList;

        if (anim_select > 0.001f || anim_hover > 0.001f) {
            float bg_alpha = ImMax(anim_select * 0.1f, anim_hover * 0.05f);
            ImU32 glow_color = GetColor(Col_Accent, bg_alpha);

            dl->AddRectFilled(bb.Min + ImVec2(4, 4), bb.Max - ImVec2(4, 4), glow_color, 12.0f);

            if (anim_select > 0.001f) {
                DrawShadow(dl, bb.Min + ImVec2(6, 8), bb.Max - ImVec2(8, 8), 10.0f, 15.0f, Col_Accent, anim_select * 0.5f);
                float bar_h = 20.0f * anim_select;
                float center_y = bb.GetCenter().y;
                dl->AddRectFilled(
                    ImVec2(bb.Min.x, center_y - bar_h / 2),
                    ImVec2(bb.Min.x + 3, center_y + bar_h / 2),
                    GetColor(Col_Accent, anim_select), 4.0f
                );
            }
        }

        ImVec4 col_base = ImLerp(Col_TextDim, Col_Text, anim_hover);
        ImVec4 col_active = Col_Accent;
        ImVec4 col_final = ImLerp(col_base, col_active, anim_select);
        ImU32 text_col_u32 = GetColor(col_final);

        if (icon_font) {
            ImGui::PushFont(icon_font);
            ImVec2 txt_sz = ImGui::CalcTextSize(icon);
            dl->AddText(bb.GetCenter() - txt_sz * 0.5f, text_col_u32, icon);
            ImGui::PopFont();
        }
        else {
            ImVec2 txt_sz = ImGui::CalcTextSize(icon);
            dl->AddText(bb.GetCenter() - txt_sz * 0.5f, text_col_u32, icon);
        }

        return pressed;
    }
    
    void BeginPanel(const char* name, ImVec2 size)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, GetColor(Col_Panel));
    ImGui::PushStyleColor(ImGuiCol_Border, GetColor(Col_Border));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15.0f);

    ImGui::BeginChild(name, size, true,
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse);

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 s = window->Size;

  /*  // normal content padding
    ImGui::SetCursorPosY(10.0f);
    ImGui::Indent(15.0f);
    ImGui::PushItemWidth(s.x - 30.0f);*/
}


   /* void BeginPanel(const char* name, ImVec2 size, bool* header_toggle_var = nullptr) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, GetColor(Col_Panel));
        ImGui::PushStyleColor(ImGuiCol_Border, GetColor(Col_Border));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15.0f);

        ImGui::BeginChild(name, size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImDrawList* dl = window->DrawList;
        ImVec2 p = window->Pos;
        ImVec2 s = window->Size;

        float header_h = 38.0f;
        dl->AddRectFilled(p, p + ImVec2(s.x, header_h), GetColor(ImVec4(0, 0, 0, 0.2f)), 15.0f, ImDrawFlags_RoundCornersTop);
        dl->AddLine(p + ImVec2(0, header_h), p + ImVec2(s.x, header_h), GetColor(Col_Border, 0.5f), 1.0f);

        dl->AddText(p + ImVec2(15, 11), GetColor(Col_Text), name);

        bool panel_disabled = false;
        if (header_toggle_var) {
            ImGui::PushID("HeaderToggle");
            float toggle_w = 36.0f;
            float toggle_h = 20.0f;
            ImVec2 t_pos = p + ImVec2(s.x - toggle_w - 12, 9);
            ImRect t_bb(t_pos, t_pos + ImVec2(toggle_w, toggle_h));

            ImGui::ItemSize(t_bb);
            if (ImGui::ItemAdd(t_bb, window->GetID("ToggleClickZone"))) {
                bool hovered, held;
                bool pressed = ImGui::ButtonBehavior(t_bb, window->GetID("ToggleClickZone"), &hovered, &held);
                if (pressed) *header_toggle_var = !(*header_toggle_var);

                float anim = GetAnim(window->GetID("ToggleAnim"), *header_toggle_var ? 1.0f : 0.0f, 18.0f);

                ImU32 bg = GetColor(ImLerp(ImVec4(0.15f, 0.14f, 0.20f, 1.f), Col_Accent, anim));
                dl->AddRectFilled(t_bb.Min, t_bb.Max, bg, 20.0f);

                float pad = 3.0f;
                float r = (toggle_h / 2.0f) - pad;
                float knob_center_x = ImLerp(t_bb.Min.x + pad + r, t_bb.Max.x - pad - r, anim);
                dl->AddCircleFilled(ImVec2(knob_center_x, t_bb.GetCenter().y), r, IM_COL32(255, 255, 255, 255));

                if (!(*header_toggle_var)) panel_disabled = true;
            }
            ImGui::PopID();
        }

        ImGui::SetCursorPosY(header_h + 10.0f);
        ImGui::Indent(15.0f);
        ImGui::PushItemWidth(s.x - 30.0f);

        if (panel_disabled) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.4f);
        }
    }*/
/*
    void EndPanel(bool* header_toggle_var = nullptr) {
        if (header_toggle_var && !(*header_toggle_var)) {
            ImGui::PopStyleVar();
            ImGui::PopItemFlag();
        }
        ImGui::PopItemWidth();
        ImGui::Unindent(15.0f);
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }
*/


void EndPanel()
{
   // ImGui::PopItemWidth();
   // ImGui::Unindent(15.0f);
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
}

    bool Checkbox(const char* label, bool* v) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;

        float pad_right = 15.0f;
        float w = ImGui::GetContentRegionAvail().x - pad_right;
        float h = 26.0f;
        ImRect bb(pos, pos + ImVec2(w, h));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        if (pressed) *v = !(*v);

        float anim = GetAnim(id, *v ? 1.0f : 0.0f, 20.0f);
        float anim_hov = GetAnim(id + 1, hovered ? 1.0f : 0.0f, 20.0f);

        ImVec4 txt_col = ImLerp(Col_TextDim, Col_Text, ImMax(anim, anim_hov));
        if (anim > 0.01f) txt_col = ImLerp(txt_col, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), anim * 0.5f);

        window->DrawList->AddText(pos + ImVec2(0, 5), GetColor(txt_col), label);

        float box_s = 18.0f;
        ImVec2 box_pos = pos + ImVec2(w - box_s, 4);
        ImRect box_bb(box_pos, box_pos + ImVec2(box_s, box_s));

        ImU32 col_bg_active = GetColor(Col_Accent, anim);
        ImU32 col_bg_inactive = GetColor(Col_Border);
        ImU32 col_border = GetColor(Col_TextDim, 0.3f + anim_hov * 0.4f);

        window->DrawList->AddRectFilled(box_bb.Min, box_bb.Max, col_bg_inactive, 5.0f);
        window->DrawList->AddRect(box_bb.Min, box_bb.Max, col_border, 5.0f);

        if (anim > 0.001f) {
            window->DrawList->AddRectFilled(box_bb.Min, box_bb.Max, GetColor(Col_Accent, anim), 5.0f);
            DrawShadow(window->DrawList, box_bb.Min, box_bb.Max, 5.0f, 10.0f, Col_Accent, anim);
            ImVec2 c = box_bb.GetCenter();
            window->DrawList->AddLine(c + ImVec2(-4, -1), c + ImVec2(-1, 3), IM_COL32(255, 255, 255, 255 * anim), 1.5f);
            window->DrawList->AddLine(c + ImVec2(-1, 3), c + ImVec2(4, -4), IM_COL32(255, 255, 255, 255 * anim), 1.5f);
        }
        return pressed;
    }

    bool Slider(const char* label, float* v, float v_min, float v_max, const char* fmt = "%.0f") {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float w = ImGui::GetContentRegionAvail().x - 15.0f;
        float h = 32.0f;
        ImRect bb(pos, pos + ImVec2(w, h));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;

        float slider_w = 120.0f;
        ImVec2 track_start = pos + ImVec2(w - slider_w, 15);
        ImVec2 track_end = pos + ImVec2(w, 15);
        ImRect track_bb(track_start - ImVec2(0, 7), track_end + ImVec2(0, 7));

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(track_bb, id, &hovered, &held);
        if (held) {
            float mx = ImGui::GetIO().MousePos.x;
            float norm = ImClamp((mx - track_start.x) / (track_end.x - track_start.x), 0.0f, 1.0f);
            *v = v_min + norm * (v_max - v_min);
        }

        float anim_hov = GetAnim(id, (hovered || held) ? 1.0f : 0.0f, 15.0f);

        ImVec4 txt_col = ImLerp(Col_TextDim, Col_Text, anim_hov);
        window->DrawList->AddText(pos + ImVec2(0, 5), GetColor(txt_col), label);

        char buf[32]; sprintf(buf, fmt, *v);
        ImVec2 val_sz = ImGui::CalcTextSize(buf);
        window->DrawList->AddText(track_start - ImVec2(val_sz.x + 12, 8), GetColor(Col_Text, 0.9f), buf);

        float norm = (*v - v_min) / (v_max - v_min);
        window->DrawList->AddRectFilled(track_start + ImVec2(0, -2), track_end + ImVec2(0, 2), GetColor(Col_Border), 2.0f);

        ImVec2 fill_end = track_start + ImVec2(norm * slider_w, 0);
        window->DrawList->AddRectFilled(track_start + ImVec2(0, -2), fill_end + ImVec2(0, 2), GetColor(Col_Accent), 2.0f);

        if (held || hovered)
            DrawShadow(window->DrawList, track_start + ImVec2(0, -2), fill_end + ImVec2(0, 2), 2.0f, 6.0f, Col_Accent, anim_hov);

        float knob_r = 5.0f + (1.0f * anim_hov);
        window->DrawList->AddCircleFilled(fill_end, knob_r, IM_COL32(255, 255, 255, 255));
        window->DrawList->AddCircleFilled(fill_end, knob_r + 2.0f, GetColor(Col_Accent, 0.4f));

        return pressed;
    }

    static int s_OpenComboID = 0;
    bool Combo(const char* label, int* current, const char* const items[], int items_count) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float w = ImGui::GetContentRegionAvail().x - 15.0f;
        float h = 30.0f;
        ImRect bb(pos, pos + ImVec2(w, h));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;

        window->DrawList->AddText(pos + ImVec2(0, 6), GetColor(Col_Text), label);

        float box_w = 120.0f;
        ImVec2 box_start = pos + ImVec2(w - box_w, 0);
        ImRect box_bb(box_start, box_start + ImVec2(box_w, h));

        bool hovered = ImGui::IsMouseHoveringRect(box_bb.Min, box_bb.Max);
        bool clicked = hovered && ImGui::IsMouseClicked(0);

        if (clicked) {
            s_OpenComboID = (s_OpenComboID == id) ? 0 : id;
        }

        bool is_open = (s_OpenComboID == id);
        float anim = GetAnim(id, is_open ? 1.0f : 0.0f, 15.0f);

        ImU32 bg_col = GetColor(Col_ComboBg);
        ImU32 border_col = is_open ? GetColor(Col_Accent) : GetColor(Col_Border);

        window->DrawList->AddRectFilled(box_bb.Min, box_bb.Max, bg_col, 5.0f);
        window->DrawList->AddRect(box_bb.Min, box_bb.Max, border_col, 5.0f);

        const char* preview = (items_count > 0 && *current >= 0) ? items[*current] : "Select";
        window->DrawList->AddText(box_bb.Min + ImVec2(10, 7), GetColor(Col_Text), preview);

        if (anim > 0.001f) {
            ImDrawList* fg_dl = ImGui::GetForegroundDrawList();

            float item_h = 26.0f;
            float total_h = items_count * item_h + 8.0f;
            float cur_h = total_h * anim;

            ImVec2 pop_start = box_bb.Max - ImVec2(box_w, 0) + ImVec2(0, 4);
            ImVec2 pop_end = pop_start + ImVec2(box_w, cur_h);

            fg_dl->PushClipRect(pop_start, pop_end, true);

            fg_dl->AddRectFilled(pop_start, pop_end, GetColor(Col_ComboBg), 5.0f);
            fg_dl->AddRect(pop_start, pop_end, GetColor(Col_Border), 5.0f);
            DrawShadow(fg_dl, pop_start, pop_end, 5.0f, 15.0f, ImVec4(0, 0, 0, 0.8f), anim);

            for (int i = 0; i < items_count; i++) {
                ImVec2 item_pos = pop_start + ImVec2(0, 4.0f + i * item_h);
                if (item_pos.y + item_h > pop_end.y) break;

                ImRect item_bb(item_pos, item_pos + ImVec2(box_w, item_h));
                bool item_hovered = ImGui::IsMouseHoveringRect(item_bb.Min, item_bb.Max);
                bool item_selected = (*current == i);

                if (item_hovered)
                    fg_dl->AddRectFilled(item_bb.Min + ImVec2(2, 0), item_bb.Max - ImVec2(2, 0), GetColor(Col_Accent, 0.15f), 4.0f);

                ImU32 txt_col = (item_selected || item_hovered) ? GetColor(Col_Text) : GetColor(Col_TextDim);
                fg_dl->AddText(item_pos + ImVec2(10, 5), txt_col, items[i]);

                if (is_open && item_hovered && ImGui::IsMouseClicked(0)) {
                    *current = i;
                    s_OpenComboID = 0;
                }
            }
            fg_dl->PopClipRect();

            if (is_open && ImGui::IsMouseClicked(0) && !hovered && !ImGui::IsMouseHoveringRect(pop_start, pop_end))
                s_OpenComboID = 0;
        }
        return false;
    }
}



    static bool b_header_gen = true;
    static bool b_aim_shoot = true;
    static bool b_draw_fov = true;
    static float f_fov = 70.0f;

    static bool b_header_exp = true;
    static bool b_enemy_only = true;
    static bool b_magic_bullet = true;
    static float f_aim_h_speed = 20.0f;
    static float f_aim_v_speed = 50.0f;

    static bool b_header_misc = true;
    static int c_body = 0; const char* bodies[] = { "Head", "Chest", "Arms", "Legs", "Feet" };
    static int c_resolver = 0; const char* resolvers[] = { "Default", "Roll", "Inverse" };
    static bool b_aim_misc = true;

    static bool b_header_sel = true;
    static float f_switch_delay = 77.0f;
    static float f_hitscan = 0.36f;
    static bool b_visible = false;
    static bool b_silent_aim = true;
    