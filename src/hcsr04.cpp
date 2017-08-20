#include "hcsr04.h"
#include "usensor_array.h"
#include <pigpio.h>
#include <iostream>


/*

The Timing diagram is shown below. You only need to supply a short 10uS
pulse to the trigger input to start the ranging, and then the module will 
send out an 8 cycle burst of ultrasound at 40 kHz and raise its echo. The 
Echo is a distance object that is pulse width and the range in proportion.
You can calculate the range through the time interval between sending trigger 
signal and receiving echo signal. Formula: uS / 58 = centimeters; or: the
range = high level time * velocity (340M/S) / 2; we suggest to use over 60ms
measurement cycle, in order to prevent trigger signal to the echo signal.
        +------+
        |      |
        |      | 10uS trig
        |      |
________|      |_______________________________________________________________

                +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ 
                | | | | | | | | | | | | | | | | 
                | | | | | | | | | | | | | | | |  8x40KHz signal 
                | | | | | | | | | | | | | | | | 
________________| |_| |_| |_| |_| |_| |_| |_| |________________________________

                                               +--------------------------+
                                               |                          |
                                               |                          |
                                      echo     |< ---- response wave ---->|
                                               |                          |
_______________________________________________|                          |____

*/


#define DEBUG

std::mutex   mx;

HCSR04::HCSR04(unsigned trigPin, unsigned echoPin)
              :trigPin(trigPin),
               echoPin(echoPin)

{
    gpioSetMode(trigPin, PI_OUTPUT);
    gpioSetMode(echoPin, PI_INPUT);
    gpioWrite(trigPin,0);
    trigPoint     = std::chrono::microseconds(100); //default
    trigDuration  = std::chrono::microseconds(10);
    cycleDuration = std::chrono::milliseconds(60);
}

void HCSR04::setTrigPoint(usDuration & t) {
    trigPoint = t;
}

void HCSR04::trig() {
   
   timePoint start;
   timePoint end;
   
#ifdef DEBUG 
   {
     sched_param sch;
     int policy; 
     pthread_getschedparam(pthread_self(), &policy, &sch);
     std::lock_guard<std::mutex> lock(mx);
     std::cout << "Thread is executing at priority " << sch.sched_priority << '\n';
   }
#endif

   gpioWrite(trigPin,0);
#ifdef DEBUG
   start = std::chrono::steady_clock::now();   
#endif

   std::this_thread::sleep_for( trigPoint );

#ifdef DEBUG
   end = std::chrono::steady_clock::now();   
   {
     std::lock_guard<std::mutex> lock(mx);
     std::cout << "Trig Point Duration "
             << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
   }
#endif     

   gpioWrite(trigPin,1);

#ifdef DEBUG
   start = std::chrono::steady_clock::now();   
#endif

   std::this_thread::sleep_for( trigDuration );

#ifdef DEBUG
   end = std::chrono::steady_clock::now();   
#endif     
   gpioWrite(trigPin,0);
   
#ifdef DEBUG    
    {
     std::lock_guard<std::mutex> lock(mx);
     std::cout << "Trig Duration "
             << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
    }
#endif    
   
   while ( gpioRead(echoPin) == PI_OFF )
     start = std::chrono::steady_clock::now();
   
   
   while ( gpioRead(echoPin) == PI_ON )
     end = std::chrono::steady_clock::now();
   
   echoDuration = end-start;
   
   std::this_thread::sleep_for( cycleDuration );
}


usDuration HCSR04::getEchoDuration() {
   return echoDuration;
}





