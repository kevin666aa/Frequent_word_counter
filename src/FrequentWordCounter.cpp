#include "FrequentWordCounter.h"

class CFrequentWordCounter::CImplementation{
public:
    class hash_table{
    public:
        int N;// num of words
        int M;   //table size
        std::vector<int> hash_tb;
        std::vector<TWordFrequency> word_tb;
        
        hash_table(int size = 100){
            M = size;
            N = 0;
            hash_tb.resize(size);
            word_tb.push_back(TWordFrequency("default", 0));
        }
        
        std::string getStr_wordtb(const int& ind){
            return std::get<0>(word_tb[ind]);
        }
        
        void resize(int n){
            std::vector<int> temp(n);
            for (auto i : hash_tb) {
                if (i != 0) {
                    int k = hash(getStr_wordtb(i), n), h = 1;
                    while ( temp.at(k) != 0) {
                        h++;
                        k = (k + h * h) % n;
                    }
                    temp.at(k) = i;
                }
            }
            hash_tb = temp;
            M = n;
        }
        //34185 and 21591 origin
        int hash(const std::string& key,const int& m)  {
            int hash = 5381;
            for (auto chr : key) {
                hash *= 33;
                hash +=chr;
                hash %= m;
            }
            return hash;
            /*int intLength = (int)key.length() / 4;
             long sum = 0, mult = 1;
             for (int j = 0; j < intLength; j++) {
             std::string temp = key.substr(j * 4, (j * 4) + 4);
             for (int k = 0; k < (int)temp.size(); k++) {
             sum += temp.at(k) * mult;
             mult *= 253;
             }
             }
             std::string temp = key.substr(intLength * 4);
             mult = 1;
             for (int k = 0; k < (int)temp.size(); k++) {
             sum += temp.at(k) * mult;
             mult *= 253;
             }
             return (int)(abs(sum) % m);*/
        }
        
        void insert(const std::string& word){
            if (N >= M / 8) {
                resize(M * 8);
            }
            int h = 1, i = hash(word, M);
            for (; hash_tb[i] != 0; i = (i + h * h) % M) {
                if (std::get<0>(word_tb[hash_tb[i]]) == word) {
                    std::get<1>(word_tb[hash_tb[i]])++;
                    return;
                }
                h++;
            }
            N++;
            word_tb.push_back(TWordFrequency(word, 1));
            hash_tb[i] = N;
        }
        //-----------------------
        int getMax(){
            int now= 0;
            for ( int i =0 ;  i< (int)word_tb.size();i++){
                if (now < std::get<1>(word_tb[i])){
                    now = std::get<1>(word_tb[i]);}
            }
            return now;
        }
        
        auto radix_sort(std::vector<TWordFrequency>& unorder, int& max){
            std::vector< std::vector<TWordFrequency> > radix_arr(10);
            for (auto i : unorder) {
                radix_arr.at(9 - (std::get<1>(i)%10)).push_back(i);
            }
            int exp = 100;
            for (; max/exp > 0; exp *= 10) {
                auto copy(radix_arr);
                radix_arr.clear(); radix_arr.resize(10);
                for (auto vec : copy) {
                    for (auto val: vec) {
                        radix_arr.at(9 - (std::get<1>(val) % exp) / (exp/10) ).push_back(val);
                    }
                }
            }
            exp /= 10;
            auto copy(radix_arr);
            radix_arr.clear();radix_arr.resize(10);
            for (auto vec : copy) {
                for (auto val: vec) {
                    radix_arr.at(9 - std::get<1>(val)/exp ).push_back(val);
                }
            }
            return radix_arr;
        }
        
        void output(std::vector< TWordFrequency > &insensitive_final, std::vector< TWordFrequency > &sensitive_final, const int& n){
            sorting(sensitive_final, word_tb, n);
            
            if (std::get<1>(sensitive_final.back()) == 0) {
                sensitive_final.pop_back();
            }
            
            std::vector<int> second_hash(M);
            std::vector<TWordFrequency> wrd_tb;
            int h = 0, i = 0;
            bool old = false;
            for (auto& tup : word_tb) {
                for (auto& i : std::get<0>(tup)) {
                    i = toupper(i);
                }
                h = 1;  i = hash(std::get<0>(tup), M);
                for (; second_hash[i] != 0; i = (i + h * h) % M) {
                    if ( std::get<0>(wrd_tb[second_hash[i]]) == std::get<0>(tup) ) {
                        std::get<1>(wrd_tb[second_hash[i]]) += std::get<1>(tup);
                        old = true;
                        break;
                    }
                    h++;
                }
                if (old) {
                    old = false;
                } else {
                    second_hash[i] = (int)wrd_tb.size();
                    wrd_tb.push_back(TWordFrequency( std::get<0>(tup), std::get<1>(tup)));
                }
            }
            
            sorting(insensitive_final, wrd_tb, n);
        }
        
        void sorting(std::vector< TWordFrequency >& input, std::vector<TWordFrequency>&  table,  const int& n) {
            int max = getMax();
            std::vector< std::vector<TWordFrequency> > radix_arr = radix_sort(table, max);
            
            int i = 0;
            int las_count = 0;
            bool flag = false;
            std::vector<std::string> buffer;
            for (auto vec : radix_arr) {
                for (auto val: vec) {
                    if (i < n) { //i = 9      buffer: 1 in   lstcount 13006
                        if (las_count != std::get<1>(val)) {
                            if ((int)buffer.size() == 1) {
                                input.push_back(std::make_tuple(buffer.at(0), las_count));
                            } else if((int)buffer.size() > 1){
                                sortname(buffer, 0, (int)buffer.size() - 1);
                                for (const auto& k : buffer) {
                                    input.push_back(std::make_tuple(k, las_count));
                                }
                            }
                            buffer.clear();
                            buffer.push_back(std::get<0>(val));
                            i++;
                            las_count = std::get<1>(val);
                        } else{
                            buffer.push_back(std::get<0>(val));
                            i++;
                        }
                    } else{
                        flag = true;
                        break;
                    }
                }
                if (flag) break;
            }
            sortname(buffer, 0, (int)buffer.size() - 1);
            for (const auto& k : buffer) {
                input.push_back(std::make_tuple(k, las_count));
            }
            
            if (std::get<1>(input.back()) == 0) {
                input.pop_back();
            }
        }
        
        void sortname(std::vector<std::string> &v, int left, int right) {
            if (v.size() <= 16) {
                for (int c = 1; c < (int)v.size(); c++) {
                    std::string transfer = v[c];
                    for (int p = c - 1; p >= 0 && transfer < v[p]; p--) {
                        v[p + 1] = v[p];
                        v[p] = transfer;
                    }
                }
            } else {
                int i = left, j = right;
                std::string tmp;
                int pivot = (left + right) / 2;
                while (i <= j) {
                    while (v[i] < v[pivot]){
                        i++;
                    }
                    while (v[j] > v[pivot]){
                        j--;
                    }
                    if (i <= j) {
                        tmp = v[i];
                        v[i] = v[j];
                        v[j] = tmp;
                        i++;
                        j--;
                    }
                }
                if (left < j){
                    sortname(v, left, j);}
                if (i < right){
                    sortname(v, i, right);
                }
            }
        }
        
    };
    hash_table sensitive_tb;
    
};

CFrequentWordCounter::CFrequentWordCounter() : DData(std::make_unique<CImplementation>()){
}

CFrequentWordCounter::~CFrequentWordCounter(){
}

void CFrequentWordCounter::AddLine(const std::string &line){
    int begin = 0, end = 0;
    for (; end < (int)line.size() ; end++) {
        for (; end < (int)line.size(); end++) {
            if(!isalnum(line[end])){
                if (begin == end) {  break;  }
                
                if (isalnum(line[end + 1])) {
                    if (line[end] == '\''){  end++;  continue;  }
                } else{
                    DData->sensitive_tb.insert(line.substr(begin, end - begin));
                    end ++;
                    break;
                }
                DData->sensitive_tb.insert(line.substr(begin, end - begin));
                break;
            }
        }
        begin = end + 1;
    }
}

void CFrequentWordCounter::GetFrequentWords(int n, std::vector< TWordFrequency > &insensitive, std::vector< TWordFrequency > &sensitive){
    DData->sensitive_tb.output(insensitive, sensitive, n);
}





