# DART

DART, short for Device Address Resolution Table, is an IOMMU found on Apple SoCs, it is similar in function to the ARM SMMU.
It maps device-visible memory addresses into CPU-visible physical addresses. It helps protect main memory from peripherals
as well as allowing peripherals that are only capable of 32-bit addressing to access the memory at all, as all memory are
above 32-bit on Apple SoCs. DARTs can be operated in either translation mode, where all the memory mapping functions can
be used, or a bypass with offset mode where the addresses translation is merely adding a configurable offset.

Full DART bypass is NOT supported on A7-A11, T2, as none of the peripheral could address the required 36-bit physical
address space. It appears that dart lock is also not supported.

There are two variants used on A7-A11, T2 SoCs:

dart,s5l8960x, used on A7, A8, A8X, A9X, A10, A10X and T2

dart,t8015, used on A11 and T2 (yes T2 has two types of darts)

These two types of DARTs share the same page table format.

## dart,s5l8960x

This DART support four streams and four TTBRs (Translation Table Base register) per stream.

### Interrupts

An interrupt will be delievered when a device fails to DMA while
translation is enabled. This is typically the first interrupt in ADT.
The handler should read [ERROR, Error indication Register](#error-error-indication-register)
for more information.

### Registers
|       Name        | Width | Offset| Short Description              | Type |
|-------------------|:-----:|:-----:|:------------------------------:|:----:|
| [STREAM_COMMAND](#stream_command-stream-commnad-register)    |   4   | 0x00  | TLB cache control              | R/W  |
| [TCR](#tcr-translation-control-register)               |   4   | 0xc   | Translation Controls           | R/W  |
| [ERROR](#error-error-indication-register)             |   4   | 0x10  | Error indicator                | R/W  |
| [ERROR_ADDR_LO](#error_addr_lo-error-address-low)             |   4   | 0x1c  | Error Address (Low)               | R/W  |
| [DIAG_CONFIG](#diag_config-diagnostic-configuration)       |   4   | 0x20  | Unknown Tunables               | R/W  |
| [BYPASS_ADDR](#bypass_addr-bypass-mode-address)       |   4   | 0x2c  | Bit[35:32] in bypass mode      | R/W  |
| [FETCH_CONFIG](#fetch_config-fetch-configuration)      |   4   | 0x30  | Unknown Tunables               | R/W  |
| [TTBR](#ttbr-translation-table-base-register)              |   4   | 0x40 + 16 * stream + 4 * idx  | Translation Table Base Address, 4 per stream | R/W  |

### STREAM_COMMAND, Stream Command Register

This register allows TLB cache of a given stream to be invalidated.

|       Bits        | Name       | Type  | Description                             |      
|-------------------|:----------:|:-----:|:---------------------------------------:|
| [11]              | STREAM_3   | R/W   | Set stream 3 as command target          |
| [10]              | STREAM_2   | R/W   | Set stream 2 as command target          |
| [9]               | STREAM_1   | R/W   | Set stream 1 as command target          |
| [8]               | STREAM_0   | R/W   | Set stream 0 as command target          |
| [3]               | BUSY       | RO    | Indicate the TLB cache is being cleared |
| [1]               | INVALIDATE | RW    | Set to invalidate the TLB cache         |

For example, to invalidate the TLB cache for stream 0, write `STREAM_0 | INVALIDATE`
into the register, and when `BUSY` clears, the operation is completed.

**IMPORTANT**: `dart,s5l8960x` requires translation to be enabled in `TCR` for TLB
invalidtion, otherwise the busy bit never clears

### TCR, Translation Control Register

This register control translation on/off. When translation is off, the DART is in
bypass mode.

|       Bits        | Name       | Type  | Description                             |      
|-------------------|:----------:|:-----:|:---------------------------------------:|
| [31]              | STREAM_3   | R/W   | Translation on/off for stream 3         |
| [23]              | STREAM_2   | R/W   | Translation on/off for stream 2         |
| [15]              | STREAM_1   | R/W   | Translation on/off for stream 1         |
| [7]               | STREAM_0   | R/W   | Translation on/off for stream 0         |

### ERROR, Error indication Register

This register indicates an error has occured during DMA while translation is
enabled. This could be because the DART is not setup properly, or the memory
is write-protected. Writing the read value back to the registers clears the error.

|       Bits        | Name              | Type  | Description                                              |      
|-------------------|:-----------------:|:-----:|:--------------------------------------------------------:|
| [31]              | FLAG              | R/W   | When set, indicate that an error has occured             |
| [4]               | READ_FAULT       | R/W   | Some memory failed to be read by the device (since T8010)    |
| [3]               | WRITE_FAULT            | R/W   | Some memory failed to be write by the device     |
| [2]               | NO_PTE            | R/W   | Page table entry invalid                            |
| [2]               | NO_PMD            | R/W   | Page Middle Directory invalid                            |
| [0]               | NO_TTBR           | R/W   | TTBR register invalid                                    |

### ERROR_ADDR_LO, Error Address (Low)

|       Bits        | Name              | Type  | Description                                              |      
|-------------------|:-----------------:|:-----:|:--------------------------------------------------------:|
| [31:0]            | ADDR              | RO    | Low 32-bit of address that caused an error as indicated in the [ERROR](#error-error-indication-register) register. |

### DIAG_CONFIG, Diagnostic Configuration

This registers holds tunables. However, the tunables not appear to be required for the DART to operate.
A recommended value is in `diag-config` property of the ADT dart node.

### BYPASS_ADDR, Bypass Mode Address

Holds bit [35:32] of the phyiscal address when operating in bypass mode.
The IOVA is OR'd with the values stored in this register to get the PA.

|       Bits        | Name              | Type  | Description                                                 |      
|-------------------|:-----------------:|:-----:|:-----------------------------------------------------------:|
| [27:24]           | STREAM_3          | R/W   | Bit [35:32] of PA of stream 3 IOVA when in bypass mode      |
| [19:16]           | STREAM_2          | R/W   | Bit [35:32] of PA of stream 2 IOVA when in bypass mode      |
| [11:8]            | STREAM_1          | R/W   | Bit [35:32] of PA of stream 1 IOVA when in bypass mode      |
| [3:0]             | STREAM_0          | R/W   | Bit [35:32] of PA of stream 0 IOVA when in bypass mode      |

### FETCH_CONFIG, Fetch Configuration

This registers holds tunables. However, the tunables not appear to be required for the DART to operate.
A recommended value is in `fetch-config` property of the ADT dart node.

### TTBR, Translation table base register

Holds the base address of the translation table.
There are 4 TTBRs per stream. Offset formula:
`0x40 + 16 * stream + 4 * idx` where `stream` is the stream ID from 0-3
and `idx` is the TTBR to use from `0-3`.

|       Bits        | Name          | Type  | Description                      |      
|-------------------|:-------------:|:-----:|:--------------------------------:|
| [31]              | VALID         | R/W   | Indicate that this TTBR is valid |
| [23:0]            | ADDR          | R/W   | Page address (Bit [35:12]) of the translation table base. The translation table must be 4K page-aligened. |

### Programming Model

1. Setup pagetables
2. Set TTBRs pointing to pagetables
3. Enable Traslation in `TCR`
4. Invalidate TLB in `STREAM_COMMAND`
5. IOVA is now mapped

## dart,t8015

This DART has a similar register and bit layout as the `dart,t8020` as used on the M1. However,
it does not support some features including locking and subpage protection. 
It also has a 4K page size. The hardware appears to be able to support SID 0-15. However, only
the `/arm-io/dart-pup` instance on Apple T2 seems to support the full range of SIDs. (see
`sids` property in DART's ADT node). It can be seen as a 4K page subset of the M1 DART.

This DART appears to be designed with 39-bit addressing in mind, which doesn't mean much since
all applicable devices only supported 36-bit PA.

### Registers
|       Name        | Width | Offset| Short Description              | Type |
|-------------------|:-----:|:-----:|:------------------------------:|:----:|
| [PARAMS](#params-hardware-parameters-t8015)_sid_N           |   4   | 0x0  | Hardware parameters           | RO  |
| [STREAM_COMMAND](#stream_command-stream-command-register-t8015)    |   4   | 0x20  | TLB cache control              | R/W  |
| [STREAM_SELECT](#stream_select-stream-selection-register-t8015)    |   4   | 0x34  | Stream selection            | R/W  |
| [ERROR](#error-error-indication-register-t8015)             |   4   | 0x40  | Error indicator                | R/W  |
| [ERROR_ADDR_LO](#error_addr_lo-error-address-low-t8015)             |   4   | 0x50  | Error Address (Low)               | R/W  |
| [CONFIG](#config-configuration-register-t8015)       |   4   | 0x60  | Configuration               | R/W  |
| [TCR](#tcr-translation-control-register-t8015)_sid_N    |   4   | 0x100 + (N * 4)  | Bit[35:32] in bypass mode      | R/W  |
| [TTBR](#ttbr-translation-table-base-register-t8015)      |   4   | 0x200  | Unknown Tunables               | R/W  |

### PARAMS, Hardware Parameters, t8015

Holds some hardware configuration. Page size is not provided here for this DART. (It is all 4K anyways)

### STREAM_COMMAND, Stream Command Register, t8015

TLB invalidation

|       Bits        | Name       | Type  | Description                             |      
|-------------------|:----------:|:-----:|:---------------------------------------:|
| [1]               | INVALIDATE | RW    | Set to invalidate the TLB cache         |
| [0]               | BUSY       | RO    | Indicate the TLB cache is being cleared |

### STREAM_SELECT, Stream Selection Register, t8015

Select which streams will be affected by the a write to the `STREAM_COMMAND` registers.
Multiple may be specified at the same time. To select stream N, bit `1 << N` needs to
be set.

|       Bits        | Name       | Type  | Description                             |      
|-------------------|:----------:|:-----:|:---------------------------------------:|
| [15:0]            | STREAM     | R/W   | Stream ID bitmask                       |

### ERROR, Error indication Register, t8015

This register indicates an error has occured during DMA while translation is
enabled. This could be because the DART is not setup properly, or the memory
is write-protected. Writing the read value back to the registers clears the error.

|       Bits        | Name              | Type  | Description                                              |      
|-------------------|:-----------------:|:-----:|:--------------------------------------------------------:|
| [31]              | FLAG              | R/W   | When set, indicate that an error has occured             |
| [4]               | READ_FAULT       | R/W   | Some memory failed to be read by the device              |
| [3]               | WRITE_FAULT            | R/W   | Some memory failed to be write by the device                              |
| [2]               | NO_PTE            | R/W   | Page table entry invalid                            |
| [2]               | NO_PMD            | R/W   | Page Middle Directory invalid                            |
| [0]               | NO_TTBR           | R/W   | TTBR register invalid                                    |

### ERROR_ADDR_LO, Error Address (Low), t8015

|       Bits        | Name              | Type  | Description                                              |      
|-------------------|:-----------------:|:-----:|:--------------------------------------------------------:|
| [31:0]            | ADDR              | RO    | Low 32-bit of address that caused an error as indicated in the [ERROR](#error-error-indication-register) register. |

### CONFIG, Configuration register, t8015

This register might be what `diag-config` in ADT is referring to.

It appears locking is not supported in this version of DART.

### TCR, Translation Control Register, t8015

One register per stream. Register for steram `N` is at `(0x100 + N * 4)`

|       Bits        | Name              | Type  | Description                                               |      
|-------------------|:-----------------:|:-----:|:---------------------------------------------------------:|
| [7]               | TRANSLATION_EN    | R/W   | Enable Translate mode for the stream                      |
| [23:16]           | BYPASS_ADDR       | R/W   | Bit [39:32] of PA of stream IOVA when in bypass mode      |

### TTBR, Translation table base register, t8015

4 registers per stream. Register for steram `N` (0-15) ttbr `M` (0-3) is at `(0x200 + N * 16 + M * 4)`

|       Bits        | Name          | Type  | Description                      |      
|-------------------|:-------------:|:-----:|:--------------------------------:|
| [31]              | VALID         | R/W   | Indicate that this TTBR is valid |
| [27:0]            | ADDR          | R/W   | Page address (Bit [39:12]) of the translation table base. The translation table must be 4K page-aligened. |

## Page Table Format

### L0

This represents the amount of TTBRs per stream. On both `dart,s5l8960x` and `dart,t8015`
there are 4 TTBRs per stream.

### L3

|       Bits        | Name          | Type  | Description                        |      
|-------------------|:-------------:|:-----:|:----------------------------------:|
| [35:12]           | OFFSET        | R/W   | 4K Page offset ([35:12] of offset) |
| [8]               | RPROTECT      | R/W   | Read protection. (Supported since T8010) |
| [7]               | WPROTECT      | R/W   | Write protection. Device cannot write this page. |
| [1:0]             | VALID         | R/W   | Indicate PTE as valid |
