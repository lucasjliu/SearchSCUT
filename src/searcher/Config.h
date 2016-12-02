#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <hash_map>
#include <hash_set>

using namespace std;

typedef string _DOC_INFO;
typedef unsigned int _DOC_ID;
typedef vector<_DOC_INFO> _DOC_VTR;

typedef unsigned int _WORD_ID;
typedef string _WORD;
typedef hash_map<_WORD, _WORD_ID> _HASH;
typedef _HASH::iterator _HASH_ITR;

typedef string _URL;
typedef map<_URL, _DOC_ID> _URL_MAP;
typedef _URL_MAP::iterator _URL_MAP_ITR;

typedef unsigned int _INT32;
enum FIELD {FIELD_CONTENT=0, FIELD_TITLE, FIELD_URL};
typedef struct Hit
{
	FIELD _field;
	Hit(FIELD type = FIELD_CONTENT)
	{
		_field = type;
	}
}Hit;
typedef list<Hit> _HIT_LIST;
typedef _HIT_LIST::iterator _HIT_LIST_ITR;
typedef set<_DOC_ID> _WORD_INDEX;
typedef hash_map<_WORD_ID, _HIT_LIST> _DOC_INDEX;
typedef hash_map<_DOC_ID, _DOC_INDEX> _FOWD_INDEX;
typedef hash_map<_WORD_ID, _WORD_INDEX> _INVT_INDEX;
typedef hash_map<_DOC_ID, vector<_INT32>> _FIELD_LENGTH;

typedef string _PATH;
typedef string _TEXT;
typedef list<_WORD> _WORD_LIST;
typedef _WORD_LIST::iterator _WORD_LIST_ITR;

typedef hash_set<_WORD> _HASH_SET;

typedef map<_DOC_ID, double> _VAL_MAP;
typedef list<_DOC_ID> _DOC_LIST;
typedef _DOC_LIST::iterator _DOC_LIST_ITR;

typedef double _VAL;

typedef map<_DOC_ID, double> _PAGERANK;

typedef ifstream _IN_FILE;
typedef ofstream _OUT_FILE;
typedef fstream _FILE;

#endif