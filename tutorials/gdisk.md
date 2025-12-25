# gdisk Tutorial

Tutorial using gdisk on jailbroken iOS/tvOS/bridgeOS.

## Notes

This tutorial is for people that do not have experience with command-line
partitioning with gdisk on Linux/Unix. If you know already, you may skip
reading. Keep in mind that the sector size is `4096`.

### Common Partition codes

These partition codes may be used later.
```
0700 = Microsoft basic data
8200 = Linux swap
8300 = Linux filesystem
AF00 = Apple HFS/HFS+
AF0A = Apple APFS
```

## Partitioning

You should have ran `resize_apfs` to resize the apfs filesystem already.
After resizing the filesystem, the partition table needs to be updated.

Start by running gdisk against the main disk.
```
# ./gdisk /dev/disk0
```

You should get:
```
GPT fdisk (gdisk) version 1.0.10

Warning: Devices opened with shared lock will not have their
partition table automatically reloaded!
Partition table scan:
  MBR: protective
  BSD: not present
  APM: not present
  GPT: present

Found valid GPT with protective MBR; using GPT.

```

Type `p` and enter to print the current partition table:

```
Command (? for help): p
Disk /dev/disk0: 62500000 sectors, 238.4 GiB
Sector size (logical): 4096 bytes
Disk identifier (GUID): 37FE5887-699F-463D-A34F-98CA0D99FBA3
Partition table holds up to 128 entries
Main partition table begins at sector 2 and ends at sector 5
First usable sector is 6, last usable sector is 62499994
Partitions will be aligned on 2-sector boundaries
Total free space is 1 sectors (4.0 KiB)

Number  Start (sector)    End (sector)  Size       Code  Name
   1               6        62499993   238.4 GiB   AF0A  Apple APFS

```

Then we will actually delete this partition since there's no such
thing as resizing partition in gdisk. The changes are not written
out until a `w` command is issued.

Type `d` and enter to delete a partition. Since there is only one
partition, the partition will be automatically selected and
deelted.

```
Command (? for help): d
Using 1

```

Then we need to create a new partition that exactly matches the size
of the resized filesystem. Start and end are inclusive and each
sector is 4096 bytes long.

Type `n`, enter, and `1`, enter. For start, keep it at `6` like it
originally is. For end, suppose the filesystem is resized to be
`192000000000` bytes (this size must be sector-aligned), then the
last sector is `(6 + (192000000000) / 4096) - 1 = 46875005` In
general for a 4096-byte aligned filesystem size `n`, the last sector
is `(start + (size) / 4096) - 1`.

Type start and end sector. As for type code, use `AF0A` for `Apple APFS`:

```
Command (? for help): n
Partition number (1-128, default 1): 1
First sector (6-62499994, default = 6) or {+-}size{KMGTP}: 6
Last sector (6-62499994, default = 62499993) or {+-}size{KMGTP}: 46875005
Current type is AF00 (Apple HFS/HFS+)
Hex code or GUID (L to show codes, Enter = AF00): AF0A
Changed type of partition to 'Apple APFS'
```

Now the APFS partition is restored. Custom partitions can now be created,
suppose we want a partition to hold the rootfs:

```
Command (? for help): n
Partition number (2-128, default 2): 2
First sector (46875006-62499994, default = 46875006) or {+-}size{KMGTP}: 46875006
Last sector (46875006-62499994, default = 62499993) or {+-}size{KMGTP}: 61035156
Current type is AF00 (Apple HFS/HFS+)
Hex code or GUID (L to show codes, Enter = AF00): 8300
Changed type of partition to 'Linux filesystem'
```

where 8300 = `Linux filesystem`

And a partition to hold the boot files:

```
Command (? for help): n
Partition number (3-128, default 3): 3
First sector (61035157-62499994, default = 61035158) or {+-}size{KMGTP}: 61035264
Last sector (61035264-62499994, default = 62499993) or {+-}size{KMGTP}: 61157119
Current type is AF00 (Apple HFS/HFS+)
Hex code or GUID (L to show codes, Enter = AF00): 8300
Changed type of partition to 'Linux filesystem'
```

where 8300 = `Linux filesystem`

A FAT32 partition to hold earlier boot files:

```
Command (? for help): n
Partition number (4-128, default 4): 4
First sector (61035157-62499994, default = 61157120) or {+-}size{KMGTP}: 61157120
Last sector (61157120-62499994, default = 62499993) or {+-}size{KMGTP}: 61222655
Current type is AF00 (Apple HFS/HFS+)
Hex code or GUID (L to show codes, Enter = af00): 0700
Changed type of partition to 'Microsoft basic data'
```

where 0700 = `Microsoft basic data`

A swap partition:

```
Command (? for help): n
Partition number (5-128, default 5): 5
First sector (61035157-62499994, default = 61222656) or {+-}size{KMGTP}: 61222656
Last sector (61222656-62499994, default = 62499993) or {+-}size{KMGTP}: 62499839
Current type is af00 (Apple HFS/HFS+)
Hex code or GUID (L to show codes, Enter = af00): 8200
Changed type of partition to 'Linux swap'
```

In this example, 4 new partitions are created and the existing APFS partition is
shrunk. Type `p` to print the new partition table and CHECK CAREFULLY to avoid
data loss:

```
Command (? for help): p
Disk /dev/disk0: 62500000 sectors, 238.4 GiB
Sector size (logical): 4096 bytes
Disk identifier (GUID): 37FE5887-699F-463D-A34F-98CA0D99FBA3
Partition table holds up to 128 entries
Main partition table begins at sector 2 and ends at sector 5
First usable sector is 6, last usable sector is 62499994
Partitions will be aligned on 2-sector boundaries
Total free space is 262 sectors (1.0 MiB)

Number  Start (sector)    End (sector)  Size       Code  Name
   1               6        46875005   178.8 GiB   AF0A  Apple APFS
   2        46875006        61035156   54.0 GiB    8300  Linux filesystem
   3        61035264        61157119   476.0 MiB   8300  Linux filesystem
   4        61157120        61222655   256.0 MiB   0700  Microsoft basic data
   5        61222656        62499839   4.9 GiB     8200  Linux swap
```

When everything is good, type `w`, enter, confirm with `y` to actually
write the partition table:

```
Command (? for help): w

Final checks complete. About to write GPT data. THIS WILL OVERWRITE EXISTING
PARTITIONS!!

Do you want to proceed? (Y/N): y
OK; writing new GUID partition table (GPT) to /dev/disk0.
Warning: Devices opened with shared lock will not have their
partition table automatically reloaded!
Warning: The kernel may continue to use old or deleted partitions.
You should reboot or remove the drive.
The operation has completed successfully.
```

Now, check `/dev` to ensure the partitions `/dev/disk0s2`,
`/dev/disk0s3`, ... are created.

Now the partitions has been created. However, there are no filesystem
on them. You will need to flash prebuilt filesystem images with `dd`,
and for some other partitions, format them once booted into Linux.
The filesystem images likely has to have a block size of 4096.

For example, to flash a filesystem image, `root.img` into the second
partition:

```
# dd if=root.img of=/dev/disk0s2
```

For Apple A11, the partition will show up as `/dev/nvme0n1p...` in Linux.
For example `/dev/disk0s2` is `/dev/nvme0n1p2` in Linux.
