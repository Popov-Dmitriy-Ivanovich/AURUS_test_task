#include <iostream>
#include <memory>
#include <vector>

#include "src/core/file_reader.h"
#include "src/core/nodes/header_node/header_node.h"
#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"
#include "src/errors/file_io_errors.h"
#include "src/types/type_defenitions.h"

#include "src/core/nodes/face_node/face_node.h"
#include "src/core/nodes/group_node/group_node.h"
#include "src/core/nodes/long_id_node/long_id_node.h"
#include "src/core/nodes/node_factory.h"
#include "src/core/nodes/object_node/object_node.h"
// enum class NodeTypes { kHeaderNode };

using file_reader::FileReader;
using node::HeaderNode;
using node::HeaderNodeFields;
using namespace node;
using node::INode;
using types::Int2;
using types::Int4;
using types::UnsignedInt2;
using types::UnsignedInt4;

int main() {
    FileReader test("../docs/Model_3_ver164.flt");
    test.Read();
    auto res = test.GetByteNodes();
    std::cout << res.size() << " " << res[0] << "\n";
    // HeaderNode headertest(res[0]);
    NodesFactory testfac;
    auto res_vec = testfac.CreateNodeVecFromBytes(res);
    for (auto const &i : res_vec) {
        std::cout << "---------------------------------------------------------"
                     "-----------------------"
                  << std::endl;
        switch (i->GetNodeType()) {
        case NodeTypes::kHeaderNode: {
            std::cout << "Header" << std::endl;
            std::shared_ptr<HeaderNode> cur =
                std::dynamic_pointer_cast<HeaderNode>(i);
            std::cout << "ID: "
                      << cur->GetFieldValueByName<std::string>(
                             HeaderNodeFields::kID)
                      << std::endl;
            break;
        }
        case NodeTypes::kObjectNode: {
            std::cout << "Object" << std::endl;
            std::shared_ptr<ObjectNode> cur =
                std::dynamic_pointer_cast<ObjectNode>(i);
            std::cout << "ID: "
                      << cur->GetFieldValueByName<std::string>(
                             ObjectNodeFileds::kID)
                      << std::endl;
            break;
        }
        case NodeTypes::kGroupNode: {
            std::cout << "Group" << std::endl;
            std::shared_ptr<GroupNode> cur =
                std::dynamic_pointer_cast<GroupNode>(i);
            std::cout << "ID: "
                      << cur->GetFieldValueByName<std::string>(
                             GroupNodeFileds::kID)
                      << std::endl;
            break;
        }
        case NodeTypes::kFaceNode: {
            std::cout << "Face" << std::endl;
            std::shared_ptr<FaceNode> cur =
                std::dynamic_pointer_cast<FaceNode>(i);
            std::cout << "ID: "
                      << cur->GetFieldValueByName<std::string>(
                             FaceNodeFields::kID)
                      << std::endl;
            std::cout << "IR color code: "
                      << cur->GetFieldValueByName<Int4>(
                             FaceNodeFields::kColorCode)
                      << std::endl;
            std::cout << "Color name index: "
                      << cur->GetFieldValueByName<UnsignedInt2>(
                             FaceNodeFields::kColorNameIndex)
                      << std::endl;
            std::cout << "Primary color index: "
                      << cur->GetFieldValueByName<UnsignedInt4>(
                             FaceNodeFields::kPrimaryColorIndex)
                      << std::endl;
            std::cout << "Material index: "
                      << cur->GetFieldValueByName<Int2>(
                             FaceNodeFields::kMaterialIndex)
                      << std::endl;
            break;
        }
        case NodeTypes::kLongIDNode: {
            std::cout << "Long ID" << std::endl;
            std::shared_ptr<LongIDNode> cur =
                std::dynamic_pointer_cast<LongIDNode>(i);
            std::cout << "String: "
                      << cur->GetFieldValueByName<std::string>(
                             LongIDNodeFields::kString)
                      << std::endl;
            break;
        }
        }
        std::cout << "---------------------------------------------------------"
                     "-----------------------"
                  << std::endl;
    }

    // std::cout << "ID: " <<
    // headertest->GetFieldValueByName<std::string>(HeaderNodeFields::kID)<<std::endl;
    return 0;
}
