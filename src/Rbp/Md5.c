#include <Rbp/Md5.h>

#define MD5_F(B, C, D) (((B) & (C)) | (~(B) & (D)))
#define MD5_G(B, C, D) (((B) & (D)) | ((C) & ~(D)))
#define MD5_H(B, C, D) ((B) ^ (C) ^ (D))
#define MD5_I(B, C, D) ((C) ^ ((B) | ~(D)))

#define MD5_CHUNK_SIZE (16 * sizeof(UINT32))

typedef struct _MD5_STATE {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
} MD5_STATE;

const UINT32 _RbpMd5_Shift[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

const MD5_STATE _RbpMd5_DefaultState = {
    .A = 0x67452301,
    .B = 0xefcdab89,
    .C = 0x98badcfe,
    .D = 0x10325476
};

UINT32 _RbpMd5_K[MD5_CHUNK_SIZE];

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
    ChunkState = State;

    if (DigestSize < sizeof(UINT32) * 4)
        return RBPEXIT_InsufficientBuffer;

    BufferSize = ALIGN_UP(InputSize + 9, MD5_CHUNK_SIZE);
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

    *((UINT64 *)(Buffer + BufferSize) - 1) = (UINT64)InputSize * 8;

    for (Index = 0; Index < BufferSize; Index++) {
        UINT32  BlockIndex;
        UINT32 *Blocks;
        UINT32  Value;
        UINT32  i;

        Blocks = (UINT32 *)(Buffer + ALIGN_DOWN(Index, MD5_CHUNK_SIZE));

        i = Index % MD5_CHUNK_SIZE;

        if (i == 0 && Index != 0) {
            State.A += ChunkState.A;
            State.B += ChunkState.B;
            State.C += ChunkState.C;
            State.D += ChunkState.D;

            ChunkState = State;
        }

        switch (i * 4 / MD5_CHUNK_SIZE) {
        case 0:
            Value = MD5_F(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = i;
            break;
        case 1:
            Value = MD5_G(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = (5 * i + 1) % 16;
            break;
        case 2:
            Value = MD5_H(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = (3 * i + 5) % 16;
            break;
        case 3:
            Value = MD5_I(ChunkState.B, ChunkState.C, ChunkState.D);
            BlockIndex = (7 * i) % 16;
            break;
        };

        Value += ChunkState.A + _RbpMd5_K[i] + Blocks[BlockIndex];

        ChunkState.A  = ChunkState.D;
        ChunkState.D  = ChunkState.C;
        ChunkState.C  = ChunkState.B;
        ChunkState.B += ROTL(32, Value, _RbpMd5_Shift[i]);
    }

    State.A += ChunkState.A;
    State.B += ChunkState.B;
    State.C += ChunkState.C;
    State.D += ChunkState.D;

    Result = memcpy_s(
        Digest,
        DigestSize,
        &State,
        sizeof(State)
    );

    if (Result != 0)
        return RBPEXIT_InternalError;

    return RBPEXIT_Success;
}
