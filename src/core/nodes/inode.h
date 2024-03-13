#ifndef INODE
#define INODE

#include <memory>

#include "src/core/nodes/node_types.h"

namespace node {

class INode {
  public:
    virtual void
    SetBytePresentation(std::shared_ptr<std::byte[]> byte_presentation) = 0;
    virtual NodeTypes GetNodeType() = 0;
    virtual bool IsPrimary() = 0;
    virtual bool IsControl() = 0;

  protected:
    template <typename RetType>
    RetType ConvertBytes(std::shared_ptr<std::byte[]> bytes, int offset,
                         int length) {
        assert(bytes);
        assert(sizeof(RetType) == length);
        RetType byte_buf = *reinterpret_cast<RetType *>(bytes.get() + offset);
        RetType res_value = 0;
        for (int i = 0; i < length; i++) {
            res_value = res_value | ((byte_buf >> (8 * i)) & 0xFF)
                                        << (8 * (length - 1 - i));
        }
        return res_value;
    }
};

class IPrimaryNode : public INode {
    bool IsPrimary() override { return true; }

    bool IsControl() override { return false; }
};

class IAncillaryNode : public INode {
    bool IsPrimary() override { return false; }
    bool IsControl() override { return false; }
};

class IControlNode : public INode {
    bool IsPrimary() override { return false; }
    bool IsControl() override { return true; }
};

class UndefinedNode : public INode {
  public:
    void SetBytePresentation(
        std::shared_ptr<std::byte[]> byte_presentation) override {}
    NodeTypes GetNodeType() override { return NodeTypes::kUndefinedNode; }
    bool IsPrimary() override { return false; }

    bool IsControl() override { return false; }
};

} // namespace node

#endif