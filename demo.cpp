/**
 * Quick demo of these utilities
 *
 * To run:
 * 1) Snag the latest ephemeris file from http://earth-info.nga.mil/GandG/sathtml/PEexe.html
 * 2) Use lharc (lha -e) to extract it
 * 3) Run ./demo FILENAME.eph
 * 4) Fire up google earth and load demo.kml
 * 5) Double click on a satellite
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

#include "demo.h"

/**
 * FixTimezone insures that time is in GMT while the server is running.
 * Your old timezone is replaced when it exits.
 *
 */

class FixTimezone {
  char *oldzone;
public:
  FixTimezone()
  {
    oldzone = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
  }

  ~FixTimezone()
  {
    if (oldzone) {
      setenv("TZ", oldzone, 1);
    } else {
      unsetenv("TZ");
    }
    tzset();
  }

};

FixTimezone ft;

AppContext *DemoHandler::context = new AppContext();

int main(int argc, char *argv[])
{

  if (argc != 2) {
    std::cout << "usage: demo filename" << std::endl;
    std::cout << "Filename is a SP3-format ephemeris file." << std::endl;
  } else {
    DemoHandler::context->cache = new EphemerisCache();
    EphemerisCacheBuilder listener(DemoHandler::context->cache);
    EphemerisLineBuilder builder;
    builder.registerListener(&listener);
    Sp3Reader reader(argv[1], &builder);
    std::cout << "Reading ephemeris file " << argv[1] << "...";
    reader.read();
    std::cout << " done." << std::endl << "Setting up server..." << std::endl;
    SocketServer<DemoHandler> server(12345);
    boost::thread *serverThread = server.start();
    std::cout << "done. Ready for connections." << std::endl;
    serverThread->join();
    delete DemoHandler::context->cache;
    delete DemoHandler::context;
  }
}
