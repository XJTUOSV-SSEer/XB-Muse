#ifndef GGMTREE_H
#define GGMTREE_H

#include <bitset>
#include <cmath>
#include <cstring>
#include <vector>

#include "GGMNode.h"
#include "EnclaveUtils.h"

using namespace std;

class GGMTree {
private:
    static const int level = GGM_LEVEL;

public:
    explicit GGMTree(long num_node);
    void static derive_key_from_tree(uint8_t *current_key, long offset, int start_level, int target_level);
    static vector<GGMNode> min_coverage(vector<GGMNode> node_list);
    static int get_level();
};


#endif
