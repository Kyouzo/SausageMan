#pragma once
bool g_Initialized, clearMousePos = true;

uintptr_t Input_GetTouch = 0;
uintptr_t Input_get_touchSupported = 0;
uintptr_t Input_get_touchCount = 0;
uintptr_t Input_get_mousePosition = 0;
uintptr_t Screen_get_width = 0;
uintptr_t Screen_get_height = 0;
uintptr_t Screen_get_dpi = 0;

enum TouchPhase : int
{
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4,
};

struct UnityEngine_Touch_Fields
{
    int32_t m_FingerId;
    Vector2 m_Position;
    Vector2 m_RawPosition;
    Vector2 m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

bool get_touchSupported(void *instance)
{
    return reinterpret_cast<bool(__fastcall *)(void *)>(Input_get_touchSupported)(instance);
}

int get_touchCount()
{
    return reinterpret_cast<int(__fastcall *)()>(Input_get_touchCount)();
}

Vector3 get_mousePosition(void *instance)
{
    return reinterpret_cast<Vector3(__fastcall *)(void *)>(Input_get_mousePosition)(instance);
}

int get_width()
{
    return reinterpret_cast<int(__fastcall *)()>(Screen_get_width)();
}

int get_height()
{
    return reinterpret_cast<int(__fastcall *)()>(Screen_get_height)();
}

int get_dpi()
{
    return reinterpret_cast<int(__fastcall *)()>(Screen_get_dpi)();
}

static bool s_TouchActive = false;
static int s_LastFingerId = -1;

void ImGui_GetTouch(ImGuiIO *io)
{
    if (!get_touchSupported(nullptr))
    {
        return;
    }

    int touchCount = get_touchCount();
    int screenHeight = get_height();

    bool anyTouchActive = false;

    for (int i = 0; i < touchCount; i++)
    {
        if (Input_GetTouch != 0)
        {
            UnityEngine_Touch_Fields touch = reinterpret_cast<UnityEngine_Touch_Fields (*)(int)>(Input_GetTouch)(i);

            if (s_LastFingerId == -1 && (touch.m_Phase == TouchPhase::Began || touch.m_Phase == TouchPhase::Moved || touch.m_Phase == TouchPhase::Stationary))
            {
                s_LastFingerId = touch.m_FingerId;
            }

            if (touch.m_FingerId == s_LastFingerId || s_LastFingerId == -1)
            {
                switch (touch.m_Phase)
                {
                case TouchPhase::Began:
                case TouchPhase::Stationary:
                case TouchPhase::Moved:
                    io->MouseDown[0] = true;
                    io->MousePos = ImVec2(touch.m_Position.x, screenHeight - touch.m_Position.y);
                    anyTouchActive = true;
                    s_TouchActive = true;
                    break;

                case TouchPhase::Ended:
                case TouchPhase::Canceled:
                    io->MouseDown[0] = false;
                    clearMousePos = true;
                    s_LastFingerId = -1;
                    s_TouchActive = false;
                    return;

                default:
                    break;
                }
            }
        }
    }

    if (!anyTouchActive && s_TouchActive)
    {
        io->MouseDown[0] = false;
        clearMousePos = true;
        s_TouchActive = false;
        s_LastFingerId = -1;
    }
}

void ImGui_GetTouch2(ImGuiIO *io, int screenHeight)
{
    void *instance;
    if (get_touchCount() > 0)
    {
        if (get_touchSupported(instance))
        {
            UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(void *, int))(Input_GetTouch))(instance, 0);
            switch (touch.m_Phase)
            {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io->MouseDown[0] = true;
                ImGui::GetIO().MousePos = ImVec2(get_mousePosition(instance).x, screenHeight - get_mousePosition(instance).y);
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io->MouseDown[0] = false;
                clearMousePos = true;
                break;
            case TouchPhase::Moved:
                ImGui::GetIO().MousePos = ImVec2(get_mousePosition(instance).x, screenHeight - get_mousePosition(instance).y);
                break;
            default:
                break;
            }
        }
    }
}