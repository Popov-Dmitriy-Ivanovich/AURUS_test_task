#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "src/core/file_reader.h"
#include "src/errors/file_io_errors.h"
#include "src/types/type_defenitions.h"

using file_reader::FileReader;
int main() {
    FileReader test("./docs/Model_3_ver164.flt");
    test.Read();
    auto res = test.GetByteNodes();
    return 0;
}
