#ifndef PJ_WORDFREQUENCYLIST_H_
#define PJ_WORDFREQUENCYLIST_H_

#include <iostream>
#include <string>

#include "trees/b-tree.cc"
#include "trees/rb-tree.cc"

class WordInf {
  friend std::ostream& operator<<(std::ostream& out, WordInf wordinf);

 public:
  std::string part_of_speech;
  double frequency;
};
std::ostream& operator<<(std::ostream& out, WordInf wordinf);

class WordFrequencyList {
 private:
  KeyValueSturct<std::string, WordInf>* list_;
  std::string implementation_;
  void WecomeScreen();
  void Initialize();
  void DeleteWord();
  void InsertWord();
  void UpdateWord();
  void DeleteByFile();
  void InsertByFile();
  void SearchWord();
  void Dump();

 public:
  WordFrequencyList(int way = 1);
  ~WordFrequencyList();
  void RunWordFrequencyList();
};

#endif