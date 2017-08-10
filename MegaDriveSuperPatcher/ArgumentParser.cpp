#include <cstring>
#include <unistd.h>
#include <iostream>

using namespace std;

#include "ips.hpp"

#include "ArgumentParser.hpp"

ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	paramNumber = argc;
	
	paramValues = (char**) calloc(argc, sizeof(char*));
	for(int i=0; i<argc; i++) {
		paramValues[i] = (char*) calloc(strlen(argv[i])+1, sizeof(char));
		strcpy(paramValues[i], argv[i]);
	}
}

ArgumentParser::~ArgumentParser()
{
}

void ArgumentParser::parse()
{
	int c;
	
	int createOption = 0;
	char *createValue = NULL;
	char *originalFilename = NULL;
	char *modifiedFilename = NULL;
	
	int applyIPSOption = 0;
	char *applyIPSValue = NULL;
	char *fileToPatchName = NULL;
	
	opterr = 0;
	
	if(paramNumber==1) {
		showHelpMenu();
	} else {
		while((c = getopt(paramNumber, paramValues, "c:i:h")) != -1)
		{
			switch(c)
			{
				case 'c':
					createOption = 1;
					createValue = optarg;
				break;
				case 'i':
					applyIPSOption = 1;
					applyIPSValue = optarg;
				break;
				case 'h':
					showHelpMenu();
				break;
				case '?':
					if(optopt == 'c') {
						cout << "Option -c requires an output filename" << endl;
					} else if(optopt == 'i') {
						cout << "Option -i requires a patch filename" << endl;
					} else {
						if(isprint(optopt)) {
							cout << "Unknown option: -" << static_cast<char>(optopt) << endl;
							cout << "Type -h for help" << endl;
						}
					}
				break;
			}
			
		}
		
		if(createOption) {
			if(optind+1>paramNumber) {
				cout << "Missing original file and modified file arguments!" << endl;
			} else if(optind+2>paramNumber) {
				cout << "Missing modified file argument!" << endl;
			} else {
				originalFilename = paramValues[optind];
				modifiedFilename = paramValues[optind+1];
				
				InternalPatchSystem::createIPS(createValue, originalFilename, modifiedFilename);
			}
		} else if(applyIPSOption) {
			if(optind+1>paramNumber) {
				cout << "Missing file to apply argument!" << endl;
			} else {
				fileToPatchName = paramValues[optind];
				
				cout << "applyIPSValue: " << applyIPSValue << endl;
				cout << "fileToPatchName: " << fileToPatchName << endl;
				InternalPatchSystem::applyIPS(applyIPSValue, fileToPatchName);
			}
		}
	}
}

void ArgumentParser::showHelpMenu() {
	cout << "Usage:" << endl;
	cout << '\t' << paramValues[0] << " -i patchname filename" << endl;
	cout << '\t' << paramValues[0] << " -c output originalfile modifiedfile" << endl;
}
