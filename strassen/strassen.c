#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <math.h> 
#include <stdlib.h>

using namespace std;

void normalize(vector<vector<int>>* m) {
  double to_fill_row = ceil(log2((*m)[0].size()));
  double to_fill_column = ceil(log2((*m).size()));
  double total_row = to_fill_row + (*m)[0].size();
  double total_column = to_fill_column + (*m).size();
  double total = max(to_fill_row, to_fill_column);
  double amount = abs(total - (*m)[0].size());
  for (auto v: (*m)) {
    for (int i = 0; i < amount; i++)
      v.push_back(0);
  }
  amount = abs(total - (*m).size());
  vector<int> tmp(total, 0);
  for (int i = 0; i < amount; i++)
    (*m).push_back(tmp);
}

void strassen(vector<vector<int>>* m1, vector<vector<int>>* m2) {

}

int main() {
  vector<int> myVector(10);
  generate(myVector.begin(), myVector.end(), rand);
  for (auto v: myVector) {
    cout << v << " ";
  }
  cout << endl;
  vector<vector<int>> test;
  test.push_back(myVector);
  normalize(&test);
  for (auto vv: test) {
    for (auto v: vv)
      cout << v << " ";
    cout << endl;
  }
}
