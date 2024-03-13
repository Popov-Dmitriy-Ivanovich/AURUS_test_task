#ifndef GROUP_NODE
#define GROUP_NODE

#include "src/core/nodes/inode.h"
#include "src/core/nodes/node_types.h"

namespace node {

enum class GroupNodeFields { kOpcode, kLength, kID };

const std::map<GroupNodeFields, std::pair<int, int>> group_node_byte_map = {
    {GroupNodeFields::kOpcode, {0, 2}},
    {GroupNodeFields::kLength, {2, 2}},
    {GroupNodeFields::kID, {4, 8}}};

class GroupNode : public IPrimaryNode {
  public:
    GroupNode()
        : byte_map(group_node_byte_map), node_type_(NodeTypes::kGroupNode){};
    GroupNode(std::shared_ptr<std::byte[]> byte_presentation)
        : byte_map(group_node_byte_map), node_type_(NodeTypes::kGroupNode) {
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
    ReturnType GetFieldValueByName(GroupNodeFields name) {
        assert(byte_map.find(name) != byte_map.end());
        int offset = byte_map.at(name).first;
        int length = byte_map.at(name).second;
        ReturnType res_value =
            ConvertBytes<ReturnType>(byte_presentation_, offset, length);
        return res_value;
    }

    NodeTypes GetNodeType() override { return node_type_; }
    static std::shared_ptr<INode>
    CreateFromBytes(std::shared_ptr<std::byte[]> bytes) {
        return std::make_shared<GroupNode>(bytes);
    }

  private:
    std::shared_ptr<std::byte[]> byte_presentation_;
    std::map<GroupNodeFields, bool> cached_fields_;
    std::map<GroupNodeFields, std::pair<int, int>> byte_map;
    NodeTypes node_type_;
};

template <> std::string GroupNode::GetFieldValueByName(GroupNodeFields name) {
    assert(byte_map.find(name) != byte_map.end());
    int offset = byte_map.at(name).first;
    const char *const str_bytes =
        reinterpret_cast<const char *const>(byte_presentation_.get() + offset);
    return std::string(str_bytes);
}

} // namespace node

#endif