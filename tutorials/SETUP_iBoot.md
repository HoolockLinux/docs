# Linux/m1n1 boot setup (iBoot method)

In this method m1n1 becomes a fake restore kernel cache, and the
device is booted as if it is about to undergo a restore.

In this method, a set of files needs to be prepared for each model of device
that you want to boot m1n1 on, and the process require internet connection.
Once prepared, booting can be done offline.

This is a slightly more complicated method, with the upside that it is
independent of the files stored on disk.

Get tools and libraries:

- [ipsw](https://github.com/blacktop/ipsw)
- [openssl 3 (static)](https://github.com/openssl/openssl) (or CommonCrypto on macOS);
- [libusb 1.0 (static)](https://github.com/libusb/libusb) (Linux only, macOS can use IOKit for libirecovery);
- [libirecovery](https://github.com/libimobiledevice/libirecovery);
- [hBootPatcher](https://github.com/HoolockLinux/hBootPatcher);
- [gaster(verygenericname)](https://github.com/verygenericname/gaster)

Make sure `hBootPatcher`, `irecovery`, `gaster` and `ipsw` is in your `PATH`

## m1n1 Mach-O blob

Create the m1n1 Mach-O blob:

```
$ cat /path/to/m1n1/build/m1n1-idevice.macho <(echo 'chosen.bootargs=<kernel command line here>')  \
	/path/to/hoolock-linux/arch/arm64/boot/dts/apple/*.dtb \
	/path/to/hoolock-linux/arch/arm64/boot/Image.gz  \
	/path/to/initramfs.gz > m1n1-linux.macho
```

## DFU mode

Put your device into [DFU mode](https://theapplewiki.com/wiki/DFU_Mode).
For iPhone, iPads and Apple TV HD, you can use palera1n's DFU helper as follows:

```
# palera1n -D
```

## Using remote_boot script

- [remote_boot](https://github.com/HoolockLinux/remote_boot)

The prepare process will download the required files from Apple's
server, patch and store them for later use when booting.

### Prepare (once per model)

```
# ./remoteboot.sh prep
```

Note that on A7, it is neccessary to replug the device multiple times for
the exploit to succeed on a Linux host.

Example output (A9X, S8001, J98aAP):

```
[*] Waiting for device in DFU mode
[*] Detected device
usb_timeout: 5
usb_abort_timeout_min: 0
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Stage: RESET
ret: true
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Stage: SETUP
ret: true
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Stage: SPRAY
ret: true
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Stage: PATCH
ret: true
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Now you can boot untrusted images.
/tmp/tmp.l0qBjoshLN /home/nick/Documents/hoolock/remote-boot
   • Parsing remote IPSW       build=20E246 device=iPad6,7 signed=false version=16.4
   • Downloading files matching pattern "^BuildManifest.plist$"
        347.50 KiB / 347.50 KiB [==========================================================| ✅  ] 
      • Created 20E246__iPad6,7/BuildManifest.plist
   • Parsing remote IPSW       build=20E246 device=iPad6,7 signed=false version=16.4
   • Downloading files matching pattern "^Firmware/dfu/iBSS.ipad6d.RELEASE.im4p$"
        232.56 KiB / 232.56 KiB [==========================================================| ✅  ] 
      • Created 20E246__iPad6,7/iBSS.ipad6d.RELEASE.im4p
   • Parsing remote IPSW       build=20E246 device=iPad6,7 signed=false version=16.4
   • Downloading files matching pattern "^Firmware/dfu/iBEC.ipad6d.RELEASE.im4p$"
        437.14 KiB / 437.14 KiB [==========================================================| ✅  ] 1.04 MiB/s
      • Created 20E246__iPad6,7/iBEC.ipad6d.RELEASE.im4p
   • Parsing remote IPSW       build=20E246 device=iPad6,7 signed=false version=16.4
   • Downloading files matching pattern "^Firmware/all_flash/DeviceTree.j98aap.im4p$"
        25.03 KiB / 25.03 KiB [==========================================================| ✅  ] 
      • Created 20E246__iPad6,7/DeviceTree.j98aap.im4p
usb_timeout: 5
usb_abort_timeout_min: 0
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Now you can boot untrusted images.
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
Found the USB handle.
usb_timeout: 5
usb_abort_timeout_min: 0
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Now you can boot untrusted images.
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
Found the USB handle.
   • Extracting Payload        path=/tmp/tmp.l0qBjoshLN/DeviceTree_j98aap_iPad6,7.bin
starting hBootPatcher
patch_iboot: iboot_base = 0x180000000
patch_iboot: iboot_end = 0x18003a1c0
patch_iboot: Image is stage1
patch_ret0: Found ret0 gadget=180002178
patch_sigcheck: Found sigcheck
starting hBootPatcher
patch_iboot: iboot_base = 0x8f0000000
patch_iboot: iboot_end = 0x8f006d400
patch_iboot: Image is stage2
patch_ret0: Found ret0 gadget=8f0003790
patch_sigcheck: Found sigcheck
patch_iorvbar: Found IORVBAR
patch_aes: Found AES
patch_recfg: Found recfg
/home/nick/Documents/hoolock/remote-boot
   • Created IMG4              path=./cache/iBSS_j98aap_iPad6,7.img4 size=244 kB
   • Created IMG4              path=./cache/iBEC_j98aap_iPad6,7.img4 size=454 kB
   • Created IMG4              path=./cache/RestoreDeviceTree_j98aap_iPad6,7.img4 size=160 kB
```

### Boot

Pass the m1n1 Mach-O blob and the stub monitor Mach-O to `remoteboot.sh`

```
# ./remoteboot.sh boot /path/to/m1n1-linux.macho /path/to/m1n1/build/monitor-stub.macho
```

On certain setups a cable replug may be required after sending iBSS or iBEC.

Example output (A9X, S8001, J98aAP):

```
[*] Waiting for device in DFU mode
[*] Detected device
usb_timeout: 5
usb_abort_timeout_min: 0
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
CPID: 0x8001
Found the USB handle.
Now you can boot untrusted images.
   • Created IMG4              path=/tmp/tmp.l6qxFOWwpV/RestoreTrustCache_j98aap_iPad6,7.img4 size=6.2 kB
   • Created IMG4              path=/tmp/tmp.l6qxFOWwpV/RestoreKernelCache_j98aap_iPad6,7.img4 size=14 MB
usb_timeout: 5
usb_abort_timeout_min: 0
[libusb] Waiting for the USB handle with VID: 0x5AC, PID: 0x1227
Found the USB handle.
[==================================================] 100.0%
[*] Sent iBSS. A cable replug may be required on some setups.
[==================================================] 100.0%
[*] Sent iBEC. A cable replug may be required on some setups.
[==================================================] 100.0%
[==================================================] 100.0%
[==================================================] 100.0%
[*] Booted device
```

In the example output the device is already pwned from the prepare step, so there
is less gaster output.

After this step, the device will boot m1n1, which will then boot Linux.
