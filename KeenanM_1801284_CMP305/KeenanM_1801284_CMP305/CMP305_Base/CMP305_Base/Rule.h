#pragma once
#include <string>
#include <iostream>

//CLASS FOR STORING A RULE FOR THE LSYSTEM
class Rule
{
public:

	//CONSTRUCTOR
	Rule();

	//CREATES A NEW RULE
	void createRule(char newInput, std::string newOutput);

	//APPLIES A RULE TO A CHARACTER
	std::string applyRule(char userInput);

private:

	//STORAGE VARIABLES
	char input;
	std::string output, error;



};

