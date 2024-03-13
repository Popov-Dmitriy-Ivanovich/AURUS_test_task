#ifndef HEADER_NODE
#define HEADER_NODE

#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"

#include <map>
#include <memory>
#include <utility>
#include <string>

namespace node {

enum class HeaderNodeFields { kOpcode, kLength, kID, kDateTime };

const std::map<HeaderNodeFields, std::pair<int, int>> head_node_byte_map = {
    {HeaderNodeFields::kOpcode, {0, 2}},
    {HeaderNodeFields::kLength, {2, 2}},
    {HeaderNodeFields::kID, {4, 8}},
    {HeaderNodeFields::kDateTime, {20, 32}}};



class HeaderNode : public IPrimaryNode {
  public:
    HeaderNode()
        : byte_map(head_node_byte_map), node_type_(NodeTypes::kHeaderNode){};
    HeaderNode(std::shared_ptr<std::byte[]> byte_presentation)
        : byte_map(head_node_byte_map), node_type_(NodeTypes::kHeaderNode) {
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
    ReturnType GetFieldValueByName(HeaderNodeFields name) {
        assert(byte_map.find(name) != byte_map.end());
        int offset = byte_map.at(name).first;
        int length = byte_map.at(name).second;
        ReturnType res_value =
            ConvertBytes<ReturnType>(byte_presentation_, offset, length);
        return res_value;
    }
    

    NodeTypes GetNodeType() override { return node_type_; }
    static std::shared_ptr<INode> CreateFromBytes(std::shared_ptr<std::byte[]> bytes){
        return std::make_shared<HeaderNode>(bytes);
    }
  private:
    std::shared_ptr<std::byte[]> byte_presentation_;
    std::map<HeaderNodeFields, bool> cached_fields_;
    std::map<HeaderNodeFields, std::pair<int, int>> byte_map;
    NodeTypes node_type_;
};

template <>
std::string HeaderNode::GetFieldValueByName(HeaderNodeFields name){
    assert(byte_map.find(name) != byte_map.end());
    int offset = byte_map.at(name).first;
    const char * const str_bytes = reinterpret_cast<const char * const>(byte_presentation_.get()+offset);
    return std::string(str_bytes);
}
} // namespace node

#endif