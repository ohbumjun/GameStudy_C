#pragma once

#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "Attribute.h"

class Object;

class ClassDef
{        //typedefs Container, Iterator for attributes
    typedef std::vector<Attribute> AttributeContainer;
public:
    typedef AttributeContainer::const_iterator AttrIterator;
    typedef Object* (*CreateObjFunc)(ClassDef const*);

    template <typename Iterator>
    ClassDef(ClassDef const* base, const std::string& name_,
        CreateObjFunc objFunc,
        Iterator attribBegin, Iterator attribEnd)
        : baseClass(base), name(name_),
#ifndef _MSC_VER
        // MS VC++ doesn't ship with a vector constructor that takes arbitrary iterators
        ownAttributes(attribBegin, attribEnd),
#endif // _MSC_VER
        createObj(objFunc), definitionFix(false),
        defaultObj(0)
    {
#ifdef _MSC_VER
        std::copy(attribBegin, attribEnd, std::back_inserter<AttributeContainer>(ownAttributes));
#endif // _MSC_VER

        // effectiveAttributes 에는 앞쪽에 상속받은 attribute 먼저
        baseInit();

        // 그 다음에 고유 attribute 오게 하기 .
        effectiveAttributes.insert(effectiveAttributes.end(), ownAttributes.begin(), ownAttributes.end());
    }

    ClassDef(ClassDef const* base, const std::string& name_, CreateObjFunc objFunc);

    std::string getName() const;
    Object* newObject() const;
    Object const* defObject() const;
    void setDefault(Object const*);
    AttrIterator attribBegin() const;
    AttrIterator attribEnd() const;
    Attribute const& getAttribute(size_t idx) const;
    void addAttribute(const Attribute&);
    size_t getAttributeCount() const;
    size_t findAttribute(std::string const& name) const;


private:
    void baseInit();

    ClassDef const* const baseClass;
    const std::string name;
    AttributeContainer ownAttributes, effectiveAttributes;
    const CreateObjFunc createObj;
    mutable bool definitionFix;
    Object const* defaultObj;
};

class ClassRegistry
{
    typedef std::list<ClassDef*> Container;
public:
    typedef Container::iterator Iterator;

    void registerClass(ClassDef* cl);
    Iterator begin();
    Iterator end();
    Iterator find(std::string const& name);
    Iterator noClass();

private:
    Container classes;
};

#endif // CLASS_H

