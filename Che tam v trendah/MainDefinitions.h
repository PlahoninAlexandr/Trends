#pragma once
#include "DataProcessing.h"

extern shared_ptr<DataProcessing> dp;

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

// ���������� ��� ������� ������ Update library � ������ ������� � �������� ���������� �� python, ��� ����� ����������� �� 108 �������,
// �� ����� ��� ������ � ����� �� �������� ~8000 �������� �������, ������ �� ��� � ������� ������� �� �������� � ���
// �� ����� ������� ����������� .py ���� � .exe � �������� ��� � ���������� ����� �������
// ����� �������� ����� ������ �������� ������������ �� ��������� ������ � ������� � ����������� ����������� �������� ������������ � .BMP
// ����� ���� ������� ��������� � ���������������� UI
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