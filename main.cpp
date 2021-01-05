// #include <iostream>
// #include <fstream>
// #include <string.h>
// #include <stdio.h>
#include <bits/stdc++.h>


using namespace std;

#define PAGE_SIZE 4096

int main(int argc, char **argv){
  int alg, pl_mm, q;
  if(argc != 4){
    cout << "wrong or no input, using default" << endl;
    // use default input
    alg= 1;
    pl_mm = 2;
    q = 10;
  }else{
    alg = atoi(argv[1]);
    pl_mm = atoi(argv[2]);
    q = atoi(argv[3]);
  }
  cout << alg  << " " << pl_mm << " "<< q << endl;

  fstream bzip;
  bzip.open("bzip.trace");
  fstream gcc;
  gcc.open("gcc.trace");

  int flag_one =0, flag_two = 0;
  while(true){
    if(!flag_one){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(bzip, line)){
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

          cout << "first: " << page << " " << act << endl;
          counter++;
        }
      }
    }

    if(!flag_two){
      int counter =0;
      while(counter < q){
        string line;
        if(!getline(gcc, line)){
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
          cout << "second: " << page << " " << act << endl;
          counter++;
        }
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
// https://www.programming9.com/programs/c-programs/285-page-replacement-programs-in-c
