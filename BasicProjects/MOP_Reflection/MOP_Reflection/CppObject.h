// �츮�� MOP �� ���ؼ��� ������ �� �ִ�
// ���� C++ Object �� ���Ѵ�.

// ���� Adapter Class �� �� ���ε�
// Object �� ��ӹް� Product �� ��������� ����

// CppObject.h
#ifndef CPP_OBJECT_H
#define CPP_OBJECT_H

#include "MemPtr.h"
#include "Object.h"

template <typename OrigClass>
class CppObject : public Object
{
	typedef MemPtr<OrigClass, FinalValue> MemberPtr;
public:
	CppObject(ClassDef const* myClass)
		: Object(myClass), myObject(), members(OrigClass::memberBegin())
	{}

	CppObject(ClassDef const* myClass, OrigClass const& obj)
		: Object(myClass), myObject(obj), members(OrigClass::memberBegin())
	{}

	static Object* newObject(ClassDef const* myClass)
	{
		return new CppObject(myClass);
	}
	virtual Object* clone() const
	{
		return new CppObject(*this);
	}

#if defined(__BORLANDC__)
	virtual Value getValue(std::string const& name) const { return Object::getValue(name); }
	virtual void setValue(std::string const& name, Value const& v) { Object::setValue(name, v); }
#else
	using Object::getValue;
	using Object::setValue;
#endif
	virtual Value getValue(size_t idx) const
	{
		return members[idx].value(myObject);
	}

	virtual void setValue(size_t idx, Value const& v)
	{
		// how right return value can become left value ?
		// BaseTargetType == �������� return ���� �ǹ��ϰ� �ȴ�.
		// ex Book::price �� ���, �׳� double �� �ƴ϶� RealValue<double> �̴�
		// ex) RealValue<double> -> ������ �����Ͽ� �θ� claa ������ �����ͷ� ��� BaseValue ?
		BaseValue* p = &(members[idx].value(myObject));
		p->set(v);
	}

protected:
	virtual BaseValue* getBaseValue(size_t idx)
	{
		// myObject Class �� Ư�� Attribute �� ����
		// pointer to member �� ���� �� ����
		return &members[idx].value(myObject);
	}

	virtual BaseValue const* getBaseValue(size_t idx) const
	{
		return &members[idx].value(myObject);
	}

private:
	// �ش� Ŭ������ ��� ��������� ����
	// pointer to member �������� vector �� ��� 
	MemberPtr* members;
	OrigClass myObject;
};

template <typename CppClass>
inline
#if defined(_MSC_VER)
ClassDef makeClass(ClassDef const* base, std::string const& name, CppClass* vcppBug)
#else
ClassDef makeClass(ClassDef const* base, std::string const& name)
#endif
{
	return ClassDef(base, name,
		CppObject<CppClass>::newObject,
		CppClass::ownAttribBegin(), 
		CppClass::ownAttribEnd());
}

template <typename CppClass>
inline
CppObject<CppClass> makeObject(ClassDef const* cl, CppClass const& obj)
{
	return CppObject<CppClass>(cl, obj);
}
#endif // CPP_OBJECT_H
