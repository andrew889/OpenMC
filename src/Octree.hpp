#ifndef OPENMC_OCTREE_HPP
#define OPENMC_OCTREE_HPP

namespace octree
{

enum Dir
{
	DIR_NWT, DIR_NET, DIR_SWT, DIR_SET,
	DIR_NWB, DIR_NEB, DIR_SWB, DIR_SEB,
	DIR_MAX
};

struct NodeBase
{
};

struct InnerNode : public NodeBase
{
	// children[0] is the parent
	NodeBase* children[4097]; // 8**4+1

	inline int getChild(int i, int child)
	{
		return 8*i + child;
	}

	inline int getParent(int i)
	{
		return i/8;
	}
};

// Will call the terrain generator and be converted into regular InnerNodes/BrickLeafs
struct SparseNode : public NodeBase
{
	// TODO: Change this to the actual terrain gen info type when that type is created
	void* terrain_gen_info;
};

struct BlockLeaf : public NodeBase
{
	// TODO: Change this to the actual block type info when that type is created
	void* block_info;
	unsigned short block_id;
	unsigned char light_level;
	unsigned char damage;
};

} // namespace octree

#endif // OPENMC_OCTREE_HPP
