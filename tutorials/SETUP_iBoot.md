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
   macOS and hacked up for the tools to be built for Linux. Some of the tools
   might be diffcult to compile on Linux and require specific compiler flags, and
   clang is required.

The reason this method exists is that it has the potential to be used
in wrappers like `pmbootstrap` (not supported yet). One is not expected
to do all of these steps manually.

## Compilation hell

At least `mach-o/loader.h` and `mach-o/nlist.h` header files are needed
by some tools. Try [these hacked headers](../include) on Linux.

> [!TIP]
> Some of the tools has flimsy build systems and you might want to skip them entirely on Linux and invoke
the clang compiler directly.

> [!TIP]
> [Futurerestore](https://github.com/futurerestore/futurerestore/tree/dev) has linux builds and uses many of
the same tools and libraries so you might want to check how their [builds are generated](https://github.com/futurerestore/futurerestore/tree/dev/.github/workflows)

Get relatively sound tools and libraries:

- [ipsw](https://github.com/blacktop/ipsw)
- [openssl 3.0.3](https://github.com/openssl/openssl) (or CommonCrypto on macOS);
- [libusb 1.0](https://github.com/libusb/libusb) (Linux only, macOS can use IOKit for libirecovery);
- [libzip](https://github.com/nih-at/libzip);
- [libplist](https://github.com/libimobiledevice/libplist);
- [libusbmuxd](https://github.com/libimobiledevice/libusbmuxd);
- [libirecovery](https://github.com/libimobiledevice/libirecovery);
- [libimobiledevice](https://github.com/libimobiledevice/libimobiledevice);
- [libimobiledevice-glue](https://github.com/libimobiledevice/libimobiledevice-glue);
- [lzfse](https://github.com/lzfse/lzfse) (Linux only, macOS has built in libcompression);
- [img4lib](https://github.com/xerub/img4lib)
- [gaster(verygenericname)](https://github.com/verygenericname/gaster)

Get these flimsy tools and libraries:

- [libgeneral](https://github.com/tihmstar/libgeneral);
- [libinsn](https://github.com/tihmstar/libinsn);
- [liboffsetfinder64(fork))](https://github.com/Cryptiiiic/liboffsetfinder64);
- [libipatcher(fork)](https://github.com/Cryptiiiic/libipatcher)
- [iBoot64Patcher](https://github.com/Cryptiiiic/iBoot64Patcher)

## Get Firmware files

Put your device into [DFU mode](https://theapplewiki.com/wiki/DFU_Mode).
For iPhone, iPads and Apple TV HD, you can use palera1n's DFU helper as follows:

```
# palera1n -D
```

Get device info:
```
# irecovery -q
```

Pwn device:

```
# gaster pwn
```

Example output:
```
usb_timeout: 5
usb_abort_timeout_min: 0
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Stage: RESET
ret: true
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Stage: SETUP
ret: true
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Stage: SPRAY
ret: true
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Stage: PATCH
ret: true
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Now you can boot untrusted images.
```

After pwning, `irecovery -q` should show: `PWND: gaster` (`PWND: checkm8` on some other tools/forks).

Download files. Note iBEC is unused on A10, A10X, T2 and A11.
The following commands uses iPhone7,2 as example with a board
ID of n61ap, with A8 chip.

iBSS and iBEC are shared between DEV and AP boards and doesn't need
the `ap` / `dev` suffix. However for DeviceTree the suffix is required.

(you have a 99.999% chance of having an AP board)

```
$ ipsw download ipsw --device "iPhone7,2" --latest --pattern 'iBSS.n61.RELEASE.im4p$'
$ ipsw download ipsw --device "iPhone7,2" --latest --pattern 'iBEC.n61.RELEASE.im4p$' # skip on A10, A10X, T2 and A11
$ ipsw download ipsw --device "iPhone7,2" --latest --pattern 'DeviceTree.n61ap.RELEASE.im4p$'
```

## Process firmware files

### Check encryption

```
$ img4 -b -i **/DeviceTree.n61ap.RELEASE.im4p
```

If you get `[e] cannot get keybag` then the file is not encrypted. If you get two kbags like this:

```
044A677C7522AA7DED7BC2476DB7142A11BE54A6354D8A5888EC485E2E51FD52D8A282D6D224EAC7D710D3D9D45F4B86
466DFB0CA20C954EA723F238C8D76ED42389AA69FF7012FAA7406407DAE1C5DB9EAB78C963545129DB00058E98295B8F
```

then the file is encrypted.

### Get decryption keys

> [!TIP]
> You can also find the keys online: https://theapplewiki.com/wiki/Firmware_Keys. This section explains how to get the key from the device.

The first kbag is for production devices. You should use this one unless your device is demoted. (change kbag!)

```
# gaster decrypt_kbag 044A677C7522AA7DED7BC2476DB7142A11BE54A6354D8A5888EC485E2E51FD52D8A282D6D224EAC7D710D3D9D45F4B86
```

Example output:
```
usb_timeout: 5
usb_abort_timeout_min: 0
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x7000
Found the USB handle.
Now you can boot untrusted images.
[IOKit] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
Found the USB handle.
IV: 3A4F1C4CECD7DA4E730F5B5525284C7A, key: 07151DAFED8E284D6D935EB04E33CC062B3D8FCC04075B836C99767E1E025ABE
```

The IV and key will be used together in most tools, so like `3A4F1C4CECD7DA4E730F5B5525284C7A07151DAFED8E284D6D935EB04E33CC062B3D8FCC04075B836C99767E1E025ABE`.

### Extract/decrypt file

For an encrypted file, extract it by passing the IV+Key to img4:

Continue with the n61ap example:

```
$ img4 -k 3A4F1C4CECD7DA4E730F5B5525284C7A07151DAFED8E284D6D935EB04E33CC062B3D8FCC04075B836C99767E1E025ABE -i **/iBSS.n61.RELEASE.im4p -o iBSS.bin
```

```
$ img4 -k 7C4CA560907B3361A9F54037151C53C6D180F568F1D8CD20613C5CE13D5C92D215C136D9C09F399B6C0B2325118EC798 -i **/iBEC.n61.RELEASE.im4p -o iBEC.bin # skip on A10, A10X, T2 and A11
```

For unencrypted file the skip the `-k` option:

```
$ img4 -i **/DeviceTree.n61ap.im4p -o DeviceTree.bin
```

### Patch files

Specifically, we need iBSS, as well as iBEC on some devices, to have their rsa/image4 callback patched.

```
$ iBoot64Patcher iBSS.bin iBSS_patched.bin
$ iBoot64Patcher iBEC.bin iBEC_patched.bin # skip on A10, A10X, T2 and A11
```

Example output:

```
main: Starting...
getting get_debug_enabled_patch() patch
getting get_sigcheck_patch() patch
applying patch=0x83000AF9C : 000080d2
applying patch=0x83000AFA0 : c0035fd6
applying patch=0x83000AFE0 : 000080d2
applying patch=0x83000C938 : 200080d2
main: Writing out patched file to iBEC_patched.bin...
main: Quitting...
```

### Wrap up files

You need an arm64 Linux initramfs. It must be compressed with gzip.
Example [initramfs.gz.zip](https://nightly.link/HoolockLinux/HoolockRD/workflows/build/master/initramfs.gz.zip).
Extract the zip before using the example.

Create the m1n1 Mach-O blob:

```
$ cat /path/to/m1n1/build/m1n1-idevice.macho <(echo 'chosen.bootargs=<kernel command line here>')  \
	/path/to/hoolock-linux/arch/arm64/boot/dts/apple/*.dtb \
	/path/to/hoolock-linux/arch/arm64/boot/Image.gz  \
	/path/to/initramfs.gz > m1n1-linux.macho
```

Get [empty trustcache](../binaries/empty_trustcache), and the apprioraite [im4m file](../binaries/im4m) for the SoC your device have (this is why a set of file is needed for every SoC)

> [!NOTE]
> An empty trustcache file is an empty list of hashes. We believe it does not the threshold of originality for copyright protection.

> [!NOTE]
> IM4M files are cryptographic signatures with a set of constriants (like chip ID). We believe it does not meet the threshold of originality for copyright protection.

Continue with the n61ap example, other SoC will use something other than 0x7000.im4m:

Wrap up iBSS, iBEC and DeviceTree:
```
$ img4 -i iBSS_patched.bin -o iBSS.img4 -A -T ibss -M 0x7000.im4m
$ img4 -i iBEC_patched.bin -o iBEC.img4 -A -T ibec -M 0x7000.im4m # skip on A10, A10X, T2 and A11
$ img4 -i DeviceTree.bin -o DeviceTree.img4 -T rdtr -M 0x7000.im4m # DeviceTree (dtre) -> RestoreDeviceTree (rdtr)
```

Wrap up the empty trustcache as well:

```
$ img4 -i empty_trustcache -o trustcache.img4 -A -T rtsc -M 0x7000.im4m # rtsc = RestoreTrustCache
```

Wrap m1n1 into a fake RestoreKernelCache. This step is different depending on the device:

> [!TIP]
> To boot just m1n1 and not Linux, use the `m1n1-idevice.macho` in the m1n1 build folder
in place of the combined `m1n1-linux.macho`file.

#### A7, A8, A8X, A9, A9X

These devices needs a Secure Monitor in additional to the kernel cache.
`monitor-stub.macho` is a stub with 2 instructions in the m1n1 build folder.

n61ap example:
```
$ ipsw img4 create --input m1n1-linux.macho --type rkrn --compress lzss --extra /path/to/m1n1/build/monitor-stub.macho --im4m 0x7000.im4m --output kernelcache.img4
```

#### A10, A10X, T2, A11

d22ap example:
```
$ img4 -i m1n1-linux.macho -o kernelcache.img4 -A -T rkrn -M 0x8015.im4m
```

## Boot the thing

You should now have:

- `iBSS.img4`
- `iBEC.img4` if on A7, A8, A8X, A9, A9X
- `trustcache.img4`
- `DeviceTree.img4`
- `kernelcache.img4` which is actually m1n1+linux

If the device hasn't been pwned, pwn now:

```
# gaster pwn
```

Then exit the shellcode mode so the device could boot the images

```
# gaster reset
```

### Entering recovery mode

Send iBSS.

```
# irecovery -f iBSS.img4
```

Wait 2 seconds.

On A10, A10X, T2, A11 after doing so the device should be in recovery mode (USB PID 0x1281),
so skip the next step where applicable.

For A7, A8, A8X, A9, A9X the device will still appear to be in DFU mode (USB PID 0x1227),
but actually iBSS is running, and if you run `irecovery -q`, `PWND: gaster` would be gone.

Send iBEC.

```
# irecovery -f iBEC.img4
```

Wait 2 seconds.

### Send files to recovery mode device

The device is now in recovery mode (USB PID 0x1281), and is ready to accept the rest
of the files:

> [!TIP]
> To debug this step, use `irecovery -s` and use `/upload path` to upload files, other commands are sent to iBoot.

Send files & command:

```
# irecovery -f DeviceTree.img4
# irecovery -c devicetree
# irecovery -f trustcache.img4
# irecovery -c firmware
# irecovery -f kernelcache.img4
# irecovery -f bootx
```

After that, the device should be booting m1n1, which will boot Linux.
