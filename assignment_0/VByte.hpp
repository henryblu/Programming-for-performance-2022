#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h> 

#include "vbit_array.hpp" 
#include "vpacked_array.hpp" 

#define better_shl(x,y) ((y)<64?(x)<<(y):0)
using namespace std;
 
class VByte{
    private:
        vector<uint64_t> vbytes;
        uint64_t blocks=0;
        uint64_t num=0;
        uint64_t k;
        uint64_t tmp=0;
        uint64_t block_count=0;
        uint64_t mask=0;
        uint64_t one=0;
        uint64_t v_size=0;
        uint64_t bits;
        uint64_t *output=nullptr;
        class pfp::bit_array *ba=nullptr;
        class pfp::packed_array *pinta=nullptr;
    public:
    VByte(uint64_t n, uint64_t b_size){
        num=n;
        k=b_size;
        v_size=k+1;
        mask = better_shl(1Ull,k)-1;
        one = better_shl(1ULL,k);
    }
    //encodes the number into the end of the vector vbytes using the vbyte tmp
    void encode(uint64_t number){
        //if the number is longer then one block, encode the first k bits with a leading 0 bit
        while (number > mask){
            tmp|=better_shl((number&mask),((v_size)*block_count%64));
            if (v_size*block_count/64!=(block_count+1)*v_size/64){
                vbytes.push_back(tmp);
                tmp=0;
                if (64-(block_count*v_size%64)==v_size){
                    number >>= k;
                }else{
                    number >>=(64-(block_count*v_size%64));
                    if (64%v_size!=0){
                        uint64_t tmp_mask=better_shl(1Ull,k-(64-((block_count)*v_size%64)))-1;
                        tmp|=number&tmp_mask;
                        number >>=k-(64-((block_count)*v_size%64));
                    }
                }
            } else{
                number >>= k;
            }
            block_count+=1;
        }
        //if the number is smaller then one block encode the numebr with a leading 1 bit
        tmp|=better_shl(((number&mask)|one),(v_size*block_count%64));
        //if the numebr neds to be pucshed onto the array, push it, reset tmp and encript any remaining bits of number onto tmp 
        if (v_size*block_count/64!=(block_count+1)*v_size/64){
            vbytes.push_back(tmp);
            tmp=0;
            number >>=(v_size-((block_count+1)*v_size%64));
            if (number>0){
                tmp|=number|(one>>(v_size-((block_count+1)*v_size%64)));
            }
        }
        block_count+=1;
    }
    // push the last tmp to the end of vbytes and then push an empty index as a buffer
    void push(){
        vbytes.push_back(tmp);
        vbytes.push_back(0ULL);
        tmp=0;
    }
    //return the block count
    uint64_t block(){
        return block_count;
    }
    //decodes the whole data structure from inidex 1 to the last index
    void decode(uint64_t sorted){
        uint64_t counter=0;
        uint64_t runner=0; 
        uint64_t srunner=0; 
        for (uint64_t i=0; i<block_count; i++){
            tmp|=(vbytes[i*v_size/64]>>(i*v_size%64))&(mask+one);
            if (64%v_size!=0) {
                tmp |=better_shl(vbytes[i*v_size/64+1],(64-(i*v_size%64)))&(mask+one);
                }
            if ((tmp&one)!=0){
                runner|=((tmp&mask)<<(counter*k));
                if (sorted==0){
                    cout << runner <<'\n';
                }else{
                    srunner+=runner;
                    cout << srunner <<'\n';
                }counter=0;
                runner=0;
            }else{
                runner|=(tmp&mask)<<(counter*k);
                counter +=1;
            }
            tmp=0;
        }
    }
    // scans through the vector vbytes one block at a time for the querry number and returns the vbyte stored at that querry
    uint64_t scan(uint64_t querry){
        uint64_t counter=0;
        uint64_t runner=0; 
        uint64_t cur=0; 
        for (uint64_t i=0; i<block_count; i++){
            tmp=(vbytes[i*v_size/64]>>(i*v_size%64));
            if (cur==querry){
                tmp&=(mask+one);
                if ((tmp&one)!=0){
                    runner|=(tmp&mask)<<(counter*k);
                    return runner;
                }else{
                    runner|=(tmp&mask)<<(counter*k);
                    counter +=1;
                }
            }
            if ((tmp&(one))!=0){
                cur +=1;
                tmp=0;
            }
        } return 0ULL;
    }
    /**
     * The final 4 functions of Vbyte.hpp are used soley for task 6
    */

    //setup bit array and packed_int_array for location based search 
    void loco_setup(){
        pinta = new pfp::packed_array(num,k);
        ba = new pfp::bit_array(3000000);
    }
    //finds the amount of blocks that a number takes up then stores the number to the packed int array and sets the bit which corrisponds to the cumulative amount of blocks in the bit_array
    void lencode(uint64_t number){
        uint64_t blocks=1;
        if (number!=0) blocks=(63-__builtin_clzll(number))/k+1;
        for (uint64_t q=0; q< blocks;q++){
            pinta -> set(number&mask);
            number >>= k;
        }
        ba -> set(blocks+block_count-1);
        block_count+=blocks;
    }
    //pushes the last packed int into the array and sets up location seaerch for the bit array
    void loco_push(){
        pinta ->push();
        ba ->set_up();
    }
    //finds the last block and the length (in blocks) of the querry using the bit array and then retrieves those blocks from the packed_int array
    uint64_t LQb_get(uint64_t querry){
        uint64_t ind = ba ->get_location(querry+1)-1;
        uint64_t length =1;
        if (querry != 0) length =ind -(ba ->get_location(querry)-1);
        return (pinta -> get_ull(ind,length-1) &((better_shl(1ULL,k*length))-1));
    }

};