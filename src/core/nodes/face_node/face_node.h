#ifndef FACE_NODE
#define FACE_NODE

#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"

namespace node {

enum class FaceNodeFileds { kOpcode, kLength, kID};

const std::map<FaceNodeFileds, std::pair<int, int>> face_node_byte_map = {
    {FaceNodeFileds::kOpcode, {0, 2}},
    {FaceNodeFileds::kLength, {2, 2}},
    {FaceNodeFileds::kID, {4, 8}}};



class FaceNode : public INode {
  public:
    FaceNode()
        : byte_map(face_node_byte_map), node_type_(NodeTypes::kFaceNode){};
    FaceNode(std::shared_ptr<std::byte[]> byte_presentation)
        : byte_map(face_node_byte_map), node_type_(NodeTypes::kFaceNode) {
        SetBytePresentation(byte_presentation);
    }
    void SetBytePresentation(
        std::shared_ptr<std::byte[]> byte_presentation) override {
        byte_presentation_ = std::move(byte_presentation);
        for (auto &[field_name, value] : cached_fields_) {
            value = false;
        }
    }
    template <typename ReturnType>
    ReturnType GetFieldValueByName(FaceNodeFileds name) {
        assert(byte_map.find(name) != byte_map.end());
        int offset = byte_map.at(name).first;
        int length = byte_map.at(name).second;
        ReturnType res_value =
            ConvertBytes<ReturnType>(byte_presentation_, offset, length);
        return res_value;
    }
    

    NodeTypes GetNodeType() override { return node_type_; }
    static std::shared_ptr<INode> CreateFromBytes(std::shared_ptr<std::byte[]> bytes){
        return std::make_shared<FaceNode>(bytes);
    }
  private:
    std::shared_ptr<std::byte[]> byte_presentation_;
    std::map<FaceNodeFileds, bool> cached_fields_;
    std::map<FaceNodeFileds, std::pair<int, int>> byte_map;
    NodeTypes node_type_;
};

template <>
std::string FaceNode::GetFieldValueByName(FaceNodeFileds name){
    assert(byte_map.find(name) != byte_map.end());
    int offset = byte_map.at(name).first;
    const char * const str_bytes = reinterpret_cast<const char * const>(byte_presentation_.get()+offset);
    return std::string(str_bytes);
}

} // namespace node

#endif