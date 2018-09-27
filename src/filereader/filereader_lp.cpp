#include "filereader_lp.h"

#include <stdexcept>
#include <stdio.h>

// see https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html
int FilereaderLp::readFile(
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
    // clear data structures
    numRow = 0;
    numCol = 0;
    objOffset = 0;
    objSense = 1;

    Astart.clear();
    Aindex.clear();
    Avalue.clear();
    colCost.clear();
    colLower.clear();
    colUpper.clear();
    rowLower.clear();
    rowUpper.clear();
    integerColumn.clear();

    FILE *file = fopen(filename, "r");
    if (file == 0) {
        // error when opening file
        return 1;
    }



    throw std::invalid_argument("LP file reader not yet implemented.");
    return 0;
};
