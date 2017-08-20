#include "Hacks.hpp"
#include "BaseEntity.hpp"
#include "Client.hpp"
#include "GlowObject.hpp"
#include <chrono>
#include <thread>

namespace hazedumper
{
  Vector3 Hacks::aimAnglesTo(BaseEntity& localPlayer, Vector3& target)
  {
    Vector3 localPosition = localPlayer.getAbsolutePosition();

    Vector3 punchAngles = localPlayer.getAimPunch();

    Vector3 dPosition = localPosition - target;

    double hypotenuse = sqrt(dPosition(0)*dPosition(0) + dPosition(1)*dPosition(1));

    Vector3 a((float)(atan2f(dPosition(2), hypotenuse) * 57.295779513082f), (float)(atanf(dPosition(1) / dPosition(0)) * 57.295779513082f), 0);

    if (dPosition(0) >= 0.f)
      a(1) += 180.0f;

    Vector3 aimAngles;
    aimAngles(0) = a(0);     // up and down
    aimAngles(1) = a(1);      // left and right

    aimAngles(0) -= punchAngles(0) * 2;
    aimAngles(1) -= punchAngles(1) * 2;

    normalizeAngles(aimAngles);
    clampAngles(aimAngles);

    aimAngles(2) = 0.f;
    return aimAngles;
  }

  Vector3 Hacks::angleDifferenceToEntity(BaseEntity& localPlayer, BaseEntity& entity, uint32_t boneId)
  {
    Vector3 viewAngles = client->getViewAngles();

    Vector3 aimAngles = aimAnglesTo(localPlayer, entity.getBonePosition(boneId));

    Vector3 dAngle(-1, -1, 0);

    if (aimAngles(0) != aimAngles(0) || aimAngles(1) != aimAngles(1))
      return dAngle;


    dAngle(0) = abs(aimAngles(0) - viewAngles(0));
    dAngle(1) = abs(aimAngles(1) - viewAngles(1));

    return dAngle;
  }

  BaseEntity Hacks::findClosestEnemyToFOV(float fov, uint32_t boneId)
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    uint32_t localPlayerTeam = localPlayer.getTeamNum();
    BaseEntity closestPlayer;

    if (!localPlayer.isValidPlayer())
      return closestPlayer;

    Vector3 localPosition = localPlayer.getAbsolutePosition();

    float closest = 999999999.f;
    for (int i = 0; i < 64; i++)
    {
      BaseEntity entity(memory, memory->read<uint32_t>(memory->clientDll + signatures::dwEntityList + 0x10 * i));

      if (!entity.isValidPlayer())
        continue;

      if (entity.getTeamNum() == localPlayerTeam)
        continue;

      Vector3 entityPosition = entity.getBonePosition(boneId);

      if (!bspParser->is_visible(localPosition, entityPosition))
        continue;

      Vector3 dAngle = angleDifferenceToEntity(localPlayer, entity, boneId);
      float screenDifferenceToEntity = sqrt(dAngle(0)*dAngle(0) + dAngle(1)*dAngle(1));
      if (screenDifferenceToEntity >= closest )
        continue;

      if (screenDifferenceToEntity >= fov)
        continue;

      closest = screenDifferenceToEntity;
      closestPlayer = entity;
    }

    return closestPlayer;
  }

  void Hacks::glowESP()
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    uint32_t localPlayerTeam = localPlayer.getTeamNum();

    for (int i = 0; i < 64; i++)
    {
      BaseEntity player(memory, memory->read<uint32_t>(memory->clientDll + signatures::dwEntityList + 0x10 * i));

      if (!player.isValidPlayer())
        continue;

      if (player.getTeamNum() == localPlayerTeam)
        continue;

      float healthPercent = player.getHealth() / 100.f;

      uint32_t glowEntity = player.getGlowEntity();
      memory->write<float>(glowEntity + offsetof(GlowObject, alpha), 0.6f);
      memory->write<float>(glowEntity + offsetof(GlowObject, red), 1.f);
      memory->write<float>(glowEntity + offsetof(GlowObject, green), healthPercent);
      memory->write<float>(glowEntity + offsetof(GlowObject, blue), healthPercent);
      memory->write<bool>(glowEntity + offsetof(GlowObject, renderWhenOccluded), true);
      memory->write<bool>(glowEntity + offsetof(GlowObject, fullBloomRender), player.getIsDefusing());
    }
  }

  void Hacks::noFlash()
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    if (localPlayer.isValidPlayer() && localPlayer.getFlashDuration() > 0.f)
      localPlayer.setFlashDuration(0.f);
  }

  void Hacks::bunnyHop()
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    if (!localPlayer.isValidPlayer())
      return;

    if (!localPlayer.isInAir())
    {
      localPlayer.setForceJump(5);
    }
    else
    {
      localPlayer.setForceJump(4);
      localPlayer.setForceJump(5);
      localPlayer.setForceJump(4);
    }
  }

  bool Hacks::aimAssist(float fov, int boneId)
  {
    const char* gameDirectory = client->getGameDirectory();
    const char* mapDirectory = client->getMapDirectory();
    bspParser->parse_map(gameDirectory, mapDirectory);

    BaseEntity localPlayer = client->getLocalPlayer();
    if (!localPlayer.isValidPlayer())
      return false;


    if (localPlayer.getShotsFired() < 3)  // aimbot begins at 3rd bullet
      return false;

    static BaseEntity target = findClosestEnemyToFOV(fov, boneId);
    static auto killTime = std::chrono::high_resolution_clock::now();                     
    static Vector3* lastPosition = NULL;

    BaseEntity newTarget = findClosestEnemyToFOV(fov, boneId);
    if (target.isValidPlayer() && !target.isInAir() && newTarget.isValidPlayer() && target.getBase() == newTarget.getBase())
    {
      killTime = std::chrono::high_resolution_clock::now();
      lastPosition = new Vector3();

      // body aim if low
      if (target.getHealth() < 50)
        *lastPosition = target.getBonePosition(CHEST_BONE_ID);
      else
        *lastPosition = target.getBonePosition(boneId);
    }

    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeSinceKill = now - killTime;

    // continue shooting at target even after they die for 0.2 seconds
    if (timeSinceKill.count() > 0.2f && lastPosition)
    {
      delete lastPosition;
      lastPosition = NULL;
    }

    if (lastPosition)
    {
      if (!bspParser->is_visible(localPlayer.getAbsolutePosition(), *lastPosition))
        return false;

      // block user input
      client->setSensitivity(0.f);

      Vector3 targetAngle = aimAnglesTo(localPlayer, *lastPosition);
      if (targetAngle(0) != targetAngle(0) || targetAngle(1) != targetAngle(1) || targetAngle(2) != targetAngle(2))
        return false;

      Vector3 viewAngles = client->getViewAngles();


      float dYawTowardsRight = viewAngles(1) - targetAngle(1);
      while (dYawTowardsRight < 0)
        dYawTowardsRight += 360.f;
      while (dYawTowardsRight > 360.f)
        dYawTowardsRight -= 360.f;
      float dYawTowardsLeft = dYawTowardsRight - 360.f;

      Vector3 dAngles = targetAngle - viewAngles;
      if (abs(dYawTowardsRight) < abs(dYawTowardsLeft))
        dAngles(1) = -dYawTowardsRight;
      else
        dAngles(1) = -dYawTowardsLeft;

      Vector3 smoothTargetAngles = viewAngles + dAngles / 5;

       normalizeAngles(smoothTargetAngles);
       clampAngles(smoothTargetAngles);

       smoothTargetAngles(2) = 0.f;
      client->setViewAngles(smoothTargetAngles);

      return true;
    }
    else
    {
      // find new target
      target = newTarget;
      killTime = std::chrono::high_resolution_clock::now();
      lastPosition = NULL;
      client->resetSensitivity();

      return false;
    }
  }

  void Hacks::aimBot(float fov, int boneId)
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    if (!localPlayer.isValidPlayer())
      return;

    BaseEntity target = findClosestEnemyToFOV(fov, boneId);

    if (!target.isValidPlayer())
      return;

    Vector3 aimAngles = aimAnglesTo(localPlayer, target.getBonePosition(boneId));
    if (aimAngles(0) != aimAngles(0) || aimAngles(1) != aimAngles(1) || aimAngles(2) != aimAngles(2))
      return;

    client->setViewAngles(aimAngles);
    //localPlayer.shoot();
  }

  bool Hacks::enemyIsInCrossHair()
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    if (!localPlayer.isValidPlayer())
      return false;
    uint32_t crossHairId = localPlayer.getCrosshairId();
    if (crossHairId <= 0 || crossHairId > 65)
      return false;

    crossHairId -= 1;

    BaseEntity target(memory, memory->read<uint32_t>(memory->clientDll + signatures::dwEntityList + 0x10 * crossHairId));

    if (!target.isValidPlayer())
      return false;

    bool isEnemy = target.getTeamNum() != localPlayer.getTeamNum();

    return isEnemy;
  }

  void Hacks::inCrossTriggerBot()
  {
    BaseEntity localPlayer = client->getLocalPlayer();
    if (enemyIsInCrossHair())
      localPlayer.shoot();
    else
      localPlayer.setForceAttack(4);
  }

  void Hacks::walkBot()
  {
    BaseEntity localPlayer = client->getLocalPlayer();

    if (!localPlayer.isValidPlayer())
      return;

    Vector3 viewAngles = client->getViewAngles();
    Vector3 velocity = localPlayer.getVelocity();
    float speed = sqrt(velocity(0) * velocity(0) + velocity(1) * velocity(1));

    if (speed < 150.f)
    {
      viewAngles(0) = 0.f;
      viewAngles(1) += 1.f;
      viewAngles(2) = 0.f;

      normalizeAngles(viewAngles);
      clampAngles(viewAngles);

      client->setViewAngles(viewAngles);
    }
  }

  void Hacks::normalizeAngles(Vector3& angles)
  {
    while (angles(0) > 89.f)
      angles(0) -= 180.f;

    while (angles(0) < -89.f)
      angles(0) += 180.f;

    while (angles(1) > 180.f)
      angles(1) -= 360.f;

    while (angles(1) < -180.f)
      angles(1) += 360.f;
  }

  void Hacks::clampAngles(Vector3& angles)
  {
    if (angles(0) > 89.0)
      angles(0) = 89.0;

    if (angles(0) < -89.0)
      angles(0) = -89.0;

    if (angles(1) > 180.0)
      angles(1) = 180.0;

    if (angles(1) < -180.0)
      angles(1) = -180.0;
  }

  Hacks::Hacks(Memory* memory, Client* client, BSPParser* bspParser)
  {
    this->memory = memory;
    this->client = client;
    this->bspParser = bspParser;
  }

  Hacks::~Hacks()
  {

  }
}
