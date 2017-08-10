#ifndef ARGUMENTPARSER_HPP
#define ARGUMENTPARSER_HPP

class ArgumentParser
{
public:
	ArgumentParser(int argc, char *argv[]);
	~ArgumentParser();
	
	void parse(void);
private:
	int paramNumber;
	char ** paramValues;
	
	void showHelpMenu(void);
};

#endif // ARGUMENTPARSER_HPP
