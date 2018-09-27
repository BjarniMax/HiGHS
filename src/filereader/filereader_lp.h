#ifndef FILEREADER_LP
#define FILEREADER_LP

#include "filereader.h"

class FilereaderLp: public Filereader {
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
    private:
        void
        readObjectiveSection();
        void
        readConstraintsSection();
        void
        readBoundsSection();
        void
        readGeneralSection();
        void
        readBinarySection();

        void
        readBound();
        void
        readConstraint();



};


#endif
