#include <monitor.h>
#include <assert.h>
#include <common.h>
#include <Windows.h>
HANDLE h;
const int buffer_len = 64 * 1024;
//FILE_NOTIFY_INFORMATION lpBuffer[buffer_len];
char lpBuffer[buffer_len];
LPDWORD lpBytesReturned;
OVERLAPPED overlapped;

void LpoverlappedCompletionRoutine(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped)
{

    auto monitor = (common::monitor::win_monitor *)overlapped.hEvent;
    auto info = (FILE_NOTIFY_INFORMATION *)lpBuffer;
    while (1)
    {
        auto filename = common::to_cstr(info->FileName, info->FileNameLength);
        auto c = new common::monitor::change();
        c->path = std::filesystem::path(monitor->path_to_watch).append(filename).string();
        delete (filename);
        monitor->onchange_cb(c, monitor->obj);
        switch (info->Action)
        {
        case 1:
            //common::print_debug(common::string_format("added:%s", c->path.c_str()));
            break;
        case 2:
            // common::print_debug(common::string_format("removed:%s", c->path.c_str()));
            break;

        case 3:
            // common::print_debug(common::string_format("modified:%s", c->path.c_str()));
            break;
        case 4:
            // common::print_debug(common::string_format("the old name:%s", c->path.c_str()));
            break;

        case 5:
            //common::print_debug(common::string_format("the new name:%s", c->path.c_str()));
            break;

        default:
            //common::print_debug(common::string_format("unrecognized change type:%d filename:%s", info->Action, c->path.c_str()));
            break;
        }
        if (info->NextEntryOffset != 0)
        {
            info = (FILE_NOTIFY_INFORMATION *)((char *)info + info->NextEntryOffset);
        }
        else
        {
            break;
        }
    }
    monitor->ReadDirectoryChangesW();
}

void common::monitor::win_monitor::ReadDirectoryChangesW()
{
    ZeroMemory(lpBuffer, buffer_len);
    if (::ReadDirectoryChangesW(h, lpBuffer, buffer_len, true,
                                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE, lpBytesReturned, &overlapped, &LpoverlappedCompletionRoutine) == 0)
    {
        common::throw_exception(common::GetLastErrorMsg("ReadDirectoryChangesW"));
    }
}
common::monitor::win_monitor::win_monitor(std::string path_to_watch)
{
    this->path_to_watch = path_to_watch;
    overlapped.hEvent = this;
}
void common::monitor::win_monitor::watch(std::string path_to_watch)
{
}
void common::monitor::win_monitor::read_async(onchange onchange, void *obj)
{
    this->onchange_cb = onchange;
    this->obj = obj;
    //get file handle
    h = CreateFileA(path_to_watch.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        common::throw_exception(common::GetLastErrorMsg("common::monitor::watch()"));
    }
    ReadDirectoryChangesW();
}