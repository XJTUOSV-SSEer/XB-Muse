#include "config.h"

#ifndef GGMNODE_H
#define GGMNODE_H

class GGMNode {
public:
    long index;
    int level;
    uint8_t key[AES_BLOCK_SIZE]{};

    GGMNode(){
        
    }

    GGMNode(long index, int level) {
        this->index = index;
        this->level = level;
    }

    GGMNode(long index, int level, uint8_t *key) {
        this->index = index;
        this->level = level;
        memcpy(this->key, key, AES_BLOCK_SIZE);
    }

    // 赋值操作符重载
    GGMNode& operator=(const GGMNode& other) {
        // 检查是否是自我赋值
        if (this == &other) {
            return *this;
        }

        // 赋值 index 和 level
        this->index = other.index;
        this->level = other.level;

        // 赋值 key 数组
        memcpy(this->key, other.key, AES_BLOCK_SIZE);

        return *this;
    }
};

#endif
