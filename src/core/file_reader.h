#ifndef FILE_READER
#define FILE_READER

#include <cassert>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "src/errors/file_io_errors.h"
#include "src/types/type_defenitions.h"

namespace file_reader {

using errors::ErrorOpeningFile;
using errors::ErrorReadingFile;
using types::Int2;
using types::UnsignedInt2;
class FileReader {
  public:
    explicit FileReader(std::string const &filename)
        : filename_(filename), data_actual_(false), error_(false) {
        Open(filename);
    }

    bool IsOpen() noexcept { return input_stream_.is_open(); }
    bool IsConsistent() noexcept { return !error_; }

    void Close() noexcept { input_stream_.close(); }

    std::vector<std::shared_ptr<std::byte[]>> GetByteNodes() {
        assert(IsConsistent());
        assert(data_actual_);
        return byte_nodes_;
    }

    void Read();
    void Open(std::string const &filename);
    void Reset();

  private:
    std::string filename_;
    bool data_actual_;
    bool error_;
    std::ifstream input_stream_;
    std::vector<std::shared_ptr<std::byte[]>> byte_nodes_;
};

} // namespace file_reader

#endif