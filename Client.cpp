#pragma once
#include "Client.hpp"
namespace hazedumper
{

  float Client::getSensitivity()
  {
    uint32_t sensitivityPtr = memory->read<uint32_t>(memory->clientDll + signatures::dwSensitivityPtr);
    uint32_t sensitivity = memory->read<uint32_t>(memory->clientDll + signatures::dwSensitivity);

  	sensitivity ^= sensitivityPtr;

  	float sens = *reinterpret_cast<float*>(&sensitivity);

  	return sens;
  }

  void Client::setSensitivity(float sens)
  {
    uint32_t sensitivityPtr = memory->read<uint32_t>(memory->clientDll + signatures::dwSensitivityPtr);
  	 uint32_t sensitivity = *reinterpret_cast<uint32_t*>(&sens) ^ sensitivityPtr;

    memory->write<uint32_t>(memory->clientDll + signatures::dwSensitivity, sensitivity);
  }

  void Client::resetSensitivity()
  {
    setSensitivity(defaultSensitivity);
  }

  bool Client::worldToScreen(const Vector3& from, Vector3& to)
  {
    WorldToScreenMatrix matrix = memory->read<WorldToScreenMatrix>(memory->clientDll + signatures::dwViewMatrix);

    float w = 0.0f;

    to(0) = matrix(0, 0) * from(0) + matrix(0, 1) * from(1) + matrix(0, 2) * from(2) + matrix(0, 3);
    to(1) = matrix(1, 0) * from(0) + matrix(1, 1) * from(1) + matrix(1, 2) * from(2) + matrix(1, 3);
    w = matrix(3, 0) * from(0) + matrix(3, 1) * from(1) + matrix(3, 2) * from(2) + matrix(3, 3);

    if (w < 0.01f)
      return false;

    float wInverse = 1.0f / w;
    to(0) *= wInverse;
    to(1) *= wInverse;

    float x = SCREEN_WIDTH / 2;
    float y = SCREEN_HEIGHT / 2;

    x += 0.5 * to(0) * SCREEN_WIDTH + 0.5;
    y -= 0.5 * to(1) * SCREEN_HEIGHT + 0.5;

    to(0) = x;
    to(1) = y;

    return true;
  }

  bool Client::getIsInGame()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    return memory->read<uint32_t>(clientState + signatures::dwClientState_State) == 6;
  }

  BaseEntity Client::getLocalPlayer()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    uint32_t localPlayerIndex = memory->read<uint32_t>(clientState + signatures::dwClientState_GetLocalPlayer);
    BaseEntity localPlayer = BaseEntity(memory, memory->read<uint32_t>(memory->clientDll + signatures::dwEntityList + 0x10 * localPlayerIndex));
    return localPlayer;
  }

  Vector3 Client::getViewAngles()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    return memory->read<Vector3>(clientState + signatures::dwClientState_ViewAngles);
  }

  void Client::setViewAngles(Vector3& viewAngles)
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    memory->write<Vector3>(clientState + signatures::dwClientState_ViewAngles, viewAngles);
  }

  const char* Client::getMapDirectory()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    static std::array<char, 0x120> mapDirectory = memory->read<std::array<char, 0x120>>(clientState + signatures::dwClientState_MapDirectory);
    return mapDirectory.data();
  }

  const char* Client::getMapName()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    static std::array<char, 0x80> mapName = memory->read<std::array<char, 0x80>>(clientState + signatures::dwClientState_Map);
    return mapName.data();
  }

  const char* Client::getGameDirectory()
  {
    uint32_t clientState = memory->read<uint32_t>(memory->engineDll + signatures::dwClientState);
    static std::array<char, 0x120> gameDirectory = memory->read<std::array<char, 0x120>>(memory->engineDll + signatures::dwGameDir);
    return gameDirectory.data();
  }

  Client::Client(Memory* memory)
  {
    this->memory = memory;
    this->defaultSensitivity = getSensitivity();
  }


  Client::~Client()
  {
  }

}
