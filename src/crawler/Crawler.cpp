/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 14/5/2015
 */
#include "Crawler.h"
Crawler::Crawler()
{
	_maxThread = MAX_THREAD;
	_maxDepth = MAX_DEPTH;
	_host = HOST;
	_startUrl = START_URL;
	_visitInterval = TIME_INTERVAL;
	_maxPageNum = MAX_PAGE_NUM;
	_userAgent = USER_AGENT;
	_logPath = DOWNLOAD_LOG_PATH;

	_downloadLog = ofstream(_logPath);
	_urlLog = ofstream(URL_LOG_PATH);
	_http = HttpClient();
	_pageNum = 0;

	_newUrlList = new vector<CrawlerUrl>[_maxDepth];
	_newPageList = new vector<CrawlerPage>[_maxDepth];
	_visitedUrlSet = set<MD5, CompMd5>();
	//_bannedPathSet = set<MD5, CompMd5>();

	pthread_mutex_init(&_httpMutex, NULL);
	pthread_mutex_init(&_pageNumMutex, NULL);
	pthread_mutex_init(&_downloadLogMutex, NULL);
	pthread_mutex_init(&_urlLogMutex, NULL);
	pthread_mutex_init(&_visitedUrlMutex, NULL);

	_newUrlListMutex = new pthread_mutex_t[_maxDepth];
	for(unsigned int i = 0; i < _maxDepth; i++)
		pthread_mutex_init(&_newUrlListMutex[i], NULL);

	_newPageListMutex = new pthread_mutex_t[_maxDepth];
	for(unsigned int i = 0; i < _maxDepth; i++)
		pthread_mutex_init(&_newPageListMutex[i], NULL);

	sem_init(&_newUrlSem, 0, 0);
	sem_init(&_newPageSem, 0, 0);

	loadData();
	addUrl(_startUrl, 0);
}
Crawler::~Crawler()
{
	delete[] _newUrlList;
	_downloadLog.close();
	_urlLog.close();

	pthread_mutex_destroy(&_httpMutex);
	pthread_mutex_destroy(&_pageNumMutex);
	pthread_mutex_destroy(&_downloadLogMutex);
	pthread_mutex_destroy(&_urlLogMutex);
	pthread_mutex_destroy(&_visitedUrlMutex);
	for(unsigned int i = 0; i < _maxDepth; i++)
		pthread_mutex_destroy(&_newPageListMutex[i]);
	for(unsigned int i = 0; i < _maxDepth; i++)
		pthread_mutex_destroy(&_newUrlListMutex[i]);

	sem_destroy(&_newUrlSem);
	sem_destroy(&_newPageSem);
}
void Crawler::start()
{
	pthread_t *fetchTid = new pthread_t[_maxThread];
	pthread_t *parseTid = new pthread_t[_maxThread];
	for(unsigned int i = 0; i < _maxThread; i++)
	{
		pthread_create(&fetchTid[i], NULL, fetchThread, this);
		pthread_create(&parseTid[i], NULL, parseThread, this);
	}
	for(unsigned int i = 0; i < _maxThread; i++)
	{
		pthread_join(fetchTid[i], NULL);
		pthread_join(parseTid[i], NULL);
	}
}
void Crawler::loadData()
{

}
bool Crawler::addUrl(string url, unsigned int depth)
{
	CrawlerUrl newUrl(url);
	if(_host != "")
	{
		if(newUrl.getHost() != _host)
			return false;
	}
	MD5 urlMd5 = genMd5(url);
	pthread_mutex_lock(&_visitedUrlMutex);
	if(_visitedUrlSet.find(urlMd5) == _visitedUrlSet.end())
	{
		pthread_mutex_unlock(&_visitedUrlMutex);
		if(depth < _maxDepth)
		{
			pthread_mutex_lock(&_newUrlListMutex[depth]);
			_newUrlList[depth].push_back(url);
			pthread_mutex_unlock(&_newUrlListMutex[depth]);
			sem_post(&_newUrlSem);
			return true;
		}
	}
	pthread_mutex_unlock(&_visitedUrlMutex);
	return false;
}
void Crawler::addPage(CrawlerPage page, unsigned int depth)
{
	if(depth >= _maxDepth)
		return;
	pthread_mutex_lock(&_newPageListMutex[depth]);
	_newPageList[depth].push_back(page);
	pthread_mutex_unlock(&_newPageListMutex[depth]);
	sem_post(&_newPageSem);

	pthread_mutex_lock(&_pageNumMutex);
	_pageNum++;
	pthread_mutex_unlock(&_pageNumMutex);
}
void Crawler::fetch()
{
	sem_wait(&_newUrlSem);
	while(true)
	{
		CrawlerUrl newUrl;
		unsigned int depth;
		for(depth = 0; depth < _maxDepth; depth++)
		{
			pthread_mutex_lock(&_newUrlListMutex[depth]);
			if(!_newUrlList[depth].empty())
			{
				newUrl = _newUrlList[depth].front();
				_newUrlList[depth].erase(_newUrlList[depth].begin());
				pthread_mutex_unlock(&_newUrlListMutex[depth]);
				break;
			}
			pthread_mutex_unlock(&_newUrlListMutex[depth]);
		}
		if(depth == _maxDepth)
		{//if all newUrlList are empty
			sem_wait(&_newUrlSem);

#ifdef THREAD_DEBUG
			cout<<"fetch thread "<<GetCurrentThreadId()<<" gain semaphore\n";
#endif
			continue;
		}
		MD5 newUrlMd5 = genMd5(newUrl.getUrl());

		pthread_mutex_lock(&_visitedUrlMutex);
		if(_visitedUrlSet.find(newUrlMd5) != _visitedUrlSet.end())
		{
			pthread_mutex_unlock(&_visitedUrlMutex);
			continue;
		}
		_visitedUrlSet.insert(newUrlMd5);
		pthread_mutex_unlock(&_visitedUrlMutex);
		CrawlerPage newPage;
		pthread_mutex_lock(&_httpMutex);
		int downloadRet = _http.download(newUrl, newPage);
		pthread_mutex_unlock(&_httpMutex);

#ifdef THREAD_DEBUG
		cout<<"fetch thraed: "<<GetCurrentThreadId()<<endl;
#endif
		logMsg("download", newUrl.getUrl(), downloadRet);
		if(downloadRet != 0)
		{
			continue;
		}
		logUrl(newUrl.getUrl());
		addPage(newPage, depth);
		Sleep(_visitInterval);
		//savePage(newPage, newUrl.getUrl());
	}
}
void Crawler::parse()
{
	sem_wait(&_newPageSem);
	while(true)
	{
		CrawlerPage newPage;
		unsigned int depth;
		for(depth = 0; depth < _maxDepth; depth++)
		{
			pthread_mutex_lock(&_newPageListMutex[depth]);
			if(!_newPageList[depth].empty())
			{
				newPage = _newPageList[depth].front();
				_newPageList[depth].erase(_newPageList[depth].begin());
				pthread_mutex_unlock(&_newPageListMutex[depth]);
				break;
			}
			pthread_mutex_unlock(&_newPageListMutex[depth]);
		}
		if(depth == _maxDepth)
		{//if all newUrlList are empty
			sem_wait(&_newPageSem);

#ifdef THREAD_DEBUG
			cout<<"parse thread"<<GetCurrentThreadId()<<" gain semaphore\n";
#endif#
			continue;
		}
		CrawlerUrl url(newPage.getUrl());
		vector<string> parseUrlList = newPage.getLinks();
		unsigned int num = parseUrlList.size();
		unsigned int addCount = 0;
		for(unsigned int i = 0; i < num; i++)
		{
			string parsedUrl = parseUrlList[i];
			normUrl(parsedUrl, url);
			if(addUrl(parsedUrl, depth+1))
				++addCount;
		}
#ifdef THREAD_DEBUG
		cout<<"parse thraed: "<<GetCurrentThreadId()<<endl;
#endif
		logMsg("parse", url.getUrl(), addCount);
	}
}
MD5 Crawler::genMd5(string str)
{
	CMD5 encoder;
	encoder.GenerateMD5((unsigned char*)str.c_str(), str.length());
	return encoder.getResult();
}
void Crawler::normUrl(string &url, CrawlerUrl newUrl)
{
	
	if(url.find("://")==string::npos)
		url = "http://" + newUrl.getHost() + '/' + url;
}
bool Crawler::savePage(CrawlerPage page, const string filepath)
{
	if(filepath != "")
	{
		ofstream saveFile(filepath);
		if(!saveFile)
		{
#ifdef DEBUG
			std::cerr<<"Fail to save "<<filepath<<endl;
#endif
			return false;
		}
		unsigned int pos = 0;
		string body = page.getBody();
		unsigned int bound = body.length();
		while(pos < bound)
			saveFile.put(body[pos++]);
		saveFile.close();
	}
	return true;
}
void Crawler::logMsg(string behavior, string url, int tag)
{
	cout<<timeStamp(1)<<behavior<<"["<<tag<<"] "<<url<<endl;
	pthread_mutex_lock(&_downloadLogMutex);
	_downloadLog<<_userAgent<<": "<<timeStamp(0)<<behavior
		<<"["<<tag<<"]"<<url<<endl;
	pthread_mutex_unlock(&_downloadLogMutex);
}
void Crawler::logUrl(string url)
{
	pthread_mutex_lock(&_urlLogMutex);
	_urlLog<<url<<endl;
	pthread_mutex_unlock(&_urlLogMutex);
}
string Crawler::timeStamp(int type)
{
	time_t t=time(NULL);
	char buf[32];
	string ret;
	if(type == 0)
	{
		strftime(buf,sizeof(buf),"[%Y-%m-%d %H:%M:%S]",localtime(&t));
		ret = string(buf);
	}
	else if(type == 1)
	{
		strftime(buf,sizeof(buf),"[%H:%M:%S]",localtime(&t));
		ret = string(buf);
	}
	return ret;
}
void *fetchThread(void *param)
{
	((Crawler*)param)->fetch();
	return (void*)0;
}
void *parseThread(void *param)
{
	((Crawler*)param)->parse();
	return (void*)0;
}