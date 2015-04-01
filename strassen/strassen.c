#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>

using namespace std;


#define MAX_NUMBER 100


void normalize(vector<vector<int>>* m) {
  double total_row = pow(2.0, ceil(log2((*m)[0].size())));
  double total_column = pow(2.0, ceil(log2((*m).size())));
  double total = max(total_row, total_column);
  double to_fill_row = total - (*m)[0].size();
  double to_fill_column = total - (*m).size();
  for (auto &v: (*m))
    for (int i = 0; i < to_fill_row; ++i)
      v.push_back(0);
  vector<int> tmp(total, 0);
  for (int i = 0; i < to_fill_column; ++i)
    (*m).push_back(tmp);
}


void sum(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  for (int i = 0; i < (*m1).size(); ++i) {
    for (int j = 0; j < (*m1).size(); ++j) {
      (*result)[i][j] = (*m1)[i][j] + (*m2)[i][j];
    }
  }
}

void substract(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  for (int i = 0; i < (*m1).size(); ++i) {
    for (int j = 0; j < (*m1).size(); ++j) {
      (*result)[i][j] = (*m1)[i][j] - (*m2)[i][j];
    }
  }
}


void multiply(vector<vector<int>>* m1, vector<vector<int>>* m2,
              vector<vector<int>>* result) {
  for (int i = 0; i < (*m1).size(); i++) {
    for (int k = 0; k < (*m1).size(); k++) {
      for (int j = 0; j < (*m1).size(); j++) {
        (*result)[i][j] += (*m1)[i][k] * (*m2)[k][j];
      }
    }
  }
}

void strassen(vector<vector<int>>* m1, vector<vector<int>>* m2,
              vector<vector<int>>* result) {

}

void print_matrix(vector<vector<int>>* m) {
  for (auto &vv: (*m)) {
    for (auto &v: vv)
      cout << v << "\t";
    cout << endl;
  }
}

void generate_random(vector<vector<int>>* m, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    vector<int> tmp(cols);
    generate(tmp.begin(), tmp.end(), rand);
    for (auto &n: tmp)
      n = n % MAX_NUMBER;
    (*m).push_back(tmp);
  }
}

int main() {
  vector<vector<int>> test;
  generate_random(&test, 3, 5);
  print_matrix(&test);
  cout << endl;
  normalize(&test);
  print_matrix(&test);
}
