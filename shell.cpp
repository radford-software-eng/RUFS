/*
Christopher Lee
2/09/21
file system for handler for rufs
this program contains all the procedures and functions for the file system  
*/
//our libraries 
#include "FileSystem.hpp"
#include <fstream>
#include <string>
#include <iostream>

Directory* root = new Directory;
//std::unique_ptr<Directory> root(new Directory);
Directory* currentDir = root;
//std::unique_ptr<Directory> currentDir;

void pushNameToArray(std::string fileName, Name &thisName, char ext){
    for (int i = 0; i < 8; i++){
        thisName.name[i] = fileName[i];
    }
    thisName.ext = ext;
}

std::string getInput(std::string title){
    std::cout << "Enter " << title << ">";
    std::string thisInput;
    getline(std::cin, thisInput);
    std::cout << thisInput << std::endl;
    return thisInput;
}
std::fstream * stream = new std::fstream();
/*Initialize creates the file io stuff and directory pointers
*/
void initialize(char** fsName){
    (*stream).open(fsName[1], std::ios::in | std::ios::out | std::ios::binary);
    if ((*stream).fail()){ //create new filesystem binary file
        (*stream).open(fsName[1], std::fstream::out);
        (*stream).eof();
        (*stream).close();
        (*stream).open(fsName[1], std::ios::in | std::ios::out | std::ios::binary);
    }else{
        //read filesystem
    }
    currentDir->parent = currentDir; //disgusting way to do it, root.parent = root
    currentDir->contents = 0;
    pushNameToArray( { 'r','o','o','t','\0','\0','\0','\0' }, currentDir->name, 'd');
}
//create file method
//updating to include string file name arguement from command
void createFile(std::string filename){
    //std::string filename = getInput("filename"); //call get input function to read input
    if((filename[filename.length()-1] == 't') && 
    (filename[filename.length()-2] == '.')){    //create textFile
        Txt *textFile = new Txt();
        pushNameToArray(filename, textFile->name, 't');//add file to data structure
        std::string textInput = getInput("contents");
        textFile->textLength = textInput.length();
        (textFile->text).assign(textInput);
        currentDir->texts.push_back(textFile);
        currentDir->contents += 1;
    }else if((filename[filename.length()-1] == 'p') && 
    (filename[filename.length()-2] == '.')){    //create program
        Program *progFile = new Program();
        pushNameToArray(filename, progFile->name, 'p');
        progFile->cpu = std::stoi(getInput("CPU requirements"));
        progFile->mem = std::stoi(getInput("memory requirements"));
        currentDir->programs.push_back(progFile);
        currentDir->contents += 1;
    }else{
        std::cout << "Not a valid filename" << filename << std::endl;
    }
}

//project 2 
// takes in a string argument from the command and splits it and determines where to change the directory if its .. goes to root
void cd (std::string arg){
    if(arg.compare("..") == 0 && currentDir != root){
      currentDir = currentDir->parent;
    }else{
      //traverse the vector of folders and find one with the same name
      for (int i=0; i<currentDir->subFolders.size(); i++ ){
        if(currentDir->subFolders.at(i)->name.name.compare(arg) == 0){//this is hacked together, just match the strings
          currentDir = currentDir->subFolders.at(i); //move into subFolders
          break; //exit loop
        }
      }
    }
}
//project 2 
//pwd method
//project 2
void pwd(){
  //string path will display our directory path
 std::string path =" ";
 std::cout << "Current Directory is " <<currentDir->name.name << std::endl ;
//loop through til we reached root
 while(currentDir != root){
   path.insert(0, "/");
   path.insert(0,currentDir->name.name);  
   currentDir = currentDir->parent;
   //path.append("debugg"); //tried using append insert is easier
 }
//add root to the path
 path.insert(0,currentDir->name.name);
 path.insert(0,"/");
 std::cout << path <<std::endl;
}
void endDir(){
    currentDir = currentDir->parent;
}
void writeName(Name &thisName){
    //std::cout << "\n\t\t\tIn Write Name \n\n";
    (*stream).write((char*)thisName.name, sizeof(thisName.name)); // 0 + 8 = 8
    //std::cout << "\n\t\t\tWrote the directory name.name \n\n";
    (*stream) << '.';
    //(*stream).write((char*)'.', sizeof('.'));                     // 8 + 1 = 9
    //std::cout << "\n\t\t\tWrote the directory name.ext \n\n";
}
void printFolder(int* byteCount, Directory *thisDir){
    std::cout << *byteCount << ":\tDirectory: " << thisDir->name.name << ".d\n";
    writeName(thisDir->name);
    (*stream) << 'd'; 
    //(*stream).write((char*)'d', sizeof('d')); 
    *byteCount += 11;
    (*stream) << thisDir->contents;
    std::cout << *byteCount << ":\tDirectory " << thisDir->name.name << " contains " << thisDir->contents << " files/directories\n";
    *byteCount += 4;
    for (int i = 0; i < thisDir->texts.size(); i++){
        //print text file
        std::cout << *byteCount << ":\tFilename: " << thisDir->texts.at(i)->name.name << "\n\tType: Text file\n";
        writeName(thisDir->texts.at(i)->name);
        (*stream).write((char*)"t\0", sizeof("t\0"));
        *byteCount += 11;
        std::cout << *byteCount << ":\tSize of text file: " << thisDir->texts.at(i)->textLength << " byte\n";
        (*stream) << thisDir->texts.at(i)->textLength;
        *byteCount += 4;
        std::cout << *byteCount << ":\tContents of text file:\t" << thisDir->texts.at(i)->text << "\n";
        *byteCount += thisDir->texts.at(i)->textLength;
        (*stream) << thisDir->texts.at(i)->text;
    }
    for (int i = 0; i < thisDir->programs.size(); i++){
        //print program 
        std::cout << *byteCount << ":\tFilename: " << thisDir->programs.at(i)->name.name << "\n\tType: Program\n";
        writeName(thisDir->programs.at(i)->name);
        (*stream).write((char*)"p\0", sizeof("p\0"));
        std::cout << "\tContents: CPU Requirement: " << thisDir->programs.at(i)->cpu << ", Memory Requirement " << thisDir->programs.at(i)->mem << "\n";
        (*stream) << thisDir->programs.at(i)->cpu;
        (*stream) << thisDir->programs.at(i)->mem;
        *byteCount += 19;
    }
    for (int i = 0; i < thisDir->subFolders.size(); i++){
        printFolder(byteCount, thisDir->subFolders.at(i));
    }
    std::cout << *byteCount << ":\tEnd of directory: " << thisDir->name.name << "\n";
    (*stream) << "End" << thisDir->name.name << ".d\0";
    *byteCount += 14;
}
void quit(){
    int byteCount = 0;
    std::cout << "Binary file structure is:" << std::endl;
    currentDir = root;
    printFolder(&byteCount, currentDir);
    
}