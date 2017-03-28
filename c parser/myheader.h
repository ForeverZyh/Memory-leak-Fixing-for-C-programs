#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
using namespace std;
struct leaf
{
	int id,tag;
	/*
		tag:
		0 for nothing
		1 for TYPEDEF
		2 for pointer
	*/
	leaf(int Id,int Tag)
	{
		id=Id;tag=Tag;
	}
};
typedef struct stype
{
	string str;
	int tag;
	vector<leaf> inside;
	stype()
	{
		str="";tag=0;inside.clear();
	}
	void add(const leaf& ele)
	{
		inside.push_back(ele);
	}
	void print_filter_tag(int Tag);
} myYYSTYPE;
struct node
{
	int id;
	vector<node*> use,def;
	node(int Id)
	{
		id=Id;
		use.clear();
		def.clear();
	}
};
int hash_string_to_int(const string&);