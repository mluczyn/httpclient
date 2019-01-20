#pragma once
#include<vector>
#include<algorithm>
#include<string>
using namespace std;

string startTag(string tag);
string endTag(string tag);
int findMatch(string tag, string range, int start);

struct elementNode
{
	string tag, content;
	int start, end;
	vector<elementNode> children;

	elementNode(string &range)
	{
		tag = range.substr(1, min(range.find(">"), range.find(" "))-1);
		start = range.find(">") + 1;
		end = findMatch(tag, range, start);
		content = range.substr(start, end - start);

		int cur = start;
		while (content.find("<", cur) != string::npos)
		{
			children.push_back(elementNode(content.substr(start, content.length() - start)));
			cur = children.back().end;
		}
	}
};
struct siteMasterNode
{
	elementNode *head, *body;
	string lowerHtml, normHtml;
	siteMasterNode(string raw)
	{
		normHtml = raw;
		for (int i = 0; i < raw.length(); i++)
			(raw.at(i) <= 90 && raw.at(i) >= 65) ? lowerHtml.push_back(raw.at(i) + 32) : lowerHtml.push_back(raw.at(i));

		head = new elementNode(lowerHtml.substr(lowerHtml.find("<head>"), lowerHtml.find("</head>")-lowerHtml.find("<head>")+7));
		body = new elementNode(lowerHtml.substr(lowerHtml.find("<body>"), lowerHtml.find("</body>") - lowerHtml.find("<body>") + 7));
	}
};

int findMatch(string tag, string range, int start)
{
	int cur = start-1;
	int n = 1;
	while (n != 0)
	{
		cur++;
		int nTag = range.find(startTag(tag), cur);
		int eTag = range.find(endTag(tag), cur);
		if (nTag < eTag && nTag != string::npos)
			cur = nTag, n++;
		else
			cur = eTag, n--;
	}
	return cur;
}
string startTag(string tag)
{
	string fullTag = "<";
	fullTag.insert(1, tag);
	return fullTag;
}
string endTag(string tag)
{
	string fullTag = "</>";
	fullTag.insert(2, tag);
	return fullTag;
}
 