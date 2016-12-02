#include "PageRank.h"
#include "Config.h"

typedef map<int,list<int> >::iterator milit;
typedef list<int>::iterator listit;
typedef map<int,double>::iterator midit;

extern const int ITERC;
extern const double DAMP;

PageRank::PageRank ( )
{
}
void PageRank::put (int docId, list<int>& docIds )
{
	out[docId] = docIds;
	return;
}
double PageRank::get ( int docId )
{
	return pr[docId];
}
void PageRank::buildInbound ( )
{
	for( milit it = out.begin(); it != out.end(); it++)
	{
		for(listit ix = (it->second).begin(); ix != (it->second).end(); ix++)
		{
			in[*ix].push_back(it->first);
		}
	}
}
void PageRank::compute ( )
{
	buildInbound();

	map<int,double>* previous = new map<int,double>();
	map<int,double>* current = new map<int,double>();

	int iter = ITERC;

	// Initialize all to 1
	for(milit it = out.begin(); it != out.end(); it++)
	{
		(*previous)[it->first] = 1.0;
	}

	for(milit it = in.begin(); it != in.end(); it++)
	{
		(*previous)[it->first] = 1.0;
	}

	while (iter--)
	{
		//Newton iteration. here default iter = 100
		for(midit it = (*previous).begin(); it != (*previous).end(); it++)
		{
			double sum = 0.0;
			for (listit is = in[it->first].begin(); is != in[it->first].end(); is++)
			{
				sum += (*previous)[*is] / out[*is].size();
			}
			//pr(i) = damp * (sum of average pr(inlink)) + 1 - damp
			(*current)[it->first] = 1 - DAMP + DAMP * sum;
		}
		delete previous;
		previous = current;
		current = new map<int,double>();
	}

	pr = *previous;
	delete previous;
	delete current;

	return;
}

bool PageRank::output ( const char* file )
{
	if(file == NULL)
		return false;
	ofstream fs(file);

	int max_index = 0;

	for( unsigned int doc = 0; doc < pr.size(); doc++)
	{
		for(unsigned int i = 0; i < pr.size(); i++)
		{
			if(pr[i]>pr[max_index])
				max_index = i;
		}
		fs << pr[max_index] <<"  " << max_index << endl;
		pr[max_index] = -1;
	}
	fs.close();
	return true;
}
/*
int main ()
{
	PageRank pr;
	list<int> x[3];
	x[0].push_back(2); x[0].push_back(3);
	x[1].push_back(1); //x[1].push_back(3);
	x[2].push_back(1); x[2].push_back(2);
	pr.Put(1,x[0]); pr.Put(2,x[1]); pr.Put(3,x[2]);
	pr.Compute();
	cout<<pr.Get(1)<<" "<<pr.Get(2)<<" "<<pr.Get(3)<<endl;
	return 0;
}
*/
