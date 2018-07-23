#include "Include.hpp"
#include "Client.hpp"
#include "BSP/BSPParser.hpp"

namespace hazedumper
{
  class Hacks
  {
  private:
    Memory* memory;
    Client* client;
    BSPParser* bspParser;
    BaseEntity findClosestEnemyToFOV(float fov, uint32_t boneId);
    Vector3 angleDifferenceToEntity(BaseEntity& localPlayer, BaseEntity& entity, uint32_t boneId);
    Vector3 aimAnglesTo(BaseEntity& localPlayer, Vector3& entity);
    void normalizeAngles(Vector3& angles);
    void clampAngles(Vector3& angles);
    bool enemyIsInCrossHair();
  public:
    void glowESP();
    void noFlash();
    void bunnyHop();
    bool aimAssist(float fov, int boneId);  // legit aimbot - if low then aim chest
    void aimBot(float fov, int boneId);     // blatant aimbot
    void inCrossTriggerBot();
    void walkBot();                         // can be used with aimAssist to make a deathmatch bot
    Hacks(Memory* memory, Client* client, BSPParser* bspParser);
    ~Hacks();
  };
}
