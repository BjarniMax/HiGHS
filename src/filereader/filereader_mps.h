#ifndef FILEREADER_MPS_H
#define FILEREADER_MPS_H

#include "filereader.h"

class FilereaderMps: public Filereader {
	public:
		int
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
				);

		};


#endif
