#include "General.h"

#include <windows.h>
#include <wininet.h>
#include <string>

namespace Eclipse
{
    std::string General::GetPublicIPAdress()
    {
        HINTERNET net = InternetOpen("IP retriever", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

        HINTERNET conn = InternetOpenUrl(net, "https://myexternalip.com/raw", NULL, 0, INTERNET_FLAG_RELOAD, 0);

        char buffer[4096];
        DWORD read;

        InternetReadFile(conn, buffer, sizeof(buffer)/sizeof(buffer[0]), &read);
        InternetCloseHandle(net);    

        return std::string(buffer, read);
    }
}