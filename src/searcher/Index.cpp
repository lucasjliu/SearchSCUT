#include "Index.h"

extern const _INT32 FIELD_NUM;

Index::Index(string filePath)
	:_filePath(filePath)
{
	_IN_FILE fsIn(_filePath, std::ios::in);
	if(fsIn.fail())
		return;

	size_t barrelSize = 0, indexSize = 0, listSize = 0;
	_DOC_ID docId;
	_WORD_ID wordId;
	int field;

	fsIn >> barrelSize;
	for(size_t barrel = 0; barrel < barrelSize; barrel++)
	{
		fsIn >> docId >> indexSize;
		for(size_t index = 0; index < indexSize; index++)
		{
			fsIn >> wordId >> listSize;
			_HIT_LIST list;
			for(size_t hit = 0; hit < listSize; hit++)
			{
				fsIn >> field;
				add_hit(docId, wordId, (FIELD)field);
			}
		}
	}
	fsIn.close();
}
void Index::add_hit(_DOC_ID docId, _WORD_ID wordId, FIELD field)
{
	//add to hitList in foward index
	_fowdIndex[docId][wordId].push_back(Hit(field));
	_WORD_INDEX wordIndex = _invtIndex[wordId];
	if(wordIndex.find(docId) == wordIndex.end())
	{
		//add to inverted index if is new doc
		_invtIndex[wordId].insert(docId);
		if(!_fieldLength[docId].size())
			for(int field = 0; field < FIELD_NUM; ++field)
				_fieldLength[docId].push_back(0);
	}
	//update fieldLength of this document
	++_fieldLength[docId][field];
}
void Index::get_hit_list(_DOC_ID docId, _WORD_ID wordId, _HIT_LIST& hitList)
{
	hitList = _fowdIndex[docId][wordId];
}
void Index::get_doc_index(_DOC_ID docId, _DOC_INDEX& docIndex)
{
	docIndex = _fowdIndex[docId];
}
void Index::get_word_index(_WORD_ID wordId, _WORD_INDEX& wordIndex)
{
	wordIndex = _invtIndex[wordId];
}
bool Index::save()
{
	_FILE fsOut(_filePath, std::ios::out | std::ios::trunc);
	if(fsOut.fail())
		return false;

	fsOut << _fowdIndex.size() << std::endl;
	for(_FOWD_INDEX::iterator barrelItr = _fowdIndex.begin();
		barrelItr != _fowdIndex.end();
		barrelItr++)
	{
		fsOut << barrelItr->first << " ";
		_DOC_INDEX& docIndex = barrelItr->second;
		fsOut << docIndex.size() << endl;
		for(_DOC_INDEX::iterator indexItr = docIndex.begin();
			indexItr != docIndex.end(); indexItr++)
		{
			fsOut<< indexItr->first << " ";
			_HIT_LIST& list = indexItr->second;
			save_hit_list(fsOut, list);
		}
	}
	fsOut.close();
	return true;
}
void Index::save_hit_list(_FILE& fs, _HIT_LIST& list)
{
	fs << list.size() << " ";
	for(_HIT_LIST::iterator itr = list.begin();
		itr != list.end(); ++itr)
		fs << itr->_field << " ";
	fs << std::endl;
}
_INT32 Index::doc_num()
{
	return _fowdIndex.size();
}
_INT32 Index::doc_num(_WORD_ID wordId)
{
	return _invtIndex[wordId].size();
}
_INT32 Index::word_num()
{
	return _invtIndex.size();
}
_INT32 Index::word_num(_DOC_ID wordId)
{
	return _fowdIndex[wordId].size();
}
_INT32 Index::hit_num(_DOC_ID docId, _WORD_ID wordId)
{
	return _fowdIndex[docId][wordId].size();
}
_INT32 Index::field_length(_DOC_ID docId, FIELD field)
{
	if(_fieldLength[docId].size() != FIELD_NUM)
		return 0;
	return _fieldLength[docId][field];
}