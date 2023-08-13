#pragma once

#include "Serializer.h"

class Serializable
{
public:
	virtual ~Serializable() = default;
	virtual void Serialize(Serializer& archive) = 0;
	virtual void Deserialize(Serializer& archive) = 0;
};

