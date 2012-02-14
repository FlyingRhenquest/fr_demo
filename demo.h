/**
 * Demo server
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

#ifndef _H_DEMO
#define _H_DEMO

#include "ephemeris_line_builder.h"
#include "ephemeris_cache.h"
#include "coordinates.h"
#include "sp3_reader.h"
#include "socket_server.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ext/stdio_filebuf.h>
#include <time.h>

class EphemerisCacheBuilder : public EphemerisBuilderListener {
  EphemerisCache *target;

public:
  EphemerisCacheBuilder(EphemerisCache *target) : target(target)
  {
  }
  
  void notify(EphemerisLine &data, std::string &satelliteName)
  {
    target->add(satelliteName, new EphemerisLine(data));
  }

};

struct AppContext {
  EphemerisCache *cache;
};

class DemoHandler {
  SocketServer<DemoHandler> *owner;
  int fdes;
  std::string buffer;
 public:
  static AppContext *context;

  DemoHandler(SocketServer<DemoHandler> *owner, int fdes) : owner(owner), fdes(fdes)
  {
  }

  void operator()()
  {
    std::string buffer;
    __gnu_cxx::stdio_filebuf<char> buf_in(fdes, std::ios_base::in, 1);
    __gnu_cxx::stdio_filebuf<char> buf_out(fdes, std::ios_base::out, 1);
    std::istream stream_in(&buf_in);
    std::ostream stream_out(&buf_out);
    getline(stream_in, buffer); // http get line from google earth
    std::vector<std::string> satelliteNames;
    context->cache->satelliteNames(satelliteNames);
    std::vector<std::string>::iterator name = satelliteNames.begin();
    stream_out << "HTTP/1.0 200 OK" << std::endl;
    stream_out << "Content-Type: application/vnd.google-earth.kml+xml" << std::endl << std::endl;

    stream_out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    stream_out << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">" << std::endl;
    stream_out << "<Document>" << std::endl;

    time_t now = time((time_t) NULL);
    now -= (2*86400);
    while(name != satelliteNames.end()) {
      EphemerisLine *current = context->cache->get(*name, (double) now);
      if (NULL == current) {
        stream_out << "No Records Found" << std::endl;
        break;
     } else {
        stream_out << std::setprecision(18);
        Latlong ll(current->getPosition()); // Convert from ECEF for Google Earth
        
        stream_out << "<Placemark>" << std::endl;
        stream_out << "   <name>" << *name << "</name>" << std::endl;
        stream_out << "   <description>GPS Satellite</description>" << std::endl;
        stream_out << "   <Point>" << std::endl;
        stream_out << "      <extrude>1</extrude>" << std::endl;
        stream_out << "      <altitudeMode>relativeToGround</altitudeMode>" << std::endl;
        stream_out << "      <coordinates>" << ll.getLat() << ",";
        stream_out << ll.getLong() << "," << ll.getAlt();
        stream_out << "</coordinates>" << std::endl;
        stream_out << "   </Point>" << std::endl;
        stream_out << "</Placemark>" << std::endl;
        name++;
      }
    }

    stream_out << "</Document>" << std::endl;
    stream_out << "</kml>" << std::endl << std::endl;
    stream_out.flush();
    boost::this_thread::sleep(boost::posix_time::seconds(3));
    close(fdes);
  }

};

#endif
