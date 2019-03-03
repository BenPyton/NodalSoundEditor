/*
* @author PELLETIER Benoit
*
* @file SquareComponent.h
*
* @date 19/01/2019
*
* @brief Implementation of squared signal generator component
*
*/

#ifndef _SQUARECOMPONENT_H
#define _SQUARECOMPONENT_H

#include "GeneratorComponent.h"

class SquareComponent : public GeneratorComponent
{
private:
public:
	SquareComponent();
	virtual ~SquareComponent();

	virtual float getOutput(float _time) override;
};

#endif // _SQUARECOMPONENT_H