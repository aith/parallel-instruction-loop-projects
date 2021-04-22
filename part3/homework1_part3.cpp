#include <iostream>       
#include <thread>        
#include <chrono>

using namespace std;
using namespace std::chrono;

#define ARR_SIZE 1024
#define K 1048576

/* For array a */
int seq_inc(int volatile *a, int a_size) {
  for (int i = 0; i < a_size; i++) {
    while (a[i] < K) {
      a[i]++;
    }
  }
  return 0;
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
  for(int i=tid; i < a_size; i+=num_threads) {
    for(int n=0; n < K; n++) {      
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

int performance_round_robin(int volatile *a, int a_size, int tid, int num_threads) {
	int amt = a_size / num_threads;
	int start = tid*amt;
	int end = start+amt-1;
	for(int i=start; i < end; i+=4) {  // Have the spots be sequential to utilize spatial locality ~ +50% speedup from b on 8 threads
		for(int n=0; n < K; n++) {
			a[i]++;  
			a[i+1]++;  // Interleave to avoid data dependency ~ +40% speedup from b on 8 threads
			a[i+2]++;
			a[i+3]++;
    }
  }            // it takes the same amount of time. The compiler precomputes the full number for us.
  return 0;
}

int performance_round_robin_inc(int volatile *a, int a_size, int num_threads) {
  thread threads[num_threads];
  for(int i=0; i<num_threads; i++) {     
    threads[i] = thread(performance_round_robin, a, a_size, i, num_threads);
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
  int num_threads = 8;

  /* Measure Speedupts */

  auto a_start = high_resolution_clock::now();
  seq_inc(a, ARR_SIZE);
  auto a_stop = high_resolution_clock::now();
  auto a_dur = duration_cast<nanoseconds>(a_stop - a_start);
  double a_sec = a_dur.count()/1000000000.0;

  auto b_start = high_resolution_clock::now();
  round_robin_inc(b, ARR_SIZE, num_threads);
  auto b_stop = high_resolution_clock::now();
  auto b_dur = duration_cast<nanoseconds>(b_stop - b_start);
  double b_sec = b_dur.count()/1000000000.0;

  auto c_start = high_resolution_clock::now();
  performance_round_robin_inc(c, ARR_SIZE, num_threads);
  auto c_stop = high_resolution_clock::now();
  auto c_dur = duration_cast<nanoseconds>(c_stop - c_start);
  double c_sec = c_dur.count()/1000000000.0;

  cout << "a total duration (sec): " << a_sec << endl;
  cout << "b total duration (sec): " << b_sec << endl;
  cout << "c total duration (sec): " << c_sec << endl;
  cout << endl;
  cout << "b's speedup from a (%): " << a_sec / b_sec << endl;
  cout << "c's speedup from a (%): " << a_sec / c_sec << endl;
  cout << "c's speedup from b (%): " << b_sec / c_sec << endl;
}

