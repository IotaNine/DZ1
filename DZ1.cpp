//#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define BLOCK_SIZE 16
#define BLOCK_SHIFT 5
// Направление сдвига влево

struct Block
{
    uint8_t Data[BLOCK_SIZE];
};

void TechFunc(std::string name)  // При необходимости дозаписывает в текст инормацию, чтобы кол-во байт было кратно BLOCK_SIZE
{
    std::fstream file(name, std::ios::app);
    file.seekg(0, file.end);
    long long bytes = file.tellg();
    file.seekg(0, file.beg);
    if (bytes % BLOCK_SIZE != 0)
    {
        int delta = BLOCK_SIZE - (bytes % BLOCK_SIZE);
        std::string tmp = std::to_string(delta);
        if (delta > 9) delta--;
        for (int i = 1; i < delta; i++)
        {
            tmp += 'X';
        }
        file << tmp;
    }
}

void ReadFromFile(std::string name, std::vector<Block>& blocks)
{
    TechFunc(name);
    std::ifstream file(name, std::ios::binary);
    Block buffer;
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&buffer), BLOCK_SIZE);
        blocks.push_back(buffer);
    }
    blocks.pop_back();
}

void WriteToFile(std::string name, std::vector<Block>& blocks)
{
    std::ofstream file(name, std::ios::binary);
    for (Block& buffer : blocks)
    {
        file.write(reinterpret_cast<char*>(&buffer), BLOCK_SIZE);
    }
}

void Gamming(std::vector<Block>& blocks, int& seed, bool dir)
{
    for (Block& bl : blocks)
    {
        srand(seed);
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            bl.Data[i] = bl.Data[i] ^ rand();
        }
        if (dir)
        {
            seed++;
        }
        else
        {
            seed--;
        }
    }
}

void Shifting(std::vector<Block>& blocks, bool dir)
{

}

//class TextObj
//{
//public:
//    std::vector<Block> Blocks;
//    uint8_t Gamma[BLOCK_SIZE];
//    TextObj();
//    uint8_t& operator^(std::vector<Block>& blocks);
//};

int main()
{
    std::vector<Block> Blocks;
    ReadFromFile("Original", Blocks);
    WriteToFile("Changed", Blocks);
    std::cout << "Hello World!\n";
}