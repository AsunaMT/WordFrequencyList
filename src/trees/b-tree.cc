#include "b-tree.h"

#include <iostream>

#define B_NODE typename BTree<Key, Value>::Node

using std::cout;
using std::endl;

template <class Key, class Value>
BTree<Key, Value>::Node::Node() {
  int t = MINUM_DEGREE;
  n_ = 0;
  key_ = new Key[2 * t];
  data_ = new Value[2 * t];
  child_ = new Node*[2 * t + 1];
}

template <class Key, class Value>
BTree<Key, Value>::BTree() : t_(MINUM_DEGREE) {
  root_ = new Node;
  root_->leaf_ = true;
  root_->n_ = 0;
}
template <class Key, class Value>
BTree<Key, Value>::~BTree() {
  DestoryTree(root_);
}

template <class Key, class Value>
void BTree<Key, Value>::DestoryTree(Node* node) {
  if (node != NULL) {
    if (!node->leaf_) {
      for (int i = 1; i <= node->n_ + 1; i++) {
        InOrder(node->child_[i]);
      }
    }
    delete node;
  }
}

template <class Key, class Value>
B_NODE* BTree<Key, Value>::GetMin(Node* node) {
  if (node->leaf_)
    return node;
  else
    return GetMin(node->child_[1]);
}

template <class Key, class Value>
B_NODE* BTree<Key, Value>::GetMax(Node* node) {
  if (node->leaf_)
    return node;
  else
    return GetMax(node->child_[node->n_ + 1]);
}

template <class Key, class Value>
Value* BTree<Key, Value>::GetNodeData(Node* node, Key key) {
  int i = 1;
  while (i <= node->n_ && key > node->key_[i]) i++;
  if (i <= node->n_ && key == node->key_[i])
    return &(node->data_[i]);
  else if (node->leaf_)
    return NULL;
  else
    return GetNodeData(node->child_[i], key);
}

template <class Key, class Value>
bool BTree<Key, Value>::Search(Node* node, Key key) {
  int i = 1;
  while (i <= node->n_ && key > node->key_[i]) i++;
  if (i <= node->n_ && key == node->key_[i])
    return true;
  else if (node->leaf_)
    return false;
  else
    return Search(node->child_[i], key);
}

template <class Key, class Value>
void BTree<Key, Value>::SplitChild(Node* node, int i) {
  Node* z = new Node;
  Node* y = node->child_[i];
  z->leaf_ = y->leaf_;
  z->n_ = t_ - 1;
  for (int j = 1; j <= t_ - 1; j++) {
    z->key_[j] = y->key_[j + t_];
    z->data_[j] = y->data_[j + t_];
  }
  if (!y->leaf_)
    for (int j = 1; j <= t_; j++) z->child_[j] = y->child_[j + t_];
  y->n_ = t_ - 1;
  for (int j = node->n_ + 1; j >= i + 1; j--)
    node->child_[j + 1] = node->child_[j];
  node->child_[i + 1] = z;
  for (int j = node->n_; j >= i; j--) {
    node->key_[j + 1] = node->key_[j];
    node->data_[j + 1] = node->data_[j];
  }
  node->key_[i] = y->key_[t_];
  node->data_[i] = y->data_[t_];
  (node->n_)++;
}

template <class Key, class Value>
void BTree<Key, Value>::InsertNonfull(Node* node, Key key, Value data) {
  int i = node->n_;
  if (node->leaf_) {
    while (i >= 1 && key < node->key_[i]) {
      node->key_[i + 1] = node->key_[i];
      node->data_[i + 1] = node->data_[i];
      i--;
    }
    node->key_[i + 1] = key;
    node->data_[i + 1] = data;
    (node->n_)++;
  } else {
    while (i >= 1 && key < node->key_[i]) i--;
    i = i + 1;
    if (node->child_[i]->n_ == 2 * t_ - 1) {
      SplitChild(node, i);
      if (key > node->key_[i]) i = i + 1;
    }
    InsertNonfull(node->child_[i], key, data);
  }
}
template <class Key, class Value>
bool BTree<Key, Value>::Insert(Key key, Value data) {
  if (Search(key)) {
    cout << "key " << key << " is confict" << endl;
    return false;
  }
  Node* r = root_;
  Node* s = new Node;
  if (r->n_ == 2 * t_ - 1) {
    root_ = s;
    s->leaf_ = false;
    s->n_ = 0;
    s->child_[1] = r;
    SplitChild(s, 1);
    InsertNonfull(s, key, data);
  } else {
    InsertNonfull(r, key, data);
  }
  return true;
}

template <class Key, class Value>
bool BTree<Key, Value>::Detele(Key key) {
  if (!Search(key)) {
    cout << "key " << key << " is missing" << endl;
    return false;
  }
  Delete(root_, key);
  return true;
}

template <class Key, class Value>
void BTree<Key, Value>::MergeChilds(Node* node, int i) {
  Node* left = node->child_[i];
  Node* right = node->child_[i + 1];
  left->key_[left->n_ + 1] = node->key_[i];
  left->data_[left->n_ + 1] = node->data_[i];
  for (int j = 1; j <= right->n_; j++) {
    left->key_[left->n_ + 1 + j] = right->key_[j];
    left->data_[left->n_ + 1 + j] = right->data_[j];
  }
  if (!left->leaf_) {
    for (int j = 1; j <= right->n_ + 1; j++) {
      left->child_[left->n_ + 1 + j] = right->child_[j];
    }
  }
  left->n_ += (right->n_ + 1);
  for (int j = i; j < node->n_; j++) {
    node->key_[j] = node->key_[j + 1];
    node->data_[j] = node->data_[j + 1];
    node->child_[j + 1] = node->child_[j + 2];
  }
  node->n_--;
  if (node->n_ == 0) {
    root_ = left;
    delete node;
  }
  delete right;
}

template <class Key, class Value>
void BTree<Key, Value>::Delete(Node* node, Key key) {
  int i = 1;
  while (i <= node->n_ && key > node->key_[i]) i++;
  if (i <= node->n_ && key == node->key_[i]) {
    if (node->leaf_) {  // case1
      for (int j = i; j < node->n_; j++) {
        node->key_[j] = node->key_[j + 1];
        node->data_[j] = node->data_[j + 1];
      }
      node->n_--;
      return;
    } else {
      if (node->child_[i]->n_ >= t_) {  // case2a
        Node* k = GetMax(node->child_[i]);
        node->key_[i] = k->key_[k->n_];
        node->data_[i] = k->data_[k->n_];
        Delete(node->child_[i], k->key_[k->n_]);
      } else {
        if (node->child_[i + 1]->n_ >= t_) {  // case2b
          Node* k = GetMin(node->child_[i + 1]);
          node->key_[i] = k->key_[1];
          node->data_[i] = k->data_[1];
          Delete(node->child_[i + 1], k->key_[1]);
        } else {
          Node* y = node->child_[i];  // case2c
          MergeChilds(node, i);
          Delete(y, key);
        }
      }
    }
  } else {
    Node* aim = node->child_[i];
    if (node->child_[i]->n_ == t_ - 1) {  // case3a
      if (i - 1 > 0 && node->child_[i - 1]->n_ >= t_) {
        Node* self = node->child_[i];
        Node* left = node->child_[i - 1];
        for (int j = self->n_ + 1; j > 1; j--) {
          self->key_[j] = self->key_[j - 1];
          self->data_[j] = self->data_[j - 1];
        }
        self->n_++;
        self->key_[1] = node->key_[i - 1];
        self->data_[1] = node->data_[i - 1];
        if (!left->leaf_) {
          for (int j = self->n_ + 1; j > 1; j--) {
            self->child_[j] = self->child_[j - 1];
          }
          self->child_[1] = left->child_[left->n_ + 1];
        }
        node->key_[i - 1] = left->key_[left->n_];
        node->data_[i - 1] = left->data_[left->n_];
        left->n_--;
      } else {
        if (i + 1 <= node->n_ + 1 && node->child_[i + 1]->n_ >= t_) {
          Node* self = node->child_[i];
          Node* right = node->child_[i + 1];
          self->key_[self->n_ + 1] = node->key_[i];
          self->data_[self->n_ + 1] = node->data_[i];
          self->n_++;
          node->key_[i] = right->key_[1];
          node->data_[i] = right->data_[1];
          for (int j = 1; j < right->n_; j++) {
            right->key_[j] = right->key_[j + 1];
            right->data_[j] = right->data_[j + 1];
          }
          if (!right->leaf_) {
            self->child_[self->n_ + 1] = right->child_[1];
            for (int j = 1; j < right->n_ + 1; j++) {
              right->child_[j] = right->child_[j + 1];
            }
          }
          right->n_--;
        } else {  // case3b
          if (i - 1 > 0 && !node->leaf_) {
            aim = node->child_[i - 1];
            MergeChilds(node, i - 1);
          } else {
            if (i <= node->n_ && !node->leaf_) {
              MergeChilds(node, i);
            }
          }
        }
      }
    }
    Delete(aim, key);  // case3
  }
}

template <class Key, class Value>
void BTree<Key, Value>::Update(Key key, Value data) {
  Value* buf = GetNodeData(root_, key);
  if (buf == NULL)
    Insert(key, data);
  else
    *buf = data;
}

template <class Key, class Value>
bool BTree<Key, Value>::Search(Key key) {
  return Search(root_, key);
}

template <class Key, class Value>
Value BTree<Key, Value>::GetData(Key key) {
  Value* buf = GetNodeData(root_, key);
  if (buf == NULL) {
    cout << "key " << key << " is missing" << endl;
    Value x;
    return x;
  }
  return *buf;
}

template <class Key, class Value>
void BTree<Key, Value>::InOrder(Node* node) {
  if (node != NULL && node->n_ > 0) {
    for (int i = 1; i <= node->n_; i++) {
      if (!node->leaf_) InOrder(node->child_[i]);
      cout << node->key_[i] << " ";
      cout << "- " << node->data_[i] << endl;
    }
    if (!node->leaf_) InOrder(node->child_[node->n_ + 1]);
  }
}

template <class Key, class Value>
void BTree<Key, Value>::InOrder() {
  InOrder(root_);
}