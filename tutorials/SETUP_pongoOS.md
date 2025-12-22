# Linux/m1n1 boot setup (pongoOS method)

## Building pongoOS

This one might be tricky to build on Linux so you could also grab the prebuilt binary: [Pongo.bin](./binaries/Pongo.bin)

### macOS

pongoOS can be built on macOS. Make sure Xcode is installed:

```
$ git clone https://github.com/checkra1n/pongoOS
$ cd pongoOS
$ git checkout -b iOS15
$ make -j$(sysctl -n hw.ncpu)
```

The built file is at `build/Pongo.bin`.

### Linux

You will need a aarch64-apple-darwin toolchain, so install [cctools-port](https://github.com/tpoechtrager/cctools-port)

You also need clang installed.

To install the toolchain from source, follow the steps below:

Export the location you want the toolchain to be in:

```
$ export PREFIX=${HOME}/.local
```

#### libtapi

You might want to update the version.

```
$ export TAPI_VERSION=1600.0.11.8
$ wget -O libtapi.tar.gz https://github.com/tpoechtrager/apple-libtapi/archive/refs/heads/${TAPI_VERSION}.tar.gz
$ tar xf ${TAPI_VERSION}.tar.gz
$ mkdir -p apple-libtapi-${TAPI_VERSION}/build
$ pushd apple-libtapi-${TAPI_VERSION}/build
$  TAPI_INCLUDE_FIX="-I $PWD/../src/llvm/projects/clang/include "
$  TAPI_INCLUDE_FIX+="-I $PWD/projects/clang/include "

$  cmake ../src/llvm \
    -DCMAKE_CXX_FLAGS="${TAPI_INCLUDE_FIX}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DTAPI_FULL_VERSION="${TAPI_VERSION}" \
    -DLLVM_ENABLE_PROJECTS="libtapi" \
    -DTAPI_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_TESTS=OFF

$  make -j$(nproc) clangBasic libtapi
$  make -j$(nproc) install-libtapi install-tapi-headers
$ popd
```

#### cctools

```
$ git clone https://github.com/tpoechtrager/cctools-port.git
$ cd cctools-port/cctools
$ ./configure \
  --prefix=${PREFIX} \
  --with-libtapi=${PREFIX} \
  --target=aarch64-apple-darwin
$ make -j$(nproc)
$ make install
$ make clean
```

Locate the linker in your install location. Make a symlink named `ld64` to it in your `PATH`.
It will be used in the following commands:

Now it is possible to build pongoOS:

```
$ git clone --recursive https://github.com/checkra1n/pongoOS
$ cd iOS15
$ git checkout -b iOS15
$ gmake -j$(nproc) EMBEDDED_CC=clang EMBEDDED_LDFLAGS=--ld-path=$(command -v ld64) all
```

The built file is at `build/Pongo.bin`.

## Building pongoterm

```
$ cd /path/to/pongoOS/scripts
```

### macOS

```
$ clang -Os -x objective-c -framework IOKit -framework CoreFoundation pongoterm.c -lobjc -framework Foundation -mmacosx-version-min=10.8 -o pongoterm
```

### Linux

libusb-1.0 is required:

```
$ clang -Os pongoterm.c -DUSE_LIBUSB -lusb-1.0 -o pongoterm
```

### Install to PATH

```
# install -m755 pongoterm /usr/local/bin/pongoterm
```

## Boot the thing

You need an arm64 Linux initramfs. It must be compressed with gzip.
Example [initramfs.gz](./binaries/initramfs.gz).

1. Prepare m1n1 blob

```
$ cat /path/to/m1n1/build/m1n1.bin <(echo 'chosen.bootargs=<kernel command line here>')  \
	/path/to/hoolock-linux/arch/arm64/boot/dts/apple/*.dtb \
	/path/to/hoolock-linux/arch/arm64/boot/Image.gz  \
	/path/to/initramfs.gz  > m1n1-linux.bin
```

2. Get palera1n

Get the latest pre-release from https://github.com/palera1n/palera1n/releases

or use their script:

```
# /bin/sh -c "$(curl -fsSL https://static.palera.in/scripts/install.sh)"
```

3. Boot pongoOS

```
# PALERA1N_BYPASS_PASSCODE_CHECK=1 palera1n -lp -k /path/to/pongoOS/build/Pongo.bin
```

Follow DFU instructions. On Apple TV 4K and T2 there you should enter DFU mode
directly beforehand. [Entering DFU Mode](https://theapplewiki.com/wiki/DFU_Mode)

There is a chance that this gets stuck after `Booting pongoOS...` but the device
visiually boots pongoOS fine. In that case it is okay to interrupt.

4. Boot m1n1+Linux

> [!TIP]
> To boot just m1n1 and not Linux, use `build/m1n1.bin` in the
m1n1 source tree instead of the combined `m1n1-linux.bin`file.

```
# printf '/send /path/to/m1n1-linux.bin\nbootm\n' | pongoterm 
```

After the device is visibly booting m1n1 it is okay to interrupt.

m1n1 will then parse the files appended to it, which, in this case is
Linux and boot it.

