#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
namespace pfp {

class bit_array{
    private:
    uint64_t max;
    uint64_t bit=1;
    std::vector<uint64_t> bit_arr;
    std::vector<uint64_t> sum_arr;
 
    public:
    bit_array(uint64_t m){
        max = m;
        bit_arr=std::vector<uint64_t>(max/64+1);
        sum_arr=std::vector<uint64_t>(max/64+1);
    }
    
    void set(uint64_t number){//sets numbers in bit_array
        bit_arr[number/64]|=bit<<(number%64);
    }
    bool get(uint64_t number){//gets numbers from bit_array
        return(bit_arr[number/64]>>(number%64))&1;
    }
    //sets up the get_sum and get_location functions by generating an array of 
    //cumulative number of set bits at the end of each bit_arr index
    void set_up(){ 
        uint64_t cur_sum=0;
        for (uint64_t i=1; i <(max/64+1);i++){
            cur_sum+=__builtin_popcountll(bit_arr[i-1]);
            sum_arr[i]=cur_sum;   
        }
    }
    //returns the number of set bits up until a given number
    u_int64_t get_sum(uint64_t number){
        return sum_arr[number/64]+__builtin_popcountll((bit_arr[number/64])&((1ULL<<(number%64))-1));
    }
    //returns the location in the bit array of the number(th) set bit
    u_int64_t get_location(uint64_t number){
        uint64_t s_prev;
        auto iter=std::upper_bound(sum_arr.begin(), sum_arr.end(),number-1);
        s_prev = u_int64_t(std::distance(sum_arr.begin(),iter)-1);
        if (sum_arr[s_prev]==number){
            return(s_prev*64);
        }
        for (uint64_t i=64; i>0; i--){
            if (__builtin_popcountll(bit_arr[s_prev]<<(i-1))+sum_arr[s_prev]==number){
                return ((s_prev+1)*64-(i-1)); 
            }
        }
        return 0ULL;
    }
};
}