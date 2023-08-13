#pragma once

#ifndef SIMPLE_OBJECT_H_
#define SIMPLE_OBJECT_H_

#include "DynaObject.h"

class SimpleObject : public DynaObject
{
public:
	explicit SimpleObject(ClassDef const*);
	SimpleObject(SimpleObject const&);

	static Object* newObject(ClassDef const*);
	virtual Object* clone() const;
};
#endif // SIMPLE_OBJECT_H_