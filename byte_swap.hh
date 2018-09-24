#ifndef YMD_BYTE_SWAP_HH
#define YMD_BYTE_SWAP_HH 1

#include <cstdint>

namespace ymd {

  constexpr inline std::uint16_t swap16(std::uint16_t value){
    return (value << 8) | (value >> 8);
  }

  constexpr inline std::uint32_t swap32(std::uint32_t value){
    return ((value << 24) |
	    ((value & 0x0000FF00) << 8) |
	    ((value & 0x00FF0000) >> 8) |
	    (value >> 24));
  }
} // namespace ymd
#endif // YMD_BYTE_SWAP_HH
