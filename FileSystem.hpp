#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//Data structure baseline for the filesystem
struct Name{
  char name[8];
  char ext;
};
struct Txt{
    Name name;
    int textLength;
    std::string text;
};
struct Program{
    Name name;
    int cpu;
    int mem;
    //update struct for project 3 to include time left and remaining 
    bool hasIO = false; 
    int timetd;
    int amountoftime;
};
//struct proccess of our programs will be used to keep track of the programs running procceses
struct Process{
    Name name; //programs name
    int cpu; //cpu requirements
    int memory; //programs mem
    bool hasIO = false;
    int timetd;
    int totaltime = -1; //running total of the time required for a process
    int amountoftime;
    bool stored = false;//boolean to check if process is stored or not
    bool running = false; //boolean to check if process is currently running
};
struct Directory{
    Name name;
    int contents;
    std::vector<Txt*>texts;
    std::vector<Program*>programs;
    std::vector<Directory*>subFolders;
    Directory *parent;
};

//void initialize();
void initialize(char**);
//createFile update for p2 to include filename in arg
void createFile(std::string);
//mkdir project 2 editing create dir in include a string
void createDir(std::string);
void endDir();
void quit();
//project 2 methods
void cat(std::string);
void ls();
void cd(std::string);
void pwd();


void printinfo(); //same as quit from before

//project 3
//this function will convert any input string to an int 
int stringToint(std::string);
void run();
void step(int n);
void start(std::string);

void setMemory(int n);
void getMemory();
void setBurst(int n);
void addProgram(std::string);
void manage();
void runTask(int n);
void buildq();