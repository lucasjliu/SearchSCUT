#include "Lexicon.h"

extern const _WORD_ID noid;

Lexicon::Lexicon(string filepath)
	:_filePath(filepath)
{
	_IN_FILE fsIn(filepath);
	if(fsIn.fail())
		return;
	string line;
	_WORD word;
	_WORD_ID wordId;
	while(getline(fsIn, line))
	{
		stringstream tmpStream(line);
		if(!(tmpStream >> word))
			continue;
		if(!(tmpStream >> wordId))
			continue;
		_lexicon[word] = wordId;
	}
	fsIn.close();
}
_WORD_ID Lexicon::add_word(_WORD word)
{
	return _lexicon[word] = _lexicon.size();
}
_WORD_ID Lexicon::get_word_id(_WORD word)
{
	_HASH_ITR itr = _lexicon.find(word);
	if(itr != _lexicon.end())
		return itr->second;
	return noid;
}
bool Lexicon::save()
{
	std::ofstream fsLinks(_filePath);
	if(fsLinks.fail())
		return false;
	while(!_lexicon.empty())
	{
		fsLinks << _lexicon.begin()->first << " "
			<< _lexicon.begin()->second << std::endl;
		_lexicon.erase(_lexicon.begin());
	}
	return true;
}