#pragma once
#include "muppet.h"
#include <vector>
#include <unordered_map>

#define CHUNK_SIZE 64
#define WORLD_HEIGHT 50
#define SEED 3212
#define SCALE 0.05f

struct block
{
	bool isEmpty;
	int8_t x;
	int8_t y;
	int8_t z;
};


class chunk
{
public:
	int xPos;
	int zPos;
	std::weak_ptr<Muppet::Object> mesh;
	std::vector<std::vector<std::vector<block>>> blocks;
	bool isEmpty(int x, int y, int z);
};

class SpatialHashTable
{
public:
	SpatialHashTable(int cellSize);
	void insert(int x, int y, chunk value);
	chunk& query(float x, float y);
private:
	float cellSize;
	std::unordered_map<int, chunk> table;
	int getHash(float x, float y);
};




class world
{
public:
	world();
	std::vector<chunk> chunks;
	SpatialHashTable chunkData;
};

class Game
{
public:
	Game();

	
	world save;

	chunk GenerateChunk(const int xPos, const int zPos);

	void GenerateMesh(chunk& chunk);

	void GenWorld(int width);

	void ManageChunks(int renderDistance, glm::vec3 p_pos);
	
	bool IsChunkNear(int x, int y);

};