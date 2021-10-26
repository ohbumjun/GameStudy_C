#include <iostream>
#include <map>
#include <string>

using namespace std;

// FlyWeight :
// 속성이 동일한 객체들은 공유해서
// 사용할 수 있게 하자.
// 플라이웨이트 + 팩토리
class Image
{
    string url;
    Image(string s) : url(s)
    {
        cout << "Downloading from " << url << endl;
    }

public:
    void Draw() { cout << "Draw Image" << url << endl; }
    friend class ImageFactory;
};

class ImageFactory
{
    map<string, Image *> image_map;

public:
    Image *CreateImage(string url)
    {
        if (image_map[url] == 0)
            image_map[url] = new Image(url);
        return image_map[url];
    }
};

int main()
{
    // 그림을 만드는 공장을 짓고
    ImageFactory factory;

    // 그림이 필요하면 공장에 요청한다
    Image *img1 = factory.CreateImage("http://www.naver.com//a.png");
    img1->Draw();
    Image *img2 = factory.CreateImage("http://www.naver.com//a.png");
    img2->Draw();
}