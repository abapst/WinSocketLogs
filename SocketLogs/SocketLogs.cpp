#include <iostream>
#include <vector>

#include "SocketLogsCore.h"

class InputParser
{
public:
    InputParser(int& argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            this->tokens.push_back(std::string(argv[i]));
        }
    }

    const std::string& getCmdOption(const std::string& option) const
    {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end())
        {
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool cmdOptionExists(const std::string& option) const
    {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }

private:
    std::vector <std::string> tokens;
};

void print_usage()
{
    printf("\n");
    printf("  Usage:\n");
    printf("      -server              Start a TCP server\n");
    printf("      -client              Start a client that listens to a server\n");
    printf("      -ip (optional)       IP address use\n");
    printf("      -port (optional)     Port to use\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    InputParser input(argc, argv);

    const char* ip_address = DEFAULT_IP;
    const char* port = DEFAULT_PORT;

    if ((argc == 1) || input.cmdOptionExists("-h"))
    {
        print_usage();
        return 0;
    }

    if (input.cmdOptionExists("-port"))
    {
        port = input.getCmdOption("-port").c_str();
    }

    if (input.cmdOptionExists("-ip"))
    {
        ip_address = input.getCmdOption("-ip").c_str();
    }

    if (input.cmdOptionExists("-server"))
    {
        LogServer server(port); // start server

        // send a ping every second
		int cnt = 0;
		do
		{
			server.SendString("Ping %d\n", cnt);
			Sleep(1000);
			cnt++;
		} while (true);

    }
    else if (input.cmdOptionExists("-client"))
    {
        LogClient client(ip_address, port); // start client
    }

    return 0;
}
