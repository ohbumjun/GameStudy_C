// open closed principle

#include<string>
#include<vector>
#include<iostream>

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
	// size
	vector<Product*> by_size(vector<Product*>items, Size size)
	{
		vector<Product*> result;
		for (auto& i : items)
			if (i->size == size)
				result.push_back(i);
		return result;
	}
	// color
	vector<Product*> by_color(vector<Product*>items, Color color)
	{
		vector<Product*> result;
		for (auto& i : items)
			if (i->color == color)
				result.push_back(i);
		return result;
	}
	// color, size
	vector<Product*> by_size_and_color(vector<Product*>items, Color color, Size size)
	{
		vector<Product*> results;
		for (auto& i : items)
			if(i->size == size && i->color == color)
				results.push_back(i);
		return results;
	}
};
template<typename T>
struct AndSpecification;
template<typename T>
struct Specification
{
	virtual bool is_satisfied(T* item) const = 0;
	/*
	* -- �Ʒ��� ���, ocp�� �ı��ϰ� �ȴ� 
	AndSpecification<T> operator &&(Specification<T>&& other)
	{
		return AndSpecification<T>(*this, other);
	}
	*/
};

struct ColorSpecification : Specification<Product>
{
	Color color;
	ColorSpecification(Color color) : color(color){}
	bool is_satisfied(Product* item) const override
	{
		return item->color == color;
	}
};

struct SizeSpecification : Specification<Product>
{
	Size size;
	SizeSpecification(Size size) : size(size){}
	bool is_satisfied(Product* item) const override
	{
		return item->size == size;
	}
};

template<typename T>
AndSpecification<T> operator &&
(const Specification<T>& first,
	const Specification<T>& second)
{
	return { first,second };
}

template<typename T >
struct AndSpecification : Specification<T>
{
	const Specification<T>& first;
	const Specification<T>& second;
	AndSpecification(
		const Specification<T>& first, 
		const Specification<T>& second
		) : first(first), second(second) {}
	bool is_satisfied(T* item) const override
	{
		return first.is_satisfied(item) && second.is_satisfied(item);
	}
};

template<typename T>
struct Filter
{
	virtual vector<T*> filter(vector<T*>items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<Product>
{
	vector<Product*> filter(vector<Product*> items, Specification<Product>& spec) 
		override
	{
		vector<Product*> result;
		for (auto& i : items)
			if (spec.is_satisfied(i))
				result.push_back(i);
		return result;
	}
};

int main()
{
	Product apple{ "Apple", Color::green, Size::small };
	Product tree{"Tree", Color::green, Size::large};
	Product house{ "House", Color::blue, Size::large };

	vector<Product*> items{ &apple,&tree,&house };

	BetterFilter bf;
	ColorSpecification green(Color::green);
	auto green_thigs = bf.filter(items, green);

	for (auto& g : green_thigs)
		cout << g-> name << endl;

	SizeSpecification large(Size::large);
	auto large_things = bf.filter(items, large);

	for (auto& l : large_things)
		cout << l->name << endl;

	AndSpecification<Product> green_large(green,large);
	auto green_large_things = bf.filter(items, green_large);

	auto spec = green && large;
}