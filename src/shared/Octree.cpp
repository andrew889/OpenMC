#include "Octree.hpp"

namespace octree {

std::pair<InnerNode*, int> SparseNode::allocateSpaceForSplitting(NodeBase** root_ptr)
{
	if (parent == 0) {
		// Case 1: SparseNode is the root of the octree. The initial InnerNode needs to be allocated.
		octree::InnerNode* new_node = new octree::InnerNode;
		new_node->parent = 0;
		*root_ptr = new_node;
		return std::make_pair(new_node, 1);
	} else {
		if (parent->isLeaf(parent_i)) {
			// Case 2: SparseNode is a leaf in an InnerNode. A new InnerNode needs to be allocated.
			octree::InnerNode* new_node = new octree::InnerNode;
			new_node->parent = parent;
			new_node->parent_i = parent_i;
			parent->children[parent_i] = new_node;
			return std::make_pair(new_node, 1);
		} else {
			// Case 3: SparseNode is not a leaf in an InnerNode. No extra space needs to be allocated.
			parent->children[parent_i] = 0;
			return std::make_pair(parent, parent->getChild(parent_i, 0));
		}
	}
}

} // namespace octree
