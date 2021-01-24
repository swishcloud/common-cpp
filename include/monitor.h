#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include <filesystem>
namespace common
{
    namespace monitor
    {
        enum class change_type
        {
            added,
            removed,
            modified,
            renamed
        };
        struct change
        {
            change_type ct;
            std::string path;
        };
        class MONITOR
        {

        public:
            using onchange = void (*)(change *change, void *obj);
            virtual void watch(std::string path_to_watch) = 0;
            virtual void read_async(onchange onchange, void *obj) = 0;

        private:
        };

        class win_monitor : public MONITOR
        {
            MONITOR::onchange onchange_cb;

        public:
            std::string path_to_watch;
            void *obj;
            win_monitor(std::string path_to_watch);
            void ReadDirectoryChangesW();
            void watch(std::string path_to_watch) override;
            void read_async(onchange onchange, void *obj) override;
        };

        class linux_monitor : public MONITOR
        {
            MONITOR::onchange onchange_cb;

        public:
            std::string path_to_watch;
            void *obj;
            linux_monitor();
            ~linux_monitor();
            void watch(std::string path_to_watch) override;
            void read_async(onchange onchange, void *obj) override;
        };
    } // namespace monitor
} // namespace common
#endif