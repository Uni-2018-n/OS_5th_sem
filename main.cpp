#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;

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


  int counter = 0;
  int flag = 1;
  while(counter < q*2){
    if(flag){ // case read first trace
      flag = 0;
      string line;
      getline(bzip, line);
      int counter = 0;
      string mem = "";
      while(line[counter] != ' '){
        mem += line[counter];
        counter++;
      }
      counter++;
      char action = line[counter];
      cout << "first: " << mem << " " << action << endl;
    }else{// case read second trace
      flag = 1;
      string line;
      getline(gcc, line);
      int counter = 0;
      string mem = "";
      while(line[counter] != ' '){
        mem += line[counter];
        counter++;
      }
      counter++;
      char action = line[counter];
      cout << "second: " << mem << " " << action << endl;
    }
    counter++;
  }

  bzip.close();
  gcc.close();
	return 0;
}
// https://www.programming9.com/programs/c-programs/285-page-replacement-programs-in-c
