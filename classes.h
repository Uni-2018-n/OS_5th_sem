#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <bits/stdc++.h>
using namespace std;

class memory;

struct list_struct{
  int pagenum;
  int memindex;
};

class proccess{
private:
  int buckets;
  list<struct list_struct> *table;
public:
  proccess(int b);
  // ~proccess();

  struct list_struct insertItem(memory *mem, int pnum, int act);
  int deleteItem(int pnum, int frame);
  int hashFunction(int x);
};


class memory{
  private:
    int mm_frames;
    deque<int> array;
    list<int> queue;
  public:
    memory(int pl);
    // ~memory();
    struct list_struct mem_update(int pnum);
};

#endif
