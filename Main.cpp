#include "BSP/BSPParser.hpp"
#include "Include.hpp"
#include "BaseEntity.hpp"
#include "Client.hpp"
#include "GlowObject.hpp"
#include "Hacks.hpp"
#include <iostream>
#include <chrono>
#include <thread>


using namespace hazedumper;
int main(int argc, char** argv)
{
  Memory memory;
  BSPParser bspParser;
  Client client(&memory);
  Hacks hacks(&memory, &client, &bspParser);

  while (client.getIsInGame())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    const char* gameDirectory = client.getGameDirectory();
    const char* mapDirectory = client.getMapDirectory();
    bspParser.parse_map(gameDirectory, mapDirectory);

    //hacks.glowESP();
    //hacks.noFlash();

    const float FOV_RANGE = 10.f;

    if ((GetAsyncKeyState(VK_XBUTTON2) & KEY_DOWN))
      hacks.aimAssist(FOV_RANGE, HEAD_BONE_ID);
    else
      client.resetSensitivity();    // re-enable user input


    if ((GetAsyncKeyState(VK_XBUTTON1) & KEY_DOWN))
      hacks.inCrossTriggerBot();


    if (GetAsyncKeyState(VK_SPACE) & KEY_DOWN)
      hacks.bunnyHop();
  }

  return 0;
}
