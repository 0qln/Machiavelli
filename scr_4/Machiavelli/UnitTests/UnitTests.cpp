#include "pch.h"
#include "CppUnitTest.h"
#include "../dll/TestClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Machiavelli::TestClass::Run();
			
		}
	};
}
