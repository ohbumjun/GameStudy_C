/*
>> Fluent Interface
- change the return type of function from
void into either a pointer or a reference to 
the type you currently
 
*/

#include<iostream>


using namespace std;

struct HtmlElement
{
	string name, text;
	vector<HtmlElement> elements; // inner html elements
	const size_t_indent_size = 2;
	
	HtmlElement(){}
	HtmlElemnt(const string& name, const string& text) : name(name),
	text(text){}
	
	// printing elements
	string str(int indent = 0) const {
		ostringsteam oss;
		string i(indent_size*indent, ' ');
		oss <i << "<" << name << "> " << endl;
		if(text.size() > 0)
			oss << string(indent_size*(indent+1),' ') << text << endl;
		for(const auto& e : elements) 
			oss << e.str(indent + 1);
		oss <i << "</" << name << ">" << endl;
		return oss.str();
	}
	// give a hint that you have to use a 
	// builder pattern 
	static HtmlBuilder build(string root_name){
		return {root_name};
	}
};

class HtmlBuilder
{
	HtmlElement root;
public :
	HtmlBuilder(string root_name){
		root.name = root_name;
		// add children to root element
	}
	// change "void" return type to "pointer or reference"
	// 1) reference
	HtmlBuilder& add_child(string child_name, string child_text)
	{
		HtmlElement e{child_name, child_text};  
		root.elements.emplace_back(e);
		return *this;
	}	
	// 2) pointer
	HtmlBuilder* add_child_2(string child_name, string child_text)
	{
		HtmlElement e{child_name, child_text};  
		root.elements.emplace_back(e);
		return this;
	}
	
	HtmlElement build(){return root;}
	
	string str() const {return root.str();} 
		
	// HtmlBuilder->add_child�� ���� HtmlBuilder�� �ƴ϶� HtmlElement ����
	operator HtmlElement() const { return root;} 
}

// --> A way to force pp to use the bulider instead of
// constructing the object via ordinary constructor
// 
class HtmlElement
{
	friend class HtmlBuilder;
	private :
		string name, text;
		vector<HtmlElement> elements; // inner html elements
		const size_t_indent_size = 2;
		
		HtmlElement(){}
		HtmlElemnt(const string& name, const string& text) : name(name),
		text(text){}
	public :
		// printing elements
		string str(int indent = 0) const {
			ostringsteam oss;
			string i(indent_size*indent, ' ');
			oss <i << "<" << name << "> " << endl;
			if(text.size() > 0)
				oss << string(indent_size*(indent+1),' ') << text << endl;
			for(const auto& e : elements) 
				oss << e.str(indent + 1);
			oss <i << "</" << name << ">" << endl;
			return oss.str();
	}
	// give a hint that you have to use a 
	// builder pattern 
	static HtmlBuilder create(string root_name){
		return {root_name};
	}
	
};

int main()
{
	auto text = "hello";
	string output;
	output += "<p>";
	output += text;
	output += "</p>"
	cout << output << endl;
	
	// you have to put string together piece by piece
	string words[] = {"hello","world"};
	ostringstream oss;
	oss << "<ul>";
	for(auto w : words)
		oss << " <li>" << w << "</li>";
	oss << "</ul>";
	cout << oss.str() << endl ;
	
	// -------------------------------------------------
	// Let's use the Html Builder
	HtmlBuilder builder{"ul"};
	// below chaining is possible, because
	// you are returning "HtmlBuilder"
	builder.add_child("li","hello").add_child("li","world");
	cout << builder.str() << endl;
	
	auto builder     = HtmlElement()::build("ul").add_child("","");
	HtmlElement elem = HtmlElement()::build("ul").add_child("","");
	
	// if you use 'class' version
	Htmlelement::create("").add_child().build();
	
	return 0;
}
