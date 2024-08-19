#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <bitset>
#include <vector>

#include "./Hash/SpookyV2.h"

using namespace std;

template <int key_len, long num_of_bits, int num_of_hashes>
class BloomFilter {
public:
    bitset<num_of_bits> bits;
public:
    void add_tag(uint8_t *key) {
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            bits.set(index);
        }
    }

    bool might_contain(uint8_t *key) {
        bool flag = true;
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            flag &= bits.test(index);
        }
        return flag;
    }

    void reset() {
        bits.reset();
    }

    vector<long> static get_index(uint8_t *key) {
        vector<long> indexes;
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            indexes.emplace_back(index);
        }
        return indexes;
    }

    vector<long> search(bool value = true) {
        vector<long> indexes;
        for (int i = 0; i < num_of_bits; ++i) {
            if(bits[i] == value) {
                indexes.emplace_back(i);
            }
        }
        return indexes;
    }

    bool operator==(const BloomFilter<key_len,num_of_bits,num_of_hashes>& other) const {
        return bits == other.bits;
    }
};

template <int key_len, long num_of_bits, int num_of_hashes>
class BloomFilterTool {
private:
    bitset<num_of_bits> *bits;
public:

    void init(bitset<num_of_bits> *bits){
        this->bits = bits;
    }

    void add_tag(uint8_t *key) {
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            bits->set(index);
        }
    }

    bool might_contain(uint8_t *key) {
        bool flag = true;
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            flag &= bits->test(index);
        }
        return flag;
    }

    void reset() {
        bits->reset();
    }

    vector<long> static get_index(uint8_t *key) {
        vector<long> indexes;
        for (int i = 0; i < num_of_hashes; ++i) {
            long index = SpookyHash::Hash64(key, key_len, i) % num_of_bits;
            indexes.emplace_back(index);
        }
        return indexes;
    }

    vector<long> search(bool value = true) {
        vector<long> indexes;
        for (int i = 0; i < num_of_bits; ++i) {
            if((*bits)[i] == value) {
                indexes.emplace_back(i);
            }
        }
        return indexes;
    }

    bool operator==(const BloomFilter<key_len,num_of_bits,num_of_hashes>& other) const {
        return bits == other.bits;
    }
};

#endif //AURA_BLOOMFILTER_H
