/**
 * This reads a SP3 style ephemeris file, like the ones published
 * by the NGA for GPS satellites.
 *
 * This ignores the header lines and just dumps out a big bunch
 * of EphemerisLines. If you want the header info, you could
 * probably build another builder, set the data into it and
 * create your own data structure. I don't think that'd
 * be terribly difficult to do.
 *
 * To read a file:
 * 1) Create an EphemerisLineBuilder
 * 2) Create an EphemrisBuilderListener class that populates, 
 *    say, an EphemerisCache
 * 3) Register your listener to your builder.
 * 4) Create a Sp3Reader for a file
 * 5) Register your builder to the reader
 * 6) Tell the reader to read the file.
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

#ifndef _H_SP3_READER
#define _H_SP3_READER

#include "ephemeris_line_builder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <time.h>

class Sp3Reader {
  EphemerisLineBuilder *builder;
  std::string filename;
  double currentTime;

 public:
  Sp3Reader(std::string filename, EphemerisLineBuilder *builder = NULL) : filename(filename), builder(builder), currentTime(0.0)
  {
  }
  
  void registerBuilder(EphemerisLineBuilder *b)
  {
    builder = b;
  }

  void read()
  {
    std::fstream f(filename.c_str(), std::fstream::in);
    if (!f.fail()) {
      while (f.good()) {
        char start;
        f >> start;
        switch(start) {
        case '*':
          // This is a time line
          readTime(f);
          break;
        case 'P':
          // This is a position line
          readPosition(f);
          break;
        case 'V':
          // This is a velocity line
          readVelocity(f);
          break;
        }
        discardLine(f); // All routines want to discard to end of line
      }
    } else {
      std::cout << "Unable to open" << filename << std::endl;
    }
  }
  
  void readTime(std::fstream &f)
  {
    struct tm myTime;
    double seconds;
    memset(&myTime, '\0', sizeof(struct tm));
    f >> myTime.tm_year;
    myTime.tm_year -= 1900; // Number of years since 1900
    f >> myTime.tm_mon;
    myTime.tm_mon--; // Number of months since jan, range = 0..11
    f >> myTime.tm_mday;
    f >> myTime.tm_hour;
    f >> myTime.tm_min;
    f >> seconds;
    myTime.tm_sec = (int) seconds;
    seconds -= myTime.tm_sec; // should just leave a fraction
    time_t time = timegm(&myTime);
    currentTime = (double) time + seconds;
   }

  void readPosition(std::fstream &f)
  {
    double x,y,z;
    readLine(f, x,y,z);
    builder->setX(x);
    builder->setY(y);
    builder->setZ(z);
  }

  void readVelocity(std::fstream &f)
  {
    double dx, dy, dz;
    readLine(f, dx, dy, dz);
    builder->setDx(dx);
    builder->setDy(dy);
    builder->setDz(dz); // Will probably cause a notify
  }

  void readLine(std::fstream &f, double &x, double &y, double &z)
  {
    std::string satelliteName;
    f >> satelliteName;
    f >> x;
    f >> y;
    f >> z;
    x *= 1000; // units are in km
    y *= 1000;
    z *= 1000;
    builder->setSatelliteName(satelliteName);
    builder->setTime(currentTime);
  }

  void discardLine(std::fstream &f)
  {
    char buffer[120];
    f.getline(buffer, sizeof(buffer));
  }

};


#endif
