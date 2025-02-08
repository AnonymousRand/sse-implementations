#pragma once

#include <vector>

#include "util.h"

class PiBasClient {
    protected:
        Db db;
        ustring key;
        int keyLen;

        EncIndex buildIndexInner(Db db);

    public:
        PiBasClient(Db db);

        /**
         * Generate a key.
         *
         * `Setup()` from original paper broken up into `setup()` and `buildIndex()`
         * to be consistent with later papers.
         */
        void setup(int secParam);

        /**
         * Build the encrypted index.
         */
        virtual EncIndex buildIndex();

        /**
         * Issue a query by computing its encrypted token.
         */
        QueryToken trpdr(KwRange kwRange);
};

class PiBasServer {
    protected:
        EncIndex encIndex;

    public:
        /**
         * Process a query and compute all results.
         */
        std::vector<Id> search(QueryToken queryToken);

        void setEncIndex(EncIndex encIndex);
};
