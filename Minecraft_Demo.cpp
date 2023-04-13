#include "Minecraft_Demo.h"
#include "muppet.h"
#include <vector>
#include <unordered_map>

struct Object
{
	float x, y;
	// additional object data
};

SpatialHashTable::SpatialHashTable(int cellSize) : cellSize(cellSize) {}

void SpatialHashTable::insert(int x, int y, chunk value)
{
	int hash = getHash(x, y);
	table[hash] = value;
}

chunk& SpatialHashTable::query(float x, float y)
{
	int hash = getHash(x, y);
	return table[hash];
}

int SpatialHashTable::getHash(float x, float y)
{
	int xCell = static_cast<int>(x / cellSize);
	int yCell = static_cast<int>(y / cellSize);
	// Use Cantor pairing function to combine xCell and yCell into a unique hash value
	return ((xCell + yCell) * (xCell + yCell + 1)) / 2 + yCell;
}


world::world() : chunkData(CHUNK_SIZE)
{

}

chunk Game::GenerateChunk(const int xPos,const int zPos)
{
	chunk ch;


	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		std::vector<std::vector<block>> v1;
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			std::vector<block> v2;

			for (int c = 0; c < CHUNK_SIZE; c++)
			{
				block f;
				f.isEmpty = true;
				v2.push_back(f);
			}
			v1.push_back(v2);
		}
		ch.blocks.push_back(v1);
	}
	
	
	block d = ch.blocks[12][12][12];

	ch.mesh = Muppet::Graphics::CreateEmptyObject();
	ch.xPos = xPos * CHUNK_SIZE;
	ch.zPos = zPos * CHUNK_SIZE;

	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		int x = i;
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			int z = j;
			int height = floor((Muppet::perlin((x + ch.xPos + SEED) * SCALE, (z + ch.zPos + SEED) * SCALE) / 2 + 0.50f) * WORLD_HEIGHT);
			for (int y = 0; y < height; y++)
			{
				block b;
				b.isEmpty = false;
				b.x = x;
				b.y = y;
				ch.blocks[x][y][z] = b;
			}
		}
	}
	return ch;
}

bool chunk::isEmpty(int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
		return blocks[x][y][z].isEmpty;
	else
		return false;
}

void Game::GenerateMesh(chunk& chunk)
{

	std::shared_ptr<Muppet::Object> mesh = chunk.mesh.lock();
	int index = 0;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (!chunk.blocks[x][y][z].isEmpty)
				{
					if (chunk.isEmpty(x-1, y, z))
					{
						mesh->AddVertex(glm::vec3(x, y, z));
						mesh->AddVertex(glm::vec3(x, y, z+1));
						mesh->AddVertex(glm::vec3(x, y+1, z));
						mesh->AddVertex(glm::vec3(x, y+1, z+1));
						
						index += 4;

						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}

					if (chunk.isEmpty(x+1, y, z))
					{
						mesh->AddVertex(glm::vec3(x+1, y, z));
						mesh->AddVertex(glm::vec3(x+1, y, z + 1));
						mesh->AddVertex(glm::vec3(x+1, y + 1, z));
						mesh->AddVertex(glm::vec3(x+1, y + 1, z + 1));
						index += 4;
						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}
					if (chunk.isEmpty(x, y-1, z))
					{
						mesh->AddVertex(glm::vec3(x, y, z));
						mesh->AddVertex(glm::vec3(x + 1, y, z));
						mesh->AddVertex(glm::vec3(x, y, z + 1));
						mesh->AddVertex(glm::vec3(x + 1, y, z + 1));
						index += 4;
						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}
					if (chunk.isEmpty(x, y+1, z))
					{
						mesh->AddVertex(glm::vec3(x, y + 1, z));
						mesh->AddVertex(glm::vec3(x + 1, y + 1, z));
						mesh->AddVertex(glm::vec3(x, y + 1, z + 1));
						mesh->AddVertex(glm::vec3(x + 1, y + 1, z + 1));
						index += 4;
						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}
					if (chunk.isEmpty(x, y, z-1))
					{
						mesh->AddVertex(glm::vec3(x, y, z));
						mesh->AddVertex(glm::vec3(x+1, y, z));
						mesh->AddVertex(glm::vec3(x, y+1, z));
						mesh->AddVertex(glm::vec3(x + 1, y+1, z));
						index += 4;
						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}
					if (chunk.isEmpty(x, y, z+1))
					{
						mesh->AddVertex(glm::vec3(x, y, z + 1));
						mesh->AddVertex(glm::vec3(x + 1, y, z + 1));
						mesh->AddVertex(glm::vec3(x, y + 1, z + 1));
						mesh->AddVertex(glm::vec3(x + 1, y + 1, z + 1));
						index += 4;
						mesh->m_indices.push_back(index - 4);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 2);
						mesh->m_indices.push_back(index - 3);
						mesh->m_indices.push_back(index - 1);
						mesh->m_indices.push_back(index - 2);
					}
				}
			}
		}
	}
	mesh->FillColor(glm::vec3(rand() % 255, rand() % 255, rand() % 255));
	mesh->m_transform.m_position = glm::vec3(chunk.xPos, 0, chunk.zPos);
	mesh->m_transform.m_rotation = glm::vec3(0,0,0);
	mesh->m_transform.m_scale = glm::vec3(1,1,1);
	mesh->UpdateMatrix();
	mesh->GenBuffers();
	mesh->UpdateColorBuffer();
	mesh->UpdateVertexBuffer();
}

void Game::GenWorld(int width)
{
	for (int x = 0 - width / 2; x < width / 2; x++)
	{
		for (int z = 0 - width / 2; z < width / 2; z++)
		{
			chunk c = GenerateChunk(x,z);
			save.chunks.push_back(c);
		}
	}

	for (int i = 0; i < save.chunks.size(); i++)
	{
		GenerateMesh(save.chunks[i]);
		save.chunks[i].blocks.clear();
	}
}

	Game::Game()
	{

	}

bool Game::IsChunkNear(int x, int z)
{
	for (auto c : save.chunks)
		if (c.xPos == x && c.zPos == z)
			return true;
	return false;
}


void Game::ManageChunks(int renderDistance, glm::vec3 p_pos)
{

	
}

