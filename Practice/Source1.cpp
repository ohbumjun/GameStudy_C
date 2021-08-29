#include<iostream>
#include<map>
#include<string>

using namespace std;

// FlyWeight :
// �Ӽ��� ������ ��ü���� �����ؼ�
// ����� �� �ְ� ����.

// �ö��̿���Ʈ + ���丮

class Image
{
	string url;
	Image(string s) : url(s)
	{
		cout << "Downloading from " << url << endl;
	}
public :
	void Draw() { cout << "Draw Image" << url << endl; }
	friend class ImageFactory;
};

class ImageFactory
{
	map<string, Image*> image_map;
public :
	Image* CreateImage(string url)
	{
		if (image_map[url] == 0)
			image_map[url] = new Image(url);
		return image_map[url];
	}
};

int main()
{
	// �׸��� ����� ������ ����
	ImageFactory factory;

	// �׸��� �ʿ��ϸ� ���忡 ��û�Ѵ�
	Image* img1 = factory.CreateImage("http://www.naver.com//a.png");
	img1->Draw();
	Image* img2 = factory.CreateImage("http://www.naver.com//a.png");
	img2->Draw();
}