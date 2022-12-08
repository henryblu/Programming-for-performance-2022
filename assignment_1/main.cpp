//TOGGLESTATE=0:1
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include "header.hpp"
#include "bit_array.hpp"

#define readx(in,x) in.read(reinterpret_cast<char*>(&x),sizeof(x))

// this fuction outputs n ULLs from file in into the given vector 
void read_n(std::istream &in, std::vector<uint64_t>&number_arr,uint64_t n){
    for (uint64_t k=0;k<n;k++){
        uint64_t tmp=0;
        readx(in,tmp);
        number_arr[k]=tmp;
    } 
}

void unpackage(std::istream &in, int type, int time, int sort_){
    uint64_t n=0, m=0;
    readx(in,n);
    readx(in,m);
    std::chrono::time_point<std::chrono::system_clock>start, end;
    std::vector<uint64_t> numbers=std::vector<uint64_t>(n);

    if (time){
        start = std::chrono::system_clock::now();
    }
    //read first n numbers and if sort, sort them
    read_n(in,numbers,n);
    if (sort_) sort(numbers.begin(),numbers.end());

    //initilize class ba and call the start function which initilises the bit_array
    pfp::bit_array<uint64_t> ba;
    ba.start(m);
    //set the numebrs in numebrs in the bit array 
    for (uint64_t i=0; i< numbers.size();i++){
        ba.set(numbers[i]);
    }
    
    if (time){
        std::chrono::duration<double> elapsed_time_s = std::chrono::system_clock::now() - start;
        std::cerr <<"set_up time: " << elapsed_time_s.count() << "s\n";
    }
    //read tehe next n numebrs and sort if needed 
    read_n(in,numbers,n); 
    if (sort_) sort(numbers.begin(),numbers.end());
    // if finding th esum set up the sum_array in the bit array class
    if (type==2){
        ba.set_up_sum();
    }
    if (time) start = std::chrono::system_clock::now();
    //go throught the set bit_array and see if the next n numbers are in that array
    if (type==1){
        for (uint64_t i=0; i< numbers.size();i++){
            std::cout<<ba.get(numbers[i])<<"\n";
        }
    } 
    //if finding sum, find the sum of set bits lower then each element in n and output it
    else if(type==2){
        for (uint64_t i=0; i< numbers.size();i++){
            std::cout<<ba.get_sum(numbers[i])<<"\n";
        } 
    }
    
    if (time){
        std::chrono::duration<double> elapsed_time_s = std::chrono::system_clock::now() - start;
        std::cerr <<"querry time: " << elapsed_time_s.count() << "s\n";
    }
}

int main(int argc, char const* argv[]) {
    //defult peramiters
    int input_file = 0; 
    int sort = 0;
    int i = 1;
    int type = 1;
    int timed = false;
    // get: type = 1, sum: type = 2
    while (i < argc) {
        std::string s(argv[i++]); 
        if (s.compare("-t") == 0)
            {timed=true;}
        else if (s.compare("-g") == 0)
            {type = 1;}
        else if (s.compare("-s") == 0)
            {type = 2;}
        else if (s.compare("-sor") == 0)
            {sort = 1;}

        else {input_file = i - 1;}
    }
    if (input_file > 0) {
        std::ifstream in(argv[input_file]);
        unpackage(in,type,timed,sort);
        }
    else {
        unpackage(std::cin,type,timed,sort);
    }
    return 0;
}