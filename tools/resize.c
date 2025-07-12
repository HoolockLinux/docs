#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <TargetConditionals.h>

#if TARGET_OS_OSX || TARGET_OS_SIMULATOR
#define APFS_FW_PATH "/System/Library/PrivateFrameworks/APFS.framework/Versions/A/APFS"
#else
#define APFS_FW_PATH "/System/Library/PrivateFrameworks/APFS.framework/APFS"
#endif

int (*APFSContainerResize)(const char *bsdname, uint64_t newSize);

int main(int argc, const char * argv[]) {
    size_t new_size;
    char* endptr;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <APFS container BSD name> <new size in bytes>\n", argv[0]);
        return -1;
    }

    new_size = strtoull(argv[2], &endptr, 0);

    if (argv[2] == endptr) {
        fprintf(stderr, "Invalid new size given\n");
        return -1;
    }

    void *handle = dlopen(APFS_FW_PATH, RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "Could not load APFS.framework: %s", dlerror());
        return -1;
    }

    APFSContainerResize = dlsym(handle, "APFSContainerResize");
    if (!APFSContainerResize) {
        fprintf(stderr, "Function APFSContainerResize not found: %s", dlerror());
        dlclose(handle);
        return -1;
    }

    printf("About to resize %s to %s bytes, in an online resize, device will become unresponsive.\n", argv[1], argv[2]);

    int result = APFSContainerResize(argv[1], new_size);
    if (result != 0) {
        fprintf(stderr,"APFSContainerResize failed: %d: %s\n", result, mach_error_string(result));
        return -1;
    }

    printf("The operation completed successfully\n");

    dlclose(handle);
    return 0;
}
