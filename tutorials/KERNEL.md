# Kernel Config

Start with the `config_16k` in the repository. A9-A11, T2 should be fine with
this config. A7-A8X will need to change the page size.

Edit config with clang using:
```
make -j$(nproc) ARCH=arm64 LLVM=1 menuconfig
```

For 4K page size (A7-A8X), enable:

- CONFIG_ARM64_4K

For 16K page size (A9-A11, T2), enable:

- CONFIG_ARM64_16K
