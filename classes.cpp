#include "classes.h"

proccess::proccess(int b){
  buckets = b;
  table = new list<struct list_struct>[buckets];
}

struct list_struct proccess::insertItem(lru_memory *mem, int pnum, int act){
  struct list_struct l = mem->mem_update(pnum, act);
  if(l.pagenum == pnum && l.memindex == -1){//no page fault, was already there
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }else if(l.pagenum == pnum && l.memindex != -1){//page fault, free space available
    mem->num_of_pf++;
    table[hashFunction(pnum)].push_back(l);
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }else{ //page fault, need to replace
    mem->num_of_pf++;
    struct list_struct temp;
    temp.pagenum = pnum;
    temp.memindex = l.memindex;
    table[hashFunction(pnum)].push_back(temp);
    if (this->deleteItem(l.pagenum, l.memindex) == 0){
      return l;
    }
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }
}

struct list_struct proccess::insertItem(secondchance_memory *mem, int pnum, int act){
  struct list_struct l = mem->mem_update(pnum, act);
  if(l.pagenum == pnum && l.memindex == -1){//no page fault, was already there
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }else if(l.pagenum == pnum && l.memindex != -1){//page fault, free space available
    mem->num_of_pf++;
    table[hashFunction(pnum)].push_back(l);
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }else{ //page fault, need to replace
    mem->num_of_pf++;
    struct list_struct temp;
    temp.pagenum = pnum;
    temp.memindex = l.memindex;
    table[hashFunction(pnum)].push_back(temp);
    if (this->deleteItem(l.pagenum, l.memindex) == 0){
      return l;
    }
    struct list_struct t;
    t.pagenum= -1;
    t.memindex= -1;
    return t;
  }
}

int proccess::deleteItem(int pnum, int frame){
  int index = hashFunction(pnum);
  list<struct list_struct> :: iterator i;
  for(i=table[index].begin(); i != table[index].end();i++){
    if((*i).pagenum == pnum && (*i).memindex == frame){
      break;
    }
  }
  if(i != table[index].end()){
    table[index].erase(i);
    return 1;
  }
  return 0;
}

int proccess::hashFunction(int x){
  char temp[32];
  sprintf(temp, "%d", x);
  unsigned int hash = 5381; // djb2 hash function from data structures class
  for(int i=0;temp[i] != '\0';i++){
    hash = (hash << 5) + hash + temp[i];
  }
  return hash % buckets;
}

/////////////////////////////////
lru_memory::lru_memory(int pl){
  mm_frames = pl;
  num_of_r =0;
  num_of_w =0;
  num_of_pf=0;
}
struct list_struct lru_memory::mem_update(int pnum, int act){
  list<int> :: iterator i;
  int flag =0;
  for(i = queue.begin(); i!= queue.end(); i++){
    if((*i) == pnum){
      flag =1;
      break;
    }
  }
  if(flag){ //if pnum already in memory
    queue.erase(i);
    queue.push_front(pnum);
    struct list_struct l;
    l.pagenum = pnum;
    l.memindex = -1;
    return l;
  }else{ // if pnum not in memory
    this->num_of_r++;
    queue.push_front(pnum);
    if(int(queue.size()) > mm_frames){//incase main memory is full we need to place our trace in a victims location
      int temp;
      temp = queue.back();
      queue.pop_back();
      for(int j=0; j<mm_frames;j++){//find the victim
        if(array[j].pagenum == temp){
          if(array[j].act == 1){//replace it with the new one
            this->num_of_w++;
          }
          array[j].pagenum = pnum;//replace it with the new one
          array[j].act = act;
          struct list_struct l;
          l.pagenum = pnum;
          l.memindex = j;
          return l;
        }
      }
    }else{//incase empty space in memory
      struct mem_item t;
      t.pagenum = pnum;
      t.act = act;
      array.push_back(t);
      struct list_struct l;
      l.pagenum = pnum;
      l.memindex = array.size();
      return l;
    }
  }
}
/////////////////////////////////
secondchance_memory::secondchance_memory(int pl){
  mm_frames = pl;
  num_of_r =0;
  num_of_w =0;
  num_of_pf=0;
}

struct list_struct secondchance_memory::mem_update(int pnum, int act){
  list<struct queue_item> :: iterator i;
  int flag =0;
  for(i = queue.begin(); i!= queue.end(); i++){
    if((*i).pagenum == pnum){
      flag =1;
      break;
    }
  }
  if(flag){ //if pnum already in memory
    list<struct queue_item> :: iterator i;
    for(i = queue.begin(); i != queue.end(); i++){
      if((*i).pagenum == pnum){
        (*i).refnum = 1;
      }
    }
    struct list_struct l;
    l.pagenum = pnum;
    l.memindex = -1;
    return l;
  }else{ // if pnum not in memory
    this->num_of_r++;
    if(int(queue.size()) < mm_frames){//if there is free space inside memory
      {
        struct queue_item t;
        t.pagenum = pnum;
        t.refnum = 1;
        queue.push_front(t);
      }
      {
        struct mem_item t;
        t.pagenum = pnum;
        t.act = act;
        array.push_back(t);
      }
      struct list_struct l;
      l.pagenum = pnum;
      l.memindex = array.size();
      return l;
    }else{//if there is no free space and we have to find a victim
      int fl = 1;
      list<struct queue_item> :: iterator i;
      while(fl){
        list<struct queue_item> temp;
        for(i = queue.begin(); i != queue.end(); i++){
          if((*i).refnum == 0){
            fl =0;
            break;
          }else{
            (*i).refnum--;
            temp.push_front((*i));//might need to push front here
          }
        }
        queue = temp;
      }
      int j;
      for(j = 0; j < mm_frames; j++){
        if(array[j].pagenum == (*i).pagenum){
          if(array[j].act == 1){//replace it with the new one
            this->num_of_w++;
          }
          array[j].pagenum = pnum;
          array[j].act = act;
          break;
        }
      }
      struct list_struct l;
      l.pagenum = pnum;
      l.memindex = j;
      return l;
    }
  }
}













//
