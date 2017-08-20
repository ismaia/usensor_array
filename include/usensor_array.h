#ifndef __SENSOR_ARRAY__
#define __SENSOR_ARRAY__ 

#include "hcsr04.h"
#include <pthread.h>
#include <thread>
#include <vector>

class SensorArray {
  
  public:
      SensorArray();
      void   registerSensor(HCSR04 & s);
      void   trigSensors();
      size_t getSensorsCount();
      void   trigSensor(HCSR04 & s);
      void   setTrigPoint(HCSR04 & s);
  
  private:
      std::vector<std::thread>                     thVec;
      std::vector<std::reference_wrapper<HCSR04>>  sVec;
      std::atomic<unsigned int>                    count;
};      

#endif
