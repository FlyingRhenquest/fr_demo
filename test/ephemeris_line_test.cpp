/**
 * Tests for ephemeris_line. The class really isn't that complicated,
 * but I should still check that you get the same numbers out of it
 * that you put into it.
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

#include "ephemeris_line.h"

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

class EphemerisLineTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(EphemerisLineTest);

  CPPUNIT_TEST(testBasic);

  CPPUNIT_TEST_SUITE_END();

public:

  void testBasic()
  {
    Ecef foo(1.0, 2.0, 3.0);
    EphemerisLine bar(foo, 4.0, 5.0, 6.0, 7.0);
    Ecef pos(bar.getPosition());
    CPPUNIT_ASSERT(pos.getX() == foo.getX());
    CPPUNIT_ASSERT(pos.getY() == foo.getY());
    CPPUNIT_ASSERT(pos.getZ() == foo.getZ());
    CPPUNIT_ASSERT(bar.getDx() == 4.0);
    CPPUNIT_ASSERT(bar.getDy() == 5.0);
    CPPUNIT_ASSERT(bar.getDz() == 6.0);
    CPPUNIT_ASSERT(bar.getTime() == 7.0);
    
    boost::numeric::ublas::vector<double> vec(6);
    bar.getVector(vec);

    CPPUNIT_ASSERT(vec(0) == 1.0);
    CPPUNIT_ASSERT(vec(1) == 2.0);
    CPPUNIT_ASSERT(vec(2) == 3.0);
    CPPUNIT_ASSERT(vec(3) == 4.0);
    CPPUNIT_ASSERT(vec(4) == 5.0);
    CPPUNIT_ASSERT(vec(5) == 6.0);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(EphemerisLineTest);
