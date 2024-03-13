#ifndef FILE_IO_ERRORS
#define FILE_IO_ERRORS
#include <exception>

namespace errors {

class ErrorOpeningFile : public std::exception {
    public:
    virtual const char *what() const noexcept {
        return "Could not open file, please check if it exists";
    }
};

class ErrorReadingFile : public std::exception {
    public:
    virtual const char *what() const noexcept {
        return "Could not read from file, please check if file readable";
    }
};

} // namespace errors

#endif