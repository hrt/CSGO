#include "Include.hpp"

namespace hazedumper
{
  class GlowObject
  {
  public:
    GlowObject();
    uint32_t baseEntity;           /// 0x00
    float    red;                  /// 0x04
    float    green;                /// 0x08
    float    blue;                 /// 0x0C
    float    alpha;                /// 0x10
  private:
    uint8_t _0x14[0x10];           /// 0x14
  public:
    bool renderWhenOccluded;       /// 0x24
    bool renderWhenUnccluded;      /// 0x25
    bool fullBloomRender;          /// 0x26
  private:
    uint8_t _0x27[0x5];            /// 0x27
  public:
    int32_t  style;                /// 0x2C
    int32_t  splitScreenSlot;      /// 0x30
    int32_t  nextFreeSlot;         /// 0x34
    static constexpr int32_t END_OF_FREE_LIST = -1;
    static constexpr int32_t ENTRY_IN_USE = -2;
  }; /// Size=0x38
}