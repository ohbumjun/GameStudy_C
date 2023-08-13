#include <iostream>

// Life without builder pattern

using namespace std;
// create obj oriented structures
// which actually represent the different parts
// of the HTML that you are building

// Htmlelement here is just a utility class for
// modeling and HTML element in an object oriented way in C++
struct HtmlElement
{
    string name, text;
    vector<HtmlElement> elements; // inner html elements
    const size_t_indent_size = 2;

    HtmlElement() {}
    HtmlElemnt(const string &name, const string &text) : name(name),
                                                         text(text) {}

    // printing elements
    string str(int indent = 0) const
    {
        ostringsteam oss;
        string i(indent_size * indent, ' ');
        oss < i << "<" << name << "> " << endl;
        if (text.size() > 0)
            oss << string(indent_size * (indent + 1), ' ') << text << endl;
        for (const auto &e : elements)
            oss << e.str(indent + 1);
        oss < i << "</" << name << ">" << endl;
        return oss.str();
    }
};

// let us define root of out element
// add children in very simple fashion
struct HtmlBuilder
{
    HtmlElement root;
    HtmlBuilder(string root_name)
    {
        root.name = root_name;
        // add children to root element
    }
    void add_child(string child_name, string child_text)
    {
        HtmlElement e{child_name, child_text};
        // emplace_back : push_back�� ������ ����
        // ������, emplace_back��, ���� ���� �Լ�
        // push_back�� ���� �Լ�
        // ���� �����Լ��� ���, ������ ��ü�� �����ڸ� ����
        // ���ڵ��� �޾�, std::vector ������ ��ü�� �����Ͽ� �����Ѵ�
        // �ӽ� ��ü�� ����,�ı�,���縦 ���� �ʾƵ� �Ǿ�
        // ���ɻ� �� �����ϴٴ� ���̴�
        root.elements.emplace_back(e);
        /*
		ex) push_back ����
		- vecVertex.push_back(CVertex(1,"�� ��°",3)); 
		--> push_back�� �����ϱ� ����, 
		1) ��ü�� ����� �����߰�
		2) ���������� �� �ӽ� ��ü�� �����
		3) ������ ����, 
		4) �ı��Ǵ� ������ ��ģ��  
		
		---------------------------------------------
		vecVertex.emplace_bak(0,"ù ��°",5);
		- ������ �ʿ��� ���ڸ� ���޹޾� ���ο���
		����, �����ϹǷ�
		
		- ������, �Ҹ��� �ѹ����� ȣ���ϱ� ������
		���ɻ� �� �����ϴ�  
		*/
    }

    string str() const { return root.str(); }
}

int
main()
{
    auto text = "hello";
    string output;
    output += "<p>";
    output += text;
    output += "</p>" cout << output << endl;

    // you have to put string together piece by piece
    string words[] = {"hello", "world"};
    ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
        oss << " <li>" << w << "</li>";
    oss << "</ul>";
    cout << oss.str() << endl;

    // -------------------------------------------------
    // Let's use the Html Builder
    HtmlBuilder builder{"ul"};
    builder.add_child("li", "hello");
    builder.add_child("li", "world");
    cout << builder.str() << endl;

    /*
		Instead of working, for example , directly where the 
		HTML element,
		we're actually avoiding it by 
		creating a dedicated component 
	
		That is, Instead of constructing the object
		in a single line, 
		what you're doing is you're using a
		specialized component
		which has the special functions for helping you
		construct the component
	*/

    return 0;
}
