/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2015
 */
#ifndef GLOBAL_H
#define GLOBAL_H
//#define DEBUG
//#define THREAD_DEBUG
#include<WinSock2.h>

//default system parameter
const int MAX_WEBPAGE_BYTES = 1024 * 1024; //1MB
const int DEFAULT_TIMEOUT = 10;
const int CONNECT_TIMEOUT = 10;

//default user parameter
int MAX_THREAD = 2;
int MAX_DEPTH = 10;
string HOST = "100steps.net";
string START_URL = "www.100steps.net";
int TIME_INTERVAL = 1000;
int MAX_PAGE_NUM = 10000;
string USER_AGENT = "201230590105";
string DOWNLOAD_LOG_PATH = "data\\log.dat";
string URL_LOG_PATH = "data\\url.dat";

//global function
bool initSocket();

#endif