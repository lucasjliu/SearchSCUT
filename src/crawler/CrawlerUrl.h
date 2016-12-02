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
#ifndef CRAWLERURL_H
#define CRAWLERURL_H
#include <string>
#include <algorithm>
using namespace std;
#define _int string::size_type
class CrawlerUrl
{
	public:
		CrawlerUrl(string url);
		CrawlerUrl();
		~CrawlerUrl();

		string getUrl() const;
		string getHost() const;
		string getPath() const;

		void setWeight(int weight);
		int getWeight() const;
	private:
		void toLower(string &str, _int pos, _int length);
		int m_weight;
		string m_url;
		string m_host;
		string m_path;
};
#endif
