#pragma once

#include <Rbp/Rbp.h>

#if RBPINTERNAL
extern UINT32 _RbpMd5_K[64];
#endif

RBPEXPORT
RBPEXIT
RBPAPI
RbpMd5Calculate(
    IN  const VOID *Input,
    IN  UINTN       InputSize,
    OUT VOID       *Digest,
    IN  UINTN       DigestSize
);
