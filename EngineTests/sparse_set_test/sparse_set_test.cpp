#include "sparse_set_test.h"

#include <engine.h>
#include <random>

void SparseSetTest::RunTests()
{
	Logger::Info("Running SparseSet tests...");

	constexpr int amt = 10000;

	std::random_device rd;
	std::mt19937 r(rd());
	std::uniform_int_distribution<> RandBool(0, 1);
	std::uniform_int_distribution<> RandUID(1, amt);

	SparseSet<int> ss;

	ASSERT_ERROR(ss.Size() == 0, "Size is not zero.");
	ASSERT_ERROR(ss.Capacity() == 0, "Capacity is not zero.");

	size_t cap = amt + 1;
	ss = SparseSet<int>(cap);

	ASSERT_ERROR(ss.Size() == 0, "Size is not zero.");
	ASSERT_ERROR(ss.Capacity() == cap, "Capacity is not zero.");

	for (UID i = 1; i <= amt; i++)
		ss.Insert(i, i * 2);

	ASSERT_ERROR(ss.Size() == amt, "Size is not amt.");

	for (UID i = 1; i <= amt; i++)
	{
		ASSERT_ERROR(ss.Has(i), "UID %d has wrong Has", i);
		ASSERT_ERROR(ss.Get(i) == i * 2, "UID %d returning %d", i, ss.Get(i) == i * 2);
	}

	// Delete at random
	std::vector<UID> deleted;
	for (int i = 0; i < amt / 2; i++)
	{
		UID del;
		do {
			del = RandUID(r);
		} while (std::find(deleted.begin(), deleted.end(), del) != deleted.end());


		ASSERT_ERROR(ss.Has(del), "UID %d wrong Has before deletion", del);
		ss.Remove(del);
		ASSERT_ERROR(!ss.Has(del), "UID %d wrong Has after deletion", del);

		deleted.push_back(del);
	}

	ASSERT_ERROR(ss.Size() == amt / 2, "Wrong Size after deletions.");

	for (UID i : deleted)
		ss.Insert(i, i * 2);

	for (UID i = 1; i < amt; i++)
	{
		ASSERT_ERROR(ss.Has(i), "UID %d has wrong Has after mass deletion", i);
		ASSERT_ERROR(ss.Get(i) == i * 2, "UID %d returning %d after mass deletion", i, ss.Get(i) == i * 2);
	}

	Logger::Info("SparseSet tests passed!");
}