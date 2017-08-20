#include "usensor_array.h"
#include <pthread.h>
#include <pigpio.h>
#include <iostream>

SensorArray::SensorArray() {
}

void SensorArray::registerSensor(HCSR04 & s) {
   sVec.push_back(s);
}

size_t SensorArray::getSensorsCount() {
  return sVec.size();
}

void SensorArray::trigSensors() {
          
  count = 0;
  
  for (auto s: sVec) {
      std::thread th( [=]() {
          sched_param sch;
          int policy; 
          pthread_getschedparam(pthread_self(), &policy, &sch);
          sch.sched_priority = 99;
          if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch)) {
             std::cerr << "Failed to setschedparam: "  << '\n';
          }

          std::this_thread::sleep_for( std::chrono::milliseconds(10) );
          
          //spin wait barrier
          while ( count.load() == 0 )
                    ;
          s.get().trig();
      });

      thVec.push_back(std::move(th));
  }
  
  //resume all threads from wait spin
  count++;
  
  
  //wait all threads finish their job
  for (auto& th: thVec)
     th.join();

  thVec.clear();

}



