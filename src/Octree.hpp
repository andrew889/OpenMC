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

struct Node : public NodeBase
{
	NodeBase* parent;
	NodeBase* children[DIR_MAX];
};

struct Leaf : public NodeBase
{
	// TODO: Change this to the actual block type info when that type is created
	void* block_info;
	unsigned short block_id;
	unsigned char light_level;
	unsigned char damage;
};

} // namespace octree

#endif // OPENMC_OCTREE_HPP
