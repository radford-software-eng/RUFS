/*
Christopher Lee
3/23/21
FileSystem handler file 
contains all the methods for our file system */
#include "FileSystem.hpp"
#include <fstream>
#include <cstring>
#include <vector>
#include <iostream>

Directory* root = new Directory;
//std::unique_ptr<Directory> root(new Directory);
Directory* currentDir = root;
//std::unique_ptr<Directory> currentDir;

//adds a file to our data structure
void pushNameToArray(std::string fileName, Name &thisName, char ext){
    for (int i = 0; i < 8; i++){
        thisName.name[i] = fileName[i];
    }
    thisName.ext = ext;
}
//method to read in commands
std::string getInput(std::string title){
    std::cout << "Enter " << title << ">";
    std::string thisInput;
    getline(std::cin, thisInput);
    std::cout << thisInput << std::endl;
    return thisInput;
}
std::fstream * stream = new std::fstream();
/*Initialize creates the file io stuff and directory pointers
this method will also write to the binary file
*/
void initialize(char** fsName){
    //(*stream).open(fsName[1], std::ios::in | std::ios::out | std::ios::binary);
    /*(*stream).open(fsName[1], std::ios::binary | std::ios::out | std::ios::in | std::ios::app);
    (*stream).close(); //creates new binary file and closes 
    (*stream).open(fsName[1], std::ios::binary | std::ios::out | std::ios::in);//reopen the same file
    (*stream).seekp(0, std::ios::beg);//begining
    currentDir->parent = currentDir; 
    currentDir->contents = 0;
    pushNameToArray( { 'r','o','o','t','\0','\0','\0','\0' }, currentDir->name, 'd');
    //().close// we leave the file open */
   
   
    //creates the binary file if it doesnt already exist

    (*stream).open(fsName[1], std::ios::in | std::ios::out | std::ios::binary);
    if ((*stream).fail()){ //create new filesystem binary file
        (*stream).open(fsName[1], std::fstream::out);
        (*stream).eof();
        (*stream).close();
        (*stream).open(fsName[1], std::ios::in | std::ios::out | std::ios::binary);
    }else{
        //read filesystem
    }
    currentDir->parent = currentDir; // root.parent = root
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
void pwd(){
  //string path will display our directory path
 std::string path =" ";
 std::cout << "Current Directory is " <<currentDir->name.name << std::endl ;
 //assign temp directory to hold currentDir
 Directory *temp = currentDir;
//loop through til we reached root
 while(currentDir != root){
   
   path.insert(0,currentDir->name.name);  
   path.insert(0, "/");
   currentDir = currentDir->parent;
   //path.append("debugg"); //tried using append insert is easier
 }
//add root to the path
//path.insert(0,"/");
 path.insert(0,currentDir->name.name);
 
 std::cout << path <<std::endl;
 //set the currentdir back to original position
 currentDir = temp;

}
//updating method to mkdir to include the dir name in command input string
void createDir(std::string dirname){
  //no longer needed for p2
    //std::string dirname = getInput("directory name"); //call get input function to read dir name
    Directory *newFolder = new Directory();
    pushNameToArray(dirname, newFolder->name, 'd'); //add dir to data structure
    newFolder->parent = currentDir;
    currentDir->subFolders.push_back(newFolder);
    currentDir->contents += 1;
    currentDir = newFolder;
}
//change directory will take in a string and compare the input command directory to current and loop through to find the dir
void cd (std::string arg){
    if(arg.compare("..") == 0 && currentDir != root){
        currentDir = currentDir->parent; 
    }
    else{ 
        //change to diffrent directory from root
        for (int g = 0; g < currentDir->subFolders.size(); g++){
            //cout << endl << "trying to change to " << dirPointer->subFolders.at(g)->name;
            if (arg.compare(currentDir->subFolders.at(g)->name.name) == 0){
                //(*dirPointer) = (*dirPointer).subFolders.at(g);
                Directory* temp = currentDir->subFolders.at(g);
                //move back the currentdir 
                (*currentDir) = (*temp);
            }else {
               std::cout << "Couldnt find " << arg << std::endl;
            }
        }
        //cout << "changing directory to: `" << s << "`" << endl;
    }
}
//end dir method brings us back to the previous dir or parent
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
//project 2 takes in the targeted file name to display its contents 
void cat(std::string fileName){
 // if(fileName.compare)
 //Directory *thisDir = currentDir;
 Directory *dirPointer = currentDir;
    for (int g = 0; g < dirPointer->texts.size(); g++){
        if (fileName.compare(dirPointer->texts.at(g)->name.name) == 0){
            //(*dirPointer) = (*dirPointer).subFolders.at(g);
            std::cout << dirPointer->texts.at(g)->text << std::endl;
        }else {
            std::cout << "Couldnt find " << fileName << std::endl;
        }
    }
}
//list contents of directory copied my printfolder method and adjusted it to not write to binary
void ls(){
std::cout <<":\tDirectory: " << currentDir->name.name << ".d\n";
    std::cout  << ":\tDirectory " << currentDir->name.name << " contains " << currentDir->contents << " files/directories\n";
    
    for (int i = 0; i < currentDir->texts.size(); i++){
        //print text file
        std::cout << ":\tFilename: " << currentDir->texts.at(i)->name.name << "\n\tType: Text file\n";
        writeName(currentDir->texts.at(i)->name);
        std::cout << ":\tSize of text file: " << currentDir->texts.at(i)->textLength << "\n";

        std::cout << ":\tContents of text file:\t" << currentDir->texts.at(i)->text << "\n";
        //*byteCount += thisDir->texts.at(i)->textLength;
       // (*stream) << thisDir->texts.at(i)->text;
    }
    for (int i = 0; i < currentDir->programs.size(); i++){
        //print program 
        std::cout<< ":\tFilename: " << currentDir->programs.at(i)->name.name << "\n\tType: Program\n";
       // writeName(thisDir->programs.at(i)->name);
        //(*stream).write((char*)"p\0", sizeof("p\0"));
        std::cout << "\tContents: CPU Requirement: " << currentDir->programs.at(i)->cpu << ", Memory Requirement " << currentDir->programs.at(i)->mem << "\n";
        //(*stream) << thisDir->programs.at(i)->cpu;
        //(*stream) << thisDir->programs.at(i)->mem;
        //*byteCount += 19;
    }
    for (int i = 0; i < currentDir->subFolders.size(); i++){
        printFolder(0, currentDir->subFolders.at(i));
    }
    std::cout << ":\tEnd of directory: " << currentDir->name.name << "\n";
    //(*stream) << "End" << thisDir->name.name << ".d\0";
}
//Start of ratch methods and structures

//create our proccess queues 
std::vector<Process>queue;
std::vector<Process>finished;

int qpos = 0, burst = 1,currtime = 0; //global variables for current position queue,for current time,cpu burst size
int memory,memoryA;//global variables for system memory and available memory, 

//set burst
void setBurst(int n){
  burst = n;
}
//set memory
void setMemory(int n){
  memory = n;
}
//get memory outputs the  memory available
void getMemory(){
  std::cout << "Memory available for system <" << memory <<">" << std::endl;
}


//add program copied from the createfile method and adjusted to take in from command 
void addProgram(std::string args){
  
    int index = 0; //index will be used to keep track of position of string arguement so we can parse each arg
    std::string name;
    //loop through the input string break if blank space is found
    for(int i = 0; i < args.length(); i++){
        name.push_back(args[i]);
        if(args[i] == ' '){
            break;
        }
        //index will be used to keep track of our string command position
        index++;
    }

    std::string stdcpu; //string of the cpu will convert to the int 
    for( int i = index+1; i < args.length(); i++){
        stdcpu.push_back(args[i]);
        if(args[i] == ' '){
            index++;
            break;
        }
        index++;
        //index will be used to keep track of our string command position
        //
    }
    int acpu = stringToint(stdcpu); //convert to int
    std::string stdmemory; //get the input from getInput() functions
    for( int i = index+1; i < args.length(); i++){
        stdmemory.push_back(args[i]);
        if(args[i] == ' '){
            index++;
            break;
        }
        index++;
    }
    int amemory = stringToint(stdmemory);
    index++; //increment the index to check if there are more remaining string command arguements left
    if(index != args.length()){
        std::string sTimetd; //will hold the string off time todo arg
        for( int i = index; i < args.length(); i++){
            sTimetd.push_back(args[i]);
            if(args[i] == ' '){ //ignore blank space
                index++;
                break;
            }
            index++;
        }
        int timetd = stringToint(sTimetd); //convert args to int 
        std::string sTimeremain; //get the input from getInput() functions
        for( int i = index; i < args.length(); i++){
            sTimeremain.push_back(args[i]);
            if(args[i] == ' '){
                break;
            }
            index++;
        }
        int timeRemain =stringToint(sTimeremain); // convert string args to int
        //create an instance of our program and add our arg values to new program
        Program *progFile = new Program();
      //pushNameToArray(filename, progFile->name, 'p');
        //progFile->cpu = std::stoi(getInput("CPU //requirements"));
       // progFile->mem = std::stoi(getInput("memory //requirements"));
        //currentDir->programs.push_back(progFile);
        //currentDir->contents += 1;     
        pushNameToArray(name , progFile->name, 'p');
        progFile->cpu = acpu;
        progFile->mem = amemory;
        progFile->hasIO = true;
        progFile->timetd = timetd;
        progFile->amountoftime =timeRemain;
        currentDir->programs.push_back(progFile);
        currentDir->contents += 1;
        
    }
    //else no remainding args just add the args to new program
    else{
        Program *progFile = new Program();
        pushNameToArray(name , progFile->name, 'p');
        progFile->cpu = acpu;
        progFile->mem = amemory;
        currentDir->programs.push_back(progFile);
        currentDir->contents += 1;
    }
}
//manage function will be used to manage memory for proccesss in queue
void manage(){
    for( int i = 0; i < queue.size(); i++){
        if(memoryA >= queue[i].memory){ //store program on disk
            memoryA -= queue[i].memory;
            queue[i].stored = false;
        }else if((i - 1 > 0) && queue[i].memory == queue[i-1].memory && queue[i-1].stored == false){
            memoryA -= queue[i].memory;
            queue[i-1].stored = true;
        }else{
             queue[i-1].stored = true;
             queue[i].stored = false;
        }
    }
}
//start program for now just have it print
//start will take in a program and add it to the simulator or queue in this case
void start(std::string fileName){
//copied from my cat method 
  Program* progfile = NULL;
  std::string pname = fileName + " "; //filename arg in sample output didnt have a .p at the end so pname creates new string with .p added 
  //Directory *dirPointer = currentDir;
  //loop through contents of current dir
//  std::cout << dirPointer->programs.size();
    for (int g = 0; g < currentDir->programs.size(); g++){
          if(strcmp(currentDir->programs[g]->name.name, pname.c_str()) == 0){
            //debug std::cout << "foound\n";
            progfile = currentDir->programs.at(g);
            Process p; //create a process
            p.name = progfile->name;
            p.cpu = progfile->cpu;
            p.memory = progfile->mem;
            if(progfile->hasIO == true){
              p.hasIO = true;
              p.timetd = progfile->timetd;
              p.amountoftime = progfile->amountoftime;
              queue.push_back(p); //add proccess to queue
              //call manage to allocate memory
              manage();
              break;
            }
            else {
              queue.push_back(p);
              manage();
              break;
            }
        }
    }
    if(progfile==NULL) {
            std::cout << "Couldnt find " << fileName << std::endl;
        }
    
  
}

//start program for now just have it print
void step(int n){
  std::cout << "Advancing the system for <" << n << "> units or all jobs finished" << std::endl;
    std::cout << "Current time <" << currtime << ">" << std::endl;
    int steps = n;
    if(queue.size() == 0){
        std::cout << "Running job is empty" << std::endl << "The queue is: empty" << std::endl;
        std::cout << "Finished jobs are:";
        if(finished.size() != 0){
            std::cout << std::endl;
            for (int i = 0; i < finished.size(); i++){
                std::cout << " " << finished[i].name.name << " " << finished[i].cpu << " " << finished[i].totaltime << std::endl;
            }
        }else{
            std::cout << " empty" << std::endl;
            
        }
    }else{
        while ((queue.size() != 0) && (steps != 0)){
            runTask(0);
            steps--;
        }
    }
}
void buildq(){
    int counter = 0;
    for (int i = 0; i < qpos; i++){
        if(queue[i].timetd > 0){
            queue.push_back(queue[i]);
        }
    }
    for (int i = qpos; i < queue.size(); i++){
        if(queue[i].timetd > 0){
            queue[counter] = queue[i];
            counter++;
        }
    }
    queue.resize(counter);
}
//booleans to keep track of our queue and finished procceses
bool jobfinished = false;
bool emptyqueue = false;

//run program for now just have it print out program
void runTask(int n){
    std::cout << "Current time <" << currtime << ">" << std::endl;

    int position = 1;
    if(queue[n].timetd > 0){ //io time left to do
        std::cout << "Running job <" << queue[n].name.name << "> has <" << queue[n].timetd << "> time left and is using <" << queue[n].memory << "> memory resources." << std::endl;

        int b = burst;
        if(queue[n].hasIO == true){
            b = queue[n].hasIO; //set burst to 1 if true
        }
        if(queue[n].stored == true){
            std::cout << queue[n].name.name << " is stored on disk" << std::endl;
            b -= 2;
            currtime += 2;
        }
        for(int i = 0; i < b; i++){ //loop through burst
                queue[n].stored = false;
                queue[n].timetd -= 1;
                currtime += 1; //add 1 for the current time
                if (queue[n].timetd <= 0){
                    jobfinished = true;
                    queue[n].totaltime = currtime;
                    queue[n].running = false;
                    finished.push_back(queue[n]);//add process to finished vector     
                    memoryA += queue[n].memory;
                    break;
                }else{
                    queue[n].running = true;
                }    
        }
        b = burst;
        Process temp = queue[n];
        buildq();
        if (queue.size() > 1){
            std::cout << "The queue is:" << std::endl;
            for (int i = 0; i < queue.size(); i++){
                if(temp.name.name != queue[i].name.name && queue[i].running == false){
                    std::cout << "   Position " << position << ": job " << queue[i].name.name << " has " << queue[i].timetd << " units left and is using " << queue[i].memory; //<< " memory resources";
                    if(queue[i].stored == true){
                      std::cout << " resource on disk.";
                      } 
                    else{
                        std::cout << " memory resources.";}
                    position++;                   
                }
            }
        }
        else{
            if(queue.size() == 0){
                std::cout << "Finished jobs are:" << std::endl;
                for (int i = 0; i < finished.size()-1; i++){
                    std::cout << "   " << finished[i].name.name << " " << finished[i].cpu << " " << finished[i].totaltime << std::endl;
                }
                std::cout << "Current time <" << currtime << ">" << std::endl;
                std::cout << "Running job is empty" << std::endl;
            }
            std::cout << "The queue is: empty" << std::endl;
        }
        if (jobfinished == true){
            std::cout << "Finished jobs are:" << std::endl;
            for (int i = 0; i < finished.size(); i++){
                std::cout << "   " << finished[i].name.name << " " << finished[i].cpu << " " << finished[i].totaltime << std::endl;
            }
        }
        qpos++;
    }
    
}
//advances simulator until no jobs left
void run(){
    if(currtime == 0){ 
        manage();
    }
    while (queue.size() != 0){
        runTask(0);
    }
    
}