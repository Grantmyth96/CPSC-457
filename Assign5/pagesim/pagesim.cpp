#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> ref_string;
int na_frames;
//----------------------------------OPTIMAL ALGORITHM---------------------------
void optimal() {
  unordered_map<int, int> frame_count;
  int page_faults = 0;
  int frame [na_frames];
  bool hit = false;

  for(int i=0; i<na_frames; i++) {
    frame[i] = -1;
  }
//----here is tte start of the algortihm----
  for(int i=0; i<ref_string.size(); i++) {

//checks to see if there is a hit
    for(int j=0; j<na_frames; j++) {
      if(frame[j] == ref_string[i]) {
        hit = true;
        //cout<<"hit is true** ";
      }
    }

//only checks at the start of the execution
    if(hit == false) {
      for(int j=0; j<na_frames; j++) {
        if(frame[j] == -1) {
          frame[j] = ref_string[i];
          //cout << "start* ";
          hit = true;
          j = na_frames;
          page_faults++;
        }
      }
    }

//when there is no hits and all frames are taken
    if(hit == false) {
      //cout<< "hit is false*** ";
      //calculating all frames and when they will be used again
      for(int j=0; j<na_frames; j++) {
        for(int l=i; l<(ref_string.size()); l++) {
          if(frame[j]!=ref_string[l]) frame_count[j] += 1;
          if(frame[j]==ref_string[l]) l = ref_string.size();
        }
      }
      int farthest_page = -1;
      int index;
      //checking if which frame is not being used for the longest time
      for (auto itr = frame_count.begin(); itr != frame_count.end(); ++itr) {
        if(farthest_page == -1) {
          farthest_page = itr-> second;
          index = itr -> first;
        }
        else {
          if(farthest_page <= itr->second) {
            farthest_page = itr -> second;
            index = itr -> first;
          }
        }
        //cout<<itr->first<<" "<<itr->second<<endl;
      }
      frame_count.clear();
      frame[index] = ref_string[i];
      //cout << "farthest and index: "<< farthest_page << " "<< index<<" ";
      page_faults++;
    }
    //cout<<"reference: "<<ref_string[i];
    //cout<<" frame: ";
    //for(int j=0; j<na_frames; j++) {
    //  cout<<frame[j]<<" ";
    //}
    //cout<<endl;
    hit = false;
  }
  cout<<" - frames: ";
  for(int j=0; j<na_frames; j++) {
    cout<<frame[j]<<" ";
  }
  cout<<endl;
  cout<<" - page faults: "<<page_faults<<endl;
}

//--------------------------------LRU ALGORITHM--------------------------------
void lru() {
  unordered_map<int, int> frame_count;
  int page_faults = 0;
  int frame [na_frames];
  bool hit = false;

  for(int i=0; i<na_frames; i++) {
    frame[i] = -1;
    frame_count[i] = 0;
  }
//----here is thte start of the algortihm----
  for(int i=0; i<ref_string.size(); i++) {

//checks to see if there is a hit
    for(int j=0; j<na_frames; j++) {
      if(frame[j] == ref_string[i]) {
        hit = true;
        frame_count[j] = 0;
        //cout<<"hit is true** ";
      }
    }

//only checks at the start of the execution
    if(hit == false) {
      for(int j=0; j<na_frames; j++) {
        if(frame[j] == -1) {
          frame[j] = ref_string[i];
          //cout << "start* ";
          hit = true;
          j = na_frames;
          page_faults++;
        }
      }
    }

//when there is no hits and all frames are taken
    if(hit == false) {
      //cout<< "hit is false*** ";
      int least_used = -1;
      int index;
      //checking if which frame is not being used for the longest time
      for (auto itr = frame_count.begin(); itr != frame_count.end(); ++itr) {
        if(least_used == -1) {
          least_used = itr-> second;
          index = itr -> first;
        }
        else {
          if(least_used <= itr->second) {
            least_used = itr -> second;
            index = itr -> first;
          }
        }
        //cout<<itr->first<<": "<<itr->second<<endl;
      }
      //frame_count.clear();
      frame[index] = ref_string[i];
      frame_count[index] = 0;
      //cout << "farthest and index: "<< farthest_page << " "<< index<<" ";
      page_faults++;
    }
    //cout<<"reference: "<<ref_string[i];
    //cout<<" frame: ";
    //for(int j=0; j<na_frames; j++) {
    //  cout<<frame[j]<<" ";
    //}
    //cout<<endl;
    hit = false;
    for (auto itr = frame_count.begin(); itr != frame_count.end(); ++itr) {
      if(frame[itr->first] != -1) {
        itr->second++;
      }
    }
  }
  cout<<" - frames: ";
  for(int j=0; j<na_frames; j++) {
    cout<<frame[j]<<" ";
  }
  cout<<endl;
  cout<<" - page faults: "<<page_faults<<endl;
}

//---------------------------------CLOCK ALGORITHM-----------------------------
void clock_alg() {
  int page_faults = 0;
  int frame [na_frames];
  int clock_int[na_frames];
  int pointer=0;
  bool hit = false;

  for(int i=0; i<na_frames; i++) {
    frame[i] = -1;
  }

  for(int i=0; i<na_frames; i++) {
    clock_int[i] = 0;
  }


  for(int i=0; i<ref_string.size(); i++) {

      for(int j=0; j<na_frames; j++) {
        if(frame[j] == ref_string[i]) {
          hit = true;
          clock_int[pointer] = 1;
          //cout<<"hit is true** ";
        }
      }
      //cout<<endl;

      /*
      for(int j=0; j<na_frames; j++) {
        cout<<" clock->"<<clock_int[j];
      }
      cout<<" pointer: "<<pointer;
      cout<<" ref: "<<ref_string[i];
      cout<<endl;
      */

      while (hit == false) {
          if(clock_int[pointer] == 0) {
            frame[pointer] = ref_string[i];
            clock_int[pointer] = 1;
            page_faults++;
            if(pointer < na_frames-1) {
              pointer ++;
            } else {
              pointer = 0;
            }
            hit = true;
          } else {
            if(clock_int[pointer] == 1)
            clock_int[pointer] = 0;
            if(pointer < na_frames-1) {
              pointer ++;
            } else {
              pointer = 0;
            }
          }
      }
    hit = false;
    //cout<<"reference: "<<ref_string[i];
    //cout<<" frame: ";
    /*
    for(int j=0; j<na_frames; j++) {
      cout<<frame[j]<<" ";
    }
    cout<<endl;
    */
  }

  cout<<" - frames: ";
  for(int j=0; j<na_frames; j++) {
    cout<<frame[j]<<" ";
  }
  cout<<endl;
  cout<<" - page faults: "<<page_faults<<endl;
}


//---------------------------------MAIN----------------------------------------

int main (int argc, char * const argv[]) {
	string input = "";
  na_frames = atoi(argv[1]);
	while(getline(cin,input)) {
    stringstream ss(input);
    int page;
    while(!ss.eof()) {
      ss>>page;
      ref_string.push_back(page);
      //cout<<page<<endl;
      ss.get();
    }
  }
  cout<<"Optimal:"<<endl;
  optimal();
  cout<<"LRU:"<<endl;
  lru();
  cout<<"Clock:"<<endl;
  clock_alg();

}
