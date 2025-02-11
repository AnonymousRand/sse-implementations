#pragma once

#include "sse.h"

class PiBasClient : public ISseClient<ustring, EncInd> {
    public:
        ustring setup(int secParam) override;
        EncInd buildIndex(ustring key, Db db) override;
        QueryToken trpdr(ustring key, KwRange kwRange) override;
};

class PiBasServer : public ISseServer<EncInd> {
    public:
        std::vector<Id> search(EncInd encInd, QueryToken queryToken) override;
};
