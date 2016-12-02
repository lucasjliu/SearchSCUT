#include"Crawler.h"
#include"Global.h"
#include"CrawlerPage.h"
#include"CrawlerUrl.h"
#include"DnsManager.h"
#include"HttpClient.h"
#include"ThreadMutex.h"
#include<WinSock2.h>
#include<iostream>

#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"x86\\pthreadVC2.lib")


int main()
{
	if(!initSocket())
	{
		std::cerr<<"winsock init Error!\n";
		exit(-1);
	}
	Crawler crawler;
	crawler.start();
	return 0;
}