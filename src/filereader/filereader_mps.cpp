#include "filereader_mps.h"

#include "../HMPSIO.h"
#ifdef Boost_FOUND
#include "../HMpsFF.h"
#endif

int FilereaderMps::readFile(
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
	int RtCd;
#if defined(Boost_FOUND) && !defined(OLD_PARSER)
    RtCd = readMPS_FF(filename,
                     numRow, numCol, objSense, objOffset,
                     Astart, Aindex, Avalue,
                     colCost, colLower, colUpper, rowLower, rowUpper);
#else
    RtCd = readMPS(filename, -1, -1,
                     numRow, numCol, objSense, objOffset,
                     Astart, Aindex, Avalue,
                     colCost, colLower, colUpper, rowLower, rowUpper, integerColumn);
#endif
    return RtCd;
};
