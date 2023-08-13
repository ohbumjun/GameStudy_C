/*
Groovy Style Builder

- let's write html right inside C++ code

*/

#include <iostream>

using namespace std;

// use Tag as base class
// in order to allow clients to actually write
// something which look a lot like HTML
class Tag
{
    string name, text;
    vector<Tag> children;
    vector<pair<string, string>> attributes;

    // constructors
protected:
public:
    Tag(const string &name, const string &text) : name(name), text(text) {}

    Tag(const string &name, const vector<Tag> &children)
        : name(name), children(children) {}
}

struct p : Tag
{
    // constructor
    P(const string &name, const string &text) : Tag("p", text) {}
    // makeing p with bunch of children
    P(initializer_list<Tag> children) : Tag{"p", children};
}

struct IMG : Tag
{
    // specifies url of image you want to show
    explicit IMG(const string &url) : Tag{"img", ""}
    {
        // instruction to add attributes
        attributes.emplace_back(make_pair("src", url));
    }
}

/*
���� ��� ������ ��ģ ������ �����ϱ� ?? 
- we've just built a very unique and
very unusual domain specific language  
which allow us to construct HTML


*/

int
main()
{
    cout << P{
                IMG{"http://pokemon.com/pikachu.png"},
            }
         << endl;
    return 0;
}

/*

#include <string>
#include <vector>
#include <iostream>

namespace html {
  struct Tag
  {
    std::string name;
    std::string text;
    std::vector<Tag> children;
    std::vector<std::pair<std::string, std::string>> attributes;

    friend std::ostream& operator<<(std::ostream& os, const Tag& tag)
    {
      os << "<" << tag.name;

      for (const auto& att : tag.attributes)
        os << " " << att.first << "=\"" << att.second << "\"";

      if (tag.children.size() == 0 && tag.text.length() == 0)
      {
        os << "/>" << std::endl;
      } 
      else
      {
        os << ">" << std::endl;

        if (tag.text.length())
          os << tag.text << std::endl;

        for (const auto& child : tag.children)
          os << child;

        os << "</" << tag.name << ">" << std::endl;
      }

      return os;
    }
  protected:

    Tag(const std::string& name, const std::string& text)
      : name{name},
        text{text}
    {
    }


    Tag(const std::string& name, const std::vector<Tag>& children)
      : name{name},
        children{children}
    {
    }
  };

  struct P : Tag
  {
    explicit P(const std::string& text)
      : Tag{"p", text}
    {
    }

    P(std::initializer_list<Tag> children)
      : Tag("p", children)
    {
    }
    
  };

  struct IMG : Tag
  {
    explicit IMG(const std::string& url)
      : Tag{"img", ""}
    {
      attributes.emplace_back(make_pair("src", url));
    }
  };
}

int main1()
{
  using namespace html;

  std::cout <<

    P {
      IMG {"http://pokemon.com/pikachu.png"}
    }

    << std::endl;

  getchar();
  return 0;
}

*/
