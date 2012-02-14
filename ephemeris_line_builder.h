/**
 * This builder builds an ephemeris line. Populate it with data and
 * when it has enough, it will create a new ephemeris line and
 * call notify on any registered listeners with the new line.
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

#ifndef _H_EPHEMERIS_LINE_BUILDER
#define _H_EPHEMERIS_LINE_BUILDER

#include "ephemeris_line.h"
#include <vector>

/**
 * Define an interface for the listener
 */

class EphemerisBuilderListener {
 public:
  virtual void notify(EphemerisLine &data, std::string &satelliteName) = 0; 
};

class EphemerisLineBuilder {
  std::vector<EphemerisBuilderListener *> listeners;
  std::string satelliteName;
  bool gotX;
  bool gotY;
  bool gotZ;
  bool gotDx;
  bool gotDy;
  bool gotDz;
  double x, y, z, dx, dy, dz, currentTime; // currentTime is technically optional
 public:
  EphemerisLineBuilder()
  {
    resetRecord();
  }

  void resetRecord()
  {
    satelliteName = "";
    x = y = z = dx = dy = dz = currentTime = 0;
    gotX = gotY = gotZ = gotDx = gotDy = gotDz = false;
  }
  
  void notifyListeners()
  {
    if (gotX && gotY && gotZ && gotDx && gotDy && gotDz) {
      std::vector<EphemerisBuilderListener *>::iterator it;
      it = listeners.begin();
      Ecef position(x, y, z);
      EphemerisLine line(position, dx, dy, dz, currentTime);
      while(it != listeners.end()) { 
        (*it)->notify(line, satelliteName);
        it++;
      }
      resetRecord();
    }   
  }

  void setX(double nx)
  {
    x = nx;
    gotX = true;
    notifyListeners();
  }

  void setY(double ny) 
  {
    y = ny;
    gotY = true;
    notifyListeners();
  }

  void setZ(double nz) 
  {
    z = nz;
    gotZ = true;
    notifyListeners();
  }

  void setDx(double ndx)
  {
    dx = ndx;
    gotDx = true;
    notifyListeners();
  }

  void setDy(double ndy)
  {
    dy = ndy;
    gotDy = true;
    notifyListeners();
  }

  void setDz(double ndz)
  {
    dz = ndz;
    gotDz = true;
    notifyListeners();
  }

  void setTime(double ntime)
  {
    currentTime = ntime;
  }

  void registerListener(EphemerisBuilderListener *l)
  {
    listeners.push_back(l);
  }

  void setSatelliteName(std::string name)
  {
    satelliteName = name;
  }
  
  // TODO: enable unregistering listeners?

};

#endif
