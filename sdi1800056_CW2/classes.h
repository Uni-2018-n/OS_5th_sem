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

class proccess{
private:
  int buckets;
  vector<list<struct list_struct>> table;
public:
  proccess(int b);

  void insertItem(int pnum, int index);
  int deleteItem(int pnum);
  int searchItem(int pnum);
  int hashFunction(int x);
};

struct lru_queue_item{
  int pagenum;
  int htn;//used to see if page is in first pt or second
  int dirty_bit;
};

class lru_memory{
  private:
    int mm_frames;
    vector<int> array;
    list<struct lru_queue_item> queue;
    proccess first;
    proccess second;
  public:
    int num_of_r;
    int num_of_w;
    int num_of_pf;
    lru_memory(int pl, int buckets);

    void insertFirst(int pnum, int act);
    void insertSecond(int pnum, int act);
};

struct queue_item{
  int pagenum;
  int refnum;
  int htn;
  int dirty_bit;
};

class secondchance_memory{
private:
  int mm_frames;
  vector<int> array;
  list<struct queue_item> queue;
  proccess first;
  proccess second;
public:
  int num_of_r;
  int num_of_w;
  int num_of_pf;
  secondchance_memory(int pl, int buckets);

  void insertFirst(int pnum, int act);
  void insertSecond(int pnum, int act);
};

#endif
