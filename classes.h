#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <bits/stdc++.h>
using namespace std;

class lru_memory;
class secondchance_memory;

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

  struct list_struct insertItem(lru_memory *mem, int pnum, int act);
  struct list_struct insertItem(secondchance_memory *mem, int pnum, int act);
  int deleteItem(int pnum, int frame);
  int hashFunction(int x);
};


class lru_memory{
  private:
    int mm_frames;
    deque<struct mem_item> array;
    list<int> queue;
  public:
    int num_of_r;
    int num_of_w;
    int num_of_pf;
    lru_memory(int pl);
    // ~memory();
    struct list_struct mem_update(int pnum, int act);
};

struct queue_item{
  int pagenum;
  int refnum;
};

class secondchance_memory{
private:
  int mm_frames;
  deque<struct mem_item> array;
  list<struct queue_item> queue;
public:
  int num_of_r;
  int num_of_w;
  int num_of_pf;
  secondchance_memory(int pl);
  // ~memory();
  struct list_struct mem_update(int pnum, int act);
};

#endif
