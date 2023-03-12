#include "rb-tree.h"

#include <iostream>

#define RB_NODE typename RBTree<Key, Value>::Node

using std::cout;
using std::endl;

template <class Key, class Value>
RBTree<Key, Value>::RBTree() {
  nil_ = new Node;
  nil_->color_ = kBlack;
  nil_->parent_ = nil_->left_child_ = nil_->right_child_ = nil_;
  root_ = nil_;
}

template <class Key, class Value>
RBTree<Key, Value>::~RBTree() {
  Node* x = root_;
  DestoryTree(root_);
  delete nil_;
}

template <class Key, class Value>
void RBTree<Key, Value>::DestoryTree(Node* node) {
  if (node != nil_ && node != NULL) {
    if (node->left_child_ != nil_) DestoryTree(node->left_child_);
    if (node->right_child_ != nil_) DestoryTree(node->right_child_);
    delete node;
  }
}
template <class Key, class Value>
RB_NODE* RBTree<Key, Value>::GetNode(Node* node, Key key) {
  if (node == nil_ || node == NULL) return NULL;
  if (node->key_ == key) return node;
  if (node->key_ > key)
    return GetNode(node->left_child_, key);
  else
    return GetNode(node->right_child_, key);
}

template <class Key, class Value>
void RBTree<Key, Value>::Transplant(Node* subtree1, Node* subtree2) {
  if (subtree1->parent_ == nil_) {
    root_ = subtree2;
  } else {
    if (subtree1 == subtree1->parent_->left_child_) {
      subtree1->parent_->left_child_ = subtree2;
    } else {
      subtree1->parent_->right_child_ = subtree2;
    }
  }
  subtree2->parent_ = subtree1->parent_;
}

template <class Key, class Value>
void RBTree<Key, Value>::InOrder(Node* node) {
  if (node != nil_ && node != NULL) {
    InOrder(node->left_child_);
    cout << node->key_ << " ";
    cout << "- " << node->data_ << endl;
    InOrder(node->right_child_);
  }
}

template <class Key, class Value>
void RBTree<Key, Value>::InOrder() {
  InOrder(root_);
}

template <class Key, class Value>
void RBTree<Key, Value>::LeftRotate(Node* node) {
  Node* y = node->right_child_;
  y->parent_ = node->parent_;
  if (y->left_child_ != nil_) {
    y->left_child_->parent_ = node;
  }
  node->right_child_ = y->left_child_;
  if (node != root_) {
    if (node->parent_->left_child_ == node) {
      node->parent_->left_child_ = y;
    } else {
      node->parent_->right_child_ = y;
    }
  } else {
    root_ = y;
  }
  y->left_child_ = node;
  node->parent_ = y;
}

template <class Key, class Value>
void RBTree<Key, Value>::RightRotate(Node* node) {
  Node* x = node->left_child_;
  x->parent_ = node->parent_;
  if (x->right_child_ != nil_) {
    x->right_child_->parent_ = node;
  }
  node->left_child_ = x->right_child_;
  if (node != root_) {
    if (node->parent_->left_child_ == node) {
      node->parent_->left_child_ = x;
    } else {
      node->parent_->right_child_ = x;
    }
  } else {
    root_ = x;
  }
  x->right_child_ = node;
  node->parent_ = x;
}

template <class Key, class Value>
void RBTree<Key, Value>::InsertFixUp(Node* node) {
  while (node->parent_->color_ == kRed) {
    if (node->parent_ == node->parent_->parent_->left_child_) {
      Node* y = node->parent_->parent_->right_child_;
      if (y->color_ == kRed) {  // case1
        node->parent_->color_ = kBlack;
        y->color_ = kBlack;
        node->parent_->parent_->color_ = kRed;
        node = node->parent_->parent_;
      } else {
        if (node == node->parent_->right_child_) {  // case2
          node = node->parent_;
          LeftRotate(node);
        }
        node->parent_->color_ = kBlack;  // case3
        node->parent_->parent_->color_ = kRed;
        RightRotate(node->parent_->parent_);
      }
    } else {
      Node* y = node->parent_->parent_->left_child_;
      if (y->color_ == kRed) {
        node->parent_->color_ = kBlack;
        y->color_ = kBlack;
        node->parent_->parent_->color_ = kRed;
        node = node->parent_->parent_;
      } else {
        if (node == node->parent_->left_child_) {
          node = node->parent_;
          RightRotate(node);
        }
        node->parent_->color_ = kBlack;
        node->parent_->parent_->color_ = kRed;
        LeftRotate(node->parent_->parent_);
      }
    }
  }
  root_->color_ = kBlack;
}

template <class Key, class Value>
void RBTree<Key, Value>::DeleteFixUp(Node* node) {
  while (node != root_ && node->color_ == kBlack) {
    if (node == node->parent_->left_child_) {
      Node* w = node->parent_->right_child_;
      if (w->color_ == kRed) {  // case1
        w->color_ = kBlack;
        node->parent_->color_ = kRed;
        LeftRotate(node->parent_);
        w = node->parent_->right_child_;
      }
      if (w->left_child_->color_ == kBlack &&
          w->right_child_->color_ == kBlack) {  // case2
        w->color_ = kRed;
        node = node->parent_;
      } else {
        if (w->right_child_->color_ == kBlack) {  // case3
          w->left_child_->color_ = kBlack;
          w->color_ = kRed;
          RightRotate(w);
          w = node->parent_->right_child_;
        }
        w->color_ = node->parent_->color_;  // case4
        node->parent_->color_ = kBlack;
        w->right_child_->color_ = kBlack;
        LeftRotate(node->parent_);
        node = root_;
      }
    } else {
      Node* w = node->parent_->left_child_;
      if (w->color_ == kRed) {
        w->color_ = kBlack;
        node->parent_->color_ = kRed;
        RightRotate(node->parent_);
        w = node->parent_->left_child_;
      }
      if (w->left_child_->color_ == kBlack &&
          w->right_child_->color_ == kBlack) {
        w->color_ = kRed;
        node = node->parent_;
      } else {
        if (w->left_child_->color_ == kBlack) {
          w->right_child_->color_ = kBlack;
          w->color_ = kRed;
          LeftRotate(w);
          w = node->parent_->left_child_;
        }
        w->color_ = node->parent_->color_;
        node->parent_->color_ = kBlack;
        w->left_child_->color_ = kBlack;
        RightRotate(node->parent_);
        node = root_;
      }
    }
  }
  node->color_ = kBlack;
}

template <class Key, class Value>
RB_NODE* RBTree<Key, Value>::GetMin(Node* node) {
  if (node == nil_) return nil_;
  while (node->left_child_ != nil_ && node->left_child_ != NULL) {
    node = node->left_child_;
  }
  return node;
}

template <class Key, class Value>
bool RBTree<Key, Value>::Detele(Key key) {
  if (!Search(key)) {
    cout << "key " << key << " is missing" << endl;
    return false;
  }
  Node* z = GetNode(root_, key);
  Node* y = z;
  Node* x = NULL;
  Color y_original_color = y->color_;
  if (z->left_child_ == nil_) {
    x = z->right_child_;
    Transplant(z, z->right_child_);
  } else {
    if (z->right_child_ == nil_) {
      x = z->left_child_;
      Transplant(z, z->left_child_);
    } else {
      y = GetMin(z->right_child_);
      y_original_color = y->color_;
      x = y->right_child_;
      if (y->parent_ == z) {
        x->parent_ = y;
      } else {
        Transplant(y, y->right_child_);
        y->right_child_ = z->right_child_;
        y->right_child_->parent_ = y;
      }
      Transplant(z, y);
      y->left_child_ = z->left_child_;
      y->left_child_->parent_ = y;
      y->color_ = z->color_;
    }
  }
  if (y_original_color == kBlack) DeleteFixUp(x);
  delete z;
  return true;
}

template <class Key, class Value>
bool RBTree<Key, Value>::Insert(Key key, Value data) {
  if (Search(key)) {
    cout << "key " << key << " is confict" << endl;
    return false;
  }
  Node* z = new Node;
  z->key_ = key;
  z->data_ = data;
  Node* y = nil_;
  Node* x = root_;
  while (x != nil_) {
    y = x;
    if (key < x->key_)
      x = x->left_child_;
    else
      x = x->right_child_;
  }
  z->parent_ = y;
  if (y == nil_) {
    root_ = z;
  } else if (z->key_ < y->key_) {
    y->left_child_ = z;
  } else {
    y->right_child_ = z;
  }
  z->left_child_ = nil_;
  z->right_child_ = nil_;
  z->color_ = kRed;
  InsertFixUp(z);
  return true;
}

template <class Key, class Value>
void RBTree<Key, Value>::Update(Key key, Value data) {
  Node* buf = GetNode(root_, key);
  if (buf == NULL)
    Insert(key, data);
  else
    buf->data_ = data;
}
template <class Key, class Value>
bool RBTree<Key, Value>::Search(Node* node, Key key) {
  if (node == nil_ || node == NULL) return false;
  if (node->key_ == key) return true;
  if (node->key_ > key)
    return Search(node->left_child_, key);
  else
    return Search(node->right_child_, key);
}

template <class Key, class Value>
bool RBTree<Key, Value>::Search(Key key) {
  return Search(root_, key);
}

template <class Key, class Value>
Value RBTree<Key, Value>::GetData(Key key) {
  Node* buf = GetNode(root_, key);
  if (buf == NULL) {
    cout << "key " << key << " is missing" << endl;
    Value x;
    return x;
  }
  return buf->data_;
}