#include <projenpp.hpp>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

int Project::initializeProject()
{
    int status = 0;
    std::string installUnitTestStr;
    std::string installCliStr;
    
    // Get user inputs
    // TODO: use getline() to allow blank default answers. see https://stackoverflow.com/questions/4999650/c-how-do-i-check-if-the-cin-buffer-is-empty#4999719
    std::cout << "What is the name of your project? ";
    std::cin >> projName;
    std::cout << "Bug reporting email/url? ";
    std::cin >> bugAddress;
    std::cout << "Boost Unit Tests? yN ";
    std::cin >> installUnitTestStr;
    if (installUnitTestStr == "y" || installUnitTestStr == "Y")
    {
        installUnitTest = true;
    }
    else
    {
        installUnitTest = false;
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
    if (installUnitTest)
    {
        dirToCreate = (projName + "/test");
        status = mkdir(dirToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status < 0)
        {
            std::cerr << "Error creating directory " << dirToCreate << std::endl;
            return status;
        }
    }

    status = createMain();
    if (installUnitTest) status = createUnitTests();
    status = createReadme();
    // TODO: Create GNU required folders/files
    std::ofstream newsFile;
    newsFile.open((projName + "/NEWS").c_str());
    newsFile.close();
    std::ofstream authorsFile;
    authorsFile.open((projName + "/AUTHORS").c_str());
    authorsFile.close();
    std::ofstream changeLogFile;
    changeLogFile.open((projName + "/ChangeLog").c_str());
    changeLogFile.close();

    status = createMakefiles();
    status = configureAutotools();

    return status;
}

int Project::createMain()
{
    int status = 0;

    std::string mainHeader;
    std::string mainFunction;
    
    if (installCli)
    {
        mainHeader =
        "#include <iostream>\n"
        "#include <boost/program_options.hpp>\n"
        "#include <boost/filesystem.hpp>\n\n"
        "using namespace std;\n"
        "namespace po = boost::program_options;\n\n";

        mainFunction =
       
        "int main (int argc, char* argv[])\n"
        "{\n"
        "	try {\n"
        "        std::string appName = boost::filesystem::basename(argv[0]);\n"
        "        bool some_flag_en;\n"
        "        string command;\n"
        "        vector<string> command_params;\n"
        "\n"
        "        po::options_description desc(\"Allowed options\");\n"
        "        desc.add_options()\n"
        "            (\"help\", \"Show this help message\")\n"
        "            (\"verbose,v\", po::value<int>()->implicit_value(1),\n"
        "                  \"Enable verbosity (optionally specify level)\")\n"
        "            (\"some_flag,t\", po::bool_switch(&some_flag_en)->default_value(false),\n"
        "                  \"Use some_flag configuration\")\n"
        "            (\"command\", po::value< vector<string> >(), \"Command\");\n"
        "\n"
        "        po::positional_options_description p;\n"
        "        p.add(\"command\", -1);\n"
        "\n"
        "        po::variables_map vm;\n"
        "        po::store(po::command_line_parser(argc, argv).\n"
        "                  options(desc).positional(p).run(), vm);\n"
        "        po::notify(vm);\n"
        "\n"
        "        if (vm.count(\"help\")) {\n"
        "            std::cout << appName + \"\\n\";\n"
        "            std::cout << \"<insert program description here>\\n\\n\";\n"
        "            std::cout << \"Usage: command [options]\\n\";\n"
        "            std::cout << desc;\n"
        "            return 0;\n"
        "        }\n"
        "\n"
        "        if (vm.count(\"command\"))\n"
        "        {\n"
        "            auto commands = vm[\"command\"].as<vector<string>>();\n"
        "            command = commands[0];\n"
        "            vector<string> command_params_new(commands.begin() + 1, commands.end());\n"
        "            command_params = command_params_new;\n"
        "        }\n"
        "\n"
        "        if (vm.count(\"verbose\")) {\n"
        "            std::cout << \"Verbosity enabled.  Level is \" << vm[\"verbose\"].as<int>()\n"
        "                 << \"\\n\";\n"
        "        }\n"
        "\n"
        "        if (some_flag_en) {\n"
        "            std::cout << \"Some flag enabled.\" << \"\\n\";\n"
        "        }\n"
        "\n"
        "        // Command Handler\n"
        "        if(command == \"help\")\n"
        "        {\n"
        "            std::cout << \"Usage: command [options]\\n\";\n"
        "            std::cout << desc;\n"
        "            return 0;\n"
        "        }\n"
        "        else\n"
        "        {\n"
        "            std::cout << \"Invalid command.\" << endl;\n"
        "            std::cout << \"Usage: command [options]\\n\";\n"
        "            std::cout << desc;\n"
        "        }\n"
        "\n"
        "    }\n"
        "    catch(std::exception& e)\n"
        "    {\n"
        "        std::cout << e.what() << \"\\n\";\n"
        "        return 1;\n"
        "    }\n"
        "\n"
        "    return 0;\n"
        "}\n";

    }
    else
    {
        mainHeader =
        "#include <iostream>\n"
        "using namespace std;\n\n";

        mainFunction =
        "int main (int argc, char* argv[])\n"
        "{\n"
        "    std::cout << \"Hello World\" << std::endl;\n"
        "    return 0;\n"
        "}\n";
    
    }

    std::ofstream mainFile;
    mainFile.open((projName + "/src/"  + "main.cpp").c_str());
    mainFile << mainHeader;
    mainFile << mainFunction;
    mainFile.close();

    return status;
}

int Project::createUnitTests()
{
    int status = 0;

    std::string runner =
    "#define BOOST_AUTO_TEST_MAIN\n"
    "#define BOOST_TEST_DYN_LINK\n"
    "#include <boost/test/auto_unit_test.hpp>\n";
    std::ofstream runnerFile;
    runnerFile.open((projName + "/test/" + "runner.cpp").c_str());
    runnerFile << runner;
    runnerFile.close();

    return status;
}

int Project::createReadme()
{
    int status = 0;

    std::string readme =
    "see README.md";
    std::ofstream readmeFile;
    readmeFile.open((projName + "/README").c_str());
    readmeFile << readme;
    readmeFile.close();

    // TODO: explain more about how to run the cli program and run tests if applicable
    std::string readmeMd =
    "## Installation"
    "```"
    "$ ./autogen.sh\n"
    "$ ./configure\n"
    "$ make\n"
    "$ sudo make install";
    std::ofstream readmeMdFile;
    readmeMdFile.open((projName + "/README.md").c_str());
    readmeMdFile << readmeMd;
    readmeMdFile.close();

    return status;
}

int Project::createMakefiles()
{
    int status = 0;
    
    std::string mainMakefile =
    "AUTOMAKE_OPTIONS = gnu\n"
    "SUBDIRS = src test\n"
    "TESTS = test/" + projName + "_tests\n";

    std::ofstream mainMakefileFile;
    mainMakefileFile.open((projName + "/Makefile.am").c_str());
    mainMakefileFile << mainMakefile;
    mainMakefileFile.close();

    std::string srcMakefile;
    if (installUnitTest)
    {
        srcMakefile =
        "bin_PROGRAMS = " + projName + "\n"
        "" + projName + "_SOURCES = main.cpp\n"
        "" + projName + "_LDADD = -lboost_program_options -lboost_filesystem -lboost_system\n";

        std::string testMakefile =
        "" + projName + "_tests_CPPFLAGS = -I../src/ ${BOOST_CPPFLAGS}\n"
        "" + projName + "_tests_LDFLAGS = ${BOOST_LDFLAGS}\n"
        "" + projName + "_tests_LDADD = ${BOOST_UNIT_TEST_FRAMEWORK_LIB}\n"
        "" + projName + "_tests_SOURCES = ../src/testfuncs.cpp runner.cpp " + projName + "_tests.cpp\n";

        std::ofstream testMakefileFile;
        testMakefileFile.open((projName + "/test/Makefile.am").c_str());
        testMakefileFile << testMakefile;
        testMakefileFile.close();
    }
    else
    {
        srcMakefile =
        "bin_PROGRAMS = " + projName + "\n"
        "projenpp_SOURCES = main.cpp\n";
    }

    std::ofstream srcMakefileFile;
    srcMakefileFile.open((projName + "/src/Makefile.am").c_str());
    srcMakefileFile << srcMakefile;
    srcMakefileFile.close();
    
    return status;
}

int Project::configureAutotools()
{
    int status = 0;
    return status;
}