#define BOOST_TEST_MODULE COMMON_TEST
#include <boost/test/included/unit_test.hpp>
#include <http.h>
BOOST_AUTO_TEST_CASE(movebycmd_test)
{
    try
    {

        common::movebycmd("test/common_test.cpp", "test/common_test.cpp/1");
        BOOST_FAIL("it should have failed,but not");
    }
    catch (const std::exception &e)
    {
        common::print_info(common::string_format("it's expect to failed:%s", e.what()));
    }
    BOOST_ASSERT(!common::file_exist("test/common_test.cpp1"));
    common::movebycmd("test/common_test.cpp", "test/common_test.cpp${a}");
    BOOST_ASSERT(common::file_exist("test/common_test.cpp${a}"));
    common::movebycmd("test/common_test.cpp${a}", "test/common_test.cpp");
    BOOST_ASSERT(!common::file_exist("test/common_test.cpp${a}"));
    common::print_info("PASS");
}
BOOST_AUTO_TEST_CASE(free_test_function)
/* Compare with void free_test_function() */
{
    common::http_client c{"blog.swish-cloud.com", 443};
    for (int i = 0; i < 5; i++)
    {
        c.GET();
        common::print_info(c.error.message());
        common::print_info(c.resp_text);
#ifndef __linux__
        common::GetLastErrorMsg(NULL);
        bool res = !c.error && !c.resp_text.empty();
        if (!res)
        {
            BOOST_FAIL(c.error.message());
            return;
        }
#endif
    }
}