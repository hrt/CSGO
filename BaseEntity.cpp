#pragma once

#include "BaseEntity.hpp"
#include "Include.hpp"
#include "GlowObject.hpp"

namespace hazedumper
{
  void BaseEntity::setAimPunch(Vector3& aimPunch)
  {
    memory->write<Vector3>(base + netvars::m_aimPunchAngle + 0xC, aimPunch);
  }

  Vector3 BaseEntity::getBonePosition(uint32_t boneId)
  {
    uint32_t boneBase = memory->read<DWORD>(base + netvars::m_dwBoneMatrix);

    Vector3 bonePosition;
    bonePosition(0) = memory->read<float>(boneBase + 0x30 * boneId + 0x0C);
    bonePosition(1) = memory->read<float>(boneBase + 0x30 * boneId + 0x1C);
    bonePosition(2) = memory->read<float>(boneBase + 0x30 * boneId + 0x2C);

    return bonePosition;
  }

  Vector3 BaseEntity::getAbsolutePosition()
  {
    Vector3 position = getFeetPosition();
    position(2) += memory->read<float>(base + 0x10c);                      // add eye height to Z
    return position;
  }

  Vector3 BaseEntity::getFeetPosition()
  {
    Vector3 position = memory->read<Vector3>(base + netvars::m_vecOrigin);
    return position;
  }

  Vector3 BaseEntity::getAimPunch()
  {
    Vector3 aimPunch = memory->read<Vector3>(base + netvars::m_aimPunchAngle);
    return aimPunch;
  }

  Vector3 BaseEntity::getVelocity()
  {
    Vector3 aimPunch = memory->read<Vector3>(base + netvars::m_vecVelocity);
    return aimPunch;
  }

  bool BaseEntity::getIsDormant()
  {
    return memory->read<bool>(base + 0x000000E9);
  }

  bool BaseEntity::getInReload()
  {
    return memory->read<bool>(base + netvars::m_bInReload);
  }

  bool BaseEntity::getIsDefusing()
  {
    return memory->read<bool>(base + netvars::m_bIsDefusing);
  }

  bool BaseEntity::isOnGround()
  {
    uint32_t flags = getFlags();
    return flags & FL_ONGROUND;
  }

  bool BaseEntity::isInAir()
  {
    uint32_t flags = getFlags();
    return flags == 256 || flags == 258 || flags == 260 || flags == 262;
  }

  bool BaseEntity::getSpottedByMask()
  {
    return memory->read<bool>(base + netvars::m_bSpottedByMask);
  }

  bool BaseEntity::isValidPlayer()
  {
    if (!memory)
      return false;

    int health = getHealth();
    bool isDormant = getIsDormant();

    return health > 0 && health <= 100 && !isDormant;
  }

  uint32_t BaseEntity::getBoneMatrix()
  {
    return memory->read<uint32_t>(base + netvars::m_dwBoneMatrix);
  }

  uint32_t BaseEntity::getFlags()
  {
    return memory->read<uint32_t>(base + netvars::m_fFlags);
  }

  float BaseEntity::getFlashDuration()
  {
    return memory->read<float>(base + netvars::m_flFlashDuration);
  }

  float BaseEntity::getFlashMaxAlpha()
  {
    return memory->read<float>(base + netvars::m_flFlashMaxAlpha);
  }

  float BaseEntity::getNextPrimaryAttack()
  {
    return memory->read<float>(base + netvars::m_flFlashMaxAlpha);
  }

  uint32_t BaseEntity::getClip1()
  {
    return memory->read<uint32_t>(base + netvars::m_iClip1);
  }

  uint32_t BaseEntity::getCrosshairId()
  {
    return memory->read<uint32_t>(base + netvars::m_iCrosshairId);
  }

  uint32_t BaseEntity::getGlowIndex()
  {
    return memory->read<uint32_t>(base + netvars::m_iGlowIndex);
  }


  uint32_t BaseEntity::getHealth()
  {
    return memory->read<uint32_t>(base + netvars::m_iHealth);
  }

  uint32_t BaseEntity::getShotsFired()
  {
    return memory->read<uint32_t>(base + netvars::m_iShotsFired);
  }

  uint32_t BaseEntity::getState()
  {
    return memory->read<uint32_t>(base + netvars::m_iState);
  }

  uint32_t BaseEntity::getTeamNum()
  {
    return memory->read<uint32_t>(base + netvars::m_iTeamNum);
  }

  uint32_t BaseEntity::getLifeState()
  {
    return memory->read<uint32_t>(base + netvars::m_lifeState);
  }

  uint32_t BaseEntity::getGlowEntity()
  {
    uint32_t glowObjectManager = memory->read<uint32_t>(memory->clientDll + signatures::dwGlowObjectManager);
    return glowObjectManager + getGlowIndex() * sizeof(GlowObject);
  }

  void BaseEntity::setFlashDuration(float duration)
  {
    memory->write<float>(base + netvars::m_flFlashDuration, duration);
  }

  void BaseEntity::setForceJump(uint32_t value)
  {
    memory->write<uint32_t>(memory->clientDll + signatures::dwForceJump, value);
  }

  uint32_t BaseEntity::getForceAttack()
  {
    return memory->read<uint32_t>(memory->clientDll + signatures::dwForceAttack);
  }

  uint32_t BaseEntity::getForceForward()
  {
    return memory->read<uint32_t>(memory->clientDll + signatures::dwForceForward);
  }

  uint32_t BaseEntity::getId()
  {
    return memory->read<uint32_t>(base + netvars::m_iAccountID);
  }

  uint32_t BaseEntity::getBase()
  {
    return base;
  }


  void BaseEntity::setForceAttack(uint32_t value)
  {
    memory->write<uint32_t>(memory->clientDll + signatures::dwForceAttack, value);
  }

  void BaseEntity::setForceAttack2(uint32_t value)
  {
    memory->write<uint32_t>(memory->clientDll + signatures::dwForceAttack2, value);
  }

  void BaseEntity::setForceForward(uint32_t value)
  {
    memory->write<uint32_t>(memory->clientDll + signatures::dwForceForward, value);
  }

  void BaseEntity::setSpottedByMask(bool b)
  {
    memory->write<bool>(base + netvars::m_bSpottedByMask, b);
  }

  void BaseEntity::shoot()
  {
    uint32_t forceAttack = getForceAttack();
    if (forceAttack == 4)
      setForceAttack(5);
    else
      setForceAttack(4);
  }

  BaseEntity::BaseEntity(Memory* memory, uint32_t base)
  {
    this->memory = memory;
    this->base = base;
  }

  BaseEntity::BaseEntity()
  {
  }

  BaseEntity::~BaseEntity()
  {

  }

}
