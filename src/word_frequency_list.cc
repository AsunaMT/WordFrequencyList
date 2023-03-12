#include "word_frequency_list.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::cin;
using std::fixed;
using std::getline;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::ostream;
using std::setprecision;
using std::string;

ostream& operator<<(ostream& out, WordInf wordinf) {
  if (wordinf.part_of_speech != "")
    out << "part of speech: " << wordinf.part_of_speech
        << " frequency: " << fixed << setprecision(1) << wordinf.frequency;
  else
    out << endl;
  return out;
}

WordFrequencyList::WordFrequencyList(int way) {
  if (way == 1) {
    list_ = new RBTree<string, WordInf>;
    implementation_ = "RB-Tree";
  }
  if (way == 2) {
    list_ = new BTree<string, WordInf>;
    implementation_ = "B-tree";
  }
}

WordFrequencyList::~WordFrequencyList() { delete list_; }

void WordFrequencyList::WecomeScreen() {
  cout << "********************************" << endl;
  cout << "********************************" << endl;
  cout << "**        English Word        **" << endl;
  cout << "**       Frequency List       **" << endl;
  cout << "********************************" << endl;
  cout << "**  Plaease input your order  **" << endl;
  cout << "********************************" << endl;
  cout << "**        #1 Initialize       **" << endl;
  cout << "**        #2 InsertWord       **" << endl;
  cout << "**        #3 DeleteWord       **" << endl;
  cout << "**        #4 UpdateWord       **" << endl;
  cout << "**        #5 SearchWord       **" << endl;
  cout << "**        #6 InsertWordByFile **" << endl;
  cout << "**        #7 DeleteWordByFile **" << endl;
  cout << "**        #8 Dump             **" << endl;
  cout << "**        #9 Exit             **" << endl;
  cout << "********************************" << endl;
  cout << "********************************" << endl;
  cout << "          #";
}
void WordFrequencyList::Initialize() {
  delete list_;
  if (implementation_ == "RB-Tree")
    list_ = new RBTree<string, WordInf>;
  else
    list_ = new BTree<string, WordInf>;
  InsertByFile();
}

void WordFrequencyList::DeleteWord() {
  fflush(stdin);
  cout << "Please input the word you want to delete. " << endl;
  string key;
  cin >> key;
  list_->Detele(key);
}

void WordFrequencyList::InsertWord() {
  fflush(stdin);
  cout << "Please input the word and it's information you want to insert. "
       << endl;
  string temp, key;
  WordInf value;
  getline(cin, temp, ' ');
  key = temp;
  getline(cin, temp, ' ');
  value.part_of_speech = temp;
  getline(cin, temp, '\n');
  value.frequency = atof(temp.c_str());
  list_->Insert(key, value);
}

void WordFrequencyList::UpdateWord() {
  fflush(stdin);
  cout << "Please input the word and it's information you want to update. "
       << endl;
  string temp, key;
  WordInf value;
  getline(cin, temp, ' ');
  key = temp;
  getline(cin, temp, ' ');
  value.part_of_speech = temp;
  getline(cin, temp, '\n');
  value.frequency = atof(temp.c_str());
  list_->Update(key, value);
}

void WordFrequencyList::DeleteByFile() {
  fflush(stdin);
  cout << "Please input the name of the file. " << endl;
  string name;
  cin >> name;
  ifstream inFile(name, ios::in);
  if (!inFile) {
    cout << "Fail!" << endl;
    return;
  }
  string line;
  getline(inFile, line);
  while (getline(inFile, line)) {
    string temp, key;
    istringstream sin(line);
    getline(sin, temp, ' ');
    key = temp;
    getline(sin, temp, '\n');
    list_->Detele(key);
  }
  inFile.close();
}

void WordFrequencyList::InsertByFile() {
  fflush(stdin);
  cout << "Please input the name of the file. " << endl;
  string name;
  cin >> name;
  ifstream inFile(name, ios::in);
  if (!inFile) {
    cout << "Fail!" << endl;
    return;
  }
  string line;
  getline(inFile, line);
  while (getline(inFile, line)) {
    string temp, key;
    WordInf value;
    istringstream sin(line);
    getline(sin, temp, ' ');
    key = temp;
    getline(sin, temp, ' ');
    value.part_of_speech = temp;
    getline(sin, temp, '\n');
    value.frequency = atof(temp.c_str());
    list_->Insert(key, value);
  }
  inFile.close();
}

void WordFrequencyList::SearchWord() {
  fflush(stdin);
  cout << "Please input the word you want to search. " << endl;
  string key;
  cin >> key;
  cout << list_->GetData(key) << endl;
  getchar();
}

void WordFrequencyList::Dump() {
  fflush(stdin);
  list_->InOrder();
  getchar();
}

void WordFrequencyList::RunWordFrequencyList() {
  string order;
  for (bool startif = true; startif;) {
    fflush(stdin);
    WecomeScreen();
    for (bool buf = true; buf;) {
      cin >> order;
      if (order.size() > 1) {
        cout << "Invalid order, please enter again!" << endl;
        cout << "          #";
        continue;
      }
      buf = false;
    }
    // system("cls");
    switch (order[0]) {
      case '1':
        Initialize();
        break;
      case '2':
        InsertWord();
        break;
      case '3':
        DeleteWord();
        break;
      case '4':
        UpdateWord();
        break;
      case '5':
        SearchWord();
        break;
      case '6':
        InsertByFile();
        break;
      case '7':
        DeleteByFile();
        break;
      case '8':
        Dump();
        break;
      case '9':
        startif = false;
        break;
      default:
        cout << "Invalid order, please enter again!" << endl;
        break;
    }
  }
}