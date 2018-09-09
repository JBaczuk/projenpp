#ifndef PROJENPP_H
#define PROJENPP_H
#include <string>

class Project
{
public:
    /* Members */
    std::string projName; 
    std::string bugAddress;
    bool installUnitTest;
    bool installCli;
    std::string version;

    /* Constructors */
    //Project();

    int initializeProject();
    int generateProject();
    int createMain();
    int createUnitTests();
    int createReadme();
    int createMakefiles();
    int configureAutotools();

}; // class project
#endif

