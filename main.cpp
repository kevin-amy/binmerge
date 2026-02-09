#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

// 定义常量
#define MAX_BIN_SIZE 4096
#define ID_BTN_FILE1 101
#define ID_BTN_FILE2 102
#define ID_BTN_MERGE 103

// 使用宽字符存储路径
wchar_t path1[MAX_PATH] = L"";
wchar_t path2[MAX_PATH] = L"";
HWND hEdit1, hEdit2;

// 文件选择函数 (Wide 版本)
void SelectFile(HWND hWnd, wchar_t* path, HWND hEdit) {
    OPENFILENAMEW ofn; // 使用 W 版本结构体
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        SetWindowTextW(hEdit, path);
    }
}

// 合并逻辑
void DoMerge(HWND hWnd) {
    if (wcslen(path1) == 0 || wcslen(path2) == 0) {
        MessageBoxW(hWnd, L"请先选择两个bin文件！", L"提示", MB_OK | MB_ICONWARNING);
        return;
    }

    wchar_t outPath[MAX_PATH] = L"merged.bin";
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = outPath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn)) {
        // 使用 _wfopen 以支持中文路径
        FILE *f1 = _wfopen(path1, L"rb");
        FILE *f2 = _wfopen(path2, L"rb");
        FILE *fout = _wfopen(outPath, L"wb");

        if (!f1 || !f2 || !fout) {
            MessageBoxW(hWnd, L"文件打开失败！", L"错误", MB_OK | MB_ICONERROR);
            if (f1) fclose(f1); if (f2) fclose(f2); if (fout) fclose(fout);
            return;
        }

        unsigned char *buffer = (unsigned char*)malloc(MAX_BIN_SIZE);
        memset(buffer, 0xFF, MAX_BIN_SIZE);

        fread(buffer, 1, MAX_BIN_SIZE, f1);
        fwrite(buffer, 1, MAX_BIN_SIZE, fout);

        unsigned char temp[1024];
        size_t n;
        while ((n = fread(temp, 1, sizeof(temp), f2)) > 0) {
            fwrite(temp, 1, n, fout);
        }

        fclose(f1); fclose(f2); fclose(fout);
        free(buffer);
        MessageBoxW(hWnd, L"合并成功！", L"完成", MB_OK | MB_ICONINFORMATION);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            // 使用 CreateWindowW 并配合 L"字符串"
            CreateWindowW(L"Static", L"文件 1 (对齐 4KB):", WS_VISIBLE | WS_CHILD, 10, 10, 150, 20, hWnd, NULL, NULL, NULL);
            hEdit1 = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 10, 35, 300, 20, hWnd, NULL, NULL, NULL);
            CreateWindowW(L"Button", L"选择文件", WS_VISIBLE | WS_CHILD, 320, 33, 80, 25, hWnd, (HMENU)ID_BTN_FILE1, NULL, NULL);

            CreateWindowW(L"Static", L"文件 2 (直接追加):", WS_VISIBLE | WS_CHILD, 10, 70, 150, 20, hWnd, NULL, NULL, NULL);
            hEdit2 = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 10, 95, 300, 20, hWnd, NULL, NULL, NULL);
            CreateWindowW(L"Button", L"选择文件", WS_VISIBLE | WS_CHILD, 320, 93, 80, 25, hWnd, (HMENU)ID_BTN_FILE2, NULL, NULL);

            CreateWindowW(L"Button", L"开始合并并导出", WS_VISIBLE | WS_CHILD, 10, 140, 390, 40, hWnd, (HMENU)ID_BTN_MERGE, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_FILE1) SelectFile(hWnd, path1, hEdit1);
            if (LOWORD(wp) == ID_BTN_FILE2) SelectFile(hWnd, path2, hEdit2);
            if (LOWORD(wp) == ID_BTN_MERGE) DoMerge(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WNDCLASSW wc = {0}; // 使用 WNDCLASSW
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"BinMergerClass";
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(wc.lpszClassName, L"bin文件合并工具 (4KB对齐)", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 430, 240, NULL, NULL, hInst, NULL);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
