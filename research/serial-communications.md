# on|poweron

* Serial established at `9600 baud`
* `MCU` (`U3`) sends `START\r\n` (`53 54 41 52 54 0D 0A`) to `JDY-40`

# on|remote

* `JDY-40` sends 3-byte packet to the `MCU` (`U3`)

> `XX` -> `0x60` + channel number [0-19]  
>  
> e.g. Channel 00 -> `0x60`  
> e.g. Channel 01 -> `0x61`  
> e.g. Channel 19 -> `0x73`

* `XXCAXX` - Tilt Up
* `XXC5XX` - Tilt Down
* `XXCCXX` - Pan Left
* `XXC3XX` - Pan Right
* `XXAnXX` - Pan Speed [`n = 1-8`]
* `XX5nXX` - Tilt Speed [`n = 1-8`]
