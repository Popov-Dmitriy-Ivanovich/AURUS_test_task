#ifndef NODE_FACTORY
#define NODE_FACTORY

#include <functional>

#include "src/core/nodes/header_node/header_node.h"
#include "src/core/nodes/face_node/face_node.h"
#include "src/core/nodes/group_node/group_node.h"
#include "src/core/nodes/inode.h"
#include "src/core/nodes/long_id_node/long_id_node.h"
#include "src/core/nodes/object_node/object_node.h"
#include "src/types/type_defenitions.h"

namespace node {
using types::Int2;

const std::map<
    int, std::function<std::shared_ptr<INode>(std::shared_ptr<std::byte[]>)>>
    opcode_to_method_bindings = {{1, HeaderNode::CreateFromBytes},
                                 {2, GroupNode::CreateFromBytes},
                                 {4, ObjectNode::CreateFromBytes},
                                 {5, FaceNode::CreateFromBytes},
                                 {33, LongIDNode::CreateFromBytes}};

class NodesFactory {
  public:
    NodesFactory() : opcode_to_method_bindings_(opcode_to_method_bindings){};
    NodesFactory(std::map<int, std::function<std::shared_ptr<INode>(
                                   std::shared_ptr<std::byte[]>)>>
                     opcode_to_method_bindings)
        : opcode_to_method_bindings_(opcode_to_method_bindings) {}
    std::vector<std::shared_ptr<INode>>
    CreateNodeVecFromBytes(std::vector<std::shared_ptr<std::byte[]>> bytes) {
        std::vector<std::shared_ptr<INode>> res;
        for (auto const &byte_pres : bytes) {
            auto node = CreateNodeFromBytes(byte_pres);
            if (node->GetNodeType() != NodeTypes::kUndefinedNode) {
                res.push_back(node);
            }
        }
        return res;
    }

  private:
    std::shared_ptr<INode>
    CreateNodeFromBytes(std::shared_ptr<std::byte[]> bytes) {
        assert(bytes);
        Int2 msb = 0, lsb = 0;
        msb = *reinterpret_cast<Int2 *>(bytes.get());
        lsb = *reinterpret_cast<Int2 *>(bytes.get() + 1);
        Int2 opcode = (msb & 0xFF) << 8 | lsb & 0xFF;
        if (opcode_to_method_bindings_.find(opcode) ==
            opcode_to_method_bindings_.end()) {
            return std::make_shared<node::UndefinedNode>();
        }
        return opcode_to_method_bindings_.at(opcode)(bytes);
    }
    std::map<int, std::function<std::shared_ptr<INode>(
                      std::shared_ptr<std::byte[]>)>>
        opcode_to_method_bindings_;
};

} // namespace node

#endif