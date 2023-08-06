#ifndef Global_h
#define Global_h

#include <string>
#include "../utils/Logger.h"
#include "EvaValue.h"

struct GlobalVar
{
    std::string name;
    EvaValue value;    
};

struct Global
{
    void define(const std::string& name)
    {
        auto index = getGlobalIndex(name);

        // already defined
        if (index != -1)
        {
            return;
        }

        // set to default number 0 => 이후 set method 로 값 세팅
        globals.push_back({name, NUMBER(0)});
    }

    GlobalVar& get(size_t index) {return globals[index];}

    void set(size_t index, const EvaValue& value)
    {
        if (index >= globals.size())
        {
            DIE << "Global " << index << " doesn't exist";
        }

        globals[index].value = value;
    }

    void set(const std::string& name, const EvaValue& value)
    {
        size_t index = getGlobalIndex(name);

        if (index >= globals.size())
        {
            DIE << "Global " << index << " doesn't exist";
        }

        globals[index].value = value;
    }

    int getGlobalIndex(const std::string& name)
    {
        if (globals.size() > 0)
        {
            for (auto i = 0; i < globals.size(); ++i)
            {
                if (globals[i].name == name)
                {
                    return i;
                }
            }
        }

        return -1;
    }

    /*
    * Add Global Const
    */
    void addConst(const std::string& name, double value)
    {
        if (exist(name))
        {
            return;
        }

        globals.push_back({name, NUMBER(value)});
    }

    bool exist(const std::string& name) {return getGlobalIndex(name) != -1;}

    std::vector<GlobalVar> globals;
};

#endif