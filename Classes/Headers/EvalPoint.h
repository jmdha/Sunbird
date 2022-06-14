#ifndef EVALPOINT
#define EVALPOINT

class EvalPoint {
public:
	EvalPoint();
	EvalPoint(int analDepth, int value);

	int GetInstanceCount();
	bool GetValue(int depth, int& value);
	void StoreEval(int depth, int value);
	void IncrementCurrentHash();
	void DecrementCurrentHash();

private:
	int instanceCount = 0;
	// Depth of analysis
	int evalDepth = -1;
	int value = 0;
};

#endif