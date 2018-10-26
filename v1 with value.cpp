// correct with value class, orgin sort

#include "FrequentWordCounter.h"
#include <iostream>
class CFrequentWordCounter::CImplementation{
public:
    class value{
    public:
        std::string word;
        int count;
        value(std::string word, int count) : word(word), count(count){ }
    };
    class hash_table{
    public:
        int N;// num of words
        int M;   //table size
        std::vector<int> hash_tb;
        std::vector<value> word_tb;
        
        hash_table(int size = 50){
            M = size;
            N = 0;
            hash_tb.resize(size);
            word_tb.push_back(value("default", 0));
        }
        
        std::string getStr_wordtb(const int& ind){
            return word_tb.at(ind).word;
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
        
        int hash(const std::string& key,const int& m)  {
            int intLength = (int)key.length() / 4;
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
            return (int)(abs(sum) % m);
        }
        
        void insert(const std::string& word){
            if (N >= M / 2) {
                resize(M * 2);
            }
            int h = 1, i = hash(word, M);
            for (; hash_tb.at(i) != 0; i = (i + h * h) % M) {
                if (getStr_wordtb(hash_tb.at(i)) == word) {
                    word_tb.at(hash_tb.at(i)).count++;
                    return;
                }
                h++;
            }
            N++;
            word_tb.push_back(value(word, 1));
            hash_tb.at(i) = N;
        }
        //-----------------------
        int getMax(){
            int now= 0;
            for ( int i =0 ;  i< word_tb.size();i++){
                if (now < word_tb.at(i).count){
                    now = word_tb.at(i).count;}
            }
            return now;
        }
        
        auto radix_sort(std::vector<value>& unorder, int& max){
            std::vector< std::vector<value> > radix_arr(10);
            for (auto i : unorder) {
                radix_arr.at(9 - (i.count%10)).push_back(i);
            }
            int exp = 100;
            for (; max/exp > 0; exp *= 10) {
                auto copy(radix_arr);
                radix_arr.clear(); radix_arr.resize(10);
                for (auto vec : copy) {
                    for (auto val: vec) {
                        radix_arr.at(9 - (val.count%exp)/(exp/10) ).push_back(val);
                    }
                }
            }
            exp /= 10;
            auto copy(radix_arr);
            radix_arr.clear();radix_arr.resize(10);
            for (auto vec : copy) {
                for (auto val: vec) {
                    radix_arr.at(9 - val.count/exp ).push_back(val);
                }
            }
            return radix_arr;
        }
        
        void output(std::vector< TWordFrequency > &input, const int& n) {
            word_tb.erase(word_tb.cbegin());
            int max = getMax();
            std::vector< std::vector<value> > radix_arr = radix_sort(word_tb, max);
            
            int i = 0;
            int las_count = 0;
            bool flag = false;
            std::vector<std::string> buffer;
            for (auto vec : radix_arr) {
                for (auto val: vec) {
                    if (i < n) { //i = 9      buffer: 1 in   lstcount 13006
                        if (las_count != val.count) {
                            if ((int)buffer.size() == 1) {
                                input.push_back(std::make_tuple(buffer.at(0), las_count));
                            } else if((int)buffer.size() > 1){
                                sortname(buffer, 0, (int)buffer.size() - 1);
                                for (const auto& k : buffer) {
                                    input.push_back(std::make_tuple(k, las_count));
                                }
                            }
                            buffer.clear();
                            buffer.push_back(val.word);
                            i++;
                            las_count = val.count;
                        } else{
                            buffer.push_back(val.word);
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
        }
        void sortname(std::vector<std::string> &v, int left, int right) {
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
        
    };
    
    hash_table insensitive_tb;
    hash_table sensitive_tb;
    void insert_both(std::string& word){
        std::string sensitive = word;
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        insensitive_tb.insert(word);
        sensitive_tb.insert(sensitive);
    }
};

CFrequentWordCounter::CFrequentWordCounter() : DData(std::make_unique<CImplementation>()){
    // Your code here
}

CFrequentWordCounter::~CFrequentWordCounter(){
    // Your code here
}

void CFrequentWordCounter::AddLine(const std::string &line){
    char words[line.size() + 1];
    strcpy(words, line.c_str());
    char* rest = words;
    char * token;
    std::string b;
    while ((token = strtok_r(rest, " \r\n\\\"!@#$%^&*()_+=-[]{};,.<>/?:|", &rest))) {
        //        std::cout << "\"" << token << "\"" << std::endl;
        b = std::string(token);
        if (b == "\'"){   continue;  }
        if (b.substr( b.length()-1) == "\'"){ b = b.substr( 0,b.length()-1);  }
        if (b.substr(0,1) == "\'"){  b = b.substr (1,b.length() -1 );   }
        DData->insert_both(b);
    }
    
}

void CFrequentWordCounter::GetFrequentWords(int n, std::vector< TWordFrequency > &insensitive, std::vector< TWordFrequency > &sensitive){
    DData->sensitive_tb.output(sensitive, n);
    DData->insensitive_tb.output(insensitive, n);
    
}





