#include "TerrainGen.hpp"
#include "Octree.hpp"

#include <algorithm>

void TerrainGen::splitSparseNode(octree::SparseNode* s_node, const std::pair<octree::InnerNode*, int>& output)
{
	octree::NodeBase** out = &output.first->children[output.second];

	for (int i = 0; i < 8; ++i) {
		out[i] = new octree::SparseNode;
		out[i]->parent = output.first;
		out[i]->parent_i = output.second + i;

		// TODO
	};
}

bool TerrainGen::isEmpty(octree::SparseNode* s_node, const math::Box& area)
{
	// TODO PI should be moved to some more global place
	static const float PI = float(std::atan(1.)*4.);
	static const int BUCKET_SIZE = octree::BucketLeaf::BUCKET_SIZE;

	// TODO
	
	if (area.origin[1] >= 128.f + 64.f)
		return true;
	if (area.origin[1] <= 128.f - 64.f)
		return false;

	int upx = int((area.origin + area.size)[0] + .5f);
	int upz = int((area.origin + area.size)[2] + .5f);
	for (int x = int(area.origin[0]); x < upx; ++x) {
		for (int z = int(area.origin[2]); z < upz; ++z) {
			float value1 = std::sin(x / 512.f * 2.f*PI);
			float value2 = std::cos(z / 512.f * 2.f*PI);
			float value3 = value1 * value2 * 64.f + 128.f;
			if (value3 > area.origin[1])
				return false;
		}
	}

	return true;
}

octree::NodeBase* TerrainGen::generateBucket(octree::SparseNode* s_node, const math::Box& area)
{
	static const int BUCKET_SIZE = octree::BucketLeaf::BUCKET_SIZE;

	octree::BucketLeaf* bucket = new octree::BucketLeaf;

	// TODO
	octree::Block block;
	block.block_id = 0;
	block.block_info = 0;
	block.damage = 0;
	block.light_level = 255;
	std::fill_n(bucket->blocks, BUCKET_SIZE*BUCKET_SIZE*BUCKET_SIZE, block);

	return bucket;
}