#ifndef OPENMC_TERRAINGEN_HPP
#define OPENMC_TERRAINGEN_HPP

namespace Octree {
	struct NodeBase;
	struct SparseNode;
}

class TerrainGen
{
public:
	// output is an 8-sized array
	void splitSparseNode(octree::SparseNode* s_node, octree::NodeBase* output);
	octree::NodeBase* generateBucket(octree::SparseNode* s_node);

private:
	int random_seed;
};

#endif // OPENMC_TERRAINGEN_HPP