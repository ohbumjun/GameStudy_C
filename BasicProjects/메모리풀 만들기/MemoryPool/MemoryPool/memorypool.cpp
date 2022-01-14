#include <iostream>
#include <stack>

template<typename Type>
class BasicPool {
public :
	BasicPool()
{
	std::cout << "Basic Constructor" << std::endl;
}
	// 복사 생성자를 막을 것이다.
	BasicPool(const  BasicPool&) = delete;
	// 이동 생성자
	BasicPool(BasicPool&& other)
	{
		std::cout << "let's move" << std::endl;;
		free(std::move(other.free));
	}
public :
	// 객체의 Instance를 만들어준다.
	template<typename ...Args>
	Type* create(Args && ...args)
	{
		// 메모리 공간을 하나 할당하고. 
		Type* place = (Type*)allocate();
		try
		{
			// 해당 메모리공간의 주소를 시작점으로 해서
			// 들어온 Args 타입의 인자들을 생성자에 넘겨 만들어낼 것이다.
			new (place) Type(std::forward<Args>(args)...);
		}
		catch (...)
		{
			free.push(place);
			throw;
		}
		return place;
	}
	// Type Instance 하나를 지운다 ( 그 다음, 빈 공간을 pool에 추가한다 )
	void remove(Type* o)
	{
		o->~Type();
		free.push(o);
	}
	// Type 하나가 필요한 메모리 공간을 추가로 할당하기
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
			// Type 크기에 맞는 메모리를 할당하고, 그 주소를 place 라는 변수에 저장해둔다.
			place = ::operator new (sizeof(Type));
		}
		return place;
	}
	// 기존에 할당해둔 메모리를 해제하고, free 공간에 추가
	// 중요한 점은, deallocate 해서 free 에 다시 넣어주는 메모리의 크기는
	// allocate 할때 free 에 넣어주는 메모리 크기와 동일해야 한다. 
	void deallocate(void* c)
	{
		free.push(static_cast<Type*>(c));
	}
	// 모든 free 공간들을 메모리 해제한다
	~BasicPool()
	{
		while(!free.empty())
		{
			::operator delete(free.top());
			free.pop();
		}
	}
private :
	// 메모리를 할당할 수 있는 여유 공간들을 담아두는 stack 
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

	// 객체를 하나 만든다
	SomeObject* o = p.create(10);

	// destroy when its done
	p.remove(o);
}
*/