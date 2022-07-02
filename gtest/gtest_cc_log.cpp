#include "cc_log.h"
#include "gtest/gtest.h"

namespace
{
    class TestCcLog : public testing::Test
    {
        void SetUp() override
        {};

        void TearDown() override
        {};
    };
}

TEST_F(TestCcLog, TEST_CC_LOG)
{
    cc_log_t *log = cc_log_init();

    ASSERT_EQ(CC_OK, cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, "alert error!\0"));
    ASSERT_EQ(CC_OK, cc_log_out(log));
}
