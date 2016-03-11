# ESP8266 Smartwatch
## Pictures / Videos
![Features overview](http://i.imgur.com/IAOPWqe.jpg)
![Side view](http://i.imgur.com/wB7QXaz.jpg)
![On a wrist](http://i.imgur.com/c6pgJzJ.jpg)

#### YouTube Videos
* [Building the watch](https://www.youtube.com/watch?v=ve8lzYSVnGo)
* [Minetest controller](https://www.youtube.com/watch?v=W3ALEUs_7kA)
* [Compass test](https://www.youtube.com/watch?v=Mj3a6peVI30)

# Hardware
## Specifications
* ESP8266 with 32Mbit of flash memory
* MPU-9250 gyroscope, accelerometer with integrated AK8963 magnetometer
* RV-3029-C2 Real Time Clock
* FT232RQ for USB communication
* SSD1306 OLED Display, 128x64
* 3 Hardware Buttons
* MCP73831 LiPo charger
* 250mAh LiPo battery
* Dimensions: 35 * 39 * 11mm
* WiFi with chip antenna
* Vibration Motor

See `board/v1/README.md` for a detailed parts list.

# Software
The firmwares in `firmwares` use the [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk). You may need to customize the `main.c` files to reflect your WiFi configuration. This is mainly a hardware project and not a software project, so the firmwares are primarily meant to be demos for the hardware features of the watch. Firmwares can be uploaded using [esptool](https://github.com/themadinventor/esptool) and debugging is possible with a serial port terminal like [espterm](https://github.com/Jeija/espterm).

# Attribution
* `libesphttpd` (used in the minetest firmware) was [written by Spritetm](https://github.com/Spritetm/libesphttpd) and contributors and is licensed under "THE BEER-WARE LICENSE".
* `httpclient.c` / `httpclient.h` (included in all firmwares) was [written by Caerbannog](https://github.com/Caerbannog/esphttpclient) and contributors and is licensed under "THE BEER-WARE LICENSE".
* `i2c_master.h` / `i2c_master.c` was written by Espressif Systems

# License
The hardware design (schematic, PCB design, footprints, ...) is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/) (CC-BY-NC-SA).

The firmware source code is licensed under the [MIT License](https://opensource.org/licenses/MIT).
