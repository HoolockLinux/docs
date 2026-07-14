---
draft: true
---
# Copyright policy

HoolockLinux is an open source project, and all contributions must follow the appropriate open source licenses.

These contribution rules are particularly important for code that is to be upstreamed into other projects, to maintain a clean paper trail of the licensing.

## Provisonal Policies

The full policies are still being worked out, in the mean time, provisonally, the following Asahi Linux's policies apply

[Asahi Linux's Copyright policy](https://github.com/AsahiLinux/AsahiLinux.github.io/blob/54f1f01bb31343c6c7c9ed65eaeed76e4032bdc2/content/copyright.md) and [Slop Policy](https://github.com/AsahiLinux/docs/blob/7c295dbb8af70a197f9ec705ca95dc35caa0a077/docs/project/policies/slop.md).

Swap `s/Asahi Linux/HoolockLinux/g` and `s/macOS/iOS/g`, note hypervisor do not work on these devices without EL2,
we offer a minimal replacement targeting iBoot at [iboot_tracer](https://github.com/hoolockLinux/iboot_tracer).

## Repositories with strict policies

Repositories downstream of Asahi Linux are also subject to [Asahi Linux's Copyright policy](https://asahilinux.org/copyright/), and
[Slop Policy](https://asahilinux.org/docs/project/policies/slop/)

Repositroies included are: m1n1, linux

For Linux specifically, in thoery only the `Apple SoC` tree is potentially subject to Asahi Linux's Copyright policy,
however given that device tree files and some platform-specific drivers will pass through that tree, it is this project's
policy to consider all contributions to Linux being potentially subject to Asahi Linux's Copyright policy.

## Exceptions

Exceptions applies to projects that are not strictly subject to Asahi Linux policies.

### Patchfinder exception

A challenge of this project directly adopting a strict reverse enginnering policy is
patchfinders being required for certain aspects, like patching iBoot for tracing and
signature check, as well as scanning the XNU kernelcache for firmwares.

It is generally impossible to use a traditional clean-room approach to writing
patchfinders, as they necessarily contain strings or instruction patterns from
the original binaries.

Therefore, it is permisible to write patchfinders using knowledge gained from binary
reverse enginnering without using a clean-room approach subject to these constriants:

**Do's:**

- Ensure a patchfinder is the optimal way of achieving the goal
- Include (sub)strings verbatim from the original binary only if is used for patchfinding
- If you use instruction bitmasks, you may add comments denoting what instructions they are
- For each instruction bitmask, you may include one example diassembly as comments,
    including only the instructions required to understand the instruction bitmask.
- If you inject shellcode into the proprietary binaries, then that shellcode must be permessively licensed
- Reimplement functions of less than 10 instructions long based on knowledge gained from binary reverse
    enginnering. (e.g. hooking system register writes)
- Stub/Nop/Fix return values of functions

**Don'ts:**

- Write patchfinders when it is not neccessary
- Reimplement functions longer than 10 instruction based on knowledge gained from binary reverse
    enginnering without using a clean-room approach documenting the function's ABI.
- Include unneccrssary strings from the original binary, be it in comments or code
- Include more than one disassembly example per instruction bitmask
- Include more instructions than neccessary in the disassembly examples
- Include decompiler outputs
- Include system register names in disassemblies, if you believe they may be sourced from leaked materials
- Inject copyleft code into proprietary binaries
