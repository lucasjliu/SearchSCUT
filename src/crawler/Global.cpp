/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2015
 */
#include "Global.h"

bool initSocket()
{
	WSAData data;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &data) != 0){
		return false;
	}
	if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2){
		return false;
	}
	return true;
}