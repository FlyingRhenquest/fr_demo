/**
 * This is a simple btree class.
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

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <stdlib.h>

template <typename KeyType, typename ValType> class Btree;

/**
 * BtreeNode contains the guts of the btree. Most of the actual
 * work in the tree takes place here.
 */

template <typename KeyType, typename ValType>
class BtreeNode {
 protected:
  typedef BtreeNode<KeyType, ValType> MyBtreeNode;
  typedef Btree<KeyType,ValType> MyBtree;
  std::pair<KeyType, ValType> keyVal;
  MyBtreeNode *left;
  MyBtreeNode *right;
  MyBtreeNode *parent;
  int leftBalance;
  int rightBalance;
  Btree<KeyType,ValType> *owner;

 public:

  /**
   * Set up a node, but you still have to add it!
   */

  BtreeNode(KeyType key, ValType val, Btree<KeyType,ValType> *owner) 
    {
      keyVal.first = key;
      keyVal.second = val;
      this->owner = owner;
      left = (MyBtreeNode *) NULL;
      right = (MyBtreeNode *) NULL;
      parent = (MyBtreeNode *) NULL;
      owner = (MyBtree *) NULL;
      leftBalance = 0;
      rightBalance = 0;
    }

  /**
   * Recursively delete the entire tree!
   */

  virtual ~BtreeNode()
    {
      if (left) {
        delete left;
      }
      if (right) {
        delete right;
      }
    }

  /**
   * recalcBalance does a full tree traverse to recalculate all the
   * node balances. This will be pretty heavy for large trees, so
   * I suggest avoiding it as much as possible.
   */

  virtual int recalcBalance()
  {
    if (left) {
      leftBalance = left->recalcBalance() + 1;
    } else {
      leftBalance = 0;
    }
    if (right) {
      rightBalance = right->recalcBalance() + 1;
    } else {
      rightBalance = 0;
    }
    return balanced();
  }

  virtual int balanced()
  {
    return leftBalance - rightBalance;
  }

  virtual bool isBalanced()
  {
    bool isLeft = true;
    bool isRight = true;
    bool isMe = false;
    if (left) {
      isLeft = left->isBalanced();
    }
    if (right) {
      isRight = right->isBalanced();
    }
    if (abs(balanced()) <= 1) {
      isMe = true;
    }
    return isLeft && isRight && isMe;
  }

  virtual void rotateLeft()
  {
    MyBtreeNode *pivot = right;
    if (NULL == pivot) {
      return; // can't rotate left
    }
    MyBtreeNode **above;
    if (parent) {
      if (this == parent->left) {
        above = &parent->left;
      } else {
        above = &parent->right;
      }
    } else {
      above = &owner->root;
    }
    *above = pivot;
    pivot->parent = parent;
    right = pivot->left;
    if (right) {
      right->parent = this;
    }
    pivot->left = this;
    parent = pivot;
    pivot->recalcBalance();
  }

  virtual void rotateRight()
  {
    MyBtreeNode *pivot = left;
    if (NULL == pivot) {
      return; // can't rotate right
    }
    MyBtreeNode **above;
    if (parent) {
      if (this == parent->left) {
        above = &parent->left;
      } else {
        above = &parent->right;
      }
    } else {
      above = &owner->root;
    }
    *above = pivot;
    pivot->parent = parent;
    left = pivot->right;
    if (left) {
      left->parent = this;
    }
    pivot->right = this;
    parent = pivot;
    pivot->recalcBalance();
  }

  /**
   * printBalance, for debugging.
   */

  virtual void printBalance()
  {
    std::cout << "Node: " << keyVal.first;
    if (parent) {
      std::cout << " Parent: " << parent->keyVal.first;
    }
    std::cout << std::endl;
    std::cout << "      Left Balance:  " << leftBalance << std::endl;
    std::cout << "      Right Balance: " << rightBalance << std::endl << std::endl;
    if (left) {
      left->printBalance();
    }
    if (right) {
      right->printBalance();
    }    
  }

  virtual void rebalance()
  {
    // recalcBalance();
    int nodeBalance = balanced();
    if (left) {
      left->rebalance();
    }
    if (right) {
      right->rebalance();
    }
    if (nodeBalance < -1) {
      rotateLeft();
    } else if (nodeBalance > 1) {
      rotateRight();
    }
  }

  /**
   * Add a new node to the tree, using recursion!
   */

  virtual void add(BtreeNode *aNode) throw(std::string)
  {
    KeyType nodeVal = aNode->keyVal.first;
    if (nodeVal < keyVal.first) {
      if (left) {
        left->add(aNode);
      } else {
        left = aNode;
        aNode->parent = this;
      }
      leftBalance++;
    } else if (nodeVal > keyVal.first) {
      if (right) {
        right->add(aNode);
      } else {
        right = aNode;
        aNode->parent = this;
      }
      rightBalance++;
    } else {
      // No matter what I do here someone's going to hate it, so
      // I'm just going to throw.
      throw std::string("Attempt to add the same value to the btree twice.");
    }
    if (abs(balanced()) > 1) {
      rebalance();
    }
  }

  /**
   * remove removes a node from the tree. Aaah, you thought I was
   * going to leave you hanging with only a way to recursively
   * delete the entire tree? :-D
   *
   * I think if I move the left node up to this one and then call
   * add with the right node, it should work out OK. It could leave
   * the tree pretty unbalanced, but I can work on that later.
   */

  virtual ValType remove(KeyType key)
  {
    if (key < keyVal.first) {
      if (left) {
        leftBalance--;
        return left->remove(key);
      } else {
        return owner->NOT_FOUND;
      }
    } else if (key > keyVal.first) {
      if (right) {
        rightBalance--;
        return right->remove(key);
      } else {
        return owner->NOT_FOUND;
      }
    } else {
      MyBtreeNode **parentBranch = (MyBtreeNode **) NULL;
      ValType removed = keyVal.second;
      if (!parent) {
        parentBranch = &(owner->root);
        (*parentBranch)->parent = (MyBtreeNode *)NULL;
      } else {
        if (this == parent->left) {
          parentBranch = &(parent->left);
        } else {
          parentBranch = &(parent->right);
        }
      }
      if (left) {
        *parentBranch = left;
        if (right) {
          (*parentBranch)->add(right);
        }
        // Prevent recursive delete of previously owned nodes
        left = (MyBtreeNode *) NULL;
        right = (MyBtreeNode *) NULL;
        delete this;
      } else if (right) {
        *parentBranch = right;
        left = (MyBtreeNode *) NULL;
        right = (MyBtreeNode *) NULL;
        delete this;
      } else {
        *parentBranch = (MyBtreeNode *) NULL;
        delete this;
      }
      return removed;
    }
    return owner->NOT_FOUND;
  }

  /**
   * Find is the fun one!
   */

  virtual ValType find(KeyType toFind)
  {
    if (toFind < keyVal.first) {
      if (left) {
        return left->find(toFind);
      } else {
        return owner->NOT_FOUND;
      }
    } else if (keyVal.first < toFind) {
      if (right) {
        return right->find(toFind);
      } else {
        return owner->NOT_FOUND;
      }
    } else {
      return keyVal.second;
    }
  }

  typedef std::vector<std::pair<KeyType, ValType> *> RangeType;

  /**
   * We're going to take find, and we're going to go one further.
   */

  virtual void findRange(RangeType &range, KeyType start, KeyType end)
  {
    if (keyVal.first >= start) {
      // No need to go left if it's less
      if (left) {
        left->findRange(range, start, end);
      }
    }

    if (keyVal.first >= start && keyVal.first <= end) {
      range.push_back(&keyVal);
    }

    if (keyVal.first <= end) {
      // No need to go right if it's greater
      if (right) {
        right->findRange(range, start, end);
      }
    }
  }
};

/**
 * Btree holds all your nodes and handles creating and manipulating
 * them for you.
 */

template<typename KeyType, typename ValType>
class Btree {
  friend class BtreeNode<KeyType, ValType>; // So I don't have to make root public
  typedef BtreeNode<KeyType, ValType> NodeType;

 protected:
  NodeType *root;
  KeyType lowest;
  KeyType highest;
  bool firstAdd;

 public:
  ValType NOT_FOUND;

  /**
   * Create with the value to be used as "NOT_FOUND"
   */

  Btree(ValType nf) : NOT_FOUND(nf)
  {
    firstAdd = true;
    root = (NodeType *) NULL;
  }

  virtual ~Btree()
    {
      delete root;
    }

  /**
   * begin is a convenience function for use with findRange. It
   * returns the lowesat thing you've added to the tree.
   */
  
  virtual KeyType begin() throw(std::string)
  {
    if (firstAdd) {
      throw std::string("Can't call begin before adding any nodes");
    }
    return lowest;
  }

  /**
   * end returns the largest thing you've added to the tree.
   */

  virtual KeyType end() throw(std::string)
  {
    if (firstAdd) {
      throw std::string("Can't call end before adding any nodes");
    }
    return highest;
  }

  virtual void add(KeyType key, ValType val) throw(std::string)
  {
    if (firstAdd) {
      firstAdd = false;
      lowest = key;
      highest = key;
    }
    if (key < lowest) {
      lowest = key;
    }
    if (key > highest) {
      highest = key;
    }

    BtreeNode<KeyType,ValType> *toAdd = new BtreeNode<KeyType,ValType>(key, val, this);
    if (root) {
      root->add(toAdd); // Can throw
    } else {
      root = toAdd;
    }
  }

  virtual ValType remove(KeyType key)
  {
    if (root) {
      return root->remove(key);
    } else {
      return NOT_FOUND;
    }
  }
  
  virtual ValType find(KeyType key)
  {
    if (root) {
      return root->find(key);
    } else {
      return NOT_FOUND;
    }
  }

  typedef std::vector<std::pair<KeyType, ValType> *> RangeType;
  
  virtual void findRange(RangeType &range, KeyType start, KeyType end)
  {
    if (root) {
      root->findRange(range, start, end);
    }
  }

  virtual void printBalance()
  {
    root->printBalance();
  }

  virtual void rebalance()
  {
    root->recalcBalance();
    root->rebalance();
  }

  virtual bool isBalanced()
  {
    root->recalcBalance();
    return root->isBalanced();
  }
  
};
