
#include <iostream>
#include <assert.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef double reduce_type;


void reference_reduction(reduce_type *b, int size) {
  for (int i = 1; i < size; i++) {
    b[0] += b[i];
  }
}
void homework_reduction(reduce_type *a, int size) {
    int part_size = size / 16;
    float part0 = 0;
    float part1 = 0;
    float part2 = 0;
    float part3 = 0;
    float part4 = 0;
    float part5 = 0;
    float part6 = 0;
    float part7 = 0;
    float part8 = 0;
    float part9 = 0;
    float part10 = 0;
    float part11 = 0;
    float part12 = 0;
    float part13 = 0;
    float part14 = 0;
    float part15 = 0;
    for(int i = 0; i<part_size; i+=1) {
      part0 += a[part_size*0 + i]; 
      part1 += a[part_size*1 + i]; 
      part2 += a[part_size*2 + i]; 
      part3 += a[part_size*3 + i]; 
      part4 += a[part_size*4 + i]; 
      part5 += a[part_size*5 + i]; 
      part6 += a[part_size*6 + i]; 
      part7 += a[part_size*7 + i]; 
      part8 += a[part_size*8 + i]; 
      part9 += a[part_size*9 + i]; 
      part10 += a[part_size*10 + i]; 
      part11 += a[part_size*11 + i]; 
      part12 += a[part_size*12 + i]; 
      part13 += a[part_size*13 + i]; 
      part14 += a[part_size*14 + i]; 
      part15 += a[part_size*15 + i]; 
    }
    part0 += part1;
    part0 += part2;
    part0 += part3;
    part0 += part4;
    part0 += part5;
    part0 += part6;
    part0 += part7;
    part0 += part8;
    part0 += part9;
    part0 += part10;
    part0 += part11;
    part0 += part12;
    part0 += part13;
    part0 += part14;
    part0 += part15;

    a[0] = part0;

}

#define SIZE (1024*1024*64)


int main() {
  reduce_type *a;
  a = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  reduce_type *b;
  b = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  for (int i = 0; i < SIZE; i++) {
    a[i] = 1;
    b[i] = 1;
  }

  auto new_start = high_resolution_clock::now();
  homework_reduction(a,SIZE);
  auto new_stop = high_resolution_clock::now();
  auto new_duration = duration_cast<nanoseconds>(new_stop - new_start);
  double new_seconds = new_duration.count()/1000000000.0;

  auto ref_start = high_resolution_clock::now();
  reference_reduction(b,SIZE);
  auto ref_stop = high_resolution_clock::now();
  auto ref_duration = duration_cast<nanoseconds>(ref_stop - ref_start);
  double ref_seconds = ref_duration.count()/1000000000.0;

  cout << "new loop time: " << new_seconds << endl; 
  cout << "reference loop time: " << ref_seconds << endl; 
  cout << "speedup: " << ref_seconds / new_seconds << endl << endl;

  return 0;
}
