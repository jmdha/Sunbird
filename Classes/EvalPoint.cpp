#include "Headers/EvalPoint.h"

EvalPoint::EvalPoint()
{
	this->evalDepth = -1;
	this->value = 0;
}

EvalPoint::EvalPoint(int evalDepth, int value) : EvalPoint()
{
	this->evalDepth = evalDepth;
	this->value = value;
}

int EvalPoint::GetInstanceCount()
{
	return instanceCount;
}

bool EvalPoint::GetValue(int depth, int& value)
{
	if (evalDepth == -1 || depth > this->evalDepth)
		return false;
	value = this->value;
	return true;
}

void EvalPoint::StoreEval(int depth, int value)
{
	if (depth <= this->evalDepth)
		return;
	this->evalDepth = depth;
	this->value = value;
}

void EvalPoint::IncrementCurrentHash()
{
	instanceCount++;
}

void EvalPoint::DecrementCurrentHash()
{
	instanceCount--;
}
