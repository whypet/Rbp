#include <math.h>

#include <Rbp/Rbp.h>
#include <Rbp/Md5.h>

VOID
RBPAPI
RbpInitialize(
    VOID
) {
    UINT32 Index;

    for (Index = 0; Index < COUNTOF(_RbpMd5_K); Index++)
        _RbpMd5_K[Index] = (UINT32)((LONGDOUBLE)(0x100000000ull) * fabsl(sinl(Index + 1)));
}
