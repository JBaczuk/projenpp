#ifndef PROJENPP_H
#define PROJENPP_H
#include <string>

class Project
{
public:
    /* Members */
    std::string projName; 
    std::string bugAddress;
    bool installBoost;
    bool installCli;
    std::string version;

    /* Constructors */
    //Project();

    int initializeProject();
    int generateProject();

}; // class project
#endif

