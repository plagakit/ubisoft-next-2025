#pragma once

//#include <vector>
//#include <functional>
//
//class TestSuite
//{
//public:
//	virtual void RegisterTests() = 0;
//	virtual void BeforeAll() {};
//	virtual void BeforeEach() {};
//
//private:
//	friend class EngineTest;
//	void RunTests();
//
//protected:
//	std::vector<std::function<bool()>> m_cases;
//};
//
//#define RUN_SUITE(suite) \
//	{ \
//		Logger::Info("Testing suite %s", #suite); \
//		suite s = suite(); \
//		s.BeforeAll(); \
//		s.RegisterTests(); \
//		s.RunTests(); \
//	}
//
//#define BEGIN_TEST(name) \
//
//#define END_TEST()
	