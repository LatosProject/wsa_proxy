#include <iostream>
#include <string>
#include <cstdlib>

namespace
{
    const int DEFAULT_WSADeveloperPort = 58526;
    const int DEFAULT_ProxyPort = 7890;
    const std::string ADB_CLEAR_PROXY_COMMAND = "adb shell settings put global http_proxy :0";

    std::string generateAdbConnectCommand(int WSADeveloperPort, int ProxyPort)
    {
        return "adb connect 127.0.0.1:" + std::to_string(WSADeveloperPort) +
               " && adb shell \"settings put global http_proxy $(ip route list match 0 table all scope global | cut -f3 -d' '):" +
               std::to_string(ProxyPort) + "\"";
    }

    int executeCommand(const std::string &command)
    {
        return system(command.c_str());
    }

    void printUsage(const char *programName)
    {
        std::cout << "Usage: " << programName << " <start|stop> [PROXY_PORT] [WSA_DEVELOPER_PORT]" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 4)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string choice = argv[1];
    int ProxyPort = DEFAULT_ProxyPort;
    int WSADeveloperPort = DEFAULT_WSADeveloperPort;

    if (argc >= 3)
    {
        try
        {
            ProxyPort = std::stoi(argv[2]);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid ProxyPort value: " << e.what() << std::endl;
            return 1;
        }
    }

    if (argc == 4)
    {
        try
        {
            WSADeveloperPort = std::stoi(argv[3]);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid WSADeveloperPort value: " << e.what() << std::endl;
            return 1;
        }
    }

    int result = 0;

    if (choice == "start")
    {
        std::string ADB_CONNECT_COMMAND = generateAdbConnectCommand(WSADeveloperPort, ProxyPort);
        result = executeCommand(ADB_CONNECT_COMMAND);
    }
    else if (choice == "stop")
    {
        result = executeCommand(ADB_CLEAR_PROXY_COMMAND);
    }
    else
    {
        printUsage(argv[0]);
        return 1;
    }

    if (result == 0)
    {
        std::cout << "Done." << std::endl;
    }
    else
    {
        std::cout << "Execution failed." << std::endl;
        return 1;
    }

    return 0;
}