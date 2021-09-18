#define BOOST_TEST_MODULE COMMON_TEST
#include <boost/test/included/unit_test.hpp>
#include <../common-cpp/include/http.h>
BOOST_AUTO_TEST_CASE(free_test_function)
/* Compare with void free_test_function() */
{
    common::http_client c{"blog.swish-cloud.com", 443};
    for (int i = 0; i < 5; i++)
    {
        c.GET();
        common::print_info(c.error.message());
        common::print_info(c.resp_text);
        common::GetLastErrorMsg(NULL);
        bool res = !c.error && !c.resp_text.empty();
        if (!res)
        {
            BOOST_FAIL(c.error.message());
            return;
        }
    }
}