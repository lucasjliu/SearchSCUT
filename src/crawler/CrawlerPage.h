/**
 * @file CrawlerPage.h
 * @version 0.0.0
 * @author zouxin ( zouxin2008@gmail.com )
 * @date 11/3/2008 0.0.0 created, by zouxin
 */
/**
 * modified by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2015
 */
#ifndef CRAWLERPAGE_H
#define CRAWLERPAGE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class CrawlerPage
{
public:
	CrawlerPage();
	CrawlerPage(const string &header,const string &body);
	~CrawlerPage();

	//parser
	int parserHeader(const string &header);
	int parserBody(const string &body);
	//get header
	int getStatusCode();
	int getContentLength();
	string getLocation();
	string getEncoding();
	string getContentType();
	string getCharset();
	string getTransferEncoding();

	//get body
	string getBody() const;
	
	//get links
	vector<string> getLinks();
	
	//get header
	string getHeader() const;

	//url
	string getUrl() const;
	void setUrl(string url);

	//ip
	string getIp() const;
	void setIp(string ip);

	//get title
	 string getTitle();
private:
    //url
    string m_url;

    //ip
    string m_ip;

    //header
    int m_headerLen;

    int m_statusCode;
    int m_contentLength;
    string m_location;
    string m_encoding;
    string m_contentType;
    string m_charset;
    string m_transferEncoding;
    bool m_connectionState;

    //body
    string m_body;

    //header 
    string m_header;

    //title
     string m_title;
private:
    void rawlinkfilter( string &rawlink);
    bool isFilterLink(const string &plink);

};
#endif
