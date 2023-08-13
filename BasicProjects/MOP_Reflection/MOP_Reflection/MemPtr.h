
// MemPtr.h - wrapper class for pointer to data members
// -  Ư�� class �� ��� memeber ���� pointer-to-member ������
//    ������ �ְ� �ʹ�. �ϳ��� Container �� ��� �ʹ�
//    �̸� ���� MemPtr �̶�� Wrapper Ŭ���� ����
//    + pointer-to-datamember ������ ����ȯ�� �����ϱ� ����
//    ��ü ��ȯ class ����

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
    // Book:* int -> Product::*int �� ��ȯ �õ� (Book �� Product Ŭ���� ���)
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
