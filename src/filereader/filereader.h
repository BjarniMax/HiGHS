#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>

class Filereader {
	public:
    static Filereader* getFilereader(const char* filename);

	virtual int
		readFile(
				const char *filename,
				int &numRow,
				int &numCol,
				int &objSense,
				double &objOffset,
				std::vector<int> &Astart,
				std::vector<int> &Aindex,
				std::vector<double> &Avalue,
				std::vector<double> &colCost,
				std::vector<double> &colLower,
				std::vector<double> &colUpper,
				std::vector<double> &rowLower,
				std::vector<double> &rowUpper,
				std::vector<int> &integerColumn
				)
	{
	    // dummy implementation, is never called!
	    // has to be there since the virtual function needs to have an implementation
	    // even though it is overridden by all derived classes
	    return 0;
	}

};

#endif
