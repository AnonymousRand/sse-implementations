#include <algorithm>
#include <set>
#include <random>

#include "log_src.h"

////////////////////////////////////////////////////////////////////////////////
// LogSrcClient
////////////////////////////////////////////////////////////////////////////////

LogSrcClient::LogSrcClient(Db db) : PiBasClient(db) {};

EncIndex LogSrcClient::buildIndex() {
    EncIndex encIndex;

    // build TDAG over keywords
    // need to find largest keyword: we can't pass in all the keywords raw, as leaves need to be contiguous
    Kw maxKw = -1;
    for (auto pair : this->db) {
        KwRange kwRange = pair.second;
        if (kwRange.second > maxKw) {
            maxKw = kwRange.second;
        }
    }
    this->tdag = TdagNode::buildTdag(maxKw);
    std::cout << this->tdag << std::endl;

    // replicate every document to all nodes/keywords ranges in TDAG that cover it
    std::unordered_map<Id, std::vector<KwRange>> dbWithReplicas;
    for (auto pair : this->db) {
        Id id = pair.first;
        KwRange kwRange = pair.second;
        std::list<TdagNode*> ancestors = this->tdag->getLeafAncestors(kwRange);
        for (TdagNode* ancestor : ancestors) {
            KwRange kwRange = ancestor->getKwRange();
            if (dbWithReplicas.count(id) == 0) {
                dbWithReplicas[id] = std::vector<KwRange> {kwRange};
            } else {
                dbWithReplicas[id].push_back(kwRange);
            }
        }
    }

    // randomly permute documents associated with same keyword range/node
    // then convert to `std::unordered_multimap` format compatible with `PiBasClient.buildIndex()`
    Db permutedDbWithReplicas;
    std::random_device rd;
    std::mt19937 rng(rd());
    for (auto pair : dbWithReplicas) {
        Id id = pair.first;
        std::vector<KwRange> kwRanges = pair.second;
        std::shuffle(kwRanges.begin(), kwRanges.end(), rng);
        for (KwRange kwRange : kwRanges) {
            permutedDbWithReplicas.insert(std::make_pair(id, kwRange));
        }
    }

    return PiBasClient::buildIndex(permutedDbWithReplicas);
}

QueryToken LogSrcClient::trpdr(KwRange kwRange) {
}
