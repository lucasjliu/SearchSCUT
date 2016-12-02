/**
 * @created by Lucas.Lau
 * @date 13/5/2015
 */
#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include "Global.h"
#include "ThreadMutex.h"
#include "DnsManager.h"
#include "CrawlerPage.h"
#include "CrawlerUrl.h"
#include <winsock2.h>
#include <queue>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

using namespace std;

class HttpClient
{
	public:
		HttpClient();
		~HttpClient();
		int download(CrawlerUrl url, CrawlerPage &page);
	private:
		int createConnection(string url, CrawlerPage &page);
		string genRequest(CrawlerUrl url);
		bool nonblockSocket(SOCKET socket);
		bool blockSocket(SOCKET socket);
private:
	DnsManager dnsManager;
};
#endif
