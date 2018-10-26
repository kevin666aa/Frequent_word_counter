#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <exception>
#include <vector>
#include "FrequentWordCounter.h"


int main(int argc, char *argv[]){
    auto Start = std::chrono::steady_clock::now();
    CFrequentWordCounter Counter;
    std::vector< CFrequentWordCounter::TWordFrequency > CaseInsensitiveWords, CaseSensitiveWords;
    int NumWords = 10;
    
    if(2 > argc){
        std::cerr<<"Syntax Error: proj3 file [numwords]"<<std::endl;
        return EXIT_FAILURE;
    }
    if(2 < argc){
        NumWords = atoi(argv[2]);
        if(0 > NumWords){
            NumWords = 1;
        }
    }
    std::ifstream InFile(argv[1]);
    while(!InFile.eof()){
        std::string NewLine;
        std::getline(InFile, NewLine);
        Counter.AddLine(NewLine);
    }
    Counter.GetFrequentWords(NumWords, CaseInsensitiveWords, CaseSensitiveWords);
    auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-Start);
    std::cout<<"Duration: "<<Duration.count()<<std::endl;
    std::cout<<"Top "<<NumWords<<" Case Insensitive: "<<std::endl;
    for(auto WordFreq : CaseInsensitiveWords){
        std::cout<<"  "<<std::get<0>(WordFreq)<<": "<<std::get<1>(WordFreq)<<std::endl;
    }
    std::cout<<"Top "<<NumWords<<" Case Sensitive: "<<std::endl;
    for(auto WordFreq : CaseSensitiveWords){
        std::cout<<"  "<<std::get<0>(WordFreq)<<": "<<std::get<1>(WordFreq)<<std::endl;
    }    
    return EXIT_SUCCESS;
}
