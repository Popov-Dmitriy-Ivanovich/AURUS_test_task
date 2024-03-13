#include <cassert>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "src/core/file_reader.h"
#include "src/core/nodes/header_node/header_node.h"
#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"
#include "src/errors/file_io_errors.h"
#include "src/types/type_defenitions.h"

#include "src/core/nodes/group_node/group_node.h"
#include "src/core/nodes/object_node/object_node.h"
#include "src/core/nodes/face_node/face_node.h"

// enum class NodeTypes { kHeaderNode };

using file_reader::FileReader;
using node::HeaderNode;
using node::HeaderNodeFields;
using namespace node;
using node::INode;
using types::Int2;

const std::map<
    int, std::function<std::shared_ptr<INode>(std::shared_ptr<std::byte[]>)>>
    opcode_to_method_bindings = {
        {1, HeaderNode::CreateFromBytes},
        {2, GroupNode::CreateFromBytes},
        {4, ObjectNode::CreateFromBytes},
        {5, FaceNode::CreateFromBytes}
    };

class NodesFactory {
  public:
    NodesFactory():opcode_to_method_bindings_(opcode_to_method_bindings){};
    NodesFactory(std::map<int, std::function<std::shared_ptr<INode>(
                                   std::shared_ptr<std::byte[]>)>>
                     opcode_to_method_bindings)
        : opcode_to_method_bindings_(opcode_to_method_bindings) {}
    std::vector<std::shared_ptr<INode>>
    CreateNodeVecFromBytes(std::vector<std::shared_ptr<std::byte[]>> bytes) {
        std::vector<std::shared_ptr<INode>> res;
        for(auto const & byte_pres : bytes){
            auto node = CreateNodeFromBytes(byte_pres);
            if(node->GetNodeType()!=NodeTypes::kUndefinedNode){
                res.push_back(node);
            }
        }
        return res;
    }

  private:
    std::shared_ptr<INode> CreateNodeFromBytes(std::shared_ptr<std::byte[]> bytes) {
        assert(bytes);
        Int2 msb=0,lsb=0;
        msb = *reinterpret_cast<Int2*>(bytes.get());
        lsb = *reinterpret_cast<Int2*>(bytes.get()+1);
        Int2 opcode = (msb&0xFF)<<8 | lsb&0xFF;
        if(opcode_to_method_bindings_.find(opcode) == opcode_to_method_bindings_.end()){
            return std::make_shared<node::UndefinedNode>();
        }
        return opcode_to_method_bindings_.at(opcode)(bytes);
    }
    std::map<int, std::function<std::shared_ptr<INode>(
                      std::shared_ptr<std::byte[]>)>>
        opcode_to_method_bindings_;
};

int main() {
    FileReader test("../docs/Model_3_ver164.flt");
    test.Read();
    auto res = test.GetByteNodes();
    std::cout << res.size() << " " << res[0] << "\n";
    //HeaderNode headertest(res[0]);
    NodesFactory testfac;
    auto res_vec = testfac.CreateNodeVecFromBytes(res);
    for(auto const & i : res_vec){
        std::cout<<"--------------------------------------------------------------------------------"<<std::endl;
        switch(i->GetNodeType()){
            case NodeTypes::kHeaderNode : {                
                std::cout<<"Header"<<std::endl;
                std::shared_ptr<HeaderNode> cur = std::dynamic_pointer_cast<HeaderNode>(i);
                std::cout<<"ID: " << cur->GetFieldValueByName<std::string>(HeaderNodeFields::kID)<<std::endl;                
                break;
            }
            case NodeTypes::kObjectNode:{
                std::cout<<"Object"<<std::endl;
                std::shared_ptr<ObjectNode> cur = std::dynamic_pointer_cast<ObjectNode>(i);
                std::cout<<"ID: " << cur->GetFieldValueByName<std::string>(ObjectNodeFileds::kID)<<std::endl;
                break;
            }
            case NodeTypes::kGroupNode:{
                std::cout<<"Group"<<std::endl;
                std::shared_ptr<GroupNode> cur = std::dynamic_pointer_cast<GroupNode>(i);
                std::cout<<"ID: " << cur->GetFieldValueByName<std::string>(GroupNodeFileds::kID)<<std::endl;
                break;
            }
            case NodeTypes::kFaceNode:{
                std::cout<<"Face"<<std::endl;
                std::shared_ptr<FaceNode> cur = std::dynamic_pointer_cast<FaceNode>(i);
                std::cout<<"ID: " << cur->GetFieldValueByName<std::string>(FaceNodeFileds::kID)<<std::endl;
                break;
            }
        }
        std::cout<<"--------------------------------------------------------------------------------"<<std::endl;
    }
    
    
    //std::cout << "ID: " << headertest->GetFieldValueByName<std::string>(HeaderNodeFields::kID)<<std::endl;
    return 0;
}
