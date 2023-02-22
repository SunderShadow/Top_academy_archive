#include <iostream>
#include <fstream>

// Copied from http://microsin.net/programming/arm/trim-c-cpp.html
void trim(std::string& s)
{
    int i = 0, j;
    while ((s[i] == ' ') || (s[i] == '\t'))
    {
        i++;
    }

    if (i > 0)
    {
        for (j = 0; j < s.size(); j++)
        {
            s[j] = s[j + i];
        }
        s[j] = '\0';
    }

    i = s.size() - 1;
    while ((s[i] == ' ') || (s[i] == '\t'))
    {
        i--;
    }

    if (i < (s.size() - 1))
    {
        s[i + 1] = '\0';
    }
}

class HandlerString
{
protected:
    std::string readIpV4(const std::string str)
    {
        int i = -1;

        while (str[++i] != ' ');

        return str.substr(0, i);
    }

    std::string readBytePart(const std::string str)
    {
        unsigned int i = str.size() + 1;

        while (str[--i] != ' ');

        return str.substr(i + 1, str.size() - i);
    }

    void disassemble(std::string str)
    {
        trim(str);

        this->ipv4 = readIpV4(str);
        this->byte = readBytePart(str);
    }
public:
    std::string ipv4;
    std::string byte;

    HandlerString(const std::string str)
    {
        this->disassemble(str);
    };
    
    std::string assemble(int space_count = 1)
    {
        return this->ipv4 + std::string(space_count, ' ') + this->byte;
    }

    std::string assemble_reverse(int space_count = 1)
    {
        return this->byte + std::string(space_count, ' ') + this->ipv4;
    }
};

class FileHandler
{
protected:
    void openDist(std::ofstream& stream)
    {
        stream.open(this->dist_filepath);
    }
    
    void openSrc(std::ifstream& stream)
    {
        stream.open(this->src_filepath);
    }

public:
    std::string dist_filepath, src_filepath;
    FileHandler(const std::string dist_filepath, const std::string src_filepath)
    {
        this->dist_filepath = dist_filepath;
        this->src_filepath = src_filepath;
    }

    void handle()
    {
        char line[256];

        std::ofstream dist_stream;
        std::ifstream src_stream;

        this->openDist(dist_stream);
        this->openSrc(src_stream);

        while (!src_stream.eof())
        {
            src_stream.getline(line, 256);

            HandlerString handled_string((std::string)line);

            dist_stream << handled_string.assemble_reverse(6) << std::endl;
        }

        src_stream.close();
        dist_stream.close();
    }
};

int main()
{
    const char DIST_FILEPATH[] = "Dist.txt";
    const char SRC_FILEPATH[] = "Src.txt";

    FileHandler handler(DIST_FILEPATH, SRC_FILEPATH);

    handler.handle();
    return 0;
}
