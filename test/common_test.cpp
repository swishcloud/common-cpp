#define BOOST_TEST_MODULE COMMON_TEST
#include <boost/test/included/unit_test.hpp>
#include <http.h>
#include <monitor.h>
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
void monitor_cb(common::monitor::change *c, void *obj)
{
    std::string changetype;
    switch (c->ct)
    {
    case common::monitor::change_type::added:
        changetype = "added";
        break;
    case common::monitor::change_type::removed:
        changetype = "removed";
        break;
    case common::monitor::change_type::modified:
        changetype = "modified";
        break;
    case common::monitor::change_type::renamed:
        changetype = "renamed";
        break;
    case common::monitor::change_type::INMOVEDFROM:
        changetype = "INMOVEDFROM";
        break;
    case common::monitor::change_type::INMOVEDTO:
        changetype = "INMOVEDTO";
        break;
    default:
        throw common::exception("unknown change type");
    }
    std::cout << "change type: " << changetype << ", change path : " << c->path << std::endl;
}
BOOST_AUTO_TEST_CASE(filemonitor_test)
{
    if (!std::filesystem::exists(".cache"))
    {
        if (!std::filesystem::create_directory(".cache"))
        {
            BOOST_FAIL("failed to create directory .cache");
        }
    }
    common::monitor::MONITOR *monitor = new common::monitor::linux_monitor();
    monitor->watch(".cache");
    monitor->read_async(monitor_cb, NULL);
    common::pause();
}
BOOST_AUTO_TEST_CASE(exec_cmd)
{
    auto p1 = ".cache/exec_cmd_test";
    auto p2 = ".cache/exec_cmd_test2";
    if (std::filesystem::exists(p1))
    {
        std::filesystem::remove(p1);
    }
    if (std::filesystem::exists(p2))
    {
        std::filesystem::remove(p2);
    }

    std::filesystem::create_directory(p1);
    common::movebycmd(p1, p2);
}