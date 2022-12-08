/**
 * Preface
 * 
 * Bellow is the code for tasts 1-4 & 6.
 * I had attempted task 5, however the code didn't work, was uncommented and was a mess
 * so it is not included in this .zip.
 * Unfortunatly, I due to time constraints I was not able to complete the written assignments
 * so all I have here is the commented code.
 * 
 * 
*/

using namespace std;
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <stdio.h>
#include <string.h>

#include "VByte.hpp"
#include "vbit_array.hpp" 
#include "vpacked_array.hpp" 

#define readx(in,x) in.read(reinterpret_cast<char*>(&x),sizeof(x))

void help() {
    cout
        << "Write your program description here\n"
        << "Usage:\n"
        << "   ./main [options] [input file]\n\n"
        << "Options:\n"
        << "-h             Outputs this message and terminates.\n"
        << "-get           Type flag =0, decodes the vbyte array \n"
        << "-scan          Type flag =1, scans the vbyte array for specified index querrys .\n"
        << "-lbq           Type flag =2, uses location based querrys to scan the vbyte array for specified index querrys\n"
        << "-time          Output timing in stderr.\n"
        << "-mem           Output total memory usage in stderr.\n"
        << "With multiple type flags, the last one is chosen.\n"
        << endl;
}

int parseMemLine(char* line){
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getKBMemUsage(){ //parseMemLine and getKBMemUsage returns the memory usage (found by Anton Bogun)
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseMemLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
// encode, encodes the input data into a Vbyte array
void encode(istream &in,VByte *bytes,uint64_t n, uint64_t sorted){
    if (sorted>0){
        uint64_t prev=0;
        for (uint64_t b=0;b<n;b++){
            uint64_t tmp=0;
            readx(in,tmp);
            bytes ->encode(tmp-prev);
            prev=tmp;
        }
    }else{
        for (uint64_t b=0;b<n;b++){
            uint64_t tmp=0;
            readx(in,tmp);
            bytes ->encode(tmp);
        }
    }bytes ->push();
}
//loco_encode, encodes the data into a packed int array and sets blocks using a bit_Array
void loco_encode(istream &in,VByte *bytes,uint64_t n){
    bytes -> loco_setup();
    for (uint64_t b=0;b<n;b++){
        uint64_t tmp=0;
        readx(in,tmp);
        bytes ->lencode(tmp);
    }bytes ->loco_push();
}
// decoding, decodes the data stored in Vbyte array
void decoding(VByte *bytes,uint64_t sorted){
    cout << bytes -> block() << '\n';
    bytes -> decode(sorted);
}
// scan, scans through the vbyte array for the search querrys stored in the input file
void scan(istream &in,VByte *bytes, uint64_t n){
    cout << bytes -> block() << '\n';
    for (uint64_t b=0;b<n;b++){
        uint64_t tmp=0;
        readx(in,tmp);
        cout << bytes ->scan(tmp) << '\n';
    }
}
// LQB_access, using the bit_array and the packed_int_array this finds and outputs the search querry based on location in the packed int array
void LQB_access(istream &in,VByte *bytes, uint64_t n){
    cout << bytes -> block() << '\n';
    for (uint64_t b=0;b<n;b++){
        uint64_t tmp=0;
        readx(in,tmp);
        cout << bytes ->LQb_get(tmp) << '\n';
    }
}
//VB_functions executes the requested type and deals with timing and memory checking
void VB_functions(istream &in, int type, int time, int memory){
    VByte *bytes=nullptr;
    uint64_t n=0, sorted=0, k=0;
    readx(in,n);
    readx(in,k);
    readx(in,sorted);
    bytes = new VByte(n,k);

    chrono::time_point<chrono::system_clock>start, end;
    if (time) start = chrono::system_clock::now();

    //encode data into vbyte
    if (type ==0 or type == 1){
        encode(in,bytes,n,sorted);
    }else if (type == 2){
        loco_encode(in,bytes,n);
    }
    

    if (time){
        chrono::duration<double> elapsed_time_s = chrono::system_clock::now() - start;
        cerr <<"encoding time: " << elapsed_time_s.count() << "s\n";
        start = chrono::system_clock::now();
    }

    //decode data depending on specified type (refer to help for type explanations)
    if (type==0){
        decoding(bytes,sorted);
    }else if (type==1){
        scan(in,bytes,n);
    }else if (type ==2){
        LQB_access(in,bytes,n);
    }

    if (time){
        std::chrono::duration<double> elapsed_time_s = std::chrono::system_clock::now() - start;
        cerr <<"querry time: " << elapsed_time_s.count() << "s\n";
    }if(memory) cerr << "memory used: " << double(getKBMemUsage())/1024<<"MB" << '\n';
}

int main(int argc, char const* argv[]) {
    int input_file = 0, type = 2, time = 1, memory = 1;
    int i = 1;
    while (i < argc) {
        string s(argv[i++]);
        if (s.compare("-h") == 0) {
            help();
            exit(0);
        }else if(s.compare("-get") == 0){
            type=0;
        }else if(s.compare("-scan") == 0){
            type=1;
        }else if(s.compare("-lqb") == 0){
            type=2;
        }else if(s.compare("-time") == 0){
            time=1;
        }else if(s.compare("-mem") == 0){
            memory=1;
        }else{
            input_file=i-1;
        }
    }
    if (input_file > 0) {
        ifstream in(argv[input_file]);
        VB_functions(in,type,time,memory);
    } else {
        VB_functions(cin,type, time,memory);
    }
    return 0;
    //printl("hello");
}
