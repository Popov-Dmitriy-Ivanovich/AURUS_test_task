#include "src/core/file_reader.h"

namespace file_reader {

void FileReader::Open(std::string const &filename) {
    filename_ = filename;
    input_stream_.open(filename, std::ios::binary);
    data_actual_ = false;
    if (!input_stream_.is_open()) {
        error_ = true;
        throw ErrorOpeningFile();
    }
}
void FileReader::Read() {
    assert(IsConsistent());

    while (!input_stream_.eof()) {
        UnsignedInt2 length = 0;

        // skipping opcode
        input_stream_.read(reinterpret_cast<char *>(&length), 2);
        if (input_stream_.eof()) {
            break;
        }

        // reading length bytes in Big-endian order
        UnsignedInt2 msb = 0;
        UnsignedInt2 lsb = 0;
        input_stream_.read(reinterpret_cast<char *>(&msb), 1);
        input_stream_.read(reinterpret_cast<char *>(&lsb), 1);
        // geting length from bytes
        length = msb << 8 | lsb;
        // restoring reading pointer to node start
        for (int i = 0; i < 4; i++) {
            input_stream_.unget();
        }

        std::shared_ptr<std::byte[]> cur_node(new std::byte[length]);
        input_stream_.read(reinterpret_cast<char *>(cur_node.get()), length);

        if (input_stream_.fail()) {
            error_ = true;
            throw ErrorReadingFile();
        }
        byte_nodes_.push_back(cur_node);
    }
    data_actual_ = true;
}

void FileReader::Reset() {
    Open(filename_);
    data_actual_ = false;
    error_ = false;
    byte_nodes_.clear();
}

} // namespace file_reader