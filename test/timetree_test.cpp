/**
 * Tests for Time Tree
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

#include "time_tree.h"

#include <cppunit/extensions/HelperMacros.h>

class TimeTreeTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TimeTreeTest);

  CPPUNIT_TEST(testFind);

  CPPUNIT_TEST_SUITE_END();

public:

  void testFind() 
  {
    TimeTree<int,int> tree(0);
    tree.add(2, 2);
    tree.add(4, 4);
    tree.add(6, 6);
    tree.add(8, 8);

    CPPUNIT_ASSERT(tree.find(1) == tree.NOT_FOUND);
    CPPUNIT_ASSERT(tree.find(2) == 2);
    CPPUNIT_ASSERT(tree.find(3) == 2);
    CPPUNIT_ASSERT(tree.find(4) == 4);
    CPPUNIT_ASSERT(tree.find(5) == 4);
    CPPUNIT_ASSERT(tree.find(6) == 6);
    CPPUNIT_ASSERT(tree.find(7) == 6);
    CPPUNIT_ASSERT(tree.find(8) == 8);
    CPPUNIT_ASSERT(tree.find(9) == 8);
    CPPUNIT_ASSERT(tree.find(42) == 8);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeTreeTest);
