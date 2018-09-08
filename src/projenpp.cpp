#include <projenpp.hpp>
#include <iostream>
#include <sys/stat.h>

int Project::initializeProject()
{
    int status = 0;
    std::string installBoostStr;
    std::string installCliStr;
    
    // Get user inputs
    // TODO: use getline() to allow blank default answers. see https://stackoverflow.com/questions/4999650/c-how-do-i-check-if-the-cin-buffer-is-empty#4999719
    std::cout << "What is the name of your project? ";
    std::cin >> projName;
    std::cout << "Bug reporting email/url? ";
    std::cin >> bugAddress;
    std::cout << "Boost Unit Tests? yN ";
    std::cin >> installBoostStr;
    if (installBoostStr == "y" || installBoostStr == "Y")
    {
        installBoost = true;
    }
    else
    {
        installBoost = false;
    }
    std::cout << "CLI? yN ";
    std::cin >> installCliStr;
    if (installCliStr == "y" || installCliStr == "Y")
    {
        installCli = true;
    }
    else 
    {
        installCli = false;
    }
    version = "0.0.1";

    generateProject();

    return status;
}

int Project::generateProject()
{
    int status = 0;
    std::string dirToCreate;

    // Create project folders
    dirToCreate = projName;
    status = mkdir(dirToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    // TODO: more concise error handling? (typ)
    if (status < 0)
    {
        std::cerr << "Error creating directory " << dirToCreate << std::endl;
        return status;
    }
    dirToCreate = (projName + "/src");
    status = mkdir(dirToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status < 0)
    {
        std::cerr << "Error creating directory " << dirToCreate<< std::endl;
        return status;
    }
    if (installBoost)
    {
        dirToCreate = (projName + "/test");
        status = mkdir(dirToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status < 0)
        {
            std::cerr << "Error creating directory " << dirToCreate << std::endl;
            return status;
        }
    }

    return status;
}
