/**
 * This class represents one line of ephemeris data. It consists
 * of an ecef point and delta x, y and z values of the satellite
 * at that point. It allows you to retrieve this information
 * as a boost math vector.
 *
 * Units should be in meters.
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

#include "coordinates.h"
#include <boost/numeric/ublas/vector.hpp>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>

#ifndef _H_EPHEMERISLINE
#define _H_EPHEMERISLINE

class EphemerisLine {
  Ecef position;
  double dx;
  double dy;
  double dz;
  double currentTime; // Optional, and not included in the vector

 public:
 EphemerisLine(Ecef pos, double x, double y, double z, double time=0) : position(pos), dx(x), dy(y), dz(z), currentTime(time) 
  {
    
  }

 EphemerisLine(double x, double y, double z, double dx, double dy, double dz, double time = 0) : position(x,y,z), dx(dx), dy(dy), dz(dz), currentTime(time)
  {

  }

  EphemerisLine(const EphemerisLine &line) : position(line.position), dx(line.dx), dy(line.dy), dz(line.dz), currentTime(line.currentTime) 
  {

  }


  Ecef &getPosition() 
  {
    return position;
  }

  double getDx() 
  {
    return dx;
  }

  double getDy() 
  {
    return dy;
  }

  double getDz() 
  {
    return dz;
  }

  double getTime() 
  {
    return currentTime;
  }

  void setDx(double x) 
  {
    dx = x;
  }

  void setDy(double y)
  {
    dy = y;
  }

  void setDz(double z)
  {
    dz = z;
  }

  void setTime(double t)
  {
    currentTime = t;
  }

  /**
   * getVector requries that a boost::numeric::ublas::vector<double>(6)
   * reference be passed in (because I don't want to allocate
   * something in this function) and will populate the
   * vector for you.
   */

  void getVector(boost::numeric::ublas::vector<double> &vec)
  {
    vec(0) = position.getX();
    vec(1) = position.getY();
    vec(2) = position.getZ();
    vec(3) = dx;
    vec(4) = dy;
    vec(5) = dz;
  }

  /**
   * For Debugging
   */

  std::string toString()
  {
    std::stringstream build("");
    char buf[40];
    time_t tm = (time_t) currentTime;
    build << std::setprecision(17);
    build << "(";
    build << position.getX() << ", ";
    build << position.getY() << ", ";
    build << position.getZ() << ", ";
    build << dx << ", ";
    build << dy << ", ";
    build << dz << ", ";
    ctime_r(&tm, buf);
    buf[strlen(buf) - 1] = '\0'; // remove CR
    build << buf << ")";
    return build.str();
  }
  
};

bool operator==(EphemerisLine &lhs, EphemerisLine &rhs);
bool operator!=(EphemerisLine &lhs, EphemerisLine &rhs);
bool equalish(double tolerance, EphemerisLine &lhs, EphemerisLine &rhs);

#endif
