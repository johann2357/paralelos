#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>


using namespace std;


#define MAX_RAND_NUMBER 10
#define NTHREADS 3


#ifdef NTHREADS
/**
 * BEGIN OF PARALLEL STUFF
**/
struct ParallelData {
  vector<vector<int>> *m1, *m2, *result;
  int begin, end;
};

void* parallel_sum(void *pd) {
  ParallelData* data = (ParallelData*)pd;
  for (int i = (*data).begin; i < (*data).end; ++i)
    for (int j = 0; j < (*(*data).m1).size(); ++j)
      (*(*data).result)[i][j] = (*(*data).m1)[i][j] + (*(*data).m2)[i][j];
}

void* parallel_subtract(void *pd) {
  ParallelData* data = (ParallelData*)pd;
  for (int i = (*data).begin; i < (*data).end; ++i)
    for (int j = 0; j < (*(*data).m1).size(); ++j)
      (*(*data).result)[i][j] = (*(*data).m1)[i][j] - (*(*data).m2)[i][j];
}

void distribution(vector<vector<int>>* m1, vector<vector<int>>* m2,
                  vector<vector<int>>* result, void* (*operation)(void*)) {
  pthread_t* thread_id = new pthread_t[NTHREADS];
  ParallelData* thread_data = new ParallelData[NTHREADS];
  int begin = 0;
  int end = 0;
  int chunk_size = (*m1).size() / NTHREADS;
  for(int i = 0; ((i < NTHREADS) && (end < (*m1).size())); ++i) {
    if (i == NTHREADS - 1)
      end = (*m1).size();
    else
      end = begin + chunk_size;
    thread_data[i].m1 = m1;
    thread_data[i].m2 = m2;
    thread_data[i].result = result;
    thread_data[i].begin = begin;
    thread_data[i].end = end;
    pthread_create(
      &thread_id[i],
      NULL,
      *operation,
      (void *) &thread_data[i]
    );
    end = begin + chunk_size;
    begin = end;
  }
  for(int j = 0; j < NTHREADS; ++j)
    pthread_join(thread_id[j], NULL);
  delete[] thread_id;
  delete[] thread_data;
}

void sum(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  distribution(m1, m2, result, &parallel_sum);
}
void subtract(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  distribution(m1, m2, result, &parallel_subtract);
}
/**
 * END OF PARALLEL STUFF
**/
#else
/**
 * BEGIN OF NON PARALLEL STUFF
**/
void sum(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  for (int i = 0; i < (*m1).size(); ++i)
    for (int j = 0; j < (*m1).size(); ++j)
      (*result)[i][j] = (*m1)[i][j] + (*m2)[i][j];
}

void subtract(vector<vector<int>>* m1, vector<vector<int>>* m2,
         vector<vector<int>>* result) {
  for (int i = 0; i < (*m1).size(); ++i)
    for (int j = 0; j < (*m1).size(); ++j)
      (*result)[i][j] = (*m1)[i][j] - (*m2)[i][j];
}
/**
 * END OF NON PARALLEL STUFF
**/
#endif

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
  /*
   * SOURCE http://en.wikipedia.org/wiki/Strassen_algorithm
  */
  if ((*m1).size() <= 1) {
    multiply(m1, m2, result);
    return;
  }
  else {
    int size = (*m1).size() / 2;
    vector<int> tmp (size);
    vector<vector<int>>
      a11(size, tmp), a12(size, tmp), a21(size, tmp), a22(size, tmp),
      b11(size, tmp), b12(size, tmp), b21(size, tmp), b22(size, tmp),
      c11(size, tmp), c12(size,tmp),
      c21(size,tmp), c22(size,tmp),
      p1(size, tmp), p2(size, tmp), p3(size, tmp), p4(size, tmp),
      p5(size, tmp), p6(size, tmp), p7(size, tmp),
      result_a(size, tmp), result_b(size, tmp);

    /*
     * CALCULATING THE SPLIT
     * STORED IN As and Bs
    */
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        a11[i][j] = (*m1)[i][j];
        b11[i][j] = (*m2)[i][j];
        a12[i][j] = (*m1)[i][j + size];
        b12[i][j] = (*m2)[i][j + size];
        a21[i][j] = (*m1)[i + size][j];
        b21[i][j] = (*m2)[i + size][j];
        a22[i][j] = (*m1)[i + size][j + size];
        b22[i][j] = (*m2)[i + size][j + size];
      }
    }


    /*
     * CALCULATING M1 - M7 (IN ORDER)
     * NAMED P1 - P7 TO AVOID CONFUSION WITH THE FUNCTION PARAMETERS
    */
    sum(&a11, &a22, &result_a);
    sum(&b11, &b22, &result_b);
    strassen(&result_a, &result_b, &p1); // (a11 + a22) * (b11 + b22)

    sum(&a21, &a22, &result_a);
    strassen(&result_a, &b11, &p2); // (a21 + a22) * (b11)

    subtract(&b12, &b22, &result_b);
    strassen(&a11, &result_b, &p3); // (a11) * (b12 - b22)

    subtract(&b21, &b11, &result_b);
    strassen(&a22, &result_b, &p4); // (a22) * (b21 - b11)

    sum(&a11, &a12, &result_a);
    strassen(&result_a, &b22, &p5); // (a11 + a12) * (b22)

    subtract(&a21, &a11, &result_a);
    sum(&b11, &b12, &result_b);
    strassen(&result_a, &result_b, &p6); // (a21 - a11) * (b11 + b12)

    subtract(&a12, &a22, &result_a);
    sum(&b21, &b22, &result_b);
    strassen(&result_a, &result_b, &p7); // (a12 - a22) * (b21 + b22)


    /*
     * CALCULATING Cs
    */
    sum(&p1, &p4, &result_a);
    sum(&result_a, &p7, &result_b);
    subtract(&result_b, &p5, &c11); // p1 + p4 - p5 + p7

    sum(&p3, &p5, &c12); // p3 + p5

    sum(&p2, &p4, &c21); // p2 + p4

    sum(&p1, &p3, &result_a);
    sum(&result_a, &p6, &result_b);
    subtract(&result_b, &p2, &c22); // p1 + p3 - p2 + p6

    for (int i = 0; i < size ; i++) {
      for (int j = 0 ; j < size ; j++) {
        (*result)[i][j] = c11[i][j];
        (*result)[i][j + size] = c12[i][j];
        (*result)[i + size][j] = c21[i][j];
        (*result)[i + size][j + size] = c22[i][j];
      }
    }
  }
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
      n = n % MAX_RAND_NUMBER;
    (*m).push_back(tmp);
  }
}

int main() {
  srand(time(0));

  int m1_rows, m1_cols, m2_rows, m2_cols;

  cout << "Matrix 1 rows: ";
  cin >> m1_rows;
  cout << "Matrix 1 cols: ";
  cin >> m1_cols;

  cout << "Matrix 2 rows: ";
  cin >> m2_rows;
  cout << "Matrix 2 cols: ";
  cin >> m2_cols;

  if (m1_cols != m2_rows) {
    cout << "Cannot multiply!!\n";
    return 1;
  }

  int result_rows_cols = max(max(m1_rows, m2_rows), max(m1_cols, m2_cols));

  cout << "MATRIX 1:\n" ;
  vector<vector<int>> m1;
  generate_random(&m1, m1_rows, m1_cols);
  normalize(&m1);
  print_matrix(&m1);

  cout << "MATRIX 2:\n" ;
  vector<vector<int>> m2;
  generate_random(&m2, m2_rows, m2_cols);
  normalize(&m2);
  print_matrix(&m2);

  cout << "RESULT:\n" ;
  vector<int> tmp(result_rows_cols);
  vector<vector<int>> result(result_rows_cols, tmp);
  normalize(&result);
  strassen(&m1, &m2, &result);
  print_matrix(&result);
}
