#ifndef OPENMC_OCTREE_HPP
#define OPENMC_OCTREE_HPP

#include "math/CompilerMath.hpp"

#include <utility>

namespace octree {

enum Dir
{
	DIR_NWT, DIR_NET, DIR_SWT, DIR_SET,
	DIR_NWB, DIR_NEB, DIR_SWB, DIR_SEB,
	DIR_MAX
};

struct InnerNode;
struct SparseNode;
struct EmptyLeaf;
struct BucketLeaf;

class OctreeNodeVisitor
{
public:
	virtual void visit(InnerNode&) = 0;
	virtual void visit(SparseNode&) = 0;
	virtual void visit(EmptyLeaf&) = 0;
	virtual void visit(BucketLeaf&) = 0;
};

struct NodeBase
{
	virtual void accept(OctreeNodeVisitor& v) = 0;

	InnerNode* parent;
	short parent_i;
};

struct InnerNode : public NodeBase
{
	static const int DEPTH = 2;
	static const int ARRAY_SIZE = csum_powers<8, DEPTH>::x;

	inline int getChild(int i, int child)
	{
		return (8 * i) + 1 + child;
	}

	inline int getParent(int i)
	{
		return (i - 1) / 8;
	}

	inline bool isRoot(int i)
	{
		return i == 0;
	}

	inline bool isLeaf(int i)
	{
		return i >= csum_powers<8, DEPTH - 1>::x;
	}

	virtual void accept(OctreeNodeVisitor& v)
	{
		v.visit(*this);
	}

	// children[0] is [strike]the parent[/strike] unused.
	// TODO: Fix the above functions so we can save a pointer.
	NodeBase* children[ARRAY_SIZE];
};

// Will call the terrain generator and be converted into regular InnerNodes/BrickLeafs
struct SparseNode : public NodeBase
{
	// std::pair<dest, dest_i> >
	std::pair<InnerNode*, int> allocateSpaceForSplitting(NodeBase** root_ptr);

	virtual void accept(OctreeNodeVisitor& v)
	{
		v.visit(*this);
	}

	// TODO: Change this to the actual terrain gen info type when that type is created
	void* terrain_gen_info;
};

struct EmptyLeaf : public NodeBase
{
	virtual void accept(OctreeNodeVisitor& v)
	{
		v.visit(*this);
	}
};

struct Block {
	// TODO: Change this to the actual block type info when that type is created
	void* block_info;
	unsigned short block_id;
	unsigned char light_level;
	unsigned char damage;
};

struct BucketLeaf : public NodeBase
{
	static const int BUCKET_SIZE = 8;

	virtual void accept(OctreeNodeVisitor& v)
	{
		v.visit(*this);
	}

	Block blocks[cpow<BUCKET_SIZE, 3>::x];
};

} // namespace octree

#endif // OPENMC_OCTREE_HPP
