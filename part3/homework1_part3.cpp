#include <iostream>       
#include <thread>        

using namespace std;

#define ARR_SIZE 1024
#define K 1048576

/* For array a */
void seq_inc(int volatile *a, int a_size) {
  for (int i = 0; i < a_size; i++) {
    while (a[i] < K) {
      a[i]++;
    }
  }
}

/*
  For array b, you must do this computation in parallel by writing an SMPD style 
  function. You must access elements in a round-robin style. That is, a thread 
  with thread id of tid must compute the elements at index i where
  i % NUM_THREADS = tid.
  A thread must complete all increment operations on a location before moving on 
  to the next location. It must operate on its location in order.
*/

int round_robin(int volatile *a, int a_size, int tid, int num_threads) {
  for(int i=0; i < K; i++) {  // This order seems better because no dependency cahin
    for(int i=tid; i < a_size; i+=num_threads) {
      a[i]++;
    }
  }
  return 0;
}

int round_robin_inc(int volatile *a, int a_size, int num_threads) {
  thread threads[num_threads];
  for(int i=0; i<num_threads; i++) {
    threads[i] = thread(round_robin, a, a_size, i, num_threads);
  }
  for(int i=0; i< num_threads; i++) {
    threads[i].join();
  }
  return 0;
}

int main() {

  int a[ARR_SIZE] = {0};  // Initialize arrays to 0
  int b[ARR_SIZE] = {0};
  int c[ARR_SIZE] = {0};
  int d[ARR_SIZE] = {0};
  int num_threads = 4;

  a[0] = 55;
  //seq_inc(a, ARR_SIZE);
  //cout << a[0] << endl;
  round_robin_inc(b, ARR_SIZE, num_threads);
  cout << b[0] << endl;
}

