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
#include "src/errors/file_io_errors.h"
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

struct NodeInfo{
    std::string Type="";
    std::string Name="";
    bool HasIndices=false;
    int PrimaryColorIndex=-1;
    int ColorNameIndex=-1;
    int MaterialIndex=-1;
};

void print_info (std::vector<std::shared_ptr<INode>> nodes ){
    auto iter = nodes.begin();
    while (iter != nodes.end()){
        
        NodeInfo cur_info;
        switch((*iter)->GetNodeType()){
            case NodeTypes::kGroupNode:{
                cur_info.Type = "Group";
                cur_info.Name = std::dynamic_pointer_cast<GroupNode>(*iter)->GetFieldValueByName<std::string>(GroupNodeFields::kID);
                break;
            }
            case NodeTypes::kObjectNode:{
                cur_info.Type = "Object";
                cur_info.Name = std::dynamic_pointer_cast<ObjectNode>(*iter)->GetFieldValueByName<std::string>(ObjectNodeFields::kID);
                break;
            }
            case NodeTypes::kFaceNode:{
                cur_info.Type = "Face";
                cur_info.HasIndices = true;
                cur_info.Name = std::dynamic_pointer_cast<FaceNode>(*iter)->GetFieldValueByName<std::string>(FaceNodeFields::kID);
                cur_info.PrimaryColorIndex = std::dynamic_pointer_cast<FaceNode>(*iter)->GetFieldValueByName<Int4>(FaceNodeFields::kPrimaryColorIndex);
                cur_info.ColorNameIndex = std::dynamic_pointer_cast<FaceNode>(*iter)->GetFieldValueByName<Int2>(FaceNodeFields::kColorNameIndex);
                cur_info.MaterialIndex = std::dynamic_pointer_cast<FaceNode>(*iter)->GetFieldValueByName<Int2>(FaceNodeFields::kMaterialIndex);
                break;
            }
            case NodeTypes::kHeaderNode: {
                cur_info.Type = "Header";
                cur_info.Name = std::dynamic_pointer_cast<HeaderNode>(*iter)->GetFieldValueByName<std::string>(HeaderNodeFields::kID);
                break;
            }
        }
        iter++;
        while(iter!=nodes.end() && !(*iter)->IsPrimary()){
            if((*iter)->GetNodeType() == NodeTypes::kLongIDNode){
                cur_info.Name = std::dynamic_pointer_cast<LongIDNode>(*iter)->GetFieldValueByName<std::string>(LongIDNodeFields::kString);
            }
            
            iter++;
        }

        std::cout<<"----------------------------------------------------------------------------------------------------"<<std::endl;
        std::cout<<cur_info.Type<<" with ID = "<<cur_info.Name<<std::endl;
        if (cur_info.HasIndices){
            std::cout<<"Primary Color Index: "<<cur_info.PrimaryColorIndex<<std::endl;
            std::cout<<"Color Name Index: "<< cur_info.ColorNameIndex<<std::endl;
            std::cout<<"Material Index: "<< cur_info.MaterialIndex<<std::endl;
        }
        std::cout<<"----------------------------------------------------------------------------------------------------"<<std::endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc!=2){
        std::cout<<"Wrond count of arguments passed";
    }
    std::string file_path(argv[1]);
    std::cout<<"Attempting to Read file "+file_path<<std::endl;
    std::vector<std::shared_ptr<std::byte[]>> structures_bytes;
    try{
        FileReader file(file_path);
        file.Read();
        structures_bytes = file.GetByteNodes();
    }
    catch(errors::ErrorOpeningFile ex){
        std::cout<<ex.what()<<std::endl;
        return 1;
    }
    catch(errors::ErrorReadingFile ex){
        std::cout<<ex.what()<<std::endl;
        return 1;
    }
    catch(std::exception ex){
        std::cout<<"Something bad happened while reading file!"<<std::endl;
        return 1;
    }
    std::cout<<"Readed sucessfully"<<std::endl;
    NodesFactory nodes_creator;
    auto nodes = nodes_creator.CreateNodeVecFromBytes(structures_bytes);
    print_info(nodes);
    return 0;
}
