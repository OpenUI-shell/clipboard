#include <windows.h>
#include <iostream>
#include <string>

void getClipboard() {
    if (!OpenClipboard(nullptr)) return;
    
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData) {
        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText) {
            std::cout << pszText;
            GlobalUnlock(hData);
        }
    }
    CloseClipboard();
}

void clearClipboard() {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        CloseClipboard();
    }
}

void setClipboard(const std::string& text) {
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hMem) {
        memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
    }

    CloseClipboard();
}

void checkClipboard() {
    std::cout << (IsClipboardFormatAvailable(CF_TEXT) ? "TRUE" : "FALSE");
}

void printHelp() {
    std::cout << "Available commands:\n"
                 "  clipboard get    - Get clipboard content\n"
                 "  clipboard clear  - Clear clipboard\n"
                 "  clipboard set \"text\" - Set clipboard content\n"
                 "  clipboard exist  - Check if clipboard has text (TRUE/FALSE)\n"
                 "  clipboard info   - Show program info\n"
                 "  clipboard help   - Show this help message\n";
}

void printInfo() {
    std::cout << "Console program clipboard version 1.0\n"
                 "Created for the OpenUI system.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string command = argv[1];

    if (command == "get") getClipboard();
    else if (command == "clear") clearClipboard();
    else if (command == "set" && argc == 3) setClipboard(argv[2]);
    else if (command == "exist") checkClipboard();
    else if (command == "info") printInfo();
    else printHelp();

    return 0;
}
