#pragma once
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include "Il2cpp/Il2cpp.h"
#include "Il2cpp/il2cpp-class.h"
class RoleControl;
class Animator;
class BattleRoleLogic;
class RoleAILogic;

static inline bool ValidPtr(void *p)
{
    return p != nullptr && (uintptr_t)p > 0x10000 && Tools::IsPtrValid(p);
}

template <typename T>
static inline T SafeRead(T *ptr, T defaultVal = (T)0)
{
    if (!ptr)
        return defaultVal;
    if (!ValidPtr((void *)ptr))
        return defaultVal;
    try
    {
        return *ptr;
    }
    catch (...)
    {
        return defaultVal;
    }
}

enum class HumanBodyBones : int
{
    Hips = 0,
    LeftUpperLeg = 1,
    RightUpperLeg = 2,
    LeftLowerLeg = 3,
    RightLowerLeg = 4,
    LeftFoot = 5,
    RightFoot = 6,
    Spine = 7,
    Chest = 8,
    UpperChest = 54,
    Neck = 9,
    Head = 10,
    LeftShoulder = 11,
    RightShoulder = 12,
    LeftUpperArm = 13,
    RightUpperArm = 14,
    LeftLowerArm = 15,
    RightLowerArm = 16,
    LeftHand = 17,
    RightHand = 18,
    LeftToes = 19,
    RightToes = 20,
    LeftEye = 21,
    RightEye = 22,
    Jaw = 23,
    LeftThumbProximal = 24,
    LeftThumbIntermediate = 25,
    LeftThumbDistal = 26,
    LeftIndexProximal = 27,
    LeftIndexIntermediate = 28,
    LeftIndexDistal = 29,
    LeftMiddleProximal = 30,
    LeftMiddleIntermediate = 31,
    LeftMiddleDistal = 32,
    LeftRingProximal = 33,
    LeftRingIntermediate = 34,
    LeftRingDistal = 35,
    LeftLittleProximal = 36,
    LeftLittleIntermediate = 37,
    LeftLittleDistal = 38,
    RightThumbProximal = 39,
    RightThumbIntermediate = 40,
    RightThumbDistal = 41,
    RightIndexProximal = 42,
    RightIndexIntermediate = 43,
    RightIndexDistal = 44,
    RightMiddleProximal = 45,
    RightMiddleIntermediate = 46,
    RightMiddleDistal = 47,
    RightRingProximal = 48,
    RightRingIntermediate = 49,
    RightRingDistal = 50,
    RightLittleProximal = 51,
    RightLittleIntermediate = 52,
    RightLittleDistal = 53,
    LastBone = 55
};

class Transform
{
public:
    Vector3 get_position()
    {
        return call_fn<Vector3>(Class_Transform_get_position, this);
    }
};

class Component
{
public:
    Transform *get_transform()
    {
        return call_fn<Transform *>(Class_Component_get_transform, this);
    }
};

class Camera : public Component
{
public:
    static Camera *get_main()
    {
        return call_fn<Camera *>(Class_Camera_get_main);
    }

    Vector3 WorldToScreenPoint(Vector3 position)
    {
        return call_fn<Vector3>(Class_Camera_WorldToScreenPoint, this, position);
    }
};

class ClientRoleAIAnimator : public Il2CppObject
{
public:
    Animator *anim()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "ClientRoleAIAnimator");
            if (klass)
                field = klass->getField("anim");
        }
        return field ? field->getValue<Animator *>(this) : nullptr;
    }
};

class ClientRoleAILogic : public Il2CppObject
{
public:
    ClientRoleAIAnimator *animator()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "ClientRoleAILogic");
            if (klass)
                field = klass->getField("animator");
        }
        return field ? field->getValue<ClientRoleAIAnimator *>(this) : nullptr;
    }
};

class RoleAILogic : public Il2CppObject
{
public:
    ClientRoleAILogic *ClientLogic()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleAILogic");
            if (klass)
                field = klass->getField("<ClientLogic>k__BackingField");
        }
        return field ? field->getValue<ClientRoleAILogic *>(this) : nullptr;
    }

    Il2CppString *GetName()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleAILogic");
            if (klass)
                field = klass->getField("<Name>k__BackingField");
        }
        return field ? field->getValue<Il2CppString *>(this) : nullptr;
    }

    float GetHealth()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleAILogic");
            if (klass)
                field = klass->getField("<Hp>k__BackingField");
        }
        return field ? field->getValue<float>(this) : 0.f;
    }

    float GetMaxHealth()
    {
        return 100.0f;
    }

    Vector3 GetPosition()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleAILogic");
            if (klass)
                field = klass->getField("<Position>k__BackingField");
        }
        return field ? field->getValue<Vector3>(this) : Vector3{0, 0, 0};
    }
};

class RoleAIManager : public Il2CppObject
{
public:
    List<RoleAILogic *> *logicList()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleAIManager");
            if (klass)
                field = klass->getField("logicList");
        }
        return field ? field->getValue<List<RoleAILogic *> *>(this) : nullptr;
    }
};

class RoleNetClient : public Il2CppObject
{
public:
    bool isLocalPlayer()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleNetClient");
            if (klass)
                field = klass->getField("isLocalPlayer");
        }
        return field ? field->getValue<bool>(this) : false;
    }
};

class RoleNet : public Il2CppObject
{
public:
    RoleNetClient *GetRoleNetClient()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleNet");
            if (klass)
                field = klass->getField("roleNetClient");
        }
        return field ? field->getValue<RoleNetClient *>(this) : nullptr;
    }
};

void Offset()
{
    auto BattleRoleLogicClass = Il2cpp::GetImage("Assembly-CSharp.dll")->getClass("BattleRoleLogic");
    if (BattleRoleLogicClass)
    {
        auto BattleRoleMethod = BattleRoleLogicClass->getMethod("$VB", 0);
        if (BattleRoleMethod)
        {
            Class_BattleRoleLogic_BattleRole = BattleRoleMethod->getAbsAddress();
            LOGI("BattleRoleLogic.BattleRole found at: 0x%lX", Class_BattleRoleLogic_BattleRole);
        }
        else
        {
            LOGI("Failed to find BattleRoleLogic.BattleRole address");
        }
    }

    auto AnimatorClass = Il2cpp::GetImage("UnityEngine.AnimationModule.dll")->getClass("UnityEngine.Animator");
    if (AnimatorClass)
    {
        auto GetBoneTransformMethod = AnimatorClass->getMethod("GetBoneTransform", 1);
        if (GetBoneTransformMethod)
        {
            Class_Animator_GetBoneTransform = GetBoneTransformMethod->getAbsAddress();
            LOGI("Animator.GetBoneTransform found at: 0x%lX", Class_Animator_GetBoneTransform);
        }
        else
        {
            LOGI("Failed to find Animator.GetBoneTransform address");
        }
    }
}
class Animator : public Il2CppObject
{
public:
    Transform *GetBoneTransform(HumanBodyBones bone)
    {
        return call_fn<Transform *>(Class_Animator_GetBoneTransform, this, (int)bone);
    }
};

class WeaponControl : public Il2CppObject
{
public:
};

class AnimatorControl : public Il2CppObject
{
public:
    WeaponControl *weaponControl()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "AnimatorControl");
            if (klass)
                field = klass->getField("weaponControl");
        }
        return field ? field->getValue<WeaponControl *>(this) : nullptr;
    }
    Animator *animator()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "AnimatorControl");
            if (klass)
                field = klass->getField("animator");
        }
        return field ? field->getValue<Animator *>(this) : nullptr;
    }
};

class RoleControl : public Il2CppObject
{
public:
    AnimatorControl *animatorControl()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "RoleControl");
            if (klass)
                field = klass->getField("animatorControl");
        }
        return field ? field->getValue<AnimatorControl *>(this) : nullptr;
    }
};

class BattleRole : public Il2CppObject
{
public:
    RoleControl *MyRoleControl()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRole");
            if (klass)
                field = klass->getField("MyRoleControl");
        }
        return field ? field->getValue<RoleControl *>(this) : nullptr;
    }
};

class BattleRoleLogic : public Il2CppObject
{
public:
    Vector3 GetPosition()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("$q");
        }
        return field ? field->getValue<Vector3>(this) : Vector3{0, 0, 0};
    }

    Il2CppString *GetName()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("NickName");
        }
        return field ? field->getValue<Il2CppString *>(this) : nullptr;
    }

    long GetTeam()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("TeamNum");
        }
        return field ? field->getValue<long>(this) : 0;
    }

    float GetHealth()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("$W");
        }
        return field ? field->getValue<float>(this) : 0.f;
    }

    float GetMaxHealth()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("$x");
        }
        return field ? field->getValue<float>(this) : 100.f;
    }

    RoleNet *GetRoleNet()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleRoleLogic");
            if (klass)
                field = klass->getField("$m");
        }
        return field ? field->getValue<RoleNet *>(this) : nullptr;
    }

    BattleRole *GetBattleRole()
    {
        return call_fn<BattleRole *>(Class_BattleRoleLogic_BattleRole, this);
    }
};

class StartGame : public Il2CppObject
{
public:
    List<BattleRoleLogic *> *realplayerlist()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "StartGame");
            if (klass)
                field = klass->getField("RoleList");
        }
        return field ? field->getValue<List<BattleRoleLogic *> *>(this) : nullptr;
    }
};

class BattleWorld : public Il2CppObject
{
public:
    RoleAIManager *roleaimanager()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleWorld");
            if (klass)
                field = klass->getField("RoleAIManager");
        }
        return field ? field->getValue<RoleAIManager *>(this) : nullptr;
    }

    StartGame *startGame()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "BattleWorld");
            if (klass)
                field = klass->getField("startGame");
        }
        return field ? field->getValue<StartGame *>(this) : nullptr;
    }
};

class GameWorldList : public Il2CppObject
{
public:
    List<BattleWorld *> *Battleworld()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "GameWorldList");
            if (klass)
                field = klass->getField("worlds");
        }
        return field ? field->getValue<List<BattleWorld *> *>(this) : nullptr;
    }
};

class GameWorldManager : public Il2CppObject
{
public:
    static GameWorldManager *GetInstance()
    {
        auto assemblyImage = Il2cpp::GetImage("Assembly-CSharp.dll");
        if (!assemblyImage)
        {
            LOGD("Failed to get Assembly-CSharp.dll image");
            return nullptr;
        }

        auto gameWorldClass = assemblyImage->getClass("GameWorldManager");
        if (!gameWorldClass)
        {
            LOGD("Failed to find GameWorldManager class");
            return nullptr;
        }

        auto instanceField = gameWorldClass->getField("instance");
        if (!instanceField)
        {
            LOGD("Failed to find instance field in GameWorldManager");
            return nullptr;
        }

        GameWorldManager *instance = instanceField->getStaticValue<GameWorldManager *>();

        if (!instance || !ValidPtr(instance))
        {
            LOGD("Invalid GameWorldManager instance");
            return nullptr;
        }

        return instance;
    }

    GameWorldList *GetWorldList()
    {
        static FieldInfo *field = nullptr;
        if (!field)
        {
            auto klass = Il2cpp::GetClass(Il2cpp::GetImage("Assembly-CSharp.dll"), "GameWorldManager");
            if (klass)
                field = klass->getField("worldList");
        }
        return field ? field->getValue<GameWorldList *>(this) : nullptr;
    }
};

static BattleWorld *GetFirstBattleWorld()
{
    auto worldManager = GameWorldManager::GetInstance();
    if (!worldManager)
        return nullptr;

    auto worldList = worldManager->GetWorldList();
    if (!worldList)
        return nullptr;

    auto battleWorlds = worldList->Battleworld();
    if (!battleWorlds || battleWorlds->size() <= 0)
        return nullptr;

    return battleWorlds->get(0);
}

Vector3 GetBonePosition(Animator *animator, HumanBodyBones bone)
{
    if (!ValidPtr(animator))
        return Vector3{0, 0, 0};

    Transform *boneTransform = animator->GetBoneTransform(bone);
    if (!ValidPtr(boneTransform))
        return Vector3{0, 0, 0};

    return boneTransform->get_position();
}

static void DrawBone(ImDrawList *draw, Camera *cam, Animator *animator)
{
    if (!ValidPtr(animator))
        return;

    Vector3 head = GetBonePosition(animator, HumanBodyBones::Head);
    Vector3 chest = GetBonePosition(animator, HumanBodyBones::Chest);
    Vector3 hips = GetBonePosition(animator, HumanBodyBones::Hips);

    Vector3 lUpperArm = GetBonePosition(animator, HumanBodyBones::LeftUpperArm);
    Vector3 lLowerArm = GetBonePosition(animator, HumanBodyBones::LeftLowerArm);
    Vector3 lHand = GetBonePosition(animator, HumanBodyBones::LeftHand);

    Vector3 rUpperArm = GetBonePosition(animator, HumanBodyBones::RightUpperArm);
    Vector3 rLowerArm = GetBonePosition(animator, HumanBodyBones::RightLowerArm);
    Vector3 rHand = GetBonePosition(animator, HumanBodyBones::RightHand);

    Vector3 lUpperLeg = GetBonePosition(animator, HumanBodyBones::LeftUpperLeg);
    Vector3 lLowerLeg = GetBonePosition(animator, HumanBodyBones::LeftLowerLeg);
    Vector3 lFoot = GetBonePosition(animator, HumanBodyBones::LeftFoot);

    Vector3 rUpperLeg = GetBonePosition(animator, HumanBodyBones::RightUpperLeg);
    Vector3 rLowerLeg = GetBonePosition(animator, HumanBodyBones::RightLowerLeg);
    Vector3 rFoot = GetBonePosition(animator, HumanBodyBones::RightFoot);

    auto W2S = [&](Vector3 p)
    {
        Vector3 s = cam->WorldToScreenPoint(p);
        if (s.z <= 0.1f)
            return ImVec2(-9999, -9999);

        s.y = sHeight - s.y;
        return ImVec2(s.x, s.y);
    };

    ImU32 col = IM_COL32(255, 255, 255, 255);

    draw->AddLine(W2S(head), W2S(chest), col, 2);
    draw->AddLine(W2S(chest), W2S(hips), col, 2);

    draw->AddLine(W2S(chest), W2S(lUpperArm), col, 2);
    draw->AddLine(W2S(lUpperArm), W2S(lLowerArm), col, 2);
    draw->AddLine(W2S(lLowerArm), W2S(lHand), col, 2);

    draw->AddLine(W2S(chest), W2S(rUpperArm), col, 2);
    draw->AddLine(W2S(rUpperArm), W2S(rLowerArm), col, 2);
    draw->AddLine(W2S(rLowerArm), W2S(rHand), col, 2);

    draw->AddLine(W2S(hips), W2S(lUpperLeg), col, 2);
    draw->AddLine(W2S(lUpperLeg), W2S(lLowerLeg), col, 2);
    draw->AddLine(W2S(lLowerLeg), W2S(lFoot), col, 2);

    draw->AddLine(W2S(hips), W2S(rUpperLeg), col, 2);
    draw->AddLine(W2S(rUpperLeg), W2S(rLowerLeg), col, 2);
    draw->AddLine(W2S(rLowerLeg), W2S(rFoot), col, 2);
}

static bool WorldToScreenBone(Camera *cam, Vector3 pos, ImVec2 &out)
{
    Vector3 s = cam->WorldToScreenPoint(pos);
    if (s.z <= 0.1f)
        return false;

    s.y = sHeight - s.y;
    out = ImVec2(s.x, s.y);
    return true;
}

static bool GetBoxFromBones(Camera *cam, Animator *animator, ImVec2 &top, ImVec2 &bottom, float &width)
{
    Vector3 head = GetBonePosition(animator, HumanBodyBones::Head);

    Vector3 leftLeg = GetBonePosition(animator, HumanBodyBones::LeftLowerLeg);
    Vector3 rightLeg = GetBonePosition(animator, HumanBodyBones::RightLowerLeg);

    Vector3 root;
    root.x = (leftLeg.x + rightLeg.x) * 0.5f;
    root.y = (leftLeg.y + rightLeg.y) * 0.5f;
    root.z = (leftLeg.z + rightLeg.z) * 0.5f;

    head.y += 0.20f;

    ImVec2 head2D, root2D;

    if (!WorldToScreenBone(cam, head, head2D))
        return false;

    if (!WorldToScreenBone(cam, root, root2D))
        return false;

    float height = fabs(head2D.y - root2D.y);

    width = height * 0.45f;

    top = ImVec2(head2D.x - width / 2, head2D.y);
    bottom = ImVec2(head2D.x + width / 2, root2D.y);

    return true;
}

static void EspBots(ImDrawList *draw, Camera *cam, BattleWorld *world, int &espCount)
{
    auto roleManager = world->roleaimanager();
    if (!roleManager)
        return;

    auto players = roleManager->logicList();
    if (!players)
        return;

    int count = players->size();
    char buffer[128];

    for (int i = 0; i < count; i++)
    {
        RoleAILogic *bot = players->get(i);
        if (!ValidPtr(bot))
            continue;

        auto clientLogic = bot->ClientLogic();
        if (!ValidPtr(clientLogic))
            continue;

        auto aiAnimator = clientLogic->animator();
        if (!ValidPtr(aiAnimator))
            continue;

        Animator *animator = aiAnimator->anim();
        if (!ValidPtr(animator))
            continue;

        ImVec2 boxTop, boxBottom;
        float width;

        if (!GetBoxFromBones(cam, animator, boxTop, boxBottom, width))
            continue;

        if (var.EspBox)
            draw->AddRect(boxTop, boxBottom, IM_COL32(0, 255, 0, 255), 0, 0, 2);

        if (var.EspBone)
        {
            DrawBone(draw, cam, animator);
        }

        if (var.EspLine)
        {
            draw->AddLine(
                ImVec2(sWidth / 2, sHeight),
                ImVec2((boxTop.x + boxBottom.x) / 2, boxBottom.y),
                IM_COL32(255, 0, 0, 255),
                1);
        }
        if (var.EspName)
        {
            auto name = bot->GetName();
            if (ValidPtr(name))
            {
                std::string nameStr = name->to_string();
                sprintf(buffer, "%s", nameStr.c_str());

                draw->AddText(
                    ImVec2(boxTop.x, boxTop.y - 15),
                    IM_COL32(255, 255, 255, 255),
                    buffer);
            }
        }

        if (var.EspHealth)
        {
            float hp = bot->GetHealth();
            float hpPercent = hp / 100.0f;

            if (hpPercent < 0)
                hpPercent = 0;
            if (hpPercent > 1)
                hpPercent = 1;

            float boxHeight = boxBottom.y - boxTop.y;
            float hpHeight = boxHeight * hpPercent;

            draw->AddRectFilled(
                ImVec2(boxTop.x - 6, boxBottom.y - hpHeight),
                ImVec2(boxTop.x - 2, boxBottom.y),
                IM_COL32(0, 255, 0, 255));
        }

        espCount++;
    }
}

static void EspRealPlayers(ImDrawList *draw, Camera *cam, BattleWorld *world, int &espCount)
{
    auto startGame = world->startGame();
    if (!startGame)
        return;

    auto players = startGame->realplayerlist();
    if (!players)
        return;

    int count = players->size();
    char buffer[128];

    for (int i = 0; i < count; i++)
    {
        BattleRoleLogic *player = players->get(i);
        if (!ValidPtr(player))
            continue;

        auto roleNet = player->GetRoleNet();
        if (ValidPtr(roleNet))
        {
            auto client = roleNet->GetRoleNetClient();
            if (ValidPtr(client) && client->isLocalPlayer())
                continue;
        }

        auto battleRole = player->GetBattleRole();
        if (!ValidPtr(battleRole))
            continue;

        auto roleControl = battleRole->MyRoleControl();
        if (!ValidPtr(roleControl))
            continue;

        auto animControl = roleControl->animatorControl();
        if (!ValidPtr(animControl))
            continue;

        Animator *animator = animControl->animator();
        if (!ValidPtr(animator))
            continue;

        ImVec2 boxTop, boxBottom;
        float width;

        if (!GetBoxFromBones(cam, animator, boxTop, boxBottom, width))
            continue;

        if (var.EspBox)
            draw->AddRect(boxTop, boxBottom, IM_COL32(0, 150, 255, 255), 0, 0, 2);

        if (var.EspBone)
        {
            DrawBone(draw, cam, animator);
        }

        if (var.EspLine)
        {
            draw->AddLine(
                ImVec2(sWidth / 2, sHeight),
                ImVec2((boxTop.x + boxBottom.x) / 2, boxBottom.y),
                IM_COL32(0, 150, 255, 255),
                1);
        }
        if (var.EspHealth)
        {
            float hp = player->GetHealth();
            float maxHp = player->GetMaxHealth();
            float hpRatio = maxHp > 0 ? hp / maxHp : 0;

            float boxHeight = boxBottom.y - boxTop.y;

            draw->AddRectFilled(
                ImVec2(boxTop.x - 6, boxBottom.y - (boxHeight * hpRatio)),
                ImVec2(boxTop.x - 2, boxBottom.y),
                IM_COL32(0, 255, 0, 255));
        }

        if (var.EspName)
        {
            auto name = player->GetName();
            if (!ValidPtr(name))
                continue;

            std::string nameStr = name->to_string();
            const char *nick = nameStr.empty() ? "Player" : nameStr.c_str();

            sprintf(buffer, "%s [T:%ld]", nick, player->GetTeam());

            draw->AddText(
                ImVec2(boxTop.x, boxTop.y - 15),
                IM_COL32(255, 255, 255, 255),
                buffer);
        }

        espCount++;
    }
}

static void EspCall(ImDrawList *draw)
{
    if (!started || !var.isEspEnabled)
        return;

    auto firstBattleWorld = GetFirstBattleWorld();
    if (!firstBattleWorld)
        return;

    Camera *cam = Camera::get_main();
    if (!ValidPtr(cam))
        return;

    int espCount = 0;
    EspBots(draw, cam, firstBattleWorld, espCount);
    EspRealPlayers(draw, cam, firstBattleWorld, espCount);

    if (var.EspCount)
    {
        char buffer[128];
        sprintf(buffer, "ESP Count: %d", espCount);
        ImVec2 textSize = ImGui::CalcTextSize(buffer);
        draw->AddText(ImVec2((sWidth - textSize.x) / 2, 20), IM_COL32(255, 255, 0, 255), buffer);
    }
}

static void Azxie(ImDrawList *draw)
{

    if (!started || !var.isEspEnabled)
        return;

    EspCall(draw);
}