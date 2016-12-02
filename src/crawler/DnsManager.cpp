/**
 * @file DnsManager.cc
 * @version 0.0.0
 * @author zouxin ( zouxin2008@gmail.com )
 * @date 11/3/2008 0.0.0 created, by zouxin
 */
/**
 * @modified by Lucas.Lau
 * @date 13/5/2015
 */

#include "DnsManager.h"

/**
 * DnsManager is in charge of maintain the DNS cache,
 * check if url is valid, and get ip from url
 */
DnsManager::DnsManager()
{
}

DnsManager::~DnsManager()
{
}

bool DnsManager::isValidHostChar(char ch)
{
	return( isalpha(ch) || isdigit(ch)
		|| ch=='-' || ch=='.' || ch==':' || ch=='_');
}

bool DnsManager::isValidHost(const char *host)
{
	if( !host ){
		return false;
	}

	if( strlen(host) < 6 ){ 
		return false;
	}

	char ch;
	for(unsigned int i=0; i<strlen(host); i++){
		ch = *(host++);
		if( !isValidHostChar(ch) ){
			return false;
		}
	}

	return true;
}

/* description:
 * 	very import function, get the ip of url
 * 	the url will be checked , invalid url will be discarded
 *
 * attention:
 * if can not lookup the url from DNS,or the IP is out of the range of IP,
 * the function will return a empty string
 */
string DnsManager::getIP(const string &url)
{
	CrawlerUrl xurl(url);
	if(url.empty())
		return "";
	
	if(!isValidHost(xurl.getHost().c_str()))
		return "";

	unsigned long inaddr=inet_addr(xurl.getHost().c_str());
	//???????????????????????????????????????????????????????????????????
	if(inaddr!=INADDR_NONE)//host is ip
	{
		if(isDomesticIP(xurl.getHost()))
			return xurl.getHost();
	}else
	{
		if(!isDomainValid(xurl.getHost()))
			return "";
		
		map<string,string>::iterator iter=dnsCache.find(xurl.getHost());
		if(iter!=dnsCache.end())//find it in cache
		{
			string ip=(*iter).second;
			inaddr=inet_addr(ip.c_str());
			if(inaddr!=INADDR_NONE)//yes,valid ip
			{
				return ip;
			}
		} 
		
		
	}		
#ifdef DEBUG
		//struct timeval starttv={0,0};
		//memset(&starttv,0,sizeof(starttv));
		//gettimeofday(&starttv,NULL);
#endif

	//not find from cache, so try to find from DNS
	struct hostent *host=gethostbyname(xurl.getHost().c_str());
#ifdef DEBUG
		//struct timeval endtv={0,0};
		//memset(&endtv,0,sizeof(endtv));
		//gettimeofday(&endtv,NULL);
		//cout<<"dns time cost "<<endtv.tv_sec-starttv.tv_sec<<endl;

#endif

	if(host==NULL)
		return "";
	char **pp=host->h_addr_list;
	if(*pp==NULL)
		return "";
	const char *result=inet_ntoa(*(struct in_addr*)(*pp));
	if(result==NULL)		
		return "";
	string ips(result);
	if(!isDomesticIP(ips))
		return "";
	//insert into cache
	dnsCache.insert(map<string,string>::value_type(xurl.getHost(),string(result)));
	//
#ifdef DEBUG
	cout<<"DNS cache has "<<dnsCache.size()<<" items"<<endl;
#endif
	return ips;
}

/*
 * check if the ip is in the range of what we want
 * e.g. China or one university
 * we will add this feature in future
 */
bool DnsManager::isDomesticIP(string ip)
{
	return true;
}

/**
 * we just visit .cn .com .org .net
 */
bool DnsManager::isDomainValid(const string &host)
{
	string::size_type pos=host.rfind('.');
	if(pos!=string::npos)
	{
		string temp=host.substr(pos);
		if(temp==".cn"||temp==".com"||
				temp==".org"||temp==".net")
			return true;
	}
	return false;
}
