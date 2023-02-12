#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include "Flag.h"

class FinalValue;
class Object;

class Type
{
public:
	explicit Type(TypeT typeId);

	FinalValue* newValue() const;

	TypeT getType() const;

	static void init();

private:
	TypeT typeId;
	static std::vector<FinalValue*> prototypes;
};

#endif // TYPE_H
