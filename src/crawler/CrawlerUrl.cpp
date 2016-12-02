/**
 * @file CrawlerUrl.cc
 * @version 0.0.0
 * @author zouxin ( zouxin2008@gmail.com )
 * @date 11/3/2008 0.0.0 created, by zouxin
 */
/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 14/5/2015
 */
#include "CrawlerUrl.h"

CrawlerUrl::CrawlerUrl(string url)
	: m_weight(10)
{
	if(url.find("://") == string::npos)
		url="http://"+url;
	_int pos;
	if((pos=url.find("www.")) != string::npos)
		url.erase(pos, 4);
	m_url=url;
	pos=m_url.find('/',7);
	if(pos==string::npos)//such as www.g.cn
	{
		m_host=m_url;
		m_url+='/';
		m_path='/';
	}
	else
	{
		m_host=m_url.substr(0,pos);
		m_path=m_url.substr(pos);
	}

	//calculate  weight
	string tempStr(m_url.substr(7));
	for(int i=0;i<tempStr.length();i++)
		if(tempStr[i]=='/'||tempStr[i]=='?'||tempStr[i]=='&')
			m_weight--;
	if(tempStr.find("search")!=string::npos)
		m_weight-=2;
	if(tempStr.find("proxy")!=string::npos)
		m_weight-=2;
	if(tempStr.find("gate")!=string::npos)
		m_weight-=2;
}

CrawlerUrl::CrawlerUrl()
	:m_url(""), m_host(""), m_path(""), m_weight(10){}

CrawlerUrl::~CrawlerUrl()
{
}

string CrawlerUrl::getUrl() const//return with "http://"
{
	return m_url;
}

string CrawlerUrl::getHost() const//return without "http://"
{
	return m_host.substr(7);
}

string CrawlerUrl::getPath() const
{
	if(m_path.size()==1)//just one '/'
		return "";
	return m_path;
}

void CrawlerUrl::setWeight(int weight)
{
	m_weight = weight;
}

int CrawlerUrl::getWeight() const
{
	return m_weight;
}
void CrawlerUrl::toLower(string &str, _int pos, _int length)
{
	while(length--)
		str[pos + length] = tolower(str[pos + length]);
}
