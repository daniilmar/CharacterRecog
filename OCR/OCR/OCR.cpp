#include <windows.h>
#include <tchar.h>
#include <opencv2/opencv.hpp>
#include "Classificator.h"

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("OCR");
HINSTANCE hInst;
bool camera = false;
bool file = false;
static char FileName[256];
Mat sample;
const string Class[11] = {"А","Б","В","Е","Н","О","П","Р","Т","У","Х"};
Classificator *predictor;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool Open(HWND hWnd, Mat &frame);
bool ReadFromCamera(Mat &frame);
void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex)){
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}
	hInst = hInstance;
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 410, 330, NULL, NULL, hInstance, NULL);
	if (!hWnd){
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	predictor = new Classificator(false);
	int id = 0;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	/*if (camera || file){
		id=predictor->Classify(sample);
	}
	/*MessageBox(hWnd,
		"Prediction",
		(LPCSTR)(Class[id]),
		MB_OK | //Да нет (кнопки)
		MB_DEFAULT_DESKTOP_ONLY
		);*/
	delete predictor;
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	static HBITMAP hBitmap = NULL;

	switch (message){
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 400, 300, LR_LOADFROMFILE);
		if (!hBitmap){
			MessageBox(hWnd, "LoadImage(): failed.", NULL, MB_OK | MB_ICONSTOP);
			PostQuitMessage(0);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBitmap(hdc, 0, 0, hBitmap);
		EndPaint(hWnd, &ps);
		break;
	
	case WM_LBUTTONDOWN:{
		int x = LOWORD(lParam), y = HIWORD(lParam);
		//if ((x - 110)*(x - 110) + (y - 170)*(y - 170) <= 4900){
		//	if (ReadFromCamera(sample)){
		//		camera = true;
		//		int id = predictor->Classify(sample);
		//		MessageBox(hWnd,
		//			Class[id-1].c_str(),
		//			"Prediction",
		//			MB_OK);
		//	}
		//	//DestroyWindow(hWnd);
		//	break;
		//}
		if ((x - 290)*(x - 290) + (y - 170)*(y - 170) <= 4900){
			if (Open(hWnd, sample)){
				file = true;
				int id = predictor->Classify(sample);
				MessageBox(hWnd,
					Class[id-1].c_str(),
					"Prediction",
					MB_OK);
				//DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

bool Open(HWND hWnd, Mat& frame){
	OPENFILENAME ofn;
	FileName[0] = 0;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "png файлы\0*.png\0jpeg файлы\0*.jpeg;*.jpg\0bmp файлы\0*.bmp";
	//ofn.lpstrDefExt = "avi";
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_NOCHANGEDIR;
	if (!GetOpenFileName(&ofn)){
		return false;
	}
	frame = imread(FileName);
	return true;
}

bool ReadFromCamera(Mat &frame){
	VideoCapture capture;
	capture.open(CV_CAP_ANY);
	if (!capture.isOpened()) return false;
	namedWindow("Take photo", CV_WINDOW_AUTOSIZE);
	bool play = true;
	while (play){
		capture >> frame;
		if (!frame.data) {
			break;
		}
		imshow("Take Photo", frame);
		char c = waitKey(33);
		if (c == 27) { // если нажата ESC - выходим
			return false;
		}
		if (c == ' ' || c == 13) { // делаем фото
			play = !play;
		}
	}
	destroyWindow("Take photo");
	destroyAllWindows();
	return true;
}

void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap){
	HBITMAP  hOldbm;
	HDC hMemDC;
	BITMAP bm;
	POINT ptSize, ptOrg;
	hMemDC = CreateCompatibleDC(hDC);
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);
	if (hOldbm){
		SetMapMode(hMemDC, GetMapMode(hDC));
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
		ptSize.x = bm.bmWidth;  // ширина
		ptSize.y = bm.bmHeight; // высота
		DPtoLP(hDC, &ptSize, 1);
		ptOrg.x = 0;
		ptOrg.y = 0;
		DPtoLP(hMemDC, &ptOrg, 1);
		BitBlt(hDC, x, y, ptSize.x, ptSize.y, hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
		SelectObject(hMemDC, hOldbm);
	}
	DeleteDC(hMemDC);
}
