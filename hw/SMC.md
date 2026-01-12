# System Management controller (SMC)

Also see: https://asahilinux.org/docs/hw/soc/smc/#smc-keys

Information available from Asahi Linux are not listed.

Applies to T8015:

## SMC keys

- `#KEY`: Number of keys
- `aBID`: possibly Brick ID
- `CHBI`: Charge Brick Current (?)
- `CHBV`: Charge Brick Voltage (?)
- `D1AR`: Dock 0 AC Input current
- `D1IR`: Dock 0 AC Input current
- `D1VR`: Dock 0 AC input voltage
- `D1NO`: Dock 0 Apply AC settings
- `D2AR`: Dock 1 AC Input current
- `D2IR`: Dock 1 AC Input current
- `D2VR`: Dock 1 AC input voltage
- `D2NO`: Dock 1 Apply AC settings
- `gP??`: GPIO pin ??, ?? start from 01
- `MBSE`: Power status key. Read as boot stage in PMIC NVRAM. Write as:
    - `offw`: shutdown warning
    - `slpw`: sleep warning
    - `rest`: restart warning
    - `off0`: Shutdown
    - `susp`: Suspend (incorrect configuration leads to restart)
    - `panb`: Panic begin
    - `pane`: Panic end
- `TG0B`: Some sort of temperature, read by iBoot
- `VBUS`: USB VBUS state
- `bHLD`: Power button state
- `bVUP`: Volume up button state
- `bVDN`: Volume down button state
- `bRIN`: Mute switch state
- `rCTL`: Reset control, written before restart/shutdown

## Notifications

### Power events

`0x71...` are power events

```
(event & 0xffffff00) == 0x71010100; /* Charger status change */
(event & 0xffffff00) == 0x71010200; /* Wireless charger status change */
(event & 0xffff0000) == 0x71060000; /* Port charge state change */
(event & 0xffff0000) == 0x71070000; /* Charging coil state change */
(event & 0xffff0000) == 0x71130000; /* Connector insert/remove event */
```

### Button events

`0x7201...` are button events. Bit[15:8] indicate the event and bit[0:7] indicate the state.
`0x01` - event asserted, `0x00` - event deasserted.


- `0x720101..`: Power button
- `0x720102..`: Volume up
- `0x720103..`: Volume down
- `0x720104..`: Mute switch
- `0x720106..`: TouchID button (seen on T2)
- `0x7201fe..`: Force restart attempted; immediately seen when power button pressed in force restart sequence on iPhone X
- `0x720100..`: Force restart imminent; seen right before force restart
