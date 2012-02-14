/**
 * Computes Julian Day Number and date, given a Posix time_t
 *
 * Copyright 2011 Bruce Ide
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <time.h>
#include <math.h>

/**
 * JD uses localtime, so if you want GMT you'll have to unset your
 * timezone and call tzset.
 */

class JD {

  time_t startTime;
  time_t jdn;
  double jd;

 public:
 JD(time_t atTime) : startTime(atTime)
  {
    struct tm timeBuf;
    localtime_r(&startTime, &timeBuf);
    int year = timeBuf.tm_year + 1900;
    int month = timeBuf.tm_mon + 1;
    int day = timeBuf.tm_mday;
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    jdn = day + floor((153 * m + 2) / 5) + 365 * y + floor(y / 4) - floor(y/100) + floor(y/400) - 32045;
    jd = (double) jdn + ((double) timeBuf.tm_hour - 12.0) / 24.0 + ((double) timeBuf.tm_min / 1440.0) + (double) timeBuf.tm_sec / 86400.0;
  }

  /**
   * Returns Julian Day number for the day
   */

  time_t getJdn() {
    return jdn;
  }

  /**
   * Returns full julian date
   */

  double getJd() {
    return jd;
  }

  /**
   * Returns time you entered as the time to convert
   */

  time_t getPosixTime() {
    return startTime;
  }

};

