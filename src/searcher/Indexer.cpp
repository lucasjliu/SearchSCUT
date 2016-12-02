#include "Indexer.h"

Indexer::Indexer(_PATH crawledDir, string workDir)
{
	if(crawledDir != "")
		if(crawledDir[crawledDir.length()-1] != '\\')
			crawledDir.append("\\");
	_repository = crawledDir;
	_links = Links(workDir + LINKS_FILE);
	_docs = Docs(workDir + DOCS_FILE);
	_lexicon = Lexicon(workDir + LEXICON_FILE);
	_stopWords = StopWords(workDir + STOPWORD_FILE);
	_index = Index(workDir + INDEX_FILE);
}
void Indexer::get_file(_DOC_ID docId, _TEXT& filebody)
{
	filebody = _TEXT();
	string filename = _docs.get_info(docId);
	_IN_FILE fsFile(_repository+filename);
	if(fsFile.fail())
		return;
	std::istreambuf_iterator<char> beginIter(fsFile), endIter;
	filebody = std::string(beginIter, endIter);
	fsFile.close();
}
void Indexer::get_title(_TEXT filebody, _TEXT& title)
{//read the whole file and find title
	title = _TEXT();
	size_t titleBegin;
	if((titleBegin = filebody.find("<title>")) != NPOS)
	{
		titleBegin += strlen("<title>");
		title = filebody.substr(titleBegin,
			filebody.find("<",titleBegin)-titleBegin);
	}
	transform(title.begin(), title.end(), title.begin(), ::tolower);
}
void Indexer::get_content(_TEXT filebody, _TEXT& content)
{//find text between each two tags and compare len(text)/len(tag) with a bias
//to determine whether the text is content
	content = _TEXT();
	_TEXT leftTag, rightTag;
	size_t lTagPos, rTagPos, startPos = 0;
	
	if((lTagPos = find_html_tag(filebody,leftTag,startPos))==NPOS)
		return;
	startPos = lTagPos + leftTag.size();
	while((rTagPos=find_html_tag(filebody,rightTag,startPos))!=NPOS)
	{
		if(rTagPos - lTagPos >= leftTag.size() + 1 && 
			leftTag.find("<script") == std::string::npos && 
			leftTag.find("<style") == std::string::npos)
		{//if has text between tags, and tag not belong to script or style
			_TEXT text = filebody.substr(lTagPos+leftTag.size(),
				rTagPos-(lTagPos+leftTag.size()));
			if(isContent(text.length(),leftTag.length()+rightTag.length()))
			{
				transform(text.begin(), text.end(), text.begin(), ::tolower);
				content.append(text).append("\n");
			}
		}
		lTagPos = rTagPos;
		leftTag = rightTag;
		startPos = lTagPos + leftTag.length();
	}
}
void Indexer::get_url(_DOC_ID docId, _TEXT& url)
{
	url = _links.get_url(docId);
	transform(url.begin(), url.end(), url.begin(), ::tolower);
}
size_t Indexer::find_html_tag(std::string text, 
									std::string& tag, 
									size_t pos)
{
	size_t tagStart, tagEnd;
	if((tagStart = text.find('<', pos)) == std::string::npos)
		return std::string::npos;
	if((tagEnd = text.find('>', tagStart + 1)) == std::string::npos)
		return std::string::npos;
	tag = text.substr(tagStart, tagEnd - tagStart + 1);
	return tagStart;
}
bool Indexer::isContent(size_t textLen, size_t tagLen)
{
	return (double)textLen/(double)tagLen > 0.8;
}
void Indexer::segment(_TEXT text, _WORD_LIST& list, StopWords& stopWords)
{
	list = _WORD_LIST();
	if(text == "")
		return;
	typedef unsigned int Position;
	Position left = 0, right = 0, curr = 0, textEnd = text.length()-1;
	_WORD word;
	while(curr <= textEnd)
	{
		while(text[curr] < 'a' || text[curr] > 'z')
			if(++curr >= textEnd)
				break;
		left = curr;
		if(curr <= textEnd)
			while(text[curr] >= 'a' && text[curr] <= 'z')
				if(++curr > textEnd)
					break;
		right = curr;
		word = text.substr(left, right - left);
		if(!stopWords.has_word(word) && word != "")
			list.push_back(word);
	}
}
void Indexer::add_hit(_DOC_ID docId, _WORD_LIST& list, FIELD type)
{
	_WORD_LIST::iterator itr = list.begin();
	for(; itr != list.end(); itr++)
	{
		_WORD word = *itr;
		_WORD_ID wordId = _lexicon.get_word_id(word);
		if(wordId == noid)
			wordId = _lexicon.add_word(word);
		_index.add_hit(docId, wordId, type);
	}
}
void Indexer::start_index()
{
	_DOC_ID docId;
	_TEXT filebody, title, content, url;
	_WORD_LIST wordList, titleWordList, urlWordList;
	//Lexicon::_WORD_ID wordId;
	_WORD word;
	for(docId = 0; docId < _docs.size(); docId++)
	{
		get_file(docId, filebody);
		get_content(filebody, content);
		get_url(docId, url);
		get_title(filebody, title);
		segment(content, wordList, _stopWords);
		segment(title, titleWordList, _stopWords);
		segment(url, urlWordList, _stopWords);
		add_hit(docId, wordList, FIELD_CONTENT);
		add_hit(docId, titleWordList, FIELD_TITLE);
		add_hit(docId, urlWordList, FIELD_URL);
	}
}
void Indexer::save()
{
	_index.save();
	//_lexicon.save();

}