# Ultrasonic Sensor Array

Real-time control of phased array composed of HC-SR04 Ultrasonic Sensors

The system is designed to run in a Raspberry PI 3 + Linux-RT

The max number of sensors is limited to GPIOs available on RPI3



## Installation
  * Dependencies: pigpio, g++, cmake, linux-rt

```
  cd
  git clone https://github.com/joan2937/pigpio
  cd pigpio
  make install
  cd
  mkdir -p build
  cd build
  cmake ../ -DCMAKE_INSTALL_PREFIX=/root/usensor_array
```




