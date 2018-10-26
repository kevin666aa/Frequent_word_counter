#ifndef FREQUENTWORDCOUNTER_H
#define FREQUENTWORDCOUNTER_H

#include <memory>
#include <string>
#include <tuple>
#include <vector>

class CFrequentWordCounter{
    private:
        class CImplementation;
        std::unique_ptr< CImplementation > DData;
        
    public:
        using TWordFrequency = std::tuple<std::string, int>;
        CFrequentWordCounter();
        ~CFrequentWordCounter();
        
        void AddLine(const std::string &line);
        void GetFrequentWords(int n, std::vector< TWordFrequency > &insensitive, std::vector< TWordFrequency > &sensitive);
};

#endif
