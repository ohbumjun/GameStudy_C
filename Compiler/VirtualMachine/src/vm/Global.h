#ifndef Global_h
#define Global_h

#include <string>
#include "../utils/Logger.h"
#include "EvaValue.h"
#include <functional>

struct GlobalVar
{
    std::string name;
    EvaValue value;    
};

struct Global
{
    Global()
    {
        std::cout << "Global Constructor" << std::endl;
    }
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

    // Global Variable 들도, 프로그램 전체로 보면, 거대한 block 안에 선언된 대상들이다.
    // - 하나의 Block 안에서 variable 들은 stack 형태로 쌓인다.
    // - vector 를 stack 자료 구조 형태로 사용하고자 하는 것이다. 따라서 , 맨 뒤에 있는 요소부터 search 를 하는 것이다. 
    int getGlobalIndex(const std::string& name)
    {
        if (globals.size() > 0)
        {
            for (size_t i = 0; i < globals.size(); ++i)
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

    /*
    * Add Native functions.
    */
    void addNativeFunction(const std::string& name, std::function<void()> fn, size_t arity)
    {
        if (exist(name))
        {
            return;
        }

        globals.push_back({name, ALLOC_NATIVE(fn, name, arity)});
    }

    bool exist(const std::string& name) {return getGlobalIndex(name) != -1;}

    std::vector<GlobalVar> globals;
};

#endif