#include "Memory.hpp"
#include "Matrix.hpp"
#include "csgo.hpp"

namespace hazedumper
{
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1280
#define	FL_ONGROUND (1<<0)
#define IN_ATTACK		(1<<0)
#define IN_ATTACK2	(1<<11)
#define IN_RELOAD		(1<<13)
#define KEY_DOWN (0x80000000)
#define MOUSE_UP (0x100)
#define HEAD_BONE_ID ((DWORD) 8)
#define CHEST_BONE_ID ((DWORD) 37)

  using Vector3 = Matrix< float, 3, 1 >;
  using WorldToScreenMatrix = Matrix< float, 4, 4 >;
}
