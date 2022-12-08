#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
namespace pfp {

template <class dtype>
class bit_array{
    private:
    uint64_t max;
    uint64_t bit=1;
    uint64_t *bit_arr=nullptr;
    uint64_t *sum_arr=nullptr;
 
    public:
    ~bit_array(){
        delete[] bit_arr;
        delete[] sum_arr;
        }
    void start(uint64_t m){
        max = m;
        bit_arr = new uint64_t[max/64+1];
    }
    void set(uint64_t number){
        *(bit_arr+number/64)|=bit<<(number%64);
    }
    bool get(uint64_t number){
        return(*(bit_arr+number/64)>>(number%64))&1;
    }
    void set_up_sum(){
        sum_arr = new uint64_t[max/64+1];
        uint64_t cur_sum=0;
        for (uint64_t i=1; i <(max/64+1);i++){
            cur_sum+=uint64_t(__builtin_popcountll(*(bit_arr+i-1)));
            sum_arr[i]=cur_sum;   
        }
    }
    u_int64_t get_sum(uint64_t number){
        return sum_arr[number/64]+__builtin_popcountll(*(bit_arr+number/64)&((1ULL<<(number%64))-1));
    }
    void print(){
        for (u_int64_t i: *bit_arr){
            std::cout << i << "\n";
        }
    }
};
}