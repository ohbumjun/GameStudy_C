#include "ClassDef.h"

#include <iterator>
#include "Object.h"
#include "Attribute.h"
#include "Value.h"

typedef std::vector<Attribute> AttributeContainer;
typedef AttributeContainer::const_iterator AttrIterator;

ClassDef::ClassDef(ClassDef const* base, const std::string& name_, CreateObjFunc objFunc)
	: baseClass(base), name(name_), createObj(objFunc), definitionFix(false),
	defaultObj(0)
{
	baseInit();
}

std::string ClassDef::getName() const
{
	return name;
}

Object* ClassDef::newObject() const
{
	// 1) CppObject �� static ���� CppObject �� ������
	// �Լ��� �����Ѵ�. ��, �Ʒ��� �Լ� �����͸� ����
	// CppObject �� �����ڴٴ� ���̴�.

	// 2) Ȥ�� DynaObject �� ��ӹ��� class �鿡 ���ǵ�
	// newObject static �Լ��� ����ǰ� �� ���̴�.
	definitionFix = true;
	return (*createObj)(this);
}

Object const* ClassDef::defObject() const
{
	return defaultObj;
}

void ClassDef::setDefault(Object const* def)
{
	defaultObj = def;
}

AttrIterator ClassDef::attribBegin() const
{
	return effectiveAttributes.begin();
}

AttrIterator ClassDef::attribEnd() const
{
	return effectiveAttributes.end();
}

Attribute const& ClassDef::getAttribute(size_t idx) const
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return effectiveAttributes[idx];
}

void ClassDef::addAttribute(const Attribute& a)
{
	if (!definitionFix)
	{
		ownAttributes.push_back(a);
		effectiveAttributes.push_back(a);
	}
}

size_t ClassDef::getAttributeCount() const
{
	return effectiveAttributes.size();
}

size_t ClassDef::findAttribute(std::string const& name) const
{
	for (AttributeContainer::const_reverse_iterator i = effectiveAttributes.rbegin();
		i != effectiveAttributes.rend();
		++i)
	{
		if (i->getName() == name)
		{
			// both returns might be somewhat confusing
			// i.base() does not point to the same attribute as i
			// due to reverse iterator characteristics,
			// so the resulting distance must be adjusted by one:
			// return std::distance(effectiveAttributes.begin(), i.base()) - 1;

			// rend() like end() points to one item beyond the last,
			// the the resulting distance must be adjusted as well:
			return std::distance(i, effectiveAttributes.rend()) - 1;
		}
	}
	return getAttributeCount();
}

void ClassDef::baseInit()
{
	if (baseClass)
	{
		baseClass->definitionFix = true;
		std::copy(baseClass->attribBegin(), baseClass->attribEnd(),
			std::back_inserter<AttributeContainer>(effectiveAttributes));
	}
}

void ClassRegistry::registerClass(ClassDef* cl)
{
	classes.push_back(cl);
}

ClassRegistry::Iterator ClassRegistry::begin()
{
	return classes.begin();
}

ClassRegistry::Iterator ClassRegistry::end()
{
	return classes.end();
}

ClassRegistry::Iterator ClassRegistry::find(std::string const& name)
{
	for (Iterator c = classes.begin(); c != classes.end(); ++c)
	{
		if ((*c)->getName() == name)
		{
			return c;
		}
	}
	return classes.end();
}

ClassRegistry::Iterator ClassRegistry::noClass()
{
	return classes.end();
}
