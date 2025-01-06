//#include "engine_test.h"
//
//#include <engine.h>
//
//void TestSuite::RunTests()
//{
//    for (auto testCase : m_cases)
//    {
//        bool passed = true;
//        try 
//        {
//            BeforeEach();
//            passed = testCase();
//        }
//        catch (const std::exception& e) 
//        {
//            std::cerr << e.what();
//            passed = false;
//        }
//
//        if (!passed)
//            Logger::Error("Test case failed.");
//    }
//}
