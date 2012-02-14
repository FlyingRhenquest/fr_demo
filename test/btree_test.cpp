/**
 * Tests for btree.
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

#include "btree.h"

#include <cppunit/extensions/HelperMacros.h>

class BtreeTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(BtreeTest);

  CPPUNIT_TEST(testAdd);
  CPPUNIT_TEST(testRemove);
  CPPUNIT_TEST(testFindRange);
  CPPUNIT_TEST(testBalance);
 
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp() {}
  void tearDown() {}

  void testAdd() 
  {
    Btree<int,int> tree(0);
    tree.add(2,3);
    CPPUNIT_ASSERT(tree.find(2) == 3);
    tree.add(1,2);
    tree.add(3,4);
    CPPUNIT_ASSERT(tree.find(1) == 2);
    CPPUNIT_ASSERT(tree.find(3) == 4);
    CPPUNIT_ASSERT(tree.find(4) == tree.NOT_FOUND);
    CPPUNIT_ASSERT(tree.isBalanced());
  }

  void testRemove()
  {
    Btree<int,int> tree(0);
    tree.add(2,3);
    tree.add(1,2);
    tree.add(3,4);
    tree.add(4,5);
    tree.remove(2);
    CPPUNIT_ASSERT(tree.find(1) == 2);
    CPPUNIT_ASSERT(tree.find(2) == tree.NOT_FOUND);
    CPPUNIT_ASSERT(tree.find(3) == 4);
    CPPUNIT_ASSERT(tree.find(4) == 5);
    tree.rebalance(); // Remove doesn't really try to preserve balance
    CPPUNIT_ASSERT(tree.isBalanced());
  }

  void testFindRange()
  {
    Btree<int,int> tree(0);
    tree.add(2,3);
    tree.add(3,4);
    tree.add(1,2);
    tree.add(4,5);
    Btree<int,int>::RangeType range;
    tree.findRange(range, 2, 4);
    CPPUNIT_ASSERT(range.size() == 3);
    CPPUNIT_ASSERT(range.at(0)->first == 2);
    CPPUNIT_ASSERT(range.at(1)->first == 3);
    CPPUNIT_ASSERT(range.at(2)->first == 4);
    CPPUNIT_ASSERT(tree.isBalanced());
  }

  void testBalance()
  {
    Btree<int,int> tree(0);
    tree.add(1,1);
    tree.add(2,1);
    tree.add(3,1);
    tree.add(4,1);
    tree.add(5,1);
    tree.add(6,1);
    CPPUNIT_ASSERT(tree.isBalanced());
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(BtreeTest);
  
