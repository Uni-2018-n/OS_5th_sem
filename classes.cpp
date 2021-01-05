#include "classes.h"

proccess::proccess(int b){
  buckets = b;
  table = new list<struct list_struct>[buckets];
}

struct list_struct proccess::insertItem(memory *mem, int pnum, int act){
  if(act){
    mem->num_of_r++;
  }else{
    mem->num_of_w++;
  }
  struct list_struct l = mem->mem_update(pnum);
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
memory::memory(int pl){
  mm_frames = pl;
  num_of_r =0;
  num_of_w =0;
  num_of_pf=0;
}
struct list_struct memory::mem_update(int pnum){
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
    queue.push_front(pnum);
    if(int(queue.size()) > mm_frames){
      int temp;
      temp = *(queue.end());
      queue.pop_back();
      for(int j=0; j<mm_frames;j++){
        if(array[j] == temp){
          array[j] = pnum;
          struct list_struct l;
          l.pagenum = pnum;
          l.memindex = j;
          return l;
        }
      }
    }else{
      array.push_back(pnum);
      struct list_struct l;
      l.pagenum = pnum;
      l.memindex = array.size();
      return l;
    }
  }
}














//
