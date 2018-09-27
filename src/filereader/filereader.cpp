#include "filereader.h"

#include <string.h>
#include <stdexcept>

#include "filereader_mps.h"
#include "filereader_lp.h"



static const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename)
        return "";
    return dot + 1;
}


Filereader* Filereader::getFilereader(const char* filename) {
    Filereader* reader;
    const char* extension = get_filename_ext(filename);

    if(strcmp(extension, "mps") == 0) {
        reader = new FilereaderMps();
    }
    else if (strcmp(extension, "lp") == 0) {
        reader = new FilereaderLp();

    }
    else if (strcmp(extension, "gz") == 0) {
        // unzip the file, remove ".gz" from filename, call this function again
    }
    else {
        throw std::invalid_argument("No suitable filereader found.");
    }

    return reader;
}
