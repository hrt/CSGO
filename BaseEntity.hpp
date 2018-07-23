#pragma once

#include "Include.hpp"

namespace hazedumper
{
  class BaseEntity
  {
  private:
    uint32_t base;
    Memory* memory = NULL;
  public:
    Vector3 getBonePosition(uint32_t boneId);
    Vector3 getAbsolutePosition();
    Vector3 getFeetPosition();
    Vector3 getAimPunch();
    Vector3 getVelocity();
    bool getIsDormant();
    bool getInReload();
    bool getIsDefusing();
    bool isValidPlayer();
    bool isOnGround();
    bool isInAir();
    bool getSpottedByMask();
    uint32_t getBoneMatrix();
    uint32_t getFlags();
    float getFlashDuration();
    float getFlashMaxAlpha();
    float getNextPrimaryAttack();
    uint32_t getClip1();
    uint32_t getCrosshairId();
    uint32_t getGlowIndex();
    uint32_t getHealth();
    uint32_t getShotsFired();
    uint32_t getState();
    uint32_t getTeamNum();
    uint32_t getLifeState();
    uint32_t getGlowEntity();
    uint32_t getForceAttack();
    uint32_t getForceForward();
    uint32_t getId();
    uint32_t getBase();
    void setFlashDuration(float duration);
    void setForceJump(uint32_t value);
    void setForceAttack(uint32_t value);
    void setForceAttack2(uint32_t value);
    void setForceForward(uint32_t value);
    void setSpottedByMask(bool b);
    void shoot();
    void setAimPunch(Vector3& aimPunch);
    BaseEntity();
    BaseEntity(Memory* memory, uint32_t base);
    ~BaseEntity();
  };
}
