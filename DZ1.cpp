#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define BLOCK_SIZE 16
#define BLOCK_SHIFT 5
//Shift direction: Left

using std::cin;
using std::cout;
using std::endl;

struct Block
{
    uint8_t Data[BLOCK_SIZE];
};

unsigned long long TextSize(std::string& name)
{
    std::fstream file(name);
    file.seekg(0, file.end);
    return file.tellg();
}

unsigned int LastBlockSize(std::string& name)   
{
    if (TextSize(name) % BLOCK_SIZE != 0)
    {
        return TextSize(name) % BLOCK_SIZE;
    }
    return BLOCK_SIZE;
}

void ReadFromFile(std::string& name, std::vector<Block>& blocks)
{
    std::ifstream file(name, std::ios::binary);
    Block buffer;
    for(unsigned long long i = TextSize(name); i >= BLOCK_SIZE; i -= BLOCK_SIZE)
    {
        file.read(reinterpret_cast<char*>(&buffer), BLOCK_SIZE);
        blocks.push_back(buffer);
        if (i - BLOCK_SIZE != 0 and i - BLOCK_SIZE < 16)
        {
            for (unsigned int j = 0; j < BLOCK_SIZE; j++)
            {
                file.read(reinterpret_cast<char*>(&buffer.Data[j]), 1);
                if (j >= i - BLOCK_SIZE)
                {
                    buffer.Data[j] = '~';
                }
            }
            blocks.push_back(buffer);
        }
    }
}

void WriteToFile(std::string& name, std::vector<Block>& blocks)
{
    std::ofstream file(name, std::ios::binary);
    for(unsigned int i = 0; i < blocks.size(); i++)
    {
        if (i == blocks.size() - 1)
        {
            for (unsigned int j = 0; j < BLOCK_SIZE; j++)
            {
                if (blocks[i].Data[j] != '~')
                {
                    file.write(reinterpret_cast<char*>(&blocks[i].Data[j]), 1);
                }
            }
        }
        else
        {
            file.write(reinterpret_cast<char*>(&blocks[i]), BLOCK_SIZE);
        }
    }
}

void Gamming(std::vector<Block>& blocks, int& seed)
{
    for (Block& bl : blocks)
    {
        srand(seed);
        for (unsigned int i = 0; i < BLOCK_SIZE; i++)  
        {
            bl.Data[i] = bl.Data[i] ^ rand();
        }
        seed++;
    }
}

void Shifting(std::vector<Block>& blocks, bool dir)
{
    if (dir == 1)
    {
        for (Block& bl : blocks)
        {
            for (unsigned int i = 0; i < BLOCK_SIZE; i++)
            {
                bl.Data[i] = bl.Data[i] << BLOCK_SHIFT | bl.Data[i] >> (8 - BLOCK_SHIFT);
            }
        }
    }
    else if (dir == 0)
    {
        for (Block& bl : blocks)
        {
            for (unsigned int i = 0; i < BLOCK_SIZE; i++)
            {
                bl.Data[i] = bl.Data[i] >> BLOCK_SHIFT | bl.Data[i] << (8 - BLOCK_SHIFT);
            }
        }
    }
}

void Encode(std::string& from, std::string& out, int& seed)
{
    std::vector<Block> Blocks;
    ReadFromFile(from, Blocks);
    Gamming(Blocks, seed);
    Shifting(Blocks, 1);
    WriteToFile(out, Blocks);
}

void Decode(std::string& from, std::string& out, int& seed)
{
    std::vector<Block> Blocks;
    ReadFromFile(from, Blocks);
    Shifting(Blocks, 0);
    Gamming(Blocks, seed);
    WriteToFile(out, Blocks);
}

int main()
{
    unsigned int option = 0;
    int key;
    std::string First;
    std::string Second;
    while (option != 3)
    {
        cout << "Select option" << endl;
        cout << "1. Encode" << endl << "2. Decode" << endl << "3. Exit" << endl;
        cout << "Option: ";
        cin >> option;
        switch (option)
        {
        case 1:
            cout << "File for encoding: ";
            cin >> First;
            cout << "File for recording: ";
            cin >> Second;
            cout << "Enter key: ";
            cin >> key;
            cout << "DONT'T FORGET THE KEY" << endl;
            Encode(First, Second, key);
            break;
        case 2:
            cout << "File for dencoding: ";
            cin >> First;
            cout << "File for recording: ";
            cin >> Second;
            cout << "Enter key: ";
            cin >> key;
            Decode(First, Second, key);
        case 3:
            break;
        default:
            cout << "There isn't such option" << endl;
            cout << "Select option" << endl;
            cout << "1. Encode" << endl << "2. Decode" << endl << "3. Exit" << endl;
            cout << "Option: ";
            cin >> option;
            break;
        }
    }
    cout << "SHUTDOWN";
}