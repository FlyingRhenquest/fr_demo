/**
 * Tests Julian Date class
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

#include "jd.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>

class JdTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(JdTest);
  CPPUNIT_TEST(testJd);
  CPPUNIT_TEST_SUITE_END();
  char *oldTz;

public:

  void setUp() {
    /*
     * Switch to GMT so the results of this test don't change based
     * on your time zone.
     */
    oldTz = getenv("TZ");
    setenv("TZ","GMT", 1);
    tzset();
  }

  void tearDown() {
    if (oldTz) {
      setenv("TZ", oldTz, 1);
    }
  }

  void testJd()
  {
    struct tm j2kish; // Noon Jan 1, 2000, GMT
    memset(&j2kish, '\0', sizeof(struct tm));
    j2kish.tm_year = 100; // y2k
    j2kish.tm_hour = 12;
    j2kish.tm_mday = 1;
    time_t j2ktt = timegm(&j2kish);
    JD j2kjd = JD(j2ktt);

    CPPUNIT_ASSERT(j2kjd.getJdn() == 2451545);
    CPPUNIT_ASSERT(j2kjd.getJd() == 2451545.0);

    struct tm todayish;
    memset(&todayish, '\0', sizeof(struct tm));
    todayish.tm_year = 111; // 2011
    todayish.tm_mday = 5;
    todayish.tm_mon = 10; // November (UNIX months start at 0)
    todayish.tm_hour = 10;
    todayish.tm_min = 27;
    todayish.tm_sec = 23;
    time_t todaytt = timegm(&todayish);
    JD todayjd(todaytt);

    double jdDiff = fabs(todayjd.getJd() - 2455870.935683);

    CPPUNIT_ASSERT(todayjd.getJdn() == 2455871);
    CPPUNIT_ASSERT(jdDiff < .000001);
    
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(JdTest);
