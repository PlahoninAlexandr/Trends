#include "DataProcessing.h"
#pragma warning(disable: 4996)


// подгоняет строку пути к изображению под нужный тип и передаёт в LoadSaveImg
//void DataProcessing::convertToBmp() {
//    for (int i = 0, j = 1; i < path[0].second.size(); ++i, ++j) {
//        str = "package\\Images\\" + to_string(j) + ".bmp";
//        strcpy(Buffer, str.c_str());
//        widestr = std::wstring(str.begin(), str.end());
//        widecstr = widestr.c_str();
//        LoadSaveImg(widecstr, Buffer);
//    }
//}

// перегоняет изображение из любого формата в .BMP
//HBITMAP DataProcessing::LoadSaveImg(LPCTSTR pszFileName, char outName[MAX_PATH]) {
//    HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
//    DWORD dwFileSize = GetFileSize(hFile, NULL);
//    HGLOBAL hGlobalA = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
//    LPVOID pvData = GlobalLock(hGlobalA);
//    DWORD dwBytesRead = 0;
//    BOOL bReadFile = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
//    GlobalUnlock(hGlobalA);
//    CloseHandle(hFile);
//    LPSTREAM pst = NULL;
//    HRESULT hr = CreateStreamOnHGlobal(hGlobalA, TRUE, &pst);
//    if (pImage) pImage->Release();
//    hr = OleLoadPicture(pst, dwFileSize, FALSE, IID_IPicture, (LPVOID*)&pImage);
//    pst->Release();
//    WCHAR fBuff[MAX_PATH];
//    mbstowcs(fBuff, outName, sizeof(fBuff) / sizeof(WCHAR));
//    OleSavePictureFile(pImage, fBuff);
//    return (HBITMAP)1;
//}

// расчитываются проценты отступов от сторон для кнопок и статиков ради адаптивного интерфейса
// создаёт кнопки, статики и ставит на кнопки изображения
void DataProcessing::MainWndAddWidgets(HWND& hWnd) {
    HMENU RootMenu = CreateMenu();
    AppendMenu(RootMenu, MF_STRING, OnUpdateClicked, L"Update library");
    AppendMenu(RootMenu, MF_STRING, OnPrevClicked, L"Previous page");
    AppendMenu(RootMenu, MF_STRING, OnNextClicked, L"Next page");
    AppendMenu(RootMenu, MF_STRING, OnExitProgram, L"Exit");
    SetMenu(hWnd, RootMenu);

    GetClientRect(hWnd, &rc);
    first_ident = (rc.right * (3.0 / 100.0));
    second_ident = (rc.right * (19.0 / 100.0));
    third_ident = (rc.right * (35.0 / 100.0));
    fourth_ident = (rc.right * (51.0 / 100.0));
    fifth_ident = (rc.right * (67.0 / 100.0));
    sixth_ident = (rc.right * (83.0 / 100.0));

    extra_height = 10.0;
    amount_of_elements = path[0].second.size();
    num = 1;

    for (int i = 0; i < amount_of_elements / 6; ++i, ++num) {
        if (num == 1) {
            CreateControls(i, str, first_ident, hWnd);
        }
        else if (num == 2) {
            CreateControls(i, str, second_ident, hWnd);
        }
        else if (num == 3) {
            CreateControls(i, str, third_ident, hWnd);
        }
        else if (num == 4) {
            CreateControls(i, str, fourth_ident, hWnd);
        }
        else if (num == 5) {
            CreateControls(i, str, fifth_ident, hWnd);
        }
        else if (num == 6) {
            CreateControls(i, str, sixth_ident, hWnd);
            extra_height += 300.0;
            num = 0;
        }
    }

    for (int i = 0; i < amount_of_elements; ++i) {
        str = processed_data[i].second.second;
        widestr = std::wstring(str.begin(), str.end());
        widecstr = widestr.c_str();
        bitmaps.push_back(
            (HBITMAP)LoadImage(NULL, widecstr, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE)
        );
    }

    for (int i = 0; i < amount_of_elements / 6; ++i) {
        SendMessage(buttons[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[i]);
    }
}

// создаёт кнопки и статики в MainWndAddWidgets
void DataProcessing::CreateControls(int& i, std::string& str, double& ident, HWND hWnd) {
    buttons.push_back(
        CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rc.left + ident, rc.top + extra_height, 166, 250, hWnd, (HMENU)i, GetModuleHandle(NULL), NULL)
    );
    str = processed_data[i].first;
    strcpy(Buffer, str.c_str());
    statics.push_back(
        CreateWindowA("static", Buffer, WS_VISIBLE | WS_CHILD | ES_LEFT, rc.left + ident, rc.top + extra_height + 250, 166, 60, hWnd, NULL, NULL, NULL)
    );
}

// асинхронно считывает названия фильмов и ссылки на них из файлов и загоняет в вектора создавая структуру со всеми данными
void DataProcessing::updateData() {
    path.clear();
    futures.clear();
    path.push_back({ "package\\trendapps\\title.txt", title });
    path.push_back({ "package\\trendapps\\link.txt", link });

    // параллельно считываю данные с 2 файлов и закидываю в вектора
    for (auto& i : path) {
        futures.push_back(
            std::async(std::launch::async, [&]() {
                ifstream base(i.first, ios::binary);

                if (base.is_open()) {
                    string line;

                    while (getline(base, line)) {
                        i.second.push_back(line);
                    }
                }

                base.close();
                })
        );
    }

    // жду пока считаются все файлы
    for (auto& i : futures) {
        i.get();
    }

    for (string& str : path[0].second) {
        auto pos = str.find_first_of('/');
        if(pos != str.npos) str.erase(pos);
    }

    // собрал из всех векторов один бутер в виде (НАЗВАНИЕ ФИЛЬМА : ССЫЛКА НА ФИЛЬМ, ПУТЬ К ПОСТЕРУ)
    for (int i = 0, j = 1; i < path[0].second.size(); ++i, ++j) {
        processed_data.push_back({ move(path[0].second[i]), {move(path[1].second[i]), "package\\Images\\" + to_string(j) + ".bmp"} });
    }
}

// по нажатии кнопки открывает фильм в дефолтном браузере
void DataProcessing::OpenUrl(const int& url) {
    str = processed_data[url + page_number].second.first;
    widestr = wstring(str.begin(), str.end());
    widecstr = widestr.c_str();
    ShellExecute(NULL, NULL, widecstr, NULL, NULL, SW_SHOWNORMAL);
}

// подтягиваем данные из файлов и рисуем UI
void DataProcessing::StartProc(HWND& hWnd) {
    updateData();
    MainWndAddWidgets(hWnd);
}

// меняет расположение кнопок при изменении размера окна и помогает картинкам не подвисать обновляя их дважды через статик
void DataProcessing::setPosition(HWND& hWnd) {
    GetClientRect(hWnd, &rc);
    first_ident = (rc.right * (3.0 / 100.0));
    second_ident = (rc.right * (19.0 / 100.0));
    third_ident = (rc.right * (35.0 / 100.0));
    fourth_ident = (rc.right * (51.0 / 100.0));
    fifth_ident = (rc.right * (67.0 / 100.0));
    sixth_ident = (rc.right * (83.0 / 100.0));

    extra_height = 10.0;
    num = 1;
    for (int i = 0; i < amount_of_elements / 6; ++i, ++num) {
        if (num == 1) {
            SetWindowPos(buttons[i], NULL, rc.left + first_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + first_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
        }
        else if (num == 2) {
            SetWindowPos(buttons[i], NULL, rc.left + second_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + second_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
        }
        else if (num == 3) {
            SetWindowPos(buttons[i], NULL, rc.left + third_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + third_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
        }
        else if (num == 4) {
            SetWindowPos(buttons[i], NULL, rc.left + fourth_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + fourth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
        }
        else if (num == 5) {
            SetWindowPos(buttons[i], NULL, rc.left + fifth_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + fifth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
        }
        else if (num == 6) {
            SetWindowPos(buttons[i], NULL, rc.left + sixth_ident, rc.top + extra_height, 166, 250, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SetWindowPos(statics[i], NULL, rc.left + sixth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            SendMessage(statics[i], WM_SETFONT, (WPARAM)font, NULL);
            extra_height += 312.0;
            num = 0;
        }

    }

    extra_height = 10.0;
    num = 1;
    for (int i = 0; i < amount_of_elements / 6; ++i, ++num) {
        if (num == 1) {
            SetWindowPos(statics[i], NULL, rc.left + first_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        else if (num == 2) {
            SetWindowPos(statics[i], NULL, rc.left + second_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        else if (num == 3) {
            SetWindowPos(statics[i], NULL, rc.left + third_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        else if (num == 4) {
            SetWindowPos(statics[i], NULL, rc.left + fourth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        else if (num == 5) {
            SetWindowPos(statics[i], NULL, rc.left + fifth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        else if (num == 6) {
            SetWindowPos(statics[i], NULL, rc.left + sixth_ident, rc.top + extra_height + 250, 166, 60, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            extra_height += 312.0;
            num = 0;
        }

    }
}

// отрисовывает новые статики и картинки при переключении страницы
void DataProcessing::UpdatePage(const int& page, HWND& hWnd) {
    for (int i = page; i < ((amount_of_elements / 6) + page); ++i) {
        str = processed_data[i].first;
        strcpy(Buffer, str.c_str());
        SetWindowTextA(statics[i - page], Buffer);
        SendMessage(statics[i - page], WM_SETFONT, (WPARAM)font, NULL);
        SendMessage(buttons[i - page], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[i]);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}