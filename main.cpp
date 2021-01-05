#include <bits/stdc++.h>
#include "classes.h"
using namespace std;

#define PAGE_SIZE 4096

int main(int argc, char **argv){
  int alg, mm_frames, q, max_traces;
  if(argc == 1){
    cout << "wrong or no input, using default" << endl;
    // use default input
    alg= 1;
    mm_frames = 100;
    q = 250;
    max_traces = 1000;
  }else if(argc == 4){
    alg = atoi(argv[1]);
    mm_frames = atoi(argv[2]);
    q = atoi(argv[3]);
    max_traces = 1000;
  }else if(argc == 5){
    alg = atoi(argv[1]);
    mm_frames = atoi(argv[2]);
    q = atoi(argv[3]);
    max_traces = atoi(argv[4]);
  }
  cout << alg  << " " << mm_frames << " "<< q << " " << max_traces << endl;


  memory *main_memory= new memory(mm_frames);

  fstream bzip;
  bzip.open("bzip.trace");
  fstream gcc;
  gcc.open("gcc.trace");
  int flag_one =0, flag_two = 0;
  int one_counter=0, two_counter=0;
  proccess first((32-log(PAGE_SIZE))/4);
  proccess second((32-log(PAGE_SIZE))/4);
  while(true){
    if(!flag_one){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(bzip, line) || one_counter > max_traces){
          flag_one= 1;
          break;
        }else{
          int temp = 0;
          string mem= "";
          while(line[temp] != ' '){
            mem += line[temp];
            temp++;
          }
          temp++;
          char action = line[temp];
          int act;
          if(action == 'R'){
            act = 0;
          }else{
            act = 1;
          }
          int t =(32-log(PAGE_SIZE))/4;
          if(t != int(t)){
            mem.resize(t+1);
          }else{
            mem.resize(t);
          }
          int page = stoi(mem, 0, 16);

          struct list_struct l = first.insertItem(main_memory, page, act);
          if(l.pagenum != -1 && l.memindex != -1){
            second.deleteItem(l.pagenum, l.memindex);
          }
          // cout << "first: " << page << " " << act << endl;
          counter++;
        }
        one_counter++;
      }
    }

    if(!flag_two){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(gcc, line) || two_counter > max_traces){
          flag_two = 1;
          break;
        }else{
          int temp = 0;
          string mem= "";
          while(line[temp] != ' '){
            mem += line[temp];
            temp++;
          }
          temp++;
          char action = line[temp];
          int act;
          if(action == 'R'){
            act = 0;
          }else{
            act = 1;
          }
          int t =(32-log(PAGE_SIZE))/4;
          if(t != int(t)){
            mem.resize(t+1);
          }else{
            mem.resize(t);
          }
          int page = stoi(mem, 0, 16);
          struct list_struct l = second.insertItem(main_memory, page, act);
          if(l.pagenum != -1 && l.memindex != -1){
            first.deleteItem(l.pagenum, l.memindex);
          }
          // cout << "second: " << page << " " << act << endl;
          counter++;
        }
        two_counter++;
      }
    }
    if(flag_one && flag_two){
      break;
    }
  }
  bzip.close();
  gcc.close();
	return 0;
}
