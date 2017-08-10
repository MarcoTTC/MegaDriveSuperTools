#include <stdio.h>

#include "ArgumentParser.hpp"

int main(int argc, char **argv)
{
	ArgumentParser *parser = new ArgumentParser(argc, argv);
	parser->parse();
	
	return 0;
}
