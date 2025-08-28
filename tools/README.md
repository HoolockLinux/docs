# Support Tools

This directory contain support tools for HoolockLinux.
They are intended to be ran under jailbroken iOS/tvOS/bridgeOS, or a SSH ramdisk.

Download precompiled tools: [iOS/iPadOS](https://nightly.link/hoolocklinux/docs/workflows/build/master/hoolock-support-iphoneos.zip) | [tvOS](https://nightly.link/hoolocklinux/docs/workflows/build/master/hoolock-support-appletvos.zip) | [bridgeOS](https://nightly.link/hoolocklinux/docs/workflows/build/master/hoolock-support-bridgeos.zip)

## Disk partition tools

> [!WARNING]
> These tools could potentially destroy all data on the main disk, so double check.

### gdisk

This is the usual gpt partitioning tool found on many unixes.

It is used to inspect and modify the partition table.

It should be signed with entitlements in `gdisk.xml`.

### resize.c / resize_apfs

This is an apfs container resize tool.

It should be signed with entitlements in `resize.xml`.

Usage: `./resize_apfs <container BSD name> <size in bytes>`

Resize the container to N bytes, such as 21474836480 in the following example.
There are alignment restrictions so pick a 1048576-byte aligned size.

```
./resize_apfs disk0s1 21474836480
```

It is also possible to resize the container to the max size:

> [!CAUTION]
> On iOS 13 and below this might ignore the gpt and resize the apfs to max which would break the filesystem. Recommend specifying the exact partition size instead.

```
./resize_apfs disk0s1 0
```

During an online resize, all file system operations will be frozen, so in normal iOS the device will become unresponsive

After resizing, the gpt can be modified with gdisk.


### Caveats

On iOS 11-13 the default snapshot makes the container unresizable.
One can delete and recreate the snapshot with a tool like [`snaputil`](https://github.com/ahl/apfs) from ramdisk and resizing should work.

In order for a resize to work, the SEP firmware must be loaded, and gigalocker
must be initialized. This is already done in normal iOS. In a ramdisk, this can
also be done on iOS/iPadOS 15 and below.
(Might also be possible on higher tvOS/bridgeOS versions)

If you use [SSHRD_Script](https://github.com/verygenericname/SSHRD_Script.git), the following command should do:

```
mount_filesystems
```

In a ramdisk, you might need somewhere writable to put the tools. On iOS 14 and above this is simple:

```
mount_tmpfs /mnt5
```

On iOS 13 and below, tmpfs is not supported, so a ramdisk needs to be created and formatted:

```
disk=$(hdik ram://16384) # 16384 KiB
newfs_hfs $disk
mount_hfs $disk /mnt5
```
