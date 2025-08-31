# Linux/m1n1 boot setup

There are two main ways in which Linux and m1n1 can be booted. The first is via pongoOS,
and the second one is directly via iBoot.

- [m1n1](https://github.com/HoolockLinux/m1n1) (idevice branch)
- [Linux](https://github.com/HoolockLinux/linux) (hoolock branch)

## Building m1n1

CI build output if you want to skip: [m1n1.zip](https://nightly.link/hoolocklinux/m1n1/workflows/build/idevice/m1n1.zip)

m1n1 can be built on Linux and macOS.

On Linux, you will need an `aarch64-linux-gnu-gcc` compiler (which is the native one if you are on arm64).

Building on ARM64 macOS is supported with clang and LLVM; you need to use Homebrew to install the required dependencies:

```
$ brew install llvm lld make
```

m1n1 also has rust parts, you will need to make sure you have the apprioriate rust toolchain.

The easiest way is to use [rustup](https://rustup.rs/)

```
$ rustup target add aarch64-unknown-none-softfloat
```

```
$ git clone --recursive https://github.com/HoolockLinux/m1n1.git
$ cd m1n1
$ gmake -j$(sysctl -n hw.ncpu) CHAINLOADING=1
```

To boot via pongoOS, you will need the `build/m1n1.bin` file.
To boot via iBoot, you will need `build/m1n1-idevice.macho` and possibly,
depending on the device, `build/monitor-stub.macho` as well.

## Building Linux

Linux must be built on Linux.

1. Get the usual build dependencies. Many package managers will have a command
    for such things. On Debian that would be `apt build-dep linux-source`.

    Building an ARM64 Linux kernel requires things than the x86 kernel though, so
    make sure `clang lld ncurses flex git bison`, and whatever build-esential/build-base
    package your distro uses is isntalled. You will also need the devicetree compiler,
    some distros call this `dtc`, some call it `devicetree-compiler`.

2. Get the source code

```
$ git clone https://github.com/hoolocklinux/linux hoolock-linux
$ cd hoolock-linux
```

3. Get an config. `config_16k` is an example one and name it `.config` inside
    the linux source code.

4. Start config

```
$ make -j$(nproc) LLVM=1 ARCH=arm64 menuconfig
```

Press `/` to search for a config option. With the example config it is fine
already on A9-A11, T2 SoCs, but if you use A7 - A8, then search for
`CONFIG_ARM64_4K` and enable that instead of `CONFIG_ARM64_16K`.

5. Compile

```
$ make -j$(nproc) LLVM=1 ARCH=arm64 Image.gz dtbs
```

The kernel is at to `arch/arm64/boot/Image.gz`, and the device trees are
at `arch/arm64/boot/dts/apple/*.dtb`

## Method-specific documentation

Choose a method.

The pongoOS method is less annoying, and you only need at most two sets of
files (4k and 16k kernel) if you plan to boot linux on different devices.
However, this only works when the device can be booted into normal OS.

The iBoot method does not depend on anything on main internal disk. However
this method is extra annoying (check the specific guide for details).

The iBoot method also may not work on T8012 (T2) because the pwned DFU tools
for that SoC is either lacking or broken.

- [pongoOS](./SETUP_pongoOS.md)
- [iBoot](./SETUP_iBoot.md)


## Interaction

This depends on what initramfs you use. If you use the example one:

- A shell will be spawned on serial console, except on Apple TV HD.
    (that one uses a different uart for that purpose and the example does not handle that)
- Emulate all of:
    - USB Ethernet Adapter
    - USB Mass storage device
    - USB Serial Adapter (likely `/dev/ttyACM0` on the host)

A shell is spwned USB serial adapter is a shell. If configured properly with dhcpcd
or NetworkManager, the device will assign itself the IPv4 `172.16.42.1`, and the
host `172.16.42.2`. A telnet shell should be available on `172.16.42.1` port 23.

The USB mass storage device is a 32 MB FAT16 ramdisk with log files in it.
