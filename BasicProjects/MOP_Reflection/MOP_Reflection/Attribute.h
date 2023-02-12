#pragma once

#include "Type.h"
#include "Flag.h"

class Attribute
{
public:
    Attribute(const string& name_, TypeT typeId)
        : name(name_), type_(Type(typeId))
    {}

    const string& getName() const
    {
        return name;
    }
    Type getType() const
    {
        return type_;
    }

private:
    string name;
    Type type_;
};