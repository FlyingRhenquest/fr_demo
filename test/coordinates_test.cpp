/**
 * Tests for coordinates.
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
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

class CoordinatesTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(CoordinatesTest);
  CPPUNIT_TEST(testCoordinates);
  CPPUNIT_TEST_SUITE_END();

public:

  void testCoordinates() 
  {
    Latlong denver1(39.75, 104.87, 1609.344); // Assuming it's about a mile high
    Ecef denEcef(denver1);
    Latlong denver2(denEcef);

    /**
     * Now here's the funny thing: if you compare with denver1.getLat() ==
     * denver2.getLong(), the assert will fail. Isn't working with
     * doubles FUN?
     *
     * Also, there seems to be a very small error in the altitude
     * when I subtract the two numbers, even though the debug
     * printfs showed that it was the same number. So I'm just going
     * to chalk that up to working with doubles too, and
     * call it good.
     */

    double testalt = denver1.getAlt() - denver2.getAlt();
    if (testalt < 0) {
      testalt *= -1;
    }

    CPPUNIT_ASSERT((denver1.getLat() - denver2.getLat()) == 0.0);
    CPPUNIT_ASSERT((denver1.getLong() - denver2.getLong()) == 0.0);
    CPPUNIT_ASSERT(testalt < .0001);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(CoordinatesTest);
