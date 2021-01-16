#include "classes.h"

proccess::proccess(int b){
  buckets = b;
  table.resize(b);
}

void proccess::insertItem(int pnum, int index){
  struct list_struct l = {pnum, index};
  table[hashFunction(pnum)].push_back(l);
}

int proccess::deleteItem(int pnum){
  int index = hashFunction(pnum);
  list<struct list_struct> :: iterator i;
  int flag =0;
  for(i=table[index].begin(); i != table[index].end();i++){
    if((*i).pagenum == pnum){
      flag =1;
      break;
    }
  }
  if(flag){
    int t= (*i).memindex;
    table[index].erase(i);
    return t;
  }
  return -1;
}

int proccess::searchItem(int pnum){
  int index = hashFunction(pnum);
  list<struct list_struct> :: iterator i;
  for(i=table[index].begin(); i != table[index].end(); i++){
    if((*i).pagenum == pnum){
      return 1;
    }
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
lru_memory::lru_memory(int pl, int buckets):
  first(buckets), second(buckets){
  mm_frames = pl;
  num_of_r =0;
  num_of_w =0;
  num_of_pf=0;
}

void lru_memory::insertFirst(int pnum, int act){
  int flag = first.searchItem(pnum);

  if(flag){ //if pnum already in memory
    list<struct lru_queue_item> :: iterator i;
    for(i = queue.begin(); i != queue.end(); i++){
      if((*i).pagenum == pnum){
        break;
      }
    }
    if((*i).dirty_bit == 0){
      (*i).dirty_bit = act;
    }
    queue.splice(queue.begin(), queue, i);
  }else{ // if pnum not in memory
    num_of_pf++;
    num_of_r++;
    struct lru_queue_item t= {pnum, 0, act};
    queue.push_front(t);
    if(int(queue.size()) > mm_frames){//incase main memory is full we need to place our trace in a victims location
      struct lru_queue_item temp = queue.back();
      queue.pop_back();
      int victim;
      if(temp.htn){
        victim = second.deleteItem(temp.pagenum);
      }else{
        victim = first.deleteItem(temp.pagenum);
      }
      if(temp.dirty_bit == 1){
        num_of_w++;
      }
      array[victim] = {pnum};
      first.insertItem(pnum, victim);
    }else{//incase empty space in memory
      struct mem_item t = {pnum};
      array.push_back(t);
      first.insertItem(pnum, array.size()-1);
    }
  }
}

void lru_memory::insertSecond(int pnum, int act){
  int flag = second.searchItem(pnum);

  if(flag){ //if pnum already in memory
    list<struct lru_queue_item> :: iterator i;
    for(i = queue.begin(); i != queue.end(); i++){
      if((*i).pagenum == pnum){
        break;
      }
    }
    if((*i).dirty_bit == 0){
      (*i).dirty_bit = act;
    }
    queue.splice(queue.begin(), queue, i);
  }else{ // if pnum not in memory
    num_of_pf++;
    num_of_r++;
    struct lru_queue_item t= {pnum, 1, act};
    queue.push_front(t);
    if(int(queue.size()) > mm_frames){//incase main memory is full we need to place our trace in a victims location
      struct lru_queue_item temp= queue.back();
      queue.pop_back();
      int victim;
      if(temp.htn){
        victim = second.deleteItem(temp.pagenum);
      }else{
        victim = first.deleteItem(temp.pagenum);
      }
      if(temp.dirty_bit == 1){
        num_of_w++;
      }
      array[victim] = {pnum};
      second.insertItem(pnum, victim);
    }else{//incase empty space in memory
      struct mem_item t = {pnum};
      array.push_back(t);
      second.insertItem(pnum, array.size()-1);
    }
  }
}
/////////////////////////////////
secondchance_memory::secondchance_memory(int pl, int buckets):
  first(buckets), second(buckets){
  mm_frames = pl;
  num_of_r =0;
  num_of_w =0;
  num_of_pf=0;
}

void secondchance_memory::insertFirst(int pnum, int act){
  int flag = first.searchItem(pnum);

  if(flag){ //if pnum already in memory
    list<struct queue_item> :: iterator i;
    for(i = queue.begin(); i!= queue.end(); i++){
      if((*i).pagenum == pnum){
        break;
      }
    }
    (*i).refnum = 1;
    if((*i).dirty_bit == 0){
      (*i).dirty_bit = act;
    }
  }else{ // if pnum not in memory
    num_of_pf++;
    num_of_r++;
    if(int(queue.size()) == mm_frames){//incase main memory is full we need to place our trace in a victims location
      int fl = 1;
      list<struct queue_item> :: iterator i;
      while(fl){
        for(i = queue.begin(); i != queue.end(); i++){
          if((*i).refnum == 0){
            fl =0;
            break;
          }else{
            (*i).refnum= 0;
          }
        }
      }
      int temp = (*i).pagenum;
      int victim;
      if((*i).pos){
        victim = second.deleteItem(temp);
      }else{
        victim = first.deleteItem(temp);
      }
      if((*i).dirty_bit == 1){
        num_of_w++;
      }

      queue.erase(i);
      struct queue_item qt = {pnum, 0, 0, act};
      queue.push_back(qt);

      array[victim] = {pnum};
      first.insertItem(pnum, victim);
    }else if(int(queue.size()) < mm_frames){//incase empty space in memory
      struct mem_item t = {pnum};
      array.push_back(t);
      first.insertItem(pnum, array.size()-1);

      struct queue_item qt = {pnum, 0, 0, act};
      queue.push_back(qt);
    }
  }
}

void secondchance_memory::insertSecond(int pnum, int act){
  int flag = second.searchItem(pnum);

  if(flag){ //if pnum already in memory
    list<struct queue_item> :: iterator i;
    for(i = queue.begin(); i!= queue.end(); i++){
      if((*i).pagenum == pnum){
        break;
      }
    }
    (*i).refnum = 1;
    if((*i).dirty_bit == 0){
      (*i).dirty_bit = act;
    }
  }else{ // if pnum not in memory
    num_of_pf++;
    num_of_r++;
    if(int(queue.size()) == mm_frames){//incase main memory is full we need to place our trace in a victims location
      int fl = 1;
      list<struct queue_item> :: iterator i;
      while(fl){
        for(i = queue.begin(); i != queue.end(); i++){
          if((*i).refnum == 0){
            fl =0;
            break;
          }else{
            (*i).refnum= 0;
          }
        }
      }
      int temp = (*i).pagenum;
      int victim;
      if((*i).pos){
        victim = second.deleteItem(temp);
      }else{
        victim = first.deleteItem(temp);
      }
      if((*i).dirty_bit == 1){
        num_of_w++;
      }

      queue.erase(i);
      struct queue_item qt = {pnum, 0, 1, act};
      queue.push_back(qt);

      array[victim] = {pnum};
      second.insertItem(pnum, victim);
    }else if(int(queue.size()) < mm_frames){//incase empty space in memory
      struct mem_item t = {pnum};
      array.push_back(t);//add it to the first available location in array
      second.insertItem(pnum, array.size()-1);//and in pt

      struct queue_item qt = {pnum, 0, 1, act};//and in queue
      queue.push_back(qt);
    }
  }
}











//
