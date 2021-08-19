# JDY-40

> 2.4 GHz wireless serial port module

|PIN|TYPE|
|:---:|:---:|
|10|RXD|
|11|TXD|
|12|SET|
|13|CS|

* Serial to communicate motor speed
* Only one value changed at time
  * `U3` cannot do both motor operations at the same time

-----

# TC118S

> DC motor driver

* `M1` - Pan motor
* `M2` - Tilt motor

## Motor voltage readouts

* Pan - 6.19 VDC
* Tilt - 5 VDC

Polarity is dependent on direction

|PIN|TYPE|
|:---:|:---:|
|1|_- nc -_|
|2|INA|
|3|INB|
|4|VDD|
|5|M+|
|6|GND|
|7|GND|
|8|M-|


---

# TM1668

> LED Display and Keypad driver

meh

---

# Chip U3

> MCU, proprietary

|PIN|MAP|DESC|
|:---:|:---:|:---|
|1|`EY-40.CS`|Control|
|2|`EY-40.RXD`|Transmit serial|
|3|`EY-40.TXD`|Receive serial|
|4|`EY-40.SET`|Control|
|5|-|???|
|6|`K1`|Power button|
|7|`GND`|Ground|
|8|-|??? Connected to `J1.3`|
|9|-|??? Connected to `J1.2`|
|10|`K2`|Channel button|
|11|`K4`|+ button|
|12|`K3`|- button|
|13|`M2.INA`|Tilt motor control|
|14|`M2.INB`|Tilt motor control|
|15|`M1.INA`|Pan motor control|
|16|`M1.INB`|Pan motor control|
|17|`TM1668.PIN3`|meh|
|18|-|???|
|19|`TM1668.PIN2`|meh|
|20|`TM1668.PIN1`|meh|

---

# `J1`

> Test connections

|PIN|MAP|DESC|
|:---:|:---:|:---|
|1|`SW`|Blue cable for tilt motor power|
|2|`U3.PIN9`|???|
|3|`U3.PIN8`|???|
|4|`GND`|Ground (?)|
|5|`EY-40.TXD`|Received wireless serial data|

-------

# Power

* `SW` - power to the tilt motor `M2` - cut when one of the stop switches are pressed
