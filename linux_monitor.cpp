#include <sys/inotify.h>
#include <monitor.h>
#include <common.h>
#include <unordered_map>
#include <unistd.h>
#include <thread>
namespace common
{
    namespace monitor
    {
        int fd;
        std::unordered_map<int, std::string> wd_map;
        linux_monitor::linux_monitor()
        {
            if ((fd = inotify_init()) == -1)
            {
                common::throw_exception("inotify_init failed");
            }
        }
        void linux_monitor::watch(std::string path_to_watch)
        {
            std::vector<std::string> paths{path_to_watch};
            common::find_files(path_to_watch, paths, true, 2);
            for (auto path : paths)
            {
                int wd;
                if ((wd = inotify_add_watch(fd, path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE)) == -1)
                {
                    common::throw_exception("inotify_add_watch failed");
                }
                else
                {
                    wd_map[wd] = path;
                    std::cout << "watching " << path << " wd " << wd << std::endl;
                }
            }
        }

        static void _read_async(common::monitor::MONITOR::onchange onchange, void *obj, linux_monitor *linux_monitor)
        {
            const int NAME_MAX = 255;
            const int BUF_LEN = (sizeof(inotify_event) + NAME_MAX + 1) * 1000;
            char buffer[BUF_LEN];
            while (1)
            {
                int r = read(fd, buffer, BUF_LEN);
                if (r == -1)
                {
                    common::throw_exception("inotify read failed");
                }
                int offset = 0;
                while (offset < r)
                {
                    inotify_event *event = (inotify_event *)(buffer + offset);
                    offset += event->len + sizeof(inotify_event);
                    std::string fullpath = std::filesystem::path(wd_map[event->wd]).append(event->name);
                    if (event->mask & IN_CREATE)
                    {
                        std::cout << common::string_format("file IN_CREATE:%s", fullpath.c_str()) << std::endl;
                    }
                    else if (event->mask & IN_DELETE)
                    {
                        std::cout << common::string_format("file IN_DELETE:%s", fullpath.c_str()) << std::endl;
                    }
                    else if (event->mask & IN_MODIFY)
                    {
                        std::cout << common::string_format("file IN_MODIFY:%s", fullpath.c_str()) << std::endl;
                    }
                    else if (event->mask & IN_MOVED_FROM)
                    {
                        std::cout << common::string_format("file IN_MOVED_FROM:%s", fullpath.c_str()) << std::endl;
                    }
                    else if (event->mask & IN_MOVED_TO)
                    {
                        std::cout << common::string_format("file IN_MOVED_TO:%s", fullpath.c_str()) << std::endl;
                    }
                    // else if (event->mask & IN_ISDIR)
                    // {
                    //     std::cout << common::string_format("file IN_ISDIR:%s", fullpath.c_str()) << std::endl;
                    // }
                    else
                    {
                        std::cout << common::string_format("file unknown event:%s", fullpath.c_str()) << std::endl;
                        //common::throw_exception("unknown event");
                    }

                    if (std::filesystem::is_directory(fullpath))
                    {
                        linux_monitor->watch(fullpath);
                    }
                    auto c = new common::monitor::change();
                    c->path = fullpath;
                    onchange(c, obj);
                }
            }
        }
        void linux_monitor::read_async(onchange onchange, void *obj)
        {
            std::thread *first = new std::thread(&_read_async, onchange, obj, this);
        }
    } // namespace monitor
} // namespace common