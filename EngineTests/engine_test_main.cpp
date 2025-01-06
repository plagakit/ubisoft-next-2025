#include <engine.h>

#include "sparse_set_test/sparse_set_test.h"

class EngineTest : public Application
{
public:
	void Init() 
	{	
		SparseSetTest::RunTests();

	}

	void Shutdown() {}
	void Update(float dt) {}
	void Render() {}
};

REGISTER_GAME(EngineTest)