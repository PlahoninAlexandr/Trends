#pragma once
#include <Windows.h>
#include <olectl.h>
#include <string>
#include <vector>
#include <future>
#include <fstream>

#define one 0
#define two 1
#define three 2
#define fhour 3
#define five 4
#define six 5
#define seven 6
#define eight 7
#define nine 8
#define ten 9
#define eleven 10
#define twelve 11
#define thirteen 12
#define fourteen 13
#define fifteen 14
#define sixteen 15
#define seventeen 16
#define eighteen 17
#define OnUpdateClicked 18
#define OnPrevClicked 19
#define OnNextClicked 20
#define OnExitProgram 22
#define Buffer_Size 1024

using namespace std;

class DataProcessing {
	vector<pair<string, pair<string, string>>> processed_data;
	vector<string> title, link, img;
	vector<pair<string, vector<string>>> path;
	vector<future<void>> futures;
	vector<HWND> buttons;
	vector<HWND> statics;
	vector<HBITMAP> bitmaps;

	LPDISPATCH pImage;
	RECT rc{};

	char Buffer[Buffer_Size];
	bool Flag = 0;
	double extra_height, first_ident, second_ident, third_ident, fourth_ident, fifth_ident, sixth_ident;
	int amount_of_elements, num;
	string str;
	wstring widestr;
	const wchar_t* widecstr;

	//HBITMAP LoadSaveImg(LPCTSTR pszFileName, char outName[MAX_PATH]);
	void CreateControls(int& i, string& str, double& ident, HWND hWnd);
	void MainWndAddWidgets(HWND& hWnd);

public:
	HANDLE read_thread;
	PAINTSTRUCT ps;
	HFONT font = CreateFont(21, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));

	int page_number = 0;

	void updateData();
	//void convertToBmp();
	void OpenUrl(const int& url);
	void StartProc(HWND& hWnd);
	void setPosition(HWND& hWnd);
	void UpdatePage(const int& page, HWND& hWnd);
};