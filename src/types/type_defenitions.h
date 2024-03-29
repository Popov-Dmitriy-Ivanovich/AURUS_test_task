#ifndef TYPE_DEFENITIONS
#define TYPE_DEFENITIONS
#include <cstdint>
namespace types {

using UnsignedInt2 = u_int16_t;
using UnsignedInt4 = u_int32_t;
using Int2 = int16_t;
using Int4 = int32_t;

static_assert(sizeof(UnsignedInt2) == 2);
static_assert(sizeof(Int2) == 2);
} // namespace types

#endif