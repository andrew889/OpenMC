#ifndef OPENMC_CHUNK_HPP
#define OPENMC_CHUNK_HPP

namespace octree {
	struct NodeBase;
}

class Chunk
{
public:
	octree::NodeBase* octree;
};

#endif // OPENMC_CHUNK_HPP
