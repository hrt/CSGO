#pragma once
#include "Include.hpp"
#include "BaseEntity.hpp"

namespace hazedumper
{
  class Client
  {
  private:
    Memory* memory;
    float defaultSensitivity;
  public:
    float getSensitivity();
    void setSensitivity(float s);
    void resetSensitivity();
    bool worldToScreen(const Vector3& from, Vector3& to);
    bool getIsInGame();
    Vector3 getViewAngles();
    void setViewAngles(Vector3& viewAngles);
    BaseEntity getLocalPlayer();
    const char* getMapDirectory();
    const char* getMapName();
    const char* getGameDirectory();
    Client(Memory* memory);
    ~Client();
  };
}
