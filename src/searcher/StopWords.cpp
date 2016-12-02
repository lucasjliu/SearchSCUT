#include "StopWords.h"

StopWords::StopWords(_PATH filepath)
	:_filePath(filepath)
{
	_IN_FILE fsIn(filepath);
	if(fsIn.fail())
		return;
	_WORD word;
	while(getline(fsIn, word))
		_words.insert(word);
	fsIn.close();
}
bool StopWords::has_word(_WORD word)
{
	return _words.find(word) != _words.end();
}