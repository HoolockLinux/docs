# Hoolock Linux Documentation

Documentation for Linux on A7-A11, T2 Apple devices.

This document contains information about building and booting the kernel.

For feature support, see [FEATURES.md](./FEATURES.md).

Excluded devices:
  - HomePod (not tested, so no device tree included)

Supported firmware versions: iOS/iPadOS/tvOS 9.0 - 18.0

## Preparing files

### Required files

- [checkra1n 1337](https://checkra.in/1337)
- [PongoOS (with bootm)](https://github.com/checkra1n/PongoOS/tree/iOS15)
- [Linux Kernel](https://github.com/hooklocklinux/linux) branch `hoolock`
- [m1n1-idevice](https://github.com/hoolocklinux/m1n1)
- [pongoterm.c](https://github.com/checkra1n/PongoOS/raw/iOS15/scripts/pongoterm.c)
- An arm64 initramfs, an example is included here as well.

Prebuilt PongoOS and can be found in this repository. You can find
prebuilt m1n1-idevice binaries from the [CI build output](https://nightly.link/hoolocklinux/m1n1/workflows/build/idevice/m1n1.zip).
`m1n1.bin` is that one that should be used alongside pongoOS like in the following instructions. `m1n1-idevice.macho` and `monitor-stub.macho`
is for booting with Apple's iBoot. That process is more complicated and has not been documented yet.

Note: palera1n's pongoOS already have bootm

### pongoterm.c compile instructions

Linux:
```
cc pongoterm.c -DUSE_LIBUSB -Os -lusb-1.0 -o pongoterm
```

macOS:
```
clang -x objective-c pongoterm.c -framework IOKit -framework CoreFoundation -framework Foundation -Os -o pongoterm
```

### Kernel config

[KERNEL.md](./KERNEL.md)

To compile the kernel with clang:
```
make -j$(nproc) ARCH=arm64 LLVM=1 Image.gz dtbs
```


### Prepare m1n1 blob

Assuming you are in the m1n1 source tree, the command should look like this.

```
cat build/m1n1.bin <(echo 'chosen.bootargs=<kernel command line here>')  \
	/path/to/linux-apple/arch/arm64/boot/dts/apple/*.dtb \
	/path/to/linux-apple/arch/arm64/boot/Image.gz  \
	/path/to/initramfs.gz  > m1n1-linux.bin
```

## Booting

### DFU mode

You can use [palera1n](https://github.com/palera1n/palera1n/releases) to
help you enter DFU mode. Command: `palera1n -Dl`

[Instructions](https://theapplewiki.com/wiki/DFU_Mode) to enter DFU without
palera1n.

### Booting PongoOS

Run checkra1n as root:

```
sudo /path/to/checkra1n -pEk /path/to/Pongo.bin
```

### Booting m1n1 and kernel

After booting pongoOS, run pongoterm as root:

```
printf '/send /path/to/m1n1-linux.bin\nbootm\n' | sudo pongoterm
```

## Acknowledgements

- [Asahi Linux](https://asahilinux.org/) - m1n1 and Apple M1 Linux support
- [checkra1n](https://checkra.in) - pongoOS and checkm8 exploit implementation
- [Konrad Dybcio](https://konradybcio.pl) - Original linux-apple porter
- [Corellium](https://github.com/corellium) - Sandcastle Linux port

----

*Linux® is the registered trademark of Linus Torvalds in the U.S. and other countries.*
