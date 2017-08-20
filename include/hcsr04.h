#ifndef __HCSRXX__
#define __HCSRXX__

#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>


using usDuration = std::chrono::duration<double, std::micro>; 
using msDuration = std::chrono::duration<double, std::milli>; 
using timePoint  = std::chrono::steady_clock::time_point;

class HCSR04 {
  
  public:
      HCSR04(unsigned, unsigned); 
      void                            trig();
      void                            setTrigPoint(usDuration & t);
      usDuration                      getEchoDuration();

  private:   
      unsigned                        trigPin;
      unsigned                        echoPin;
      usDuration                      trigPoint;
      usDuration                      trigDuration;
      usDuration                      echoDuration; 
      msDuration                      cycleDuration;

};

#endif


