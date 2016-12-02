/**
 * @file CrawlerPage.cc
 * @version 0.0.0
 * @author zouxin ( zouxin2008@gmail.com )
 * @date 11/3/2008 0.0.0 created, by zouxin
 */
/**
 * modified by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2015
 */
#include "CrawlerPage.h"

/**
 * you can get the attributes of one web page from this class 
 */
const string ENCODING="Content-Encoding: ";
const string TYPE="Content-Type: ";//Content-Type: text/html; charset=GB2312
const string CHARSET="charset=";
const string LENGTH="Content-Length: ";
const string LOCATION="Location: ";

CrawlerPage::CrawlerPage():m_contentLength(-1)
{
}

CrawlerPage::CrawlerPage(const string &header,const string &body):m_contentLength(-1)
{
	parserHeader(header);
	parserBody(body);
}

CrawlerPage::~CrawlerPage()
{
}

string CrawlerPage::getBody() const 
{
	return m_body;
}

string CrawlerPage::getHeader() const
{
	return m_header;
}
//if successfully parsed, return 0
//otherwize return negative number
int CrawlerPage::parserHeader(const string &header)
{
	m_header=header;
#ifdef DEBUG
	//cout<<header;
#endif

	try{
		m_statusCode=atoi(header.substr(9,3).c_str());

		string temp=header;
		string field;
		string::size_type pos,fieldpos;
		
		while((pos=temp.find("\r\n"))>0)
		{
#ifdef DEBUG
			//cout<<temp.substr(0,pos)<<"---------"<<endl;
#endif
			field=temp.substr(0,pos);
			//////////////
			fieldpos=field.find(TYPE);
			if(fieldpos!=string::npos)
				m_contentType=field.substr(fieldpos+TYPE.length());
			else if( (fieldpos=field.find(ENCODING))!=string::npos)
				m_encoding=field.substr(fieldpos+ENCODING.length());
			else if( (fieldpos=field.find(LENGTH))!=string::npos)
				m_contentLength=atoi(field.substr(fieldpos+LENGTH.length()).c_str());
			else if( (fieldpos=field.find(LOCATION))!=string::npos)
				m_location=field.substr(fieldpos+LOCATION.length()).c_str();

			temp=temp.substr(pos+2);
			
		}//end while
	//some web server may output wrong header,so we have to catch the exception
	}catch(const out_of_range &excp)
	{
		std::cerr<<"the crawler catch out of range in parserHeader()" 
			"exception ,may be caused by the bad Web Server"<<endl;
		return -1;
	}
	
	return 0;

}

//if successfully parsed,return 0
//otherwize return negative number
int CrawlerPage::parserBody(const string &body)
{
	m_body.append(body);
	return 0;
}

int CrawlerPage::getStatusCode()
{
	return m_statusCode;
}

int CrawlerPage::getContentLength()
{
	return m_contentLength;
}

string CrawlerPage::getLocation()
{
	return m_location;
}

string CrawlerPage::getEncoding()
{
	return m_encoding;
}

string CrawlerPage::getContentType()
{
	return m_contentType;
}

string CrawlerPage::getCharset()
{
	string::size_type pos=m_contentType.find(CHARSET);
	if(pos!=string::npos)
		m_charset=m_contentType.substr(pos+CHARSET.length());
	return m_charset;
}

string CrawlerPage::getTransferEncoding()
{
	return m_transferEncoding;
}

string CrawlerPage::getUrl() const
{
	return m_url;
}

void CrawlerPage::setUrl(string url)
{
	m_url=url;
}


string CrawlerPage::getIp() const
{
	return m_ip;
}


void CrawlerPage::setIp(string ip)
{
	m_ip=ip;
}

//if no title ,return url
string CrawlerPage::getTitle()  
{
	string::size_type start=m_body.find("<title>");
	string::size_type end;
	if(start!=string::npos)
	{
		if((end=m_body.find("</title>"))!=string::npos)
		{
			m_title=m_body.substr(start+7,end-start-7);
			return m_title;
		}		
	}
	return getUrl();
}

/**
 * just find "href=", and try to find links 
 * note that this funtion provide an easy way to find links from web pages,
 * though it may get wrong links.
 * we will provid more accurate method to find links in future
 */
vector<string> CrawlerPage::getLinks()
{
	vector<string> links;
	if(m_header.empty())
		return links;
	string::size_type pos=0;
	string::size_type temp;
	while(  (pos=m_body.find("href=",pos))!=string::npos)
	{
		char sign = m_body[pos+5];//the char after "href=" as a begin and end sign
		temp = m_body.find(sign,pos+6);
		string rawlink=m_body.substr(pos+6,temp-pos-6);
		rawlinkfilter(rawlink);
		if(!rawlink.empty())
			links.push_back(rawlink);
		//cout<<m_body.substr(pos+5,temp-pos-5)<<endl;
		pos+=5;
	}
	return links;
}

/*
 * adjust a rawlink to normal link which can be crawled,
 * and discard obvious invalid links
 */
void CrawlerPage::rawlinkfilter(string &rawlink)
{
	if(rawlink.empty())
		return;

	rawlink.erase(0, rawlink.find_first_not_of("\'\""));
	rawlink.erase(rawlink.find_last_not_of("\'\"")+1);
	rawlink.erase(0, rawlink.find_first_not_of(" \t\n\r/"));
	rawlink.erase(rawlink.find_last_not_of(" \t\n\r")+1);

	string::size_type pos;
	unsigned int indexNum=10;
	char c;
	c=rawlink.at(rawlink.length()-1);
	if(c=='#'||c=='?'||c==';')
		rawlink=rawlink.substr(0,rawlink.length()-1);
	if(rawlink.empty())
		return;

	if((rawlink.length()>=6&&rawlink.substr(0,6)=="mailto")||
		(rawlink.length()>=3&&rawlink.substr(0,3)=="ftp")||
		(rawlink.length()>=10&&rawlink.substr(0,10)=="javascript")||
		(rawlink.length()>=1&&rawlink.at(0)=='#')||
		(rawlink.length()>=1&&rawlink.at(0)=='+'))
	{
		rawlink="";
		return;
	}
	
	if(rawlink.empty())
		return;
	if(isFilterLink(rawlink))//filter spam links
	{
		rawlink="";
		return;
	}

	//filter jpg,gif,mp3,wma,pdf,doc,swf,exe,rar,zip,rm files
	int templength=rawlink.length()-4;
	if( rawlink.rfind(".jpg")==templength||
			rawlink.rfind(".gif")==templength||
			rawlink.rfind(".mp3")==templength||
			rawlink.rfind(".wma")==templength||
			rawlink.rfind(".pdf")==templength||
			rawlink.rfind(".doc")==templength||
			rawlink.rfind(".swf")==templength||
			rawlink.rfind(".exe")==templength||
			rawlink.rfind(".rar")==templength||
			rawlink.rfind(".zip")==templength||
			rawlink.rfind(".ico")==templength||
			rawlink.rfind(".png")==templength||
			rawlink.rfind(".css")==templength)
		rawlink="";
}

/* this code belong to TSE
 * Filter spam links
 * If it is, return ture; otherwise false
 */
bool CrawlerPage::isFilterLink(const string &plink)
{
	if( plink.size() > 256 ) return true;

	string link = plink, tmp;
	string::size_type idx = 0;

	const char *filter_str[]={"rss",	"atom"
	};
	int filter_str_num = 2;
	for(int i=0; i<filter_str_num; i++){
		if( link.find(filter_str[i]) != string::npos)
		return true;
	}	

	return false;
}

