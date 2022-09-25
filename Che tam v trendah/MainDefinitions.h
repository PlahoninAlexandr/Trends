#pragma once
#include "DataProcessing.h"

extern shared_ptr<DataProcessing> dp;

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

// вызывается при нажатии кнопки Update library и создаёт процесс с парсером написанным на python, там стоит ограничение на 108 фильмов,
// но можно его убрать и тогда он запарсит ~8000 названий фильмов, ссылок на них и скачает столько же постеров к ним
// но тогда придётся перебилдить .py файл в .exe и закинуть его в директорию этого проекта
// после парсинга новые данные начинают перегоняться из текстовых файлов в вектора и параллельно изображения постеров перегоняются в .BMP
// после чего процесс убивается и перерисовывается UI
DWORD WINAPI UpdateData(LPVOID lpParameter) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    TCHAR p[] = TEXT("package\\trendapps\\parser.exe");

    string str;
    wstring widestr;
    const wchar_t* widecstr;

    CreateProcess(NULL, p, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    WaitForSingleObject(pi.hProcess, INFINITE);
    dp->updateData();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1;
}