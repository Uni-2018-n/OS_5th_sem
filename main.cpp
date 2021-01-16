#include <bits/stdc++.h>
#include "classes.h"
using namespace std;

#define PAGE_SIZE 4096

void lru_implementation(int mm_frames, int q, int max_traces);
void secondchance_implementation(int mm_frames, int q, int max_traces);


int main(int argc, char **argv){
  int alg, mm_frames, q, max_traces;
  if(argc == 1){
    cout << "wrong or no input, using default" << endl;
    // use default input
    alg= 1;
    mm_frames = 100;
    q = 250;
    max_traces = 20000;
  }else if(argc == 2){
    alg = atoi(argv[1]);
    mm_frames = 100;
    q = 250;
    max_traces = 20000;
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

  if(alg){
    lru_implementation(mm_frames, q, max_traces);
  }else{
    secondchance_implementation(mm_frames, q, max_traces);
  }

	return 0;
}

void lru_implementation(int mm_frames, int q, int max_traces){
  lru_memory main_memory(mm_frames, mm_frames);
  fstream bzip;
  bzip.open("bzip.trace");
  fstream gcc;
  gcc.open("gcc.trace");
  int flag_one =0, flag_two = 0;
  int counter_t=0;
  while(true){
    if(!flag_one){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(bzip, line) || counter_t >= max_traces){
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
          main_memory.insertFirst(page, act);
          counter++;
        }
        counter_t++;
      }
    }

    if(!flag_two){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(gcc, line) || counter_t >= max_traces){
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
          main_memory.insertSecond(page, act);
          counter++;
        }
        counter_t++;
      }
    }
    if(flag_one && flag_two){
      break;
    }
  }
  bzip.close();
  gcc.close();
  cout << "Read counter: " << main_memory.num_of_r << endl;
  cout << "Write counter: " << main_memory.num_of_w << endl;
  cout << "Page fault counter: " << main_memory.num_of_pf << endl;
  cout << "Number of traces(combined for both proccesses): " << counter_t << endl;
  cout << "Available frames in memory: " << mm_frames << endl;
}

void secondchance_implementation(int mm_frames, int q, int max_traces){
  secondchance_memory main_memory(mm_frames, mm_frames);
  fstream bzip;
  bzip.open("bzip.trace");
  fstream gcc;
  gcc.open("gcc.trace");
  int flag_one =0, flag_two = 0;
  int counter_t=0;
  while(true){
    if(!flag_one){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(bzip, line) || counter_t >= max_traces){
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
          main_memory.insertFirst(page, act);
          counter++;
        }
        counter_t++;
      }
    }

    if(!flag_two){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(gcc, line) || counter_t >= max_traces){
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
          main_memory.insertSecond(page, act);
          counter++;
        }
        counter_t++;
      }
    }
    if(flag_one && flag_two){
      break;
    }
  }
  bzip.close();
  gcc.close();
  cout << "Read counter: " << main_memory.num_of_r << endl;
  cout << "Write counter: " << main_memory.num_of_w << endl;
  cout << "Page fault counter: " << main_memory.num_of_pf << endl;
  cout << "Number of traces(combined for both proccesses): " << counter_t << endl;
  cout << "Available frames in memory: " << mm_frames << endl;
}
