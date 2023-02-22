#include <iostream>
#include <fstream>

// Copied from http://microsin.net/programming/arm/trim-c-cpp.html
void trim(std::string& s)
{
    int i = 0, j;
    while (s[i] == ' ' || s[i] == '\t') i++;

    if (i > 0)
    {
        for (j = 0; j < s.size(); j++)
        {
            s[j] = s[j + i];
        }

        s[j] = '\0';
    }

    i = s.size() - 1;

    while (s[i] == ' ' || (s[i--] == '\t'));
    if (i < s.size() - 1) s[i + 1] = '\0';
}

class HandledString
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
        this->mac = readBytePart(str);
    }
public:
    std::string ipv4;
    std::string mac;

    HandledString(const std::string str)
    {
        this->disassemble(str);
    };
    
    std::string assemble(int space_count = 1)const
    {
        return this->ipv4 + std::string(space_count, ' ') + this->mac;
    }

    std::string assemble_reverse(int space_count = 1)const
    {
        return this->mac + std::string(space_count, ' ') + this->ipv4;
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

    void virtual writeFile(std::ofstream& dist_stream, const HandledString& str)
    {
        dist_stream << str.assemble_reverse(6) << std::endl;
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

            HandledString handled_string((std::string)line);
            this->writeFile(dist_stream, handled_string);
        }

        src_stream.close();
        dist_stream.close();
    }
};

class SecondFileHandler : public FileHandler
{
public:
    SecondFileHandler(const std::string dist_filepath, const std::string src_filepath): FileHandler(dist_filepath, src_filepath)
    {
    }
protected:
    void writeFile(std::ofstream& dist_stream, const HandledString& str)
    {
        std::string ipv4(str.ipv4);
        std::string mac(str.mac);

        for (int i = 0; i < mac.size(); i++)
        {
            if (mac[i] == '-') mac[i] = ':';
        }

        dist_stream
            << "host 201-" << 1 << std::endl
            << '{' << std::endl
            << '\t' << "hardware ethernet" << '\t' << mac << std::endl
            << '\t' << "fixed-address" << '\t' << '\t' << ipv4 << std::endl
            << '}' << std::endl
            << std::endl;
    }
};

int main()
{
    const std::string DIST_FILEPATH_1 = "Dist_1.txt";
    const std::string DIST_FILEPATH_2 = "Dist_2.txt";
    const std::string SRC_FILEPATH    = "Src.txt";

    FileHandler handler(DIST_FILEPATH_1, SRC_FILEPATH);
    SecondFileHandler secondFileHandler(DIST_FILEPATH_2, SRC_FILEPATH);

    handler.handle();
    secondFileHandler.handle();

    return 0;
}
