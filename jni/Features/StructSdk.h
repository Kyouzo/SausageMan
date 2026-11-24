#pragma once

#include <cstdint>

uintptr_t Class_Transform_get_position;
uintptr_t Class_Component_get_transform;
uintptr_t Class_Camera_get_main;
uintptr_t Class_Camera_WorldToScreenPoint;
uintptr_t Class_Animator_GetBoneTransform;
uintptr_t Class_BattleRoleLogic_BattleRole;
uintptr_t Class_RoleControl_SetRota;


template <typename Ret, typename... Args>
inline Ret call_fn(uintptr_t fn_ptr, Args... args)
{
  return reinterpret_cast<Ret (*)(Args...)>(fn_ptr)(args...);
}
