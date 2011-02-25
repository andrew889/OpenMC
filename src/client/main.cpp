#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <algorithm>
#include <stack>
#include <utility>
#include <cassert>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <cstdlib>

#include "../Chunk.hpp"
#include "../Octree.hpp"
#include "../TerrainGen.hpp"
#include "gl/BufferObject.hpp"
#include "../math/Geometry.hpp"

math::vec3 getCorner(const math::Box& b, octree::Dir dir)
{
	switch (dir) {
	case octree::DIR_NWT: return b.origin + math::make_vec(0.f,       b.size[1], 0.f);
	case octree::DIR_NET: return b.origin + math::make_vec(b.size[0], b.size[1], 0.f);
	case octree::DIR_SWT: return b.origin + math::make_vec(0.f,       b.size[1], b.size[2]);
	case octree::DIR_SET: return b.origin + math::make_vec(b.size[1], b.size[1], b.size[2]);
	case octree::DIR_NWB: return b.origin;
	case octree::DIR_NEB: return b.origin + math::make_vec(b.size[0], 0.f, 0.f);
	case octree::DIR_SWB: return b.origin + math::make_vec(0.f,       0.f, b.size[2]);
	case octree::DIR_SEB: return b.origin + math::make_vec(b.size[1], 0.f, b.size[2]);
	default: assert(false); return math::vec3();
	}
}

math::Box splitArea(const math::Box& b, octree::Dir dir)
{
	return math::Box(getCorner(b, dir), b.center(), 0);
}

class RenderOctreeVisitor : public octree::OctreeNodeVisitor
{
public:
	void visit(octree::InnerNode&);
	void visit(octree::SparseNode&);
	void visit(octree::EmptyLeaf&);
	void visit(octree::BucketLeaf&);

	int depth;
	math::Box area;

	Chunk* chunk;
	TerrainGen* terrain_gen;
	gl::BufferObject* vbo;
	gl::BufferObject* ibo;
};

void render(RenderOctreeVisitor& visitor, octree::NodeBase* node, int depth, const math::Box& area)
{
	visitor.depth = depth;
	visitor.area = area;
	node->accept(visitor);
}

void RenderOctreeVisitor::visit(octree::InnerNode& n)
{
	// tuple<node_index, depth, area>
	typedef boost::tuple<int, int, math::Box> NodeIndex;

	std::stack<NodeIndex, std::vector<NodeIndex>> nodes;

	for (int i = 1; i < 8+1; ++i) {
		nodes.push(NodeIndex(i, 0, splitArea(area, octree::Dir(i-1))));
	}

	while (!nodes.empty()) {
		NodeIndex node_i = nodes.top();
		nodes.pop();
		octree::NodeBase* node = n.children[node_i.get<0>()];

		if (node == 0) {
			assert(node_i.get<1>() < octree::InnerNode::DEPTH);

			for (int i = 0; i < 8; ++i) {
				nodes.push(NodeIndex(n.getChild(node_i.get<0>(), i), node_i.get<1>() + 1, splitArea(node_i.get<2>(), octree::Dir(i))));
			}
		} else {
			render(*this, node, node_i.get<1>(), node_i.get<2>());
		}
	}
}

void RenderOctreeVisitor::visit(octree::SparseNode& node)
{
	glSecondaryColor3f(1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(area.origin[0], area.origin[1], area.origin[2]);
	glScalef(area.size[0], area.size[1], area.size[2]);
	glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_BYTE, 0);
	glPopMatrix();
}

void RenderOctreeVisitor::visit(octree::EmptyLeaf&)
{
	return;
	glSecondaryColor3f(0.f, 0.f, 1.f);
	glPushMatrix();
	glTranslatef(area.origin[0], area.origin[1], area.origin[2]);
	glScalef(area.size[0], area.size[1], area.size[2]);
	glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_BYTE, 0);
	glPopMatrix();
}

void RenderOctreeVisitor::visit(octree::BucketLeaf&)
{
	glSecondaryColor3f(0.f, 1.f, 0.f);
	glPushMatrix();
	glTranslatef(area.origin[0], area.origin[1], area.origin[2]);
	glScalef(area.size[0], area.size[1], area.size[2]);
	glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_BYTE, 0);
	glPopMatrix();
}




class UpdateOctreeVisitor : public octree::OctreeNodeVisitor
{
public:
	void visit(octree::InnerNode&);
	void visit(octree::SparseNode&);
	void visit(octree::EmptyLeaf&);
	void visit(octree::BucketLeaf&);

	int depth;
	math::Box area;
	Chunk* chunk;
	TerrainGen* terrain_gen;
};

void update(UpdateOctreeVisitor& visitor, octree::NodeBase* node, int depth, const math::Box& area)
{
	visitor.depth = depth;
	visitor.area = area;
	node->accept(visitor);
}

void UpdateOctreeVisitor::visit(octree::InnerNode& n)
{
	// tuple<node_index, depth, area>
	typedef boost::tuple<int, int, math::Box> NodeIndex;

	std::stack<NodeIndex, std::vector<NodeIndex>> nodes;

	for (int i = 1; i < 8+1; ++i) {
		nodes.push(NodeIndex(i, depth, splitArea(area, octree::Dir(i-1))));
	}

	while (!nodes.empty()) {
		NodeIndex node_i = nodes.top();
		nodes.pop();
		octree::NodeBase* node = n.children[node_i.get<0>()];

		if (node == 0) {
			assert(!n.isLeaf(node_i.get<0>()));

			for (int i = 0; i < 8; ++i) {
				nodes.push(NodeIndex(n.getChild(node_i.get<0>(), i), node_i.get<1>() + 1, splitArea(node_i.get<2>(), octree::Dir(i))));
			}
		} else {
			update(*this, node, node_i.get<1>() + 1, node_i.get<2>());
		}
	}
}

void UpdateOctreeVisitor::visit(octree::SparseNode& node)
{
	if (terrain_gen->isEmpty(&node, area)) {
		octree::EmptyLeaf* new_node = new octree::EmptyLeaf;

		new_node->parent = node.parent;
		new_node->parent_i = node.parent_i;
		node.parent->children[node.parent_i] = new_node;
	} else {
		if (depth == Chunk::TREE_DEPTH - 1) {
			octree::NodeBase* new_node = terrain_gen->generateBucket(&node, area);

			new_node->parent = node.parent;
			new_node->parent_i = node.parent_i;
			node.parent->children[node.parent_i] = new_node;
		} else {
			std::pair<octree::InnerNode*, int> dest = node.allocateSpaceForSplitting(&chunk->octree);

			// Split node into sub-SparseNodes
			terrain_gen->splitSparseNode(&node, dest);

			// Retry rendering(?)
		}
	}

	delete &node;
}

void UpdateOctreeVisitor::visit(octree::EmptyLeaf&)
{
	// Do nothing
}

void UpdateOctreeVisitor::visit(octree::BucketLeaf&)
{
	// Render
}


struct vertex_data {
	float x, y, z;
	unsigned char r, g, b;
};

int main(int argc, char** argv)
{
	Chunk chunk;
	chunk.octree = new octree::SparseNode;
	chunk.octree->parent = 0;

	TerrainGen terrain_gen;

	RenderOctreeVisitor visitor;
	visitor.chunk = &chunk;
	visitor.terrain_gen = &terrain_gen;

	sf::Window app(sf::VideoMode(800, 600, 32), "OpenMC", sf::Style::Default, sf::ContextSettings(24, 8, 8, 1, 5));

	glewInit();

	sf::Clock clock;

	glClearColor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_SUM);
	glDisable(GL_CULL_FACE);

	static const vertex_data vertices_data[8] = {
		{0.f, 0.f, 0.f, 255, 255, 255}, // 0
		{1.f, 0.f, 0.f, 64,   64,  64}, // 1
		{1.f, 1.f, 0.f, 255, 255, 255}, // 2
		{0.f, 1.f, 0.f, 64,   64,  64}, // 3
		{0.f, 0.f, 1.f, 255, 255, 255}, // 4
		{1.f, 0.f, 1.f, 64,   64,  64}, // 5
		{1.f, 1.f, 1.f, 255, 255, 255}, // 6
		{0.f, 1.f, 1.f, 64,   64,  64}  // 7
	};

	static const unsigned char vertices_index[6*2*3] = {
		3,0,1, 3,1,2,
		2,1,5, 2,5,6,
		6,5,4, 6,4,7,
		7,4,0, 7,0,3,
		0,4,5, 0,5,1,
		7,3,2, 7,2,6
	};

	gl::BufferObject vbo;
	visitor.vbo = &vbo;
	vbo.bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), static_cast<const void*>(vertices_data), GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, sizeof(vertex_data), (void*)offsetof(vertex_data, x));
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertex_data), (void*)offsetof(vertex_data, r));
	glEnableClientState(GL_COLOR_ARRAY);

	gl::BufferObject ibo;
	visitor.ibo = &ibo;
	ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_index), static_cast<const void*>(vertices_index), GL_STATIC_DRAW);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, 800.f/600.f, 128.f, 4024.f);

	bool run = true;
	while (run) {
		sf::Event ev;
		while (app.GetEvent(ev)) {
			if (ev.Type == sf::Event::Closed) {
				run = false;
			} else if (ev.Type == sf::Event::Resized) {
			    glViewport(0, 0, ev.Size.Width, ev.Size.Height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(70.f, ev.Size.Width/double(ev.Size.Height), 128.f, 4024.f);
			} else if (ev.Type == sf::Event::MouseButtonPressed) {
				UpdateOctreeVisitor uvisitor;
				uvisitor.chunk = &chunk;
				uvisitor.terrain_gen = &terrain_gen;
				std::cout << "DOOP!" << std::endl;
				update(uvisitor, chunk.octree, 0, math::Box(math::make_vec(0.f, 0.f, 0.f), math::make_vec(512.f, 512.f, 512.f)));
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -1024.f);
		glRotatef(clock.GetElapsedTime() * 20, 0.f, 1.f, 0.f);
		glTranslatef(-256.f, -256.f, -256.f);

		//glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
		//glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
		//glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);
		render(visitor, chunk.octree, 0, math::Box(math::make_vec(0.f, 0.f, 0.f), math::make_vec(512.f, 512.f, 512.f)));

		app.Display();
	}

	return 0;
}
