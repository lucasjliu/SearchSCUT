#ifndef _LEXICON_H_
#define _LEXICON_H_

#include "Config.h"

#include <string>
#include <hash_map>
#include <fstream>
#include <sstream>

class Lexicon
{
private:
	_HASH _lexicon;
	string _filePath;
public:
	Lexicon(std::string filepath);
	Lexicon(){}
	bool save();
	_WORD_ID add_word(_WORD word);
	_WORD_ID get_word_id(_WORD word);
};

#endif