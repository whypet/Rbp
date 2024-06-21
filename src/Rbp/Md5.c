#include <Rbp/Md5.h>

#define MD5_F(B, C, D) (((B) & (C)) | (~(B) & (D)))
#define MD5_G(B, C, D) (((B) & (C)) | ((C) & ~(D)))
#define MD5_H(B, C, D) ((B) ^ (C) ^ (D))
#define MD5_I(B, C, D) ((C) ^ ((B) | ~(D)))

typedef struct _MD5_STATE {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
} MD5_STATE;

const UINT32 _RbpMd5_Shift[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

const MD5_STATE _RbpMd5_DefaultState = {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476
};

UINT32 _RbpMd5_K[64];

RBPEXIT
RBPAPI
RbpMd5Calculate(
    IN  const VOID *Input,
    IN  UINTN       InputSize,
    OUT VOID       *Digest,
    IN  UINTN       DigestSize
) {
    errno_t Result;

    UINTN  BufferSize;
    UINT8 *Buffer;
    UINT32 Index;

    MD5_STATE State;
    MD5_STATE ChunkState;

    State = _RbpMd5_DefaultState;

    if (DigestSize < sizeof(UINT32) * 4)
        return RBPEXIT_InsufficientBuffer;

    BufferSize = ALIGN_UP(InputSize + 9, 64);
    Buffer = (UINT8 *)malloc(BufferSize);

    if (Buffer == NULL)
        return RBPEXIT_InternalError;

    Result = memcpy_s(
        Buffer,
        BufferSize,
        Input,
        InputSize
    );

    if (Result != 0)
        return RBPEXIT_InternalError;

    Buffer[InputSize] = (UINT8)1 << 7;

    memset(
        Buffer + InputSize + 1,
        0,
        BufferSize - sizeof(UINT64) - (InputSize + 1)
    );

    *((UINT64 *)(Buffer + BufferSize) - 1) = (UINT64)InputSize;

    for (Index = 0; Index < BufferSize * 4; Index++) {
        UINT32  BlockIndex;
        UINT32 *Blocks;
        UINT32  Value;
        UINT32  Shift;
        UINT32  j;
        UINT32  k;

        j = Index / 4;
        k = j % 64;

        Blocks = (UINT32 *)(Buffer + ALIGN_DOWN(j, 64));

        if (k == 0)
            ChunkState = State;

        switch (k / 16) {
        case 0:
            Value = MD5_F(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = k;
            break;
        case 1:
            Value = MD5_G(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = (5 * k + 1) % 16;
            break;
        case 2:
            Value = MD5_H(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = (3 * k + 5) % 16;
            break;
        case 3:
            Value = MD5_I(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = 7 * k % 16;
            break;
        };

        Value += ChunkState.A + _RbpMd5_K[k] + Blocks[BlockIndex];

        Shift = _RbpMd5_Shift[k];

        ChunkState.A  = ChunkState.D;
        ChunkState.B += ROTL(Value, Shift, 32);
        ChunkState.C  = ChunkState.B;
        ChunkState.D  = ChunkState.C;

        if (k == 0) {
            State.A += ChunkState.A;
            State.B += ChunkState.B;
            State.C += ChunkState.C;
            State.D += ChunkState.D;
        }
    }

    *((UINT32 *)Digest + 0) = State.A;
    *((UINT32 *)Digest + 1) = State.B;
    *((UINT32 *)Digest + 2) = State.C;
    *((UINT32 *)Digest + 3) = State.D;

    return RBPEXIT_Success;
}
