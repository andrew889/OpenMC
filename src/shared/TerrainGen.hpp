#ifndef OPENMC_TERRAINGEN_HPP
#define OPENMC_TERRAINGEN_HPP

#include <utility>

#include "math/Geometry.hpp"

namespace octree {
	struct NodeBase;
	struct SparseNode;
	struct InnerNode;
}

class TerrainGen
{
public:
	// output is an 8-sized array (of SparseNode or EmptyLeaf)
	void splitSparseNode(octree::SparseNode* s_node, const std::pair<octree::InnerNode*, int>& output);
	bool isEmpty(octree::SparseNode* s_node, const math::Box& area);
	// Can return either a BucketLeaf or an EmptyLeaf
	octree::NodeBase* generateBucket(octree::SparseNode* s_node, const math::Box& area);

private:
	int random_seed;
};

#endif // OPENMC_TERRAINGEN_HPP