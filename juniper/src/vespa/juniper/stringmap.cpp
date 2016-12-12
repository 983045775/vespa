// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#include <vespa/fastos/fastos.h>
#include "stringmap.h"

void Fast_StringMap::Insert(const char* key, const char* value)
{
    _backing[key] = value;
}


const char *
Fast_StringMap::Lookup(const char *key, const char *defval)
{
    Map::const_iterator found(_backing.find(key));
    return (found != _backing.end()) ? found->second.c_str() : defval;
}
