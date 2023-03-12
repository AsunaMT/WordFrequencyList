#ifndef PJ_TREES_RBTREE_H_
#define PJ_TREES_RBTREE_H_

#include <string>

#include "key-value_struct.h"

enum Color { kRed, kBlack };

template <class Key, class Value>
class RBTree : public KeyValueSturct<Key, Value> {
 private:
  struct Node {
    Color color_;
    Key key_;
    Value data_;
    Node* parent_ = NULL;
    Node* left_child_ = NULL;
    Node* right_child_ = NULL;
  };
  Node* root_;
  Node* nil_;
  void DestoryTree(Node* node);
  Node* GetMin(Node* node);
  Node* GetNode(Node* node, Key key);
  void Transplant(Node* subtree1, Node* subtree2);
  void InOrder(Node* node);
  bool Search(Node* node, Key key);
  void LeftRotate(Node* node);
  void RightRotate(Node* node);
  void InsertFixUp(Node* node);
  void DeleteFixUp(Node* node);

 public:
  RBTree();
  ~RBTree();
  bool Insert(Key key, Value data);
  bool Detele(Key key);
  void Update(Key key, Value data);
  bool Search(Key key);
  Value GetData(Key key);
  void InOrder();
};

#endif