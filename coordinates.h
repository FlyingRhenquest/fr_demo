/**
 * Latlong and ecef coordinates
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

#include <math.h>

#ifndef _H_COORDINATES
#define _H_COORDINATES

/**
 * I can't get away with a forward-declare of Latlong here,
 * since I want to use bits of the class in an Ecef constructor. I could
 * break that constructor out and define it in a .cpp file
 * but then I actually have to juggle an object file when
 * I'm making, and I don't really care to do that.
 *
 * You shouldn't ever have to use this interface in your own code,
 * just use a Latlong instead.
 */

class LatlongInterface {
 public:
  virtual ~LatlongInterface() {}
  virtual const double getLat() = 0;
  virtual const double getLong() = 0;
  virtual const double getAlt() = 0;
  friend bool operator==( LatlongInterface &lhs, LatlongInterface &rhs);
};

/**
 * An ECEF coordinate. This is the coordinate system GPS uses, internally.
 */

class Ecef {

  double x,y,z;

 public:

 Ecef(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {
  }

 Ecef(const Ecef &toCopy) : x(toCopy.x), y(toCopy.y), z(toCopy.z) {}

  Ecef(LatlongInterface &ll, double ae = 6378137.0, double ee=0.00669437999014)
    {
      double pi = atan2(1,1) * 4;
      double slat = sin(ll.getLat() * pi / 180);
      double clat = cos(ll.getLat() * pi / 180);
      double slon = sin(ll.getLong() * pi / 180);
      double clon = cos(ll.getLong() * pi / 180);
      double n = ae / sqrt(1.0 - ee * pow(slat, 2));
      x = (n + ll.getAlt()) * clat * clon;
      y = (n + ll.getAlt()) * clat * slon;
      z = (n * (1.0 - ee) + ll.getAlt()) * slat;
    }

  const inline double getX() {
    return x;
  }

  const inline double getY() {
    return y;
  }

  const inline double getZ() {
    return z;
  }

  friend bool operator==(Ecef &lhs, Ecef &rhs);

};

/**
 * Latlong coordinates. Ecef provides an altitude that you don't
 * usually see with latlongs. I'm providing an altitude, too.
 * Since this is mainly to convert from ecef, the altitude will
 * be in meters.
 */

class Latlong : public LatlongInterface {
  double lat, longitude, alt; // pro tip, don't use "long" as a variable name
 public:

 Latlong(double inLat, double inLong, double inAlt) : lat(inLat), longitude(inLong), alt(inAlt) {

  }

  Latlong(Ecef &xyz, double ae = 6378137.0, double ee = 0.00669437999014, double tolerance = 0.0000000001)
    {
      double pi = atan2(1.0, 1.0) * 4;
      double diff = 2 * tolerance;
      double t = ee * xyz.getZ();
      double n = 0.0;
      double nph = 0.0;
      double sinPhi = 0.0;
      
      longitude = atan2(xyz.getY(), xyz.getX()) * 180 / pi;
      while(diff > tolerance) {
        double zT = xyz.getZ() + t;
        nph = sqrt(pow(xyz.getX(), 2) + pow(xyz.getY(), 2) + pow(zT, 2));
        sinPhi = zT / nph;
        n = ae / sqrt(1 - ee * sinPhi * sinPhi);
        double told = t;
        t = n * ee * sinPhi;
        diff = fabs(t - told);
      }
      
      lat = asin(sinPhi) * 180 / pi;
      alt = nph - n;
    }

  const inline double getLat() {
    return lat;
  }

  const inline double getLong() {
    return longitude;
  }

  const inline double getAlt() {
    return alt;
  }

};

#endif
