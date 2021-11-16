// virtual proxy
/*
>> Virtual Proxy
- gives you the appearance of working
with the same object that you're used to working with
even though object might not have even been created


*/

#include <iostream>
#include <string>
using namespace std;
struct Image
{
	virtual void draw() = 0;
	// virtual void load() = 0;
};

struct Bitmap : Image {
	Bitmap(const string& filename)
	{
		cout << "Loading bitmap from" << filename << endl;
		load();
	};
	void draw() override {
		cout << "Drawing bitmap" << endl;
	}
	void load()
	{
		// ~
	}
};

// 위 코드의 문제점은,
// 사실 우리는 draw() 코드를 실제 실행하기 전까지는
// load함수가 필요없다  

// 만약 bitmap으로 돌아가서
// modify할 수 있는 상황이 아니라고 해보자  

// 우리가 할 수 있는 선택지는
// build a virtual proxy which has the
// same interface as the bitmap
// meaning it has constructor 
// whcih takes filename, as well as
// a method called draw  

// but this lazy proxy, that I am going to build
// is actually going to defer the loading
// of the original bitmap until somebody
// wants to draw it
struct LazyBitmap : Image
{
	// we are not going to load
	// we are going to just save the filename
	LazyBitmap(const string& filename) : filename(filename) {
		// remember ,
		// bitmap constructor is the thing that
		// loads the stuff from the filename

	}

	void draw() override {
		if (!bmp)
			bmp = new Bitmap(filename);
		bmp->draw();
	}

	// so what we want is, we want lazyBitmap to
	// act in identical way to an ordinary bitmap
private:
	string filename;
	// bitmap pointer
	Bitmap* bmp = nullptr;
};

int main()
{
	// Bitmap bmp{"pokemon.png"};
	LazyBitmap bmp("pokemon.png");
	// now, the file is going to be uploaded
	// only when somebody actually calls the
	// draw method. 
	bmp.draw()
		;	return 0;
}