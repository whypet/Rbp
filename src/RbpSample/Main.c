#include <stdio.h>

#include <Rbp/Rbp.h>
#include <Rbp/Md5.h>

int main(void) {
    CHAR8        Digest[sizeof(UINT32) * 4];
    const CHAR8 *Hello;
    RBPEXIT      Exit;

    RbpInitialize();

    Hello = "Hello world";

    printf("Message: \"%s\"\n", Hello);

    Exit = RbpMd5Calculate(
        Hello,
        sizeof(Hello),
        Digest,
        sizeof(Digest)
    );

    if (RBPFAIL(Exit)) {
        printf("Error: failed to calculate MD5 hash\n");

        return 1;
    }

    printf(
        "Hash: %08x%08x%08x%08x",
        *((UINT32 *)Digest + 3),
        *((UINT32 *)Digest + 2),
        *((UINT32 *)Digest + 1),
        *((UINT32 *)Digest + 0)
    );

    return 0;
}
