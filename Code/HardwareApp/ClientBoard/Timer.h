/*

   Timer class that manages timing
*/

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
  public:

    Timer(int d) {
      duration = d;
      done = false;
      enable = true;
    }

    //update time
    void update(unsigned long time) {
      if (enable == true) {
        if (time - cTime >  duration) {
          done = true;
          enable = false;
          //Serial.println("passed");
          //Serial.println(time - cTime);
        }
      }
    }

    //return done state
    boolean isDone() {
      return done;
    }

    //reset the time once its done
    void reset() {
      cTime = millis();
      done = false;
      enable = true;
      //Serial.println("rest");
    }

  private:

    boolean enable;
    boolean done;

    unsigned long cTime;
    unsigned long pTime;

    unsigned long duration;
};


#endif
