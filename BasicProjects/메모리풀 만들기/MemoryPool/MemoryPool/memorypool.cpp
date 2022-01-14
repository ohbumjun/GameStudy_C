#include <iostream>
#include <stack>

template<typename Type>
class BasicPool {
public :
	BasicPool()
{
	std::cout << "Basic Constructor" << std::endl;
}
	// ���� �����ڸ� ���� ���̴�.
	BasicPool(const  BasicPool&) = delete;
	// �̵� ������
	BasicPool(BasicPool&& other)
	{
		std::cout << "let's move" << std::endl;;
		free(std::move(other.free));
	}
public :
	// ��ü�� Instance�� ������ش�.
	template<typename ...Args>
	Type* create(Args && ...args)
	{
		// �޸� ������ �ϳ� �Ҵ��ϰ�. 
		Type* place = (Type*)allocate();
		try
		{
			// �ش� �޸𸮰����� �ּҸ� ���������� �ؼ�
			// ���� Args Ÿ���� ���ڵ��� �����ڿ� �Ѱ� ���� ���̴�.
			new (place) Type(std::forward<Args>(args)...);
		}
		catch (...)
		{
			free.push(place);
			throw;
		}
		return place;
	}
	// Type Instance �ϳ��� ����� ( �� ����, �� ������ pool�� �߰��Ѵ� )
	void remove(Type* o)
	{
		o->~Type();
		free.push(o);
	}
	// Type �ϳ��� �ʿ��� �޸� ������ �߰��� �Ҵ��ϱ�
	void * allocate()
	{
		void* place;
		if (!free.empty())
		{
			place = static_cast<void*>(free.top());
			free.pop();
		}
		else
		{
			// Type ũ�⿡ �´� �޸𸮸� �Ҵ��ϰ�, �� �ּҸ� place ��� ������ �����صд�.
			place = ::operator new (sizeof(Type));
		}
		return place;
	}
	// ������ �Ҵ��ص� �޸𸮸� �����ϰ�, free ������ �߰�
	// �߿��� ����, deallocate �ؼ� free �� �ٽ� �־��ִ� �޸��� ũ���
	// allocate �Ҷ� free �� �־��ִ� �޸� ũ��� �����ؾ� �Ѵ�. 
	void deallocate(void* c)
	{
		free.push(static_cast<Type*>(c));
	}
	// ��� free �������� �޸� �����Ѵ�
	~BasicPool()
	{
		while(!free.empty())
		{
			::operator delete(free.top());
			free.pop();
		}
	}
private :
	// �޸𸮸� �Ҵ��� �� �ִ� ���� �������� ��Ƶδ� stack 
	std::stack<Type*> free; 
};

struct SomeObject {
	int val;
	SomeObject(int val_in) : val(val_in){}
};

/*
int main()
{
	BasicPool<SomeObject> p;

	// ��ü�� �ϳ� �����
	SomeObject* o = p.create(10);

	// destroy when its done
	p.remove(o);
}
*/