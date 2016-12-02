/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 14/5/2015
 */
#ifndef CRAWLER_H
#define CRAWLER_H
#include "CMD5.h"
#include "Global.h"
#include "CrawlerUrl.h"
#include "CrawlerPage.h"
#include "HttpClient.h"
#include "ThreadMutex.h"
#include "ThreadSem_t.h"
#include <ctime>
#include <time.h>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <hash_set>

struct CompMd5
{
	bool operator ()(const MD5 &m1, const MD5 &m2) const
	{
		for(unsigned int i = 0; i < 4; i++)
		{
			if(m1.data[i] < m2.data[i])
				return true;
			else if(m1.data[i] > m2.data[i])
				return false;
		}
		return false;
	}
};
void *fetchThread(void *param);
void *parseThread(void *param);
class Crawler
{
private:
	void loadData();//check and load cache data from last stop
	bool addUrl(string url, unsigned int depth);//check and add a url to new list
	void addPage(CrawlerPage page, unsigned int depth);//add a page to new list
	MD5 genMd5(string st);
	void normUrl(string &url, CrawlerUrl newUrl);//parse a page in blocking way
	bool savePage(CrawlerPage page, const string filepath);
	void logMsg(string behavior, string url, int tag);
	void logUrl(string url);
	string timeStamp(int type);
public:
	Crawler();//if program return normally, no cache create
	~Crawler();
	void start();//begin crawling and parsing thread
	void fetch();//fetch thread logic function
	void parse();//parse thread logic function
	//bool pause();//yield all the crawling and parsing thread
	//bool stop();//destroy all the thread 
				//save new url&page and visited url in cache file
private:
	//user setting
	unsigned int _maxThread;//default 2
	unsigned int _maxDepth;//default 
	string _host;//default ""
	string _startUrl;//default www.scut.com.cn
	string _logPath;//default ".//download//"_host+"//log.dat"
	string _userAgent;//default 201230590105
	unsigned int _visitInterval;//default 1000
	unsigned int _maxPageNum;//default 10000
	//string _descriptor;//default test

	//utility member
	HttpClient _http;//need lock
	unsigned int _pageNum;//init 0, need lock
	ofstream _downloadLog;//need lock
	ofstream _urlLog;

	//cache list
	vector<CrawlerUrl> *_newUrlList;
	vector<CrawlerPage> *_newPageList;
	set<MD5, CompMd5> _visitedUrlSet;
	//set<MD5, CompMd5> _bannedPathSet;

	//mutex to protect shared memory.
	pthread_mutex_t _httpMutex;
	pthread_mutex_t _pageNumMutex;
	pthread_mutex_t _downloadLogMutex;
	pthread_mutex_t _urlLogMutex;
	pthread_mutex_t *_newUrlListMutex;
	pthread_mutex_t _visitedUrlMutex;
	pthread_mutex_t *_newPageListMutex;
	/*ThreadMutex _bannedPathMutex;*/

	//semaphore to synchronize fetch and parse thread. used as counter
	sem_t _newUrlSem;
	sem_t _newPageSem;
};
#endif