With this code, you can display CO2, temperature, and humidity readings with a ESP32-2432S028 dev board (aka Cheap Yellow Display) and a SCD40/41 sensor.

Two versions of code, one is display only and the other will display and send readings to influxdb.

20ppm is added in the code as the SCD4x sensors auto calibrate to 400ppm, and atmospheric CO2 is closer to 420ppm (you can adjust as you like)

Use the cable supplied with the CYD plugged in to CN1

Black - Ground

Red - VCC

Blue - SCL

Yellow - SDA

You will need these libraries https://github.com/sparkfun/SparkFun_SCD4x_Arduino_Library and https://github.com/Bodmer/TFT_eSPI

![display](https://github.com/HenrysCat/ESP32-2432S028-SCD40x/assets/47726287/a4542213-df97-4935-a0f1-8af952b48c1c)

![ESP32-2432S028](https://github.com/HenrysCat/ESP32-2432S028-SCD40x/assets/47726287/e045494c-21f7-4ed2-9f22-85446f40eca7)

![SCD40](https://github.com/HenrysCat/ESP32-2432S028-SCD40x/assets/47726287/9dc3e654-f0bc-48ee-bf8c-4b21bd2c1f0c)
