#include "pch.h"
#include "../RemoteConsole/server_executor.h"

using testing::_;
using testing::Return;
using testing::Invoke;

//
//class ServerExecutorMock : public ServerExecutor
//{
//public:
//	MOCK_METHOD0(initialize, bool());
//	MOCK_METHOD1(execute, bool(const std::wstring&));
//	MOCK_CONST_METHOD0(getResult, std::wstring());
//
//	MOCK_METHOD0(send_error_message, void());
//	MOCK_METHOD2(create_sub_process, bool(PROCESS_INFORMATION&, wchar_t*));
//
//	void delegateToOrigin_execute()
//	{
//		ON_CALL(*this, execute(_))
//			.WillByDefault(Invoke(&m_real, &ServerExecutor::execute));
//	}
//	void delegateToOrigin_initialize()
//	{
//		ON_CALL(*this, initialize())
//			.WillByDefault(Invoke(&m_real, &ServerExecutor::initialize));
//	}
//
//	void delegateToFake_create_sub_process()
//	{
//		ON_CALL(*this, create_sub_process(_, _)).WillByDefault(Return(false));
//	}
//	void delegateToFake_initialize()
//	{
//		ON_CALL(*this, initialize()).WillByDefault(Return(false));
//	}
//
//private:
//
//	ServerExecutor m_real;
//};
//
//
//
//TEST(ServerExecutorTest, test)
//{
//
//
//    ServerExecutor executor;
//    EXPECT_TRUE(executor.initialize());
//
//
//    ServerExecutorMock mock;
//    //EXPECT_CALL(mock, initialize);
//    EXPECT_TRUE(mock.initialize());
//
//   
//}





TEST(ServerExecutorTest, getResult_ReturnEmptyWstringIfNoActionsDone)
{
	ServerExecutor executor;
	std::wstring
		output = executor.getResult(),
		result;

	EXPECT_EQ(result, output);
}

TEST(ServerExecutorTest, execute_ReturnFalseIfNotInitialized)
{
	ServerExecutor executor;
	bool output = executor.execute(L"help");

	EXPECT_FALSE(output);
}

