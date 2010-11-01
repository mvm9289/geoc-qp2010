#include "data.h"
#include "graphics.h"
#include "parser.h"
#include "viewer.h"
#include "writer.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "exer3.h"

using namespace std;

void read_parameters(int argc, char **argv);

void execute(){

    clearAll(); //Delete all the data

    if (inFileRead && readFile(inFile)){
        cerr << "There was a problem reading file '" << inFile << "'." << endl;
        exit(1);
    }

    /*

      Put your code here.

     */
    if (inFileRead) exer3initialization(true, inFile);
    else exer3initialization();
}

int main(int argc, char** argv){
    read_parameters(argc, argv);

    execute();

    initializeGraphics(&argc, argv);
}

void read_parameters(int argc, char **argv){
    int i = 1;

    while (i < argc){
        if (strcmp(argv[i], "-i") == 0 && ++i < argc ){
            inFile = argv[i];
            inFileRead = true;
        }
        else if (strcmp(argv[i], "-o") == 0 && ++i < argc){
            outFile = argv[i];
        }
        ++i;
    }
}
