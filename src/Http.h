#pragma once
#include <algorithm>
#include <string>
#include "TCP.h"
#include "HTML.h"
#define FONT_BASE 11
using namespace std;

struct website
{
	string host, request, url, rawHtml;
	int hostLen;
public:
	website() {}
	int getSite(HWND &urlBar)
	{
		serverCom hostCom("tab1");
		char buf[9999];

		GetWindowText(urlBar, buf, 9999);
		url = string(buf);

		if (url.find("http://") == -1)
			url.insert(0, "http://");
		if (url.find("/", 8) == -1)
			url.append("/");
		SetWindowText(urlBar, url.c_str());

		hostLen = url.find("/", 8);
		host = url.substr(7, hostLen - 7);
		request = getRequest(url);

		if (hostCom.setup(host, "80") != 0)
			return -1;
		hostCom.openSoc();
		hostCom.requestFile(request, rawHtml);
		hostCom.closeSoc();
		return 0;
	}
	void drawSite(HWND &box)
	{
		siteMasterNode site(rawHtml);

		SetWindowText(box, site.body->content.c_str());
	}
private:
	string getRequest(string url)
	{
		int slashLoc = url.find("/", 8);
		string request;
		request.append("GET ");
		request.append(url.substr(slashLoc));
		request.append(" HTTP/1.1\n");
		request.append("User-Agent: Custom\n");
		request.append("Host: ");
		request.append(host);
		request.append("\nContent-Type: text/html; charset = utf-8");
		request.append("\n\n");
		return request;
	}
	 
};

 