#include "LSystem.h"

//CONSTRUCTOR
LSystem::LSystem( D3D* rend)
{
	mLine = new LineMesh(rend->getDevice(), rend->getDeviceContext());
	renderer = rend;
}

//ITERATES THE SYSTEM A NUMBER OF TIMES EQUAL TO COUNT
void LSystem::Run(const int count) {
	mCurrentSystem = mAxiom;
	for (int i = 0; i < count; i++) {
		Iterate();
	}
}

//ADDS A NEW RULE TO THE LSYSTEM
void LSystem::AddRule(char predecessor, string successor) {
	
	Rule newRule;
	newRule.createRule(predecessor, successor);

	//ADDS THE RULE TO THE VECTOR OF RULES
	rules.push_back(newRule);

}

//ITERATE THE SYSTEM ONCE
void LSystem::Iterate() {

	std::queue <char> brokenAxiom;
	int axiomSize = GetAxiom().size();

	//BREAKS THE AXIOM/SYSTEM INTO INDIVIDUAL CHARS
	for (int i = 0; i < axiomSize; i++)
	{
		brokenAxiom.push(mAxiom.at(i));
	}

	//CLEARS THE AXIOM
	mAxiom.clear();

	//ITERATES THROUGH THE BROKEN AXIOM AND EVOLVES IT BASED ON THE ESTABLISHED RULES
	for (int i = 0; i <= axiomSize-1; i++)
	{
		//IF THE VALUE IS A CONSTANT JUST ADD IT BACK TO THE AXIOM
		if (brokenAxiom.front() == '[' || brokenAxiom.front() == ']' || brokenAxiom.front() == '-' || brokenAxiom.front() == '+')
		{
			mAxiom.append(std::string(1, brokenAxiom.front()));
		}
		else
		{
			for (auto it : rules)
			{
				std::string temp = it.applyRule(brokenAxiom.front());
				if (temp != "error")
					mAxiom.append(temp);

			}
		}

		//POP THE FIRST CHAR
		brokenAxiom.pop();
	}

	//SETS THE SYSTEM TO THE GENERATED SYSTEM
	mCurrentSystem = mAxiom;
	cycle++;
}


//GENERATES A LINE BASED ON THE CURRENT SYSTEM
void LSystem::Generate(bool type, int iteratorIncrease)
{
	//Clear any lines we might already have
	mLine->Clear();

	//Get the current L-System string, right now we have a place holder
	std::string systemString = mCurrentSystem;


	float iterator = 1;

	//ITERATE IN A CIRCLE 
	do
	{


		//Initialise some variables
		XMVECTOR pos = XMVectorReplicate(0.0f);	//Current position
		XMVECTOR dir = XMVectorSet(0, 1, 0, 1);	//Current direction
		XMVECTOR fwd = XMVectorSet(cos(iterator) + sin(iterator), 0, -sin(iterator) + cos(iterator), 1);	//Rotation axis
		XMMATRIX currentRotation = XMMatrixRotationRollPitchYaw(0, 0, 0);
		std::vector<XMVECTOR> positionMatrix; //MATRIX TO STORE POSITIONS
		std::vector<XMMATRIX> rotationMatrix; //MATRIX TO STORE ROTATIONS

		//INITIALISE THE MATRIX
		positionMatrix.push_back(pos); 
		rotationMatrix.push_back(currentRotation);


		//ITERATE THROUGH THE STRING AND GENERATE ACCORDINLY
		for (int i = 0; i < systemString.length(); i++) {
			switch (systemString[i]) {
				//GENERATE A NEW LINE
			case 'A':
				mLine->AddLine(pos, pos + XMVector3Transform(dir, currentRotation / cycle));
				pos += XMVector3Transform(dir, currentRotation) / cycle;						
				break;
				//GENERATE A NEW LINE
			case 'B':
				mLine->AddLine(pos, pos + XMVector3Transform(dir, currentRotation / cycle));	
				pos += XMVector3Transform(dir, currentRotation) / cycle;		
				break;
				//SAVE THE POSITION AND ROTATION AND ROTATE
			case '[':
				positionMatrix.push_back(pos);
				rotationMatrix.push_back(currentRotation);
				currentRotation *= XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(25.0f));
				break;
			case ']':
				//RESET THE POSITIONS AND REMOVE THE NEWEST SAVED POSITION
				currentRotation = rotationMatrix.back();
				pos = positionMatrix.back();
				positionMatrix.pop_back();
				rotationMatrix.pop_back();
				currentRotation *= XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(335.0f));
				break;
			case 'F':
				//GENERATE A NEW LINE
				mLine->AddLine(pos, pos + XMVector3Transform(dir, currentRotation / cycle));	
				pos += XMVector3Transform(dir, currentRotation) / cycle;						
				break;
			case 'G':
				//GENERATE A NEW LINE
				mLine->AddLine(pos, pos + XMVector3Transform(dir, currentRotation / cycle));	
				pos += XMVector3Transform(dir, currentRotation) / cycle;						
				break;
			case '+':
				//ROTATE LEFT
				currentRotation *= XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(120.0f));
				break;
			case '-':
				//ROTATE RIGHT
				currentRotation *= XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(240.0f));
				break;

			}
		}

		iterator += iteratorIncrease;


	} while (iterator <= 360 && type == true);

	//Build the vertices
	mLine->BuildLine(renderer->getDeviceContext(), renderer->getDevice());
}

//GET THE LINE
LineMesh* LSystem::GetGeneration()
{
	return mLine;
}

//RESET THE CYCLE
void LSystem::resetCycle()
{
	cycle = 1;
}