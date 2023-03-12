#include "word_frequency_list.h"

int main() {
  int way = 0;
  while (way != 1 && way != 2) {
    cout << "Please chooose the type of the tree:" << endl;
    cout << "(RB-Tree~1  B-Tree~2)" << endl;
    scanf("%d", &way);
  }
  WordFrequencyList a(way);
  a.RunWordFrequencyList();
  return 0;
}