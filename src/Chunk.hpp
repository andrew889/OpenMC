#ifndef OPENMC_CHUNK_HPP
#define OPENMC_CHUNK_HPP

#include "math/CompilerMath.hpp"
#include "Octree.hpp"

namespace octree {
	struct NodeBase;
}

class Chunk
{
public:
	static const int CHUNK_SIZE = 512;
	static const int TREE_DEPTH = clog<2, CHUNK_SIZE / octree::BucketLeaf::BUCKET_SIZE>::x;

	octree::NodeBase* octree;
};

#endif // OPENMC_CHUNK_HPP
