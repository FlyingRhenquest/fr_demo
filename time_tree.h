/**
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
 *
 * It's a tree so EXTRA-SPECIAL it stores not only data, but time...
 * itself! Ingenious... I know...
 *
 * A time tree is sort of like a btree, but finds will find
 * values between two nodes. So if you have 2,4,6 and 8 in the
 * tree, a find on 3 should return 2, and a find on 5 should return
 * 4.
 *
 * A find on 1 should return NOT_FOUND but any larger value than
 * 8 will always return 8, so if you want the top value to expire
 * at some time, you'll need to handle that in your code.
 */

#include "btree.h"

template <typename KeyType, typename ValType, typename Deallocator> class TimeTree;

template <typename KeyType, typename ValType>
  class TimeTreeNode : public BtreeNode<KeyType, ValType> {
  typedef TimeTreeNode<KeyType, ValType> MyBtreeNode;
  typedef Btree<KeyType,ValType> MyBtree;

 public:

 TimeTreeNode(KeyType key, ValType val, MyBtree *owner) : BtreeNode<KeyType,ValType>(key,val,owner)
    {
    }

  ValType find(KeyType toFind) {
    if (toFind < this->keyVal.first) {
      if (this->left) {
        return this->left->find(toFind);
      } else {
        return this->owner->NOT_FOUND;
      }
    } else if (this->keyVal.first < toFind) {
      ValType anyMoreRight = this->owner->NOT_FOUND;
      if (this->right) {
        anyMoreRight = this->right->find(toFind);
      }
      if (anyMoreRight != this->owner->NOT_FOUND) {
        return anyMoreRight;
      } else {
        return this->keyVal.second;
      }
    } else {
      return this->keyVal.second;
    }
  }
};

/**
 * I don't mind allocating things to go in time trees, but sometimes
 * don't want to have to worry about deallocating them except when
 * the time tree is freed. I don't need a full C++ allocator for
 * this, but I do need a deallocator class.
 */

template <typename ValType>
class DefaultTimeTreeDeallocator {
 public:
  void operator()(ValType value) {}; // Do Nothing
};

template <typename KeyType, typename ValType, typename Deallocator = DefaultTimeTreeDeallocator<ValType> >
class TimeTree : public Btree<KeyType, ValType>
{
  friend class TimeTreeNode<KeyType,ValType>;
  typedef TimeTreeNode<KeyType, ValType> NodeType;

 public:
  /**
   * Create with the value to be used for "Not Found"
   */

  TimeTree(ValType nf) : Btree<KeyType, ValType>(nf)
    {
    }

  virtual ~TimeTree()
  {
    typename Btree<KeyType,ValType>::RangeType range;
    typename Btree<KeyType,ValType>::RangeType::iterator iter;
    Deallocator dealloc;
    findRange(range, this->begin(), this->end());
    iter = range.begin();
    while(iter != range.end()) {
      dealloc((*iter)->second);
      iter++;
    }
  }
  
  virtual void add(KeyType key, ValType val) throw(std::string)
  {
    if (this->firstAdd) {
      this->firstAdd = false;
      this->lowest = key;
      this->highest = key;
    }
    if (key < this->lowest) {
      this->lowest = key;
    }
    if (key > this->highest) {
      this->highest = key;
    }
    TimeTreeNode<KeyType, ValType> *toAdd = new TimeTreeNode<KeyType,ValType>(key,val,this);
    if (this->root) {
      this->root->add(toAdd);
    } else {
      this->root = toAdd;
    }
  }

};
