# Linux/m1n1 boot setup (iBoot method)

In this method m1n1 becomes a fake restore kernel cache, and the
device is booted as if it is about to undergo a restore.

> [!WARNING]
> This method require a set of files for every chip you want
to boot Linux on.

> [!WARNING]
> After booting, the hardware is more locked down in this method.

> [!WARNING]
> This method required a bunch of flimsy tools that are not sound and might crash if anything is not right.

> [!WARNING]
> The tools are often ambigously forked. A fork that works on one version/device
combination might not work on another version/device combination. Another
fork might fix it but might break some other version/device combination.

> [!WARNING]
>  Some of the tools uses mach-o format headers that one would need to copy from
   macOS and hacked up for the tools to be built for. Some of the tools might be
   diffcult to compile on Linux and require specific compiler flags, and clang
   is required.

The reason this method exists is that it has the potential to be used
in wrappers like `pmbootstrap` (not supported yet). One is not expected
to do all of these steps manually.

Get tools and libraries:

- [ipsw](https://github.com/blacktop/ipsw)
- [curl](https://github.com/curl/curl) (Linux/Windows only, macOS already has curl preinstalled);
- [openssl 3.0.3](https://github.com/openssl/openssl) (or CommonCrypto on macOS);
- [libusb 1.0](https://github.com/libusb/libusb) (Linux only, macOS can use IOKit for libirecovery);
- [libzip](https://github.com/nih-at/libzip);
- [libplist](https://github.com/libimobiledevice/libplist);
- [libusbmuxd](https://github.com/libimobiledevice/libusbmuxd);
- [libirecovery](https://github.com/libimobiledevice/libirecovery);
- [libimobiledevice](https://github.com/libimobiledevice/libimobiledevice);
- [libimobiledevice-glue](https://github.com/libimobiledevice/libimobiledevice-glue);
- [lzfse](https://github.com/lzfse/lzfse) (Linux/Windows only, macOS has built in libcompression);
- [img4lib](https://github.com/xerub/img4lib)
- [libgeneral](https://github.com/tihmstar/libgeneral);
- [libinsn](https://github.com/tihmstar/libinsn);
- [liboffsetfinder64(fork))](https://github.com/Cryptiiiic/liboffsetfinder64);
- [libipatcher(fork)](https://github.com/Cryptiiiic/libipatcher)
- [gaster](https://github.com/0x7ff/gaster)
- [iBoot64Patcher](https://github.com/Cryptii)

TODO
