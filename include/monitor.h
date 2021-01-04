#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include <filesystem>
#include <Windows.h>
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
            virtual void watch() = 0;

        private:
        };

        class win_monitor : public MONITOR
        {
        public:
            std::string path_to_watch;
            using onchange = void (*)(change *change, void *obj);
            onchange onchange_cb;
            void *obj;
            win_monitor(onchange onchange, void *obj, std::string path_to_watch);
            void ReadDirectoryChangesW();
            void watch() override;
        };
    } // namespace monitor
} // namespace common
#endif