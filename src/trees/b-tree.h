#ifndef PJ_TREES_BTREE_H_
#define PJ_TREES_BTREE_H_

#include "key-value_struct.h"

const int MINUM_DEGREE = 3;

template <class Key, class Value>
class BTree : public KeyValueSturct<Key, Value> {
 private:
  class Node {
   public:
    int n_;
    Key* key_;
    Value* data_;
    bool leaf_;
    Node** child_;
    Node();
  };
  Node* root_;
  void DestoryTree(Node* node);
  Node* GetMin(Node* node);
  Node* GetMax(Node* node);
  bool Search(Node* node, Key key);
  void InOrder(Node* node);
  Value* GetNodeData(Node* node, Key key);
  void SplitChild(Node* node, int i);
  void InsertNonfull(Node* node, Key key, Value value);
  void Delete(Node* node, Key key);
  void MergeChilds(Node* node, int i);

 public:
  const int t_;  // minmum degree
  BTree();
  ~BTree();
  bool Insert(Key key, Value data);
  bool Detele(Key key);
  void Update(Key key, Value data);
  bool Search(Key key);
  Value GetData(Key key);
  void InOrder();
};
#endif