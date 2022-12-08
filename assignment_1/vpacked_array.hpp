#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#define better_shl(x,y) ((y)<64?(x)<<(y):0)
using namespace std;
namespace pfp {
class packed_array{
    private:
        vector<uint64_t> pintarr;
        uint64_t num;
        uint64_t k;
        uint64_t i=0;
        uint64_t tmp=0;
        uint64_t offset=0;
    public:
    packed_array(uint64_t n, uint64_t size){
        num=n;
        k=size;
    }
    //sets the bits in the givern number into value tmp and increments the next set by k 
    void set(uint64_t number){
        tmp|=better_shl((number),(i*k%64));
        if (((i+1)*k)%64>i*k%64){
            i+=1; 
        }else{
            pintarr.push_back(tmp);
            tmp=0;
            i+=1;
            number >>=(k-(i*k%64));
            tmp|=number;
        }
    }
    //pushes the last packed int onto the end of the array and then pushes a 0ULL "buffer"
    void push(){
        pintarr.push_back(tmp);
        pintarr.push_back(0ULL);
        i=0;
        tmp=0;
    }
    //searches the array and gets only the k bits in the index requested (not used in the Vbyte)
    uint64_t get(uint64_t number){
        uint64_t res;
        res=(pintarr[k*number/64]>>(k*number%64))&(better_shl(1ULL,k)-1);
        if (64%k!=0) {
            res |=better_shl(pintarr[(k*number/64)+1],(64-(k*number%64)))&(better_shl(1ULL,k)-1);
        }
        return res;
    }
    //searches the array and returns the entire 64bits after the start of the index requested 
    uint64_t get_ull(uint64_t number,uint64_t length){
        uint64_t res;
        res=(pintarr[k*(number-length)/64]>>(k*(number-length)%64));
        res |=better_shl(pintarr[(k*(number-length)/64)+1],(64-(k*(number-length)%64)));
        return res;
    }
    };
}


