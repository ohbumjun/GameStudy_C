
// MemPtr.h - wrapper class for pointer to data members
// -  특정 class 의 모든 memeber 로의 pointer-to-member 정보를
//    가지고 있고 싶다. 하나의 Container 에 담고 싶다
//    이를 위해 MemPtr 이라는 Wrapper 클래스 제작
//    + pointer-to-datamember 사이의 형변환을 적용하기 위해
//    자체 변환 class 제작

#ifndef MEMPTR_H
#define MEMPTR_H
// naming:
// BaseType:		the class to which a pointer-to-member is applied
// TargetType:		the result type to which a pointer-to-member points
// BaseTargetType:	a base class of TargetType

template <typename BaseType, typename BaseTargetType>
class MemPtrBase
{
public:
    virtual BaseTargetType& value(BaseType& obj) const = 0;
    virtual BaseTargetType const& value(BaseType const& obj) const = 0;
    virtual ~MemPtrBase() {};

protected:
    MemPtrBase() {}

private:
    MemPtrBase(MemPtrBase const&);
    MemPtrBase& operator=(MemPtrBase const&);
};

template <typename BaseType, typename BaseTargetType, typename TargetType>
class TypedMemPtr : public MemPtrBase<BaseType, BaseTargetType>
{
public:
    TypedMemPtr(TargetType BaseType::* ptr)
        : p(ptr)
    {}

    BaseTargetType& value(BaseType& obj) const
    {
        return obj.*p;
    }

    BaseTargetType const& value(BaseType const& obj) const
    {
        return obj.*p;
    }

private:
    TargetType BaseType::* p;
};

template <typename BaseType, typename BaseTargetType>
class MemPtr // this is a handle only
{
public:
    // Book:* int -> Product::*int 로 변환 시도 (Book 은 Product 클래스 상속)
    template <typename BaseType2, typename TargetType>
    explicit MemPtr(TargetType BaseType2::* ptr)
        : p(new TypedMemPtr<BaseType, BaseTargetType, TargetType>
            (static_cast<TargetType BaseType::*>(ptr)))
    {}

    ~MemPtr()
    {
        delete p;
    }

    BaseTargetType& value(BaseType& obj) const
    {
        return p->value(obj);
    }

    BaseTargetType const& value(BaseType const& obj) const
    {
        return p->value(obj);
    }

private:
    MemPtrBase<BaseType, BaseTargetType>* p;
};
#endif // MEMPTR_H
