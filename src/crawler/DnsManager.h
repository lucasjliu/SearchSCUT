/**
 * @file DnsManager.h
 * @version 0.0.0
 * @author zouxin ( zouxin2008@gmail.com )
 * @date 11/3/2008 0.0.0 created, by zouxin
 */
/**
 * @modified by Lucas.Lau
 * @date 13/5/2015
 */
#ifndef DNSMANAGER_H
#define DNSMANAGER_H
#include "CrawlerUrl.h"
#include "ThreadMutex.h"
#include "Global.h"
#include <winsock2.h>
#include<iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

class DnsManager
{
	public:
		DnsManager();
		~DnsManager();

		string getIP(const string &host);
	private:
		bool isValidHostChar(char ch);
		bool isValidHost(const char *host);	
		bool isDomesticIP(string ip);
		bool isDomainValid(const string &host);


		 map<string,string>dnsCache;
		set<string>domainfilter;
};
#endif
