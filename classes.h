#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <bits/stdc++.h>
using namespace std;

class memory;

struct list_struct{
  int pagenum;
  int memindex;
};

struct mem_item{
  int pagenum;
  int act;
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
    deque<struct mem_item> array;
    list<int> queue;
  public:
    int num_of_r;
    int num_of_w;
    int num_of_pf;
    memory(int pl);
    // ~memory();
    struct list_struct mem_update(int pnum, int act);
};

#endif
