// 우리는 MOP 를 통해서라도 접근할 수 있는
// 실제 C++ Object 를 원한다.

// 따라서 Adapter Class 를 둘 것인데
// Object 를 상속받고 Product 를 멤버변수로 갖기

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
		// BaseTargetType == 최종적인 return 값을 의미하게 된다.
		// ex Book::price 의 경우, 그냥 double 이 아니라 RealValue<double> 이다
		// ex) RealValue<double> -> 다형성 적용하여 부모 claa 형태의 포인터로 담기 BaseValue ?
		BaseValue* p = &(members[idx].value(myObject));
		p->set(v);
	}

protected:
	virtual BaseValue* getBaseValue(size_t idx)
	{
		// myObject Class 의 특정 Attribute 에 대한
		// pointer to member 를 통해 값 리턴
		return &members[idx].value(myObject);
	}

	virtual BaseValue const* getBaseValue(size_t idx) const
	{
		return &members[idx].value(myObject);
	}

private:
	// 해당 클래스의 모든 멤버변수에 대한
	// pointer to member 변수들을 vector 로 담기 
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
