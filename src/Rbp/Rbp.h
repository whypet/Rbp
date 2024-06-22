#pragma once

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define _RBP_

#if RBPEXPORTS
    #define RBPEXPORT __declspec(dllexport)
#else
    #define RBPEXPORT __declspec(dllimport)
#endif

#if defined(__x86_64__)
    #define RBPAPI __fastcall
#elif defined(__i386__)
    #define RBPAPI __cdecl
#else
    #define RBPAPI
#endif

#define NULL (void *)0
#define FALSE 0
#define TRUE 1

#define IN
#define OUT
#define OPTIONAL

#define UNREFERENCED_PARAMETER(x)
#define COUNTOF _countof
#define SIZEOF_FIELD(type, field) (sizeof(((type *)NULL)->field))

#define ALIGN_DOWN(x, n) (((x) / (n)) * (n))
#define ALIGN_UP(x, n)   ((((x) + (n) - 1) / (n)) * (n))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define INT_BITS(x)  _BitInt(x)
#define UINT_BITS(x) unsigned _BitInt(x)

typedef void    VOID;
typedef char    CHAR8;
typedef uint8_t BOOLEAN;

typedef int8_t    INT8;
typedef int16_t   INT16;
typedef int32_t   INT32;
typedef int64_t   INT64;
typedef ptrdiff_t INTN;

typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef size_t   UINTN;

typedef float       FLOAT32;
typedef double      FLOAT64;
typedef long double LONGDOUBLE;

#define ROTL(bits, x, n) ((UINT##bits)(x) << (UINT##bits)(n) | (UINT##bits)(x) >> (bits##u - (UINT##bits)(n)))
#define ROTR(bits, x, n) ((UINT##bits)(x) >> (UINT##bits)(n) | (UINT##bits)(x) << (bits##u - (UINT##bits)(n)))

typedef enum _RBPEXIT {
    RBPEXIT_Success,
    RBPEXIT_InternalError,
    RBPEXIT_InsufficientBuffer
} RBPEXIT;

#define RBPSUCCESS(Exit) ((Exit) == RBPEXIT_Success)
#define RBPFAIL(Exit)    ((Exit) != RBPEXIT_Success)

RBPEXPORT
VOID
RBPAPI
RbpInitialize(
    VOID
);
