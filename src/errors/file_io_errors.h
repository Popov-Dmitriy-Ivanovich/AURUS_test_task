#ifndef FILE_IO_ERRORS
#define FILE_IO_ERRORS
#include <exception>

namespace errors {

class ErrorOpeningFile : std::exception {
    virtual const char *what() {
        return "Could not open file, please check if it exists";
    }
};

class ErrorReadingFile : std::exception {
    virtual const char *what() {
        return "Could not read from file, please check if file readable";
    }
};

} // namespace errors

#endif