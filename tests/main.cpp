#include <fstream>

#include <gtest/gtest.h>

#include "src/core/file_reader.h"
#include "src/core/nodes/node_factory.h"

using namespace node;

const std::string PATH_TO_EXAMPLE = "../example.flt";

class TestReadingFromFlt : public ::testing::Test {};

TEST_F(TestReadingFromFlt, test_data_existing) {
    file_reader::FileReader testing_obj(PATH_TO_EXAMPLE);
    testing_obj.Read();
    ASSERT_TRUE(testing_obj.GetByteNodes().size() > 0);
}

namespace {
class TestingData {
  private:
    u_int32_t data_;
    std::string path_;
    NodeTypes expected_type_;
  public:
    TestingData(u_int32_t data, std::string path, NodeTypes expected_type) : data_(data), path_(path), expected_type_(expected_type) {
        //create binary
        std::ofstream outp_file(path,std::ios::binary | std::ios::out);
        outp_file.write(reinterpret_cast<char*>(&data),sizeof(data));
        outp_file.close();
    }
    std::string GetPath() const { return path_; }
    NodeTypes GetExpectedType() const {return expected_type_;}
};
class TestReadingConcreteRecords : public ::testing::TestWithParam<TestingData> {
};
} // namespace

TEST_P(TestReadingConcreteRecords, test_recordtype_deduction) { 
    TestingData const& load(GetParam());
    
    file_reader::FileReader input_file(load.GetPath());
    input_file.Read();
    node::NodesFactory parser;
    ASSERT_EQ(parser.CreateNodeVecFromBytes(input_file.GetByteNodes()).at(0)->GetNodeType(),load.GetExpectedType());
    input_file.Close();
}

INSTANTIATE_TEST_SUITE_P(
        TestReadingConcreteRecords, TestReadingConcreteRecords,
        ::testing::Values(
            TestingData(0x04000100,"./1.flt",NodeTypes::kHeaderNode),
            TestingData(0x04000200,"./2.flt",NodeTypes::kGroupNode),
            TestingData(0x04000400,"./3.flt",NodeTypes::kObjectNode),
            TestingData(0x04000500,"./4.flt",NodeTypes::kFaceNode),
            TestingData(0x04002100,"./5.flt",NodeTypes::kLongIDNode)
            ));

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}