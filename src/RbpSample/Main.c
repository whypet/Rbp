#include <stdio.h>

#include <Rbp/Rbp.h>
#include <Rbp/Md5.h>

int main(void) {
    const CHAR8 Hello[] = "Hello world";

    UINT8   Digest[sizeof(UINT32) * 4];
    UINTN   Index;
    RBPEXIT Exit;

    RbpInitialize();

    printf("Message: \"%s\"\n", Hello);

    Exit = RbpMd5Calculate(
        Hello,
        sizeof(Hello) - 1,
        Digest,
        sizeof(Digest)
    );

    if (RBPFAIL(Exit)) {
        printf("Error: failed to calculate MD5 hash\n");

        return 1;
    }

    printf("Hash: ");

    for (Index = 0; Index < sizeof(Digest); Index++)
        printf("%02x", Digest[Index]);

    return 0;
}
