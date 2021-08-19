# Method 1

Replace the `EY-40` module with an MCU (i.e. `ESP8266` / `ESP32`) to allow WiFi control.  
Will however not allow both motors to operate simultaneously as the original MCU (`U3`)  does not support it

---

# Method 2

Place the new `MCU` inline between the `EY-40` and `U3`, passthrough serial commands but also add own with priority.  
Need to ensure that the `EY-40` isn't interrupted (are they always 3 byte messages?).  \

---

# Method 3

Place both the `EY-40.TX` and new `MCU.TX` parallel to `U3.RX`.  
Will need to pull `MCU.TX` (high???) to allow `EY-40.TX` to still operate.  
Or maybe use diodes?

---

# Method 4

Replace `U3` entirely.  
Big ceebs.

---

# Method 5

Place the new `MCU` inline between `U3` and the two motor drivers (`TC118S.PAN`, `TC118S.TILT`).  
Power from `SW` to `TC118S.TILT` is connected on the PCB, so just intercept and pass through `INA` and `INB` states.  
A bit laborious to desolder the TC118S ICs and then find a PCB to put them on

---

# Method 6

Buy my own `JDY-40` and just emit the same wireless serial signals from an `MCU`.  
Could be either built into the device, or just near me.