#include <htmlcxx/html/ParserDom.h>
#include <string>
#include <iostream>

using namespace std;
using namespace htmlcxx;

int main()
{
  //Parse some html code
  string html = "<html><body>hey<p>hhh<span>ret</span></p></body></html>";
  HTML::ParserDom parser;
  tree<HTML::Node> dom = parser.parseTree(html);
  
  //Print whole DOM tree
  cout << dom << endl;
  //cout << "test ..." << endl;
  
  //Dump all links in the tree
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it)
  {
  	if (it->tagName() == "A")
  	{
		//string href = "href";
  		it->parseAttributes();
  		//cout << it->attributes();
  	}
  }
  
  //Dump all text of the document
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it)
  {
  	if ((!it->isTag()) && (!it->isComment()))
  	{
		//cout << it->tagName() << endl;
  		cout << it->text();
  		cout <<endl;
  	}
  }
  
  return 0;
}
