#include "Rule.h"

//CONSTRUCTOR
Rule::Rule()
{
	error = "error";
}

//CREATUES A NEW RULE
void Rule::createRule(char newInput, std::string newOutput)
{
	input = newInput;
	output = newOutput;
}

//APPLIES THE RULE AND RETURN AN ERROR IF THE SYMBOL DOESNT EXIST/MATCH
std::string Rule::applyRule(char userInput)
{
	if (userInput == input)
	{
		return output;
	}
	else
	{
		std::cout << "Error, incorrect input at rule " << input << " = " << output << "\n";
		return error;
	}
}