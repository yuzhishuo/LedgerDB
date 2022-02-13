#pragma once

#include "store/IStore.h"

class IDisposable
{
public:
    virtual void dispose() = 0;
    virtual ~IDisposable() = default;
};