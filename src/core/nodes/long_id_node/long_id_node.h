#ifndef INDEXED_STRING_NODE
#define INDEXED_STRING_NODE

namespace node {

enum class LongIDNodeFields { kOpcode, kLength, kIndex, kString };

const std::map<LongIDNodeFields, std::pair<int, int>> long_id_node_byte_map = {
    {LongIDNodeFields::kOpcode, {0, 2}},
    {LongIDNodeFields::kLength, {2, 2}},
    {LongIDNodeFields::kString, {4, -1}}};

class LongIDNode : public IAncillaryNode {
  public:
    LongIDNode()
        : byte_map(long_id_node_byte_map), node_type_(NodeTypes::kLongIDNode){};
    LongIDNode(std::shared_ptr<std::byte[]> byte_presentation)
        : byte_map(long_id_node_byte_map), node_type_(NodeTypes::kLongIDNode) {
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
    ReturnType GetFieldValueByName(LongIDNodeFields name) {
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
        return std::make_shared<LongIDNode>(bytes);
    }

  private:
    std::shared_ptr<std::byte[]> byte_presentation_;
    std::map<LongIDNodeFields, bool> cached_fields_;
    std::map<LongIDNodeFields, std::pair<int, int>> byte_map;
    NodeTypes node_type_;
};

template <> std::string LongIDNode::GetFieldValueByName(LongIDNodeFields name) {
    assert(byte_map.find(name) != byte_map.end());
    int offset = byte_map.at(name).first;
    const char *const str_bytes =
        reinterpret_cast<const char *const>(byte_presentation_.get() + offset);
    return std::string(str_bytes);
}

} // namespace node

#endif