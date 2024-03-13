#ifndef OBJECT_NODE
#define OBJECT_NODE

#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"

namespace node {

enum class ObjectNodeFields { kOpcode, kLength, kID};

const std::map<ObjectNodeFields, std::pair<int, int>> object_node_byte_map = {
    {ObjectNodeFields::kOpcode, {0, 2}},
    {ObjectNodeFields::kLength, {2, 2}},
    {ObjectNodeFields::kID, {4, 8}}};



class ObjectNode : public IPrimaryNode {
  public:
    ObjectNode()
        : byte_map(object_node_byte_map), node_type_(NodeTypes::kObjectNode){};
    ObjectNode(std::shared_ptr<std::byte[]> byte_presentation)
        : byte_map(object_node_byte_map), node_type_(NodeTypes::kObjectNode) {
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
    ReturnType GetFieldValueByName(ObjectNodeFields name) {
        assert(byte_map.find(name) != byte_map.end());
        int offset = byte_map.at(name).first;
        int length = byte_map.at(name).second;
        ReturnType res_value =
            ConvertBytes<ReturnType>(byte_presentation_, offset, length);
        return res_value;
    }
    

    NodeTypes GetNodeType() override { return node_type_; }
    static std::shared_ptr<INode> CreateFromBytes(std::shared_ptr<std::byte[]> bytes){
        return std::make_shared<ObjectNode>(bytes);
    }
  private:
    std::shared_ptr<std::byte[]> byte_presentation_;
    std::map<ObjectNodeFields, bool> cached_fields_;
    std::map<ObjectNodeFields, std::pair<int, int>> byte_map;
    NodeTypes node_type_;
};

template <>
std::string ObjectNode::GetFieldValueByName(ObjectNodeFields name){
    assert(byte_map.find(name) != byte_map.end());
    int offset = byte_map.at(name).first;
    const char * const str_bytes = reinterpret_cast<const char * const>(byte_presentation_.get()+offset);
    return std::string(str_bytes);
}

} // namespace node

#endif