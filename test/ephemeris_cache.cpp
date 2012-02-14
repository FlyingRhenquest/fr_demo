/**
 * Tests for Ephemeris cache -- make sure get and add work correctly,
 * and that data point interval calculaton isn't broken.
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

#include <cppunit/extensions/HelperMacros.h>
#include "ephemeris_cache.h"
#include "ephemeris_line.h"

#include <string>

class EphemerisCacheTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(EphemerisCacheTest);
  CPPUNIT_TEST(testTwoPoints);
  CPPUNIT_TEST_SUITE_END();

public:
  void testTwoPoints()
  {
    EphemerisCache cache;
    std::string foo("Foo");

    // Coordinates don't really matter here, time does. Two points,
    // starting at time 1.5 and ending at points 3.0
    cache.add("Foo", new EphemerisLine(Ecef(1,2,3), 4, 5, 6, 1.5));
    cache.add("Foo", new EphemerisLine(Ecef(7,8,9), 10, 11, 12, 3.0));
    EphemerisLine *check = cache.get(foo, 1.7); // Should get first line
    CPPUNIT_ASSERT(NULL != check);
    CPPUNIT_ASSERT(1.5 == cache.getDataInterval(foo));
    CPPUNIT_ASSERT(check->getDx() == 4);
    CPPUNIT_ASSERT(check->getDy() == 5);
    CPPUNIT_ASSERT(check->getDz() == 6);
    CPPUNIT_ASSERT(check->getTime() == 1.5);
    check = cache.get(foo, 3.5);
    CPPUNIT_ASSERT(NULL != check);
    CPPUNIT_ASSERT(check->getTime() == 3.0);
    check = cache.get(foo, 20.0);
    CPPUNIT_ASSERT(NULL == check);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(EphemerisCacheTest);
