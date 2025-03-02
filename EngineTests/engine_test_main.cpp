#include <engine.h>

#include "sparse_set_test/sparse_set_test.h"
#include "ecs_test/ecs_test.h"
#include "matrix_test/matrix_test.h"

class EngineTest : public Application
{
public:
	void Init() 
	{	
		SparseSetTest::RunTests();
		ECSTest::RunTests();
		MatrixTest::RunTests();
	}

	void Shutdown() {}
	void Update(float dt) {}
	void Render() {}
};

REGISTER_GAME(EngineTest)