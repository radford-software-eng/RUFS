/*Christopher Lee
3/23/21
Project 3 RATCH aka RU Batch
Professor Ray
Objective: This project The goal of this project is to build ontop of the previous project and all of the
commands from project 2 will still run. This new program adds the
ability to start programs and simulate their execution. The system will have a set amount of memory, a variable burst time, and uses a round robin queue for
keeping track of running processes.

*/
#include "FileSystem.hpp"
#include <sstream>

std::string command;
std::string *argsPointer(new std::string);

//enumeration for each input string command will be used as a hash key
  enum string_code {
      cCreateDir, //maps to the create dir command
      cCreateFile, //createfile command
      cEndDir,//for enddir
      cquit, //for the quit 
      ccd, //cd
      cls, //ls
      cpwd, //pwd
      ccat, //cat
      crun,// run
      cstart,//start
      cstep,//step
      cprint,//print info
      csetburst, //set burst
      csetmem, //set memory
      cgetmem, //get memory
      caddpro, //add program
    // make text will use later cMktxt,   
      cError
  };
  //hash function that takes in a string commad from input and returns an enum value
  string_code hashit (std::string const& inString) {
    //now makedir
      if (inString == "mkdir") return cCreateDir;
      if (inString == "createFile") return cCreateFile;
      //no longer needed 
      if (inString == "endDir") return cEndDir;
      if (inString == "quit") return cquit;
      //p2 
      if (inString == "cd") return ccd;
      //list current contents of the directory project 2
      if (inString == "ls") return cls;
      //project 2 pwd
      if (inString == "pwd") return cpwd;
      if (inString == "cat") return ccat;
      if (inString == "printinfo") return cprint;
//project 3 commands
      if (inString == "run") return crun;
      if (inString == "start") return cstart;
      if (inString == "step") return cstep;
      if (inString == "setBurst") return csetburst;
      if (inString == "setMemory") return csetmem;
      if (inString == "getMemory") return cgetmem;
      if (inString == "addProgram") return caddpro;

      else return cError; 
  }
  //function to convert a string from the shell to int
  int stringToint(std::string args){
    int i; //temp integer 
    std::string s = args; 
    std::istringstream ss(s); //convert the string to a string stream
    ss >> i; //stream the string to the integer type
    return i; //return integer i
}
//main function
int main(int argc, char** argv) {
  std::cout << "Welcome to RATCH.\n";
  //std::cout << "CreateDir or CreateFile or EndDir or quit\n";
  initialize(argv);
  //initialize(argv);
  bool open = true;
  while(open){
    //now take in the command
    std::cout << "Command>";
    getline(std::cin, command);
    std::cout<< command << std::endl;
    
    //break command into base command and string of the arguments
    //also a work around for extrenous whitespace
    std::size_t space=command.find(' ');
    if (space!=std::string::npos){ //found a space in the command
      argsPointer->assign(command.substr(space+1,command.length()));
      command = command.substr(0,space);\
      
    }
    //Switch case for commands to function calls
    //i used a hash function to create a switch menu 
    switch(hashit(command)){
      case cCreateFile:
        createFile(*argsPointer);
        break;
      case cCreateDir:
        createDir(*argsPointer);
        break;
      case cEndDir:
        endDir();
        break;
        //need to udpate quit
        //quit now sucessfullys exit program
      case cquit:
      //exit program
      std::cout << "Terminating..\n";
       quit(); //call quit command to save info to binary file and print out directory information
       exit(0); 
       // quit();
       // open = false;
       break;
        //change directory
        //need project 2 commands

      case cls:
        ls();
        break;
      case ccd:
        cd(*argsPointer);
        break;
      case cpwd:
        pwd();
        break;
      case ccat:
        cat(*argsPointer);
        break;
        //RATCH
      case crun:
        run();
        break;
      case cstep:
        step(stringToint(*argsPointer));
        break;
      case cstart:
        start(*argsPointer);
        break;
      case csetburst:
        setBurst(stringToint(*argsPointer));
        break;
      case csetmem:
        setMemory(stringToint(*argsPointer));
        break;
      case cgetmem:
          getMemory();    
        break;
      case caddpro:
        addProgram(*argsPointer);
        break;
        //print info
      case cprint:
        //prints quit from project 1 quit
        quit();
        break;
      
        //error checking
      default:
        std::cout << ">" << command << ": command not found\n";
    }
  }
  return 0;
}