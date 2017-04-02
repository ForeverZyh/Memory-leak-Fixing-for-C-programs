#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;
ifstream in;
ofstream out;
char s[1010];
bool check(char *s)
{
	return s[1]!=';';
}
bool useful(char *s,int l,int r)
{
	if (s[l]=='\'') return 0;
	if (s[l]>='A'&&s[l]<='Z') return 0;
	return 1;
}
int find_next(char *s,int st,int l)
{
	int en=st;
	for(;en<l;en++)
		if (s[en]==' ') break;
	return en;
}
void process(char *fa)
{
	out<<fa<<endl;
	char s[1010];
	int l;
	while (in.getline(s,1000))
	{
		bool flag=0;
		l=strlen(s);
		out<<s<<endl;
		if (check(s))
		{
			int st=3,en;
			int cnt=1;
			while (1)
			{
				en=find_next(s,st,l);
				if (useful(s,st,en))
				{
					if (!flag)
					{
						out<<"\t{"<<endl;
						out<<"\t\t$$=new node();\n";
						out<<"\t\tforest.push_back($$);\n";
						out<<"\t\t$$->str=\""<<fa<<"\";\n";
						flag=1;
					}
					out<<"\t\tif ($"<<cnt<<"){\n";
					out<<"\t\t\t$$->son.push_back($"<<cnt<<");\n";
					out<<"\t\t\t$"<<cnt<<"->fa=$$;}\n";
				}
				st=en+1;
				if (en==l) 
				{
					if (flag) out<<"\t}"<<endl; break;
				}
				cnt++;
			}
		}
		else break;
	}
}
int main()
{
	in.open("ori.txt");
	out.open("out.txt");
	while (in.getline(s,1000))
	{
		int l=strlen(s);
		if (l==0) out<<s<<endl;
		else process(s);
	}
}