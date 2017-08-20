#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "usensor_array.h"

#define S1_TRIG 23
#define S1_ECHO 24

#define S2_TRIG 8
#define S2_ECHO 25


int main(int argc, char *argv[])
{
  
  std::cout << "Utrasonic Sensor Array\n";

  if (gpioInitialise() < 0) {
      std::cerr << "pigpio initialisation failed\n" << std::endl;
      return 1;
  }

  SensorArray sa;

  HCSR04 s1(S1_TRIG,S1_ECHO);
  HCSR04 s2(S2_TRIG,S2_ECHO);
  
  sa.registerSensor(s1);
  sa.registerSensor(s2);
  
  std::chrono::duration<double, std::micro> t1(10);
  std::chrono::duration<double, std::micro> t2(25);

  std::cout << "Sensors Count: " << sa.getSensorsCount() << std::endl;
  
  s1.setTrigPoint(t1);
  s2.setTrigPoint(t2);
  
  for (int i=0; i < 1 ; i++) {
    sa.trigSensors();

    std::cout << "----------------- Sensor 1------------------\n";
    std::cout << "Echo Duration: " << s1.getEchoDuration().count() << " us\n";
    std::cout << "Distance : " << s1.getEchoDuration().count() / 58.0 << " cm\n";
  
    std::cout << "----------------- Sensor 2------------------\n";
    std::cout << "Echo Duration: " << s2.getEchoDuration().count() << " us\n";
    std::cout << "Distance : " << s2.getEchoDuration().count() / 58.0 << " cm\n";
  }

}
