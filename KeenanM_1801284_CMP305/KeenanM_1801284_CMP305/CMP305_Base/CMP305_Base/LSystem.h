#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Rule.h"
#include "D3D.h"
#include "LineMesh.h"
#include "assimp/defs.h"
using std::string;



class LSystem
{
public:
	LSystem( D3D* rend);

	inline void SetAxiom(string Axiom) { mAxiom = Axiom; }

	//GETTER FOR THE CURRENT LSYSTEM
	inline const string GetCurrentSystem() { return mCurrentSystem; }
	//GETTER FOR THE AXIOM
	inline const string GetAxiom() { return mAxiom; }
	//GETTER FOR THE LINE MESH
	LineMesh* GetGeneration();

	//ADDS A RULE TO THE RULE VECTOR
	void AddRule(const char, const string);		

	//ITERATES THE SYSTEM A NUMBER OF TIMES EQUAL TO COUNT
	void Run(const int count);					

	//ITERATES THE SYSTEM ONCE
	void Iterate();		

	//GENERATES THE LINE MESH BASED ON THE SYSTEM
	void Generate(bool type, int iteratorIncrease);

	//RESETS THE CYCLE COUNTER
	void resetCycle();

private:

	//STRING VARIABLES FOR SYSTEM
	 string mAxiom;
	 string mCurrentSystem;

	 //RULE VECTOR
	 std::vector <Rule> rules;

	 //MESH
	 LineMesh* mLine;

	 //MISC VARIABLES
	 int cycle = 1;
	 D3D* renderer;
	 
};

