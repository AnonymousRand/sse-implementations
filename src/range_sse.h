#pragma once

#include "util/util.h"

////////////////////////////////////////////////////////////////////////////////
// Client
////////////////////////////////////////////////////////////////////////////////

template <typename Underlying>
class IRangeSseClient {
    protected:
        Underlying underlying;

    public:
        IRangeSseClient(Underlying underlying);
};

////////////////////////////////////////////////////////////////////////////////
// Server
////////////////////////////////////////////////////////////////////////////////

template <typename Underlying>
class IRangeSseServer {
    protected:
        Underlying underlying;

    public:
        IRangeSseServer(Underlying underlying);
};
