#ifndef PJ_TREES_KEYVALUESTRUCT_H_
#define PJ_TREES_KEYVALUESTRUCT_H_

template <class Key, class Value>
class KeyValueSturct {
 public:
  virtual bool Insert(Key key, Value data) = 0;
  virtual bool Detele(Key key) = 0;
  virtual void Update(Key key, Value data) = 0;
  virtual Value GetData(Key key) = 0;
  virtual bool Search(Key key) = 0;
  virtual void InOrder() = 0;
};

#endif