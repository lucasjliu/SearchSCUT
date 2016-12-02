#ifndef _STOPWORDS_H_
#define _STOPWORDS_H_

#include "Config.h"

#include <string>
#include <hash_set>
#include <fstream>
#include <sstream>

class StopWords
{
private:
	_HASH_SET _words;
	_PATH _filePath;
public:
	StopWords(_PATH filepath);
	StopWords(){}
	bool has_word(_WORD word);
};

#endif