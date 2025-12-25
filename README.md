# Hoolock Linux Documentation

Documentation for Linux on A7-A11, T2 Apple devices.

This document contains information about building and booting the kernel.

For feature support, see [FEATURES.md](./FEATURES.md).

Excluded devices:
  - HomePod (not tested, so no device tree included)

Supported firmware versions: iOS/iPadOS/tvOS 9.0 - 18.0

## How to boot?

Check [SETUP.md](./tutorials/SETUP.md). There are two methods but you are recommended
to use the pongoOS method.

Internal storage is supported on Apple A11. Compiled [tools](./tools/README.md) are
provided to help you partition the disk.

Click [here](./images/m1n1/README.md) to see how the screen looks like while in m1n1.

## Support & Community

You can open an [issue](https://github.com/hoolocklinux/docs/issues/new) on this repository for support.

The following chatrooms are also available:

- [Matrix (The Apple Basement)](https://matrix.to/#/#theapplebasement:matrix.org)

This one is for development and **NOT** for support:

- [Discord (Hack Different #linux)](https://discord.gg/NAxRYvysuc)


## Acknowledgements

- [Asahi Linux](https://asahilinux.org/) - m1n1 and Apple M1 Linux support
- [checkra1n](https://checkra.in) - pongoOS and checkm8 exploit implementation
- [Konrad Dybcio](https://konradybcio.pl) - Original linux-apple porter
- [Corellium](https://github.com/corellium) - Sandcastle Linux port

----

*Linux® is the registered trademark of Linus Torvalds in the U.S. and other countries.*
