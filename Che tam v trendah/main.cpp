#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <memory>
#include "DataProcessing.h"
#include "resource.h"
#include "MainDefinitions.h"

shared_ptr<DataProcessing> dp(new DataProcessing);

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR args, _In_ int ncmdshow) {
	WNDCLASS MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWndClass", MainProcedure);

	if (!RegisterClassW(&MainClass)) { return -1; }
	MSG MainMessage = { 0 };

	CreateWindow(L"MainWndClass", L"Che tam v trendah?", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 335, 37, 1250, 1005, NULL, NULL, NULL, NULL);
	MessageBox(nullptr, L"В связи с блокировкой ресурсов\nдля корректной работы приложения\nвам может понадобиться VPN", L"Attention", MB_OK);

	while (GetMessage(&MainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}
	TerminateThread(dp->read_thread, 0);
	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;
	NWC.hbrBackground = CreatePatternBrush((HBITMAP)LoadImage(NULL, TEXT("package\\trendapps\\back_img.bmp"), 0, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));

	return NWC;
}

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	HDC hdc = (HDC)wp;
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
		case OnUpdateClicked:
			dp->read_thread = CreateThread(NULL, 0, UpdateData, NULL, 0, NULL);
			dp->UpdatePage(dp->page_number, hWnd);
			break;
		case OnPrevClicked:
			if (dp->page_number != 0) {
				dp->page_number -= 18;
				dp->UpdatePage(dp->page_number, hWnd);
			}
			break;
		case OnNextClicked:
			if (dp->page_number != 90) {
				dp->page_number += 18;
				dp->UpdatePage(dp->page_number, hWnd);
			}
			break;
		case OnExitProgram:
			PostQuitMessage(0);
			break;
		case one:
			dp->OpenUrl(one);
			break;
		case two:
			dp->OpenUrl(two);
			break;
		case three:
			dp->OpenUrl(three);
			break;
		case fhour:
			dp->OpenUrl(fhour);
			break;
		case five:
			dp->OpenUrl(five);
			break;
		case six:
			dp->OpenUrl(six);
			break;
		case seven:
			dp->OpenUrl(seven);
			break;
		case eight:
			dp->OpenUrl(eight);
			break;
		case nine:
			dp->OpenUrl(nine);
			break;
		case ten:
			dp->OpenUrl(ten);
			break;
		case eleven:
			dp->OpenUrl(eleven);
			break;
		case twelve:
			dp->OpenUrl(twelve);
			break;
		case thirteen:
			dp->OpenUrl(thirteen);
			break;
		case fourteen:
			dp->OpenUrl(fourteen);
			break;
		case fifteen:
			dp->OpenUrl(fifteen);
			break;
		case sixteen:
			dp->OpenUrl(sixteen);
			break;
		case seventeen:
			dp->OpenUrl(seventeen);
			break;
		case eighteen:
			dp->OpenUrl(eighteen);
			break;
		default: break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wp, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 0)); // 3, 87, 255
		return (LRESULT)GetStockObject(NULL_BRUSH);
	case WM_SIZE:
		dp->setPosition(hWnd);
		dp->UpdatePage(dp->page_number, hWnd);
		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CREATE:
		dp->StartProc(hWnd);
		break;
	case WM_DESTROY:
		CloseHandle(dp->read_thread);
		ExitThread(0);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}