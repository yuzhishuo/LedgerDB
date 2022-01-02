#pragma once

#include "IStore.h"

class IDisposable
{
public:
    virtual void dispose() = 0;
    virtual ~IDisposable() = default;
};