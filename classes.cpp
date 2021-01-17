#include "classes.h"

proccess::proccess(int b){
  buckets = b;
  table.resize(b);//this is helpfull so we use the array with O(1)
}

void proccess::insertItem(int pnum, int index){
  table[hashFunction(pnum)].push_back({pnum, index});
}

int proccess::deleteItem(int pnum){
  int index = hashFunction(pnum);
  int flag =0;
  list<struct list_struct> :: iterator i;
  for(i=table[index].begin(); i != table[index].end();i++){//find the desired page need to delete
    if((*i).pagenum == pnum){
      flag =1;
      break;
    }
  }
  if(flag){//used this in case loop finished witout finding it
    int t= (*i).memindex;
    table[index].erase(i);//if found delete it
    return t;//and return the page's location in memory for the programm to use
  }
  return -1;
}

int proccess::searchItem(int pnum){
  int index = hashFunction(pnum);
  list<struct list_struct> :: iterator i;
  for(i=table[index].begin(); i != table[index].end(); i++){//used this to check if page already in memory
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
    if((*i).dirty_bit == 0){//incase now we have a write command
      (*i).dirty_bit = act;
    }
    queue.splice(queue.begin(), queue, i);
  }else{ // if pnum not in memory
    num_of_pf++;//cause we readed from the disl
    num_of_r++;//cause page isnt in our main memory yet
    queue.push_front({pnum, 0, act});
    if(int(queue.size()) > mm_frames){//incase main memory is full we need to place our trace in a victims location
      struct lru_queue_item temp = queue.back();
      queue.pop_back();
      int victim;
      if(temp.htn){//delete from pt and get the memory's index of the page
        victim = second.deleteItem(temp.pagenum);
      }else{
        victim = first.deleteItem(temp.pagenum);
      }
      if(temp.dirty_bit == 1){//simulator way of saying write back to the disk
        num_of_w++;
      }
      array[victim] = pnum;//update the page in main memory
      first.insertItem(pnum, victim);//insert the new page inside the pt
    }else{//incase empty space in memory
      array.push_back(pnum);
      first.insertItem(pnum, array.size()-1);//there is no way of setting a main's memory locatin again to empty so this works
    }
  }
}

void lru_memory::insertSecond(int pnum, int act){//same as before but now everything for second pt
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
    queue.push_front({pnum, 1, act});
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
      array[victim] = pnum;
      second.insertItem(pnum, victim);
    }else{//incase empty space in memory
      array.push_back(pnum);
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

void secondchance_memory::insertFirst(int pnum, int act){//used simmilar logic as before but now with a second chance logic
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
      while(fl){//used this way of making a "circled list"
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
      if((*i).htn){
        victim = second.deleteItem(temp);
      }else{
        victim = first.deleteItem(temp);
      }
      if((*i).dirty_bit == 1){
        num_of_w++;
      }

      queue.erase(i);
      queue.push_back({pnum, 0, 0, act});//as all, refrence number starts with 0 and becomes 1 if we re-request the page

      array[victim] = pnum;
      first.insertItem(pnum, victim);
    }else if(int(queue.size()) < mm_frames){//incase empty space in memory
      array.push_back(pnum);
      first.insertItem(pnum, array.size()-1);//again cause we wont ever completly delete something from the array, just replace it

      queue.push_back({pnum, 0, 0, act});
    }
  }
}

void secondchance_memory::insertSecond(int pnum, int act){//same as before but for second pt
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
      if((*i).htn){
        victim = second.deleteItem(temp);
      }else{
        victim = first.deleteItem(temp);
      }
      if((*i).dirty_bit == 1){
        num_of_w++;
      }

      queue.erase(i);
      queue.push_back({pnum, 0, 1, act});

      array[victim] = pnum;
      second.insertItem(pnum, victim);
    }else if(int(queue.size()) < mm_frames){//incase empty space in memory
      array.push_back(pnum);
      second.insertItem(pnum, array.size()-1);

      queue.push_back({pnum, 0, 1, act});
    }
  }
}
