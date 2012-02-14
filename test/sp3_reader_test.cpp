/**
 * Tests SP3 Reader
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

#include "sp3_reader.h"
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include "time_tree.h"
#include <iomanip>

class Sp3ReaderTest : public CppUnit::TestFixture, public EphemerisBuilderListener {
 
  CPPUNIT_TEST_SUITE(Sp3ReaderTest);
  CPPUNIT_TEST(checkSomeLines);
  CPPUNIT_TEST_SUITE_END();
  class MyTimeTreeDeallocator {
  public:
    void operator()(EphemerisLine *value)
    {
      delete value;
    }
  };
  double tolerance; // Because double checks are squishy
  typedef TimeTree<double, EphemerisLine *, MyTimeTreeDeallocator> MyTimeTree;
  typedef std::map <std::string, MyTimeTree *> SatMap;
  SatMap satmap;
  int linecount;

public:
  
  Sp3ReaderTest()
  {
    linecount = 0;
    tolerance = .01;
  }

  void setUp()
  {
    EphemerisLineBuilder builder;
    builder.registerListener(this);
    Sp3Reader reader("nga16556.eph", &builder);
    reader.read();
  }

  void tearDown()
  {
    SatMap::iterator it = satmap.begin();
    while (it != satmap.end())  {
      delete it->second;
      it++;
    }
  }

  void notify(EphemerisLine &data, std::string &satelliteName)
  {
    SatMap::iterator it = satmap.find(satelliteName);
    MyTimeTree *tt;
    if (it == satmap.end()) {
      tt = new MyTimeTree(NULL);
      satmap[satelliteName] = tt;
    } else {
      tt = it->second;
    }
    tt->add(data.getTime(), new EphemerisLine(data));
    linecount++;
  }

  /**
   * retrieveAndCheck takes a satellite name, time and an expected value.
   * It pulls up the satellite from the TimeTree and validates that
   * the EphemerisLine is correct.
   *
   * This fixes the times for UNIX, so just send it the values
   * from the test file.
   */

  bool retrieveAndCheck(std::string satellite, int year, int month, int day, int hour, int min, int sec, EphemerisLine expected)
  {
    SatMap::iterator it;
    MyTimeTree *tt;
    it = satmap.find(satellite);
    if (it == satmap.end()) {
      return false;
    } else {
      tt = it->second;
    }

    struct tm convertTime;
    memset(&convertTime, '\0', sizeof(struct tm));
    convertTime.tm_year = year - 1900;
    convertTime.tm_mon = month - 1;
    convertTime.tm_mday = day;
    convertTime.tm_hour = hour;
    convertTime.tm_min = min;
    convertTime.tm_sec = sec;
    time_t t = timegm(&convertTime);
    expected.setTime((double) t);
    
    EphemerisLine *l = tt->find((double) t);

    if (! equalish(tolerance, *l, expected)) {
      std::cout << "ERROR: Expected " << expected.toString() << " but got " << l->toString() << std::endl;
    }
    
    return equalish(tolerance, *l, expected);
    
  }

  void checkSomeLines()
  {
    SatMap::iterator it;
    MyTimeTree *tt;
    it = satmap.find(std::string("1"));
    CPPUNIT_ASSERT(it != satmap.end());
    tt = it->second;
    MyTimeTree::RangeType range;
    MyTimeTree::RangeType::iterator rangeIter;
    tt->findRange(range, tt->begin(), tt->end());
    rangeIter = range.begin();
    
    CPPUNIT_ASSERT(linecount > 0);
    CPPUNIT_ASSERT(retrieveAndCheck(std::string("1"), 2011, 10, 1, 0, 0, 5, EphemerisLine(9499209.153, 13635225.244, -20730158.212, -26090984.781, 3407283.545, -9719569.472)));
    CPPUNIT_ASSERT(retrieveAndCheck(std::string("11"), 2011, 10, 1, 0, 0, 5, EphemerisLine(3505841.129, 16649715.117, -20723333.624, -24548731.022, 8294919.140, 2903355.907)));
    CPPUNIT_ASSERT(retrieveAndCheck(std::string("1"), 2011, 10, 1, 2, 30, 5, EphemerisLine(-10693666.727, 21291403.180, -11748917.864, -12494956.571, 8462310.326, 26731994.731)));
  }
  
};

CPPUNIT_TEST_SUITE_REGISTRATION(Sp3ReaderTest);
