/**
 * @created by Lucas.Lau
 * @date 13/5/2014
 */
#include "HttpClient.h"


HttpClient::HttpClient()
{
	dnsManager = DnsManager();
}

HttpClient::~HttpClient()
{
}

/** download and fullfill the page,
 * if everything is ok,return 0
 * else return error code
 *  -1-->empty ip in createConnection
 *  -2-->socket create error in createConnection
 *  -3-->fcntl error 
 *  -4-->select error when connecting
 *  -5-->select timout when connecting
 *  -6-->status code>=400
 *  -7-->content too long
 *  -8-->unwanted type
 *  -9-->read error
 *  -10-->direct connect and nonblock connect are all error or timeout
 *  -11-->select read error
 *  -12-->select read timeout
 *  -13-->write error
 *  -14-->catch parserHeader exception
 */
int HttpClient::download(CrawlerUrl url,CrawlerPage &page)
{
	SOCKET sockfd=createConnection(url.getUrl(),page);
	if(sockfd<0)
		return sockfd;

	
	page.setUrl(url.getUrl());
	string location;
	if(url.getPath().empty()) //path is empty,we can assume request main page
		location="/";
	else
		location=url.getPath();
	 const string reqHeader = genRequest(url);
	int writeRet = send(sockfd, reqHeader.c_str(), reqHeader.length(), 0);	//win32 version
	if(writeRet == -1 || writeRet == 0)
	{
#ifdef DEBUG
		cerr<<"write error in HttpClient"<<endl;
#endif
		perror("write");
		closesocket(sockfd);
		return -13;
	}

	fd_set nonbset, readyset;
	FD_ZERO(&nonbset);
	FD_SET(sockfd,&nonbset);

	struct timeval waittv;
	waittv.tv_sec=DEFAULT_TIMEOUT;
	waittv.tv_usec=0;

	int selectRet=-1,readRet=-1;
	int bytesRead=0;
	int newlines=0;
	char tempBuffer;
	string pageHeader;
	pageHeader.reserve(1024);
	while(newlines<2)
	{
        readyset = nonbset;
		selectRet=select(sockfd+1,&readyset,NULL,NULL,&waittv);
		if(selectRet<0)
		{
			
#ifdef DEBUG
			cerr<<"select error"<<endl;
#endif
			closesocket(sockfd);
			return -11;
		}else if(selectRet==0)
		{
#ifdef DEBUG
			cerr<<"select timeout"<<endl;
#endif
			closesocket(sockfd);
			return -12;
		}
		if( (readRet=recv(sockfd, &tempBuffer, 1, 0))==-1)
		{
#ifdef DEBUG
			cerr<<"read header error"<<endl;
#endif
			closesocket(sockfd);
			return -9;
		}
		if(tempBuffer=='\r')
		{
		}else if(tempBuffer=='\n')
			newlines++;
		else
			newlines=0;
		pageHeader+=tempBuffer;
	}
	if(page.parserHeader(pageHeader)<0)
	{
		closesocket(sockfd);
		return -14;//catch exception
	}
	if(page.getStatusCode()>=400||page.getStatusCode()<200)
	{
#ifdef DEBUG
		cerr<<"status code err"<<page.getStatusCode()<<endl;
#endif
		closesocket(sockfd);
		return -6;
	}
	int contentLength=page.getContentLength();
	if(contentLength>MAX_WEBPAGE_BYTES)
	{
		closesocket(sockfd);
		return -7;
	}
	if(page.getContentType().empty()==false&&
			page.getContentType().find("text/html")==string::npos&&
			page.getContentType().find("text/plain")==string::npos&&
			page.getContentType().find("text/xml")==string::npos&&
			page.getContentType().find("text/html")==string::npos)
	{
#ifdef DEBUG
		cerr<<"unwanted type :"<<page.getContentType()<<endl;
#endif
		closesocket(sockfd);
		return -8;
	}

#ifdef DEBUG
	//cout<<page.getHeader()<<endl;
#endif
	string pageBody;
	char readBuffer[4096];
	if(contentLength>0)
		pageBody.reserve(page.getContentLength());

	selectRet=select(sockfd+1,&nonbset,NULL,NULL,&waittv);
	if(selectRet<0)
	{
#ifdef DEBUG
		cerr<<"select error"<<endl;
#endif
		closesocket(sockfd);
		return -11;
	}else if(selectRet==0)
	{
#ifdef DEBUG
		cerr<<"select timeout"<<endl;
#endif
		closesocket(sockfd);
		//do something,maybe register this host
		return -12;
	}	
	while( (readRet=recv(sockfd, readBuffer, sizeof(readBuffer), 0))>0 )
	{
		pageBody+=string(readBuffer,0,readRet);
	}
	page.parserBody(pageBody);
	closesocket(sockfd);
	return 0;
}

/*if successfuly connect to the host,will return the socketfd,otherwize
 *will return negative number
 *should modify into nonblock connect
 */
int HttpClient::createConnection(string url, CrawlerPage &page)
{
	string ip=dnsManager.getIP(url);
	if(ip.empty())
		return -1;
	page.setIp(ip);

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(80);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	SOCKET sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("socket");
		return -2;
	}
	int optval=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char*)&optval,sizeof(optval));
	struct timeval connectv;
	connectv.tv_sec=CONNECT_TIMEOUT;
	connectv.tv_usec=0;
	setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char*)&connectv,sizeof(struct timeval));
	if(connect(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))==0)
	{
		return sockfd;
	}
	if(errno==EINPROGRESS)
	{
		if(!nonblockSocket(sockfd))
		{
#ifdef DEBUG
			cerr<<"wrong in ioctlsocket"<<endl;
#endif
			closesocket(sockfd);
			return -3;
		}
		fd_set nonbset;
		FD_ZERO(&nonbset);
		FD_SET(sockfd,&nonbset);

		struct timeval tv;
		tv.tv_sec=DEFAULT_TIMEOUT;
		tv.tv_usec=0;

		int selectRet=select(sockfd+1,&nonbset,NULL,NULL,&tv);

		if(selectRet==-1)
		{
			closesocket(sockfd);
			return -4;
		}else if(selectRet==0)
		{
			closesocket(sockfd);
			return -5;
		}
		//nonblocking connect success
		return sockfd;
	}
	
	//both direct connect and nonblock connect are all failed or timeout 
	closesocket(sockfd);
	return -10;
}
string HttpClient::genRequest(CrawlerUrl url)
{
	string path = url.getPath().empty() ? "/" : url.getPath();
	string request = 
			"GET " + path + " HTTP/1.1\r\n"
			"Host:" + url.getHost() + "\r\n"
			"Accept:*/*\r\n"
			"User-Agent:201230590105\r\n"
			"Pragma:   no-cache\r\n"
			"Cache-Control:   no-cache\r\n"
			"Connection:Close\r\n\r\n";
	return request;
}
/*
 * win32 socket settings.
 */
bool HttpClient::nonblockSocket(SOCKET socket)
{
	unsigned long flags = 1;
	if(ioctlsocket(socket, FIONBIO, &flags) != 0)
		return false;
	return true;
}
bool HttpClient::blockSocket(SOCKET socket)
{
	unsigned long flags = 0;
	if(ioctlsocket(socket, FIONBIO, &flags) != 0)
		return false;
	return true;
}
