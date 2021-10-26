
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


// 어떠한 상품을 팔고 있고
// 각각의 상품에 대한 정보를 저장하고자 한다
// 너의 매니저는 상품을 특정 기준을 바탕으로 필터링 하고자 한다  

/* ------------------------------------------------------------
Open/Closed Principle
그래서 의미가 뭔데 ?? 
- software entities( classes, modules, functions etc )
should be open for extension, but closed for
modification
- 즉, Prouduct Filter 안에서 , 
by_size_and_color 이렇게 매번 요구사항에 따라
추가하지는 않되,
다양한 종류의 필터가 추가될 수 있게 끔 하는 원리   
------------------------------------------------------------- */

using namespace std;

enum class Color {red,green,blue};
enum class Size {small, medium, large};

struct Product
{
	string name;
	Color color;
	Size size;
};

struct ProductFilter
{
	vector<Product*> by_color(vector<Product*> items, Color color)
	{
		vector<Product*> result;
		for(auto &i : items)
			if(i->color == color)
				result.push_back(i);
		return result;
	}
	vector<Product*> by_size(vector<Product*> items, Size size)
	{
		vector<Product*> result;
		for(auto &i : items)
			if(i->size == size)
				result.push_back(i);
		return result;
	}
	// 이번에는 color와 size 둘다 필터 적용해줘!! 
	// 문제는 이런식의 접근은, 무한정으로 함수들을 늘려가버릴 수 있다  
	vector<Product*> by_size_and_color(vector<Product*> items, 
						Size size, Color color)
	{
		vector<Product*> result;
		for(auto &i : items)
			if(i->size == size && i->color == color)
				result.push_back(i);
		return result;
	}
}

// interface 작성 --> filter 작성하기  
template<typename T> struct Specification
{
	virtual bool is_satisfied(T* item) = 0;
	// && operator를 작성해도 된다
	AndSpecification<T> operator&&(Specification<T> && other)
	{
		return AndSpecification<T>(*this,other);
	 } 
 } 
template <typename T> struct Filter
{
	virtual vector<T*> filter(vector<T*> items, 
							Specification<T> & spec) = 0;
}
struct BetterFilter : Filter<Product>
{
	vector<Product* > filter(vector<Product*> items, 
						Specification<Product> &spec)
	override{
		vector<Product*> result;
		for(auto & item : items)
			if(spec.is_satisfied(item))
				result.push_back(item);
		return result;
	}
}

struct ColorSpecification : Specification<Product>
{
	Color color;
	ColorSpecification(Color color) : color(color){}
	bool is_satisfied(Product* item) override {
		return item->color == color;
	}
}

struct SizeSpecification : Specification<Product>
{
	Size size;
	// explicit : 자신이 원하지 않는 형변환이 일어나지 않도록
	// 제한하는 키워드  
	explicit SizeSpecification(Size size) : size(size){}
	bool is_satisfied(Product *item) override {
		return item->size == size;
	}
}

// filter combinator
template <typename T > struct AndSpecification : Specification<T>
{
	Specification<T> &first;
	Specification<T> &second;
	
	AndSpecification(Specification<T> &first,Specification<T> &second )
	: first(first), second(second) {};
	
	bool is_satisfied(T *item) override {
		return first.is_satisfied(item) && second.is_satisfied(item);
	};
}
 

int main()
{
	Product apple("Apple",Color::green, Size::small);
	Product tree("Tree", Color::green, Size::large);
	Product house("House",Color::blue, Size::large);
	
	vector<Product*> items{&apple, &tree, &house};
	/*
	ProductFilter pf;
	auto green_things = pf.by_color(items, Color::green);
	for(auto& item : green_things)
		cout << items->name << " is green\n";
	*/
	BetterFilter bf;
	ColorSpecification green(Color::green);
	for(auto& item : bf.filter(items,green))
		cout << item->name << " is green\n";
	SizeSpecification.large(Size::large);
	AndSpecification<Product> green_and_large(green,large);
	
	auto spec = Colorspecification(Color::green) && SizeSpecification(Size::large); 
	
	for(auto& item : bf.filter(items,green_and_large))
		cout << item->name << " is green and large\n";
	
	for(auto& item : bf.filter(items,spec))
		cout << item->name << " is green and large\n";
	
	return 0;
}
