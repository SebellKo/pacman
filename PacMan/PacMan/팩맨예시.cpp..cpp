
// PacMan.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "PacMan.h"
#include <stdio.h>
#include <string.h>
#include <gl.h>
#include <glu.h>
#include <math.h>
#include <glut.h>
#include <string>
#include <Windows.h>
#include <time.h>

using namespace std;

#define IDT_TIMER   1

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];             // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

////////////////////// sk
HDC hDeviceContext;                        // current device context
HGLRC hRenderingContext;                  // current rendering context

bool bSetupPixelFormat(HDC hdc);
void Resize(int width, int height);
void DrawScene(HDC MyDC);

void InitStage(void);

bool lose_flag = 0;
bool win_flag = 0;
bool answer_flag = 0;

bool DoCollide(void);
//float centerPos[2][2] = { { 0.0f, 0.0f }, { 0.2f, 0.2f } };
float moveDirection[2][2] = { { 0.05f, 0.0f }, { 0.0f, 0.05f } };

void SetItems(void);
bool itemSettingFlag = 0;

void EatItems();

void DrawScore(float posX, float posY);
void DrawMaze(void);
void DrawPacman(void);
void DrawItems(void);
void DrawGhost(void);
void DrawGhost2(void);

float pacman_pos[2] = { 0.1f, 0.1f };
int pacman_pos_rel[2] = { 4, 5 }; // �Ѹ� ��ġ(row, col)
int pacman_dir = 0; //0 = ����, 1 = ������, 2 = ����, 3 = �Ʒ���
int pacman_next_dir = 0; //0 = ����, 1 = ������, 2 = ����, 3 = �Ʒ���
bool pacman_move_stat = 0; //0 = ����, 1 = ������ Ȱ��ȭ
void MovePacman(void);
bool pacman_opmouth_flag = 1; //0=�� �ٹ���, 1=�� ������
void OpenPacmanMouth(void);
int rotAngle[4] = { 180, 0, 90, 270 };
bool bRight = true;

void SetPacmanPosRel(void);
int move_cnt = 0;

int ghost_move_cnt = 0;
int ghost_dir = 0; //0 = ����, 1 = ������, 2 = ����, 3 = �Ʒ���
bool set_ghost_new_dir_flag = 0;
void SetGhostDir(void);
bool DoGhostCollide(void);

float ghost_pos[2] = { 0.1f, 0.7f };
int ghost_pos_rel[2] = { 1, 5 }; // ��Ʈ ��ġ(row, col)
float ghost_move = 0.05f;
void MoveGhost(void);
bool DoGhostPacmanCollide(void);
void SetGhostPosRel(void);
bool ghost_move_stat = 1;

//ghost2
int ghost2_move_cnt = 0;
int ghost2_dir = 0; //0 = ����, 1 = ������, 2 = ����, 3 = �Ʒ���
bool set_ghost2_new_dir_flag = 0;
void SetGhost2Dir(void);
bool DoGhost2Collide(void);

float ghost2_pos[2] = { -0.3f, -0.3f };
int ghost2_pos_rel[2] = { 6, 3 }; // ��Ʈ ��ġ(row, col)
float ghost2_move = 0.05f;
void MoveGhost2(void);
bool DoGhost2PacmanCollide(void);
void SetGhost2PosRel(void);
bool ghost2_move_stat = 1;

int cnt2 = 0;

//

bool items[10][10];
int num_items = 0, total_items = 0;

float wall_width = 0.2f;
bool maze[10][10] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                      { 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
                      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                      { 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
                      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                      { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };


GLubyte wall_pattern[] = { 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe,
                           0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
                           0xff, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };




/////////////////////////////

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0; // MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.


    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT rect;

    switch (message) {
    case WM_CREATE:
        srand(time(NULL));
        // Initialize for the OpenGL rendering
        hDeviceContext = GetDC(hWnd);
        if (!bSetupPixelFormat(hDeviceContext)) {
            MessageBox(hWnd, "Error in setting up pixel format for OpenGL", "Error", MB_OK | MB_ICONERROR);
            DestroyWindow(hWnd);
        }
        hRenderingContext = wglCreateContext(hDeviceContext);
        wglMakeCurrent(hDeviceContext, hRenderingContext);

        // Create a timer
        SetTimer(hWnd, IDT_TIMER, 50, NULL);

        // Use font bitmaps
        SelectObject(hDeviceContext, GetStockObject(SYSTEM_FONT));
        wglUseFontBitmaps(hDeviceContext, 0, 255, 1000);
        glListBase(1000);

        break;
        /*
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        */
    case WM_PAINT:
    {
        if (lose_flag == 1 && answer_flag == 0) {
            answer_flag = 1;
            float pacman_pos[2] = { 0.1f, 0.1f };
            float ghost_pos[2] = { 0.1f, 0.7f };
            KillTimer(hWnd, IDT_TIMER);
            if (MessageBox(NULL, "You Lose.\nDo you want to play again?", "Lose", MB_OKCANCEL) == IDOK) {
                InitStage();
                SetTimer(hWnd, IDT_TIMER, 50, NULL);

            }
            else {
                PostQuitMessage(0);
            }

        }
        else if (win_flag == 1 && answer_flag == 0) {
            answer_flag = 1;
            float pacman_pos[2] = { 0.1f, 0.1f };
            float ghost_pos[2] = { 0.1f, 0.7f };
            KillTimer(hWnd, IDT_TIMER);
            if (MessageBox(NULL, "You Win.\nDo you want to play again?", "Win", MB_OKCANCEL) == IDOK) {
                InitStage();
                SetTimer(hWnd, IDT_TIMER, 50, NULL);

            }
            else {
                PostQuitMessage(0);
            }


        }
        else DrawScene(hDeviceContext);
        ValidateRect(hWnd, NULL);
    }
    break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        Resize(rect.right, rect.bottom);

        InvalidateRect(hWnd, NULL, false);

        break;

    case WM_DESTROY:
        if (hRenderingContext)
            wglDeleteContext(hRenderingContext);
        if (hDeviceContext)
            ReleaseDC(hWnd, hDeviceContext);

        // Destroy a timer
        KillTimer(hWnd, IDT_TIMER);

        // Use font bitmaps
        glDeleteLists(1000, 255);

        PostQuitMessage(0);
        break;

    case WM_TIMER:
        switch (wParam) {
        case IDT_TIMER:
            //DoCollide();
            if (pacman_move_stat == 1) { //�������� Ȱ��ȭ�� ����
                if (!(move_cnt == 0 && DoCollide() == true)) {
                    MovePacman();
                    if (num_items == 102) win_flag = 1;
                    OpenPacmanMouth();
                }

            }
            MoveGhost();
            MoveGhost2();

            InvalidateRect(hWnd, NULL, false);
            break;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            if (pacman_move_stat == 0) pacman_dir = 0;
            pacman_next_dir = 0;
            pacman_move_stat = 1;
            //pacman_pos[0] -= 0.2f;
            InvalidateRect(hWnd, NULL, false);
            break;
        case VK_RIGHT:
            if (pacman_move_stat == 0) pacman_dir = 1;
            pacman_next_dir = 1;
            pacman_move_stat = 1;
            //pacman_pos[0] += 0.2f;
            InvalidateRect(hWnd, NULL, false);
            break;
        case VK_UP:
            if (pacman_move_stat == 0) pacman_dir = 2;
            pacman_next_dir = 2;
            pacman_move_stat = 1;
            //pacman_pos[1] += 0.2f;
            InvalidateRect(hWnd, NULL, false);
            break;
        case VK_DOWN:
            if (pacman_move_stat == 0) pacman_dir = 3;
            pacman_next_dir = 3;
            pacman_move_stat = 1;
            //pacman_pos[1] -= 0.2f;
            InvalidateRect(hWnd, NULL, false);
            break;
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;

        default:
            break;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool bSetupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    int pixelformat;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;

    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0) {
        MessageBox(NULL, "ChoosePixelFormat() failed!!!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) {
        MessageBox(NULL, "SetPixelFormat() failed!!!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

void Resize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int cx = width;
    int cy = height;
    glViewport(0, 0, cx, cy);

    // 3D orthographic viewing
    if (cx <= cy) {
        float ratio = (float)cy / (float)cx;
        glOrtho(-1.0, 1.0, -ratio, ratio, -1.0, 1.0);
    }
    else {
        float ratio = (float)cx / (float)cy;
        glOrtho(-ratio, ratio, -1.0, 1.0, -1.0, 1.0);
    }

    return;

}

void DrawScene(HDC MyDC) {
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ��Ʈ �׸���
    DrawGhost();

    // �ι�° ��Ʈ �׸���

    DrawGhost2();

    // �Ѹ� �׸���
    DrawPacman();

    // ������ �׸���
    SetItems();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawItems();


    // �̷� �׸���
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawMaze();

    // ���� ��Ÿ����
    glColor3f(0.0f, 0.0f, 0.0f);
    DrawScore(1.1f, 0.8f);


    SwapBuffers(MyDC);

    return;
}

bool DoCollide(void) {
    int next_pacman_pos_rel[2] = { pacman_pos_rel[0],pacman_pos_rel[1] };
    if (pacman_move_stat == 0) {

    }
    else if (pacman_move_stat == 1) {
        switch (pacman_next_dir) {
        case 0: // ����
            next_pacman_pos_rel[1] -= 1;
            break;
        case 1: // ������
            next_pacman_pos_rel[1] += 1;
            break;
        case 2: // ����
            next_pacman_pos_rel[0] -= 1;
            break;
        case 3: // �Ʒ���
            next_pacman_pos_rel[0] += 1;
            break;
        default:
            break;
        }
    }

    if (maze[next_pacman_pos_rel[0]][next_pacman_pos_rel[1]] == 1) {
        pacman_move_stat = 0;
        return true;
    }
    else return false;
}

void SetPacmanPosRel(void) {
    switch (pacman_dir) {
    case 0: // ����
        pacman_pos_rel[1] -= 1;
        break;
    case 1: // ������
        pacman_pos_rel[1] += 1;
        break;
    case 2: // ����
        pacman_pos_rel[0] -= 1;
        break;
    case 3: // �Ʒ���
        pacman_pos_rel[0] += 1;
        break;
    default:
        break;
    }
}

void EatItems(void) {
    if (items[pacman_pos_rel[0]][pacman_pos_rel[1]] == 1) {
        items[pacman_pos_rel[0]][pacman_pos_rel[1]] = 0;
        int row = pacman_pos_rel[0];
        int col = pacman_pos_rel[1];
        if ((row + col) % 7 == 5 xor (row - col) * (row - col) % 25 == 0) {
            num_items += 5;
        }
        else num_items += 1;
    }
}

void SetItems(void) {
    if (itemSettingFlag == 0) { // �ѹ��� �����.
        itemSettingFlag = 1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (maze[i][j] == 0) {
                    items[i][j] = 1;
                }
                else items[i][j] = 0;
            }
        }
    }
}

void DrawItems(void) {
    glEnable(GL_POLYGON);
    float x0 = -1.0f, y0 = 1.0f; // start from left-top
    glColor3f(0.8f, 0.4f, 0.7f);
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            if (!((row + col) % 7 == 5 xor (row - col) * (row - col) % 25 == 0)) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                if (items[row][col] == 1) {
                    glColor3f(0.8f, 0.4f, 0.7f);
                    glRectf(x0 + 0.08f, y0 - 0.08f, x0 + 0.12f, y0 - 0.12f);
                }
            }
            else {
                if (items[row][col] == 1) {
                    //glRectf(x0 + 0.08f, y0 - 0.08f, x0 + 0.12f, y0 - 0.12f);
                    double rad = 0.15;
                    double x[360], y[360];
                    //for (int i = 0; i < 360; i++) {
                    //    double angle;
                    //    angle = i * 3.141592 / 180;
                    //    x[i] = 0.35 * rad * cos(angle);
                    //    y[i] = 0.5 * rad * sin(angle);
                    //}

                    rad = 0.07;
                    for (int i = 0; i < 360; i++) {
                        double angle;
                        angle = i * 3.141592 / 180;
                        x[i] = 0.35 * rad * cos(angle);
                        y[i] = 0.5 * rad * sin(angle);
                    }
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glBegin(GL_POLYGON);
                    for (int i = 00; i < 360; i++) {
                        glVertex2f(x[i] + x0 + 0.1, y[i] + y0 - 0.1);
                        glVertex2f(x[i + 1] + x0 + 0.1, y[i + 1] + y0 - 0.1);
                    }
                    glEnd();

                    rad = 0.1;
                    for (int i = 0; i < 360; i++) {
                        double angle;
                        angle = i * 3.141592 / 180;
                        x[i] = 0.35 * rad * cos(angle);
                        y[i] = 0.5 * rad * sin(angle);
                    }
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glBegin(GL_POLYGON);
                    for (int i = 00; i < 360; i++) {
                        glVertex2f(x[i] + x0 + 0.1, y[i] + y0 - 0.1);
                        glVertex2f(x[i + 1] + x0 + 0.1, y[i + 1] + y0 - 0.1);
                    }
                    glEnd();

                    rad = 0.07;
                    for (int i = 0; i < 360; i++) {
                        double angle;
                        angle = i * 3.141592 / 180;
                        x[i] = 0.35 * rad * cos(angle);
                        y[i] = 0.5 * rad * sin(angle);
                    }
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glColor3f(0.8f, 0.8f, 0.2f);
                    glBegin(GL_POLYGON);
                    for (int i = 00; i < 360; i++) {
                        glVertex2f(x[i] + x0 + 0.1, y[i] + y0 - 0.1);
                        glVertex2f(x[i + 1] + x0 + 0.1, y[i + 1] + y0 - 0.1);
                    }
                    glEnd();

                    rad = 0.1;
                    for (int i = 0; i < 360; i++) {
                        double angle;
                        angle = i * 3.141592 / 180;
                        x[i] = 0.35 * rad * cos(angle);
                        y[i] = 0.5 * rad * sin(angle);
                    }
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glColor3f(1.0f, 1.0f, 0.2f);
                    glBegin(GL_POLYGON);
                    for (int i = 00; i < 360; i++) {
                        glVertex2f(x[i] + x0 + 0.1, y[i] + y0 - 0.1);
                        glVertex2f(x[i + 1] + x0 + 0.1, y[i + 1] + y0 - 0.1);
                    }
                    glEnd();

                }
            }
            x0 += wall_width;
        }
        x0 = -1.0f;
        y0 -= wall_width;
    }
    glDisable(GL_POLYGON);

    return;
}

void DrawPacman(void) {
    double rad = 0.1;
    double x[360], y[360];
    for (int i = 0; i < 360; i++) {
        double angle;
        angle = i * 3.141592 / 180;
        x[i] = rad * cos(angle);
        y[i] = rad * sin(angle);
    }

    //glColor3f(0.0f, 0.0f, 1.0f);
    //glBegin(GL_POLYGON);
    //glVertex2f(0, 0);
    //glVertex2f(0, 0.01);
    //glVertex2f(0.01, 0);
    //glEnd();

    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(pacman_pos[0], pacman_pos[1], 0);
    glRotatef(rotAngle[pacman_dir], 0, 0, 1);
    glTranslatef(-pacman_pos[0], -pacman_pos[1], 0);
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(pacman_pos[0], pacman_pos[1]);
    if (pacman_opmouth_flag == 0) {
        for (int i = 00; i < 360; i++) {
            glRotatef(90, 0, 0, 1);
            glVertex2f(x[i] + pacman_pos[0], y[i] + pacman_pos[1]);
            glVertex2f(x[i + 1] + pacman_pos[0], y[i + 1] + pacman_pos[1]);
        }
    }
    else if (pacman_opmouth_flag == 1) {
        for (int i = 30; i < 330; i++) {
            glRotatef(90, 0, 0, 1);
            glVertex2f(x[i] + pacman_pos[0], y[i] + pacman_pos[1]);
            glVertex2f(x[i + 1] + pacman_pos[0], y[i + 1] + pacman_pos[1]);
        }
    }
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(pacman_pos[0], pacman_pos[1]);
    if (pacman_opmouth_flag == 0) {
        for (int i = 00; i < 360; i++) {
            glRotatef(90, 0, 0, 1);
            glVertex2f(x[i] + pacman_pos[0], y[i] + pacman_pos[1]);
            glVertex2f(x[i + 1] + pacman_pos[0], y[i + 1] + pacman_pos[1]);
        }
    }
    else if (pacman_opmouth_flag == 1) {
        for (int i = 30; i < 330; i++) {
            glRotatef(90, 0, 0, 1);
            glVertex2f(x[i] + pacman_pos[0], y[i] + pacman_pos[1]);
            glVertex2f(x[i + 1] + pacman_pos[0], y[i + 1] + pacman_pos[1]);
        }
    }
    glEnd();
    glPopMatrix();

    //string xtest = to_string(rad);
    //OutputDebugString(xtest.c_str());

    return;
}

void DrawMaze(void) {
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(wall_pattern);

    float x0 = -1.0f, y0 = 1.0f; // start from left-top
    glColor3f(0.5f, 0.0f, 0.0f);
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            if (maze[row][col]) {
                glRectf(x0, y0, x0 + wall_width, y0 - wall_width);
            }
            x0 += wall_width;
        }
        x0 = -1.0f;
        y0 -= wall_width;
    }
    glDisable(GL_POLYGON_STIPPLE);

    return;
}

void DrawScore(float posX, float posY) {
    // Use font bitmaps
    char str[256];
    sprintf_s(str, "Score: %2d", num_items);

    glRasterPos2f(posX, posY);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);

    glColor3f(0.75f, 0.75f, 0.75f);
    glRectf(posX - 0.02f, posY - 0.02f, posX + 0.23, posY + 0.05f);

    return;
}

void MovePacman(void) {
    move_cnt += 1;
    switch (pacman_dir) {
    case 0: // ����
        pacman_pos[0] -= 0.02f;
        break;
    case 1: // ������
        pacman_pos[0] += 0.02f;
        break;
    case 2: // ����
        pacman_pos[1] += 0.02f;
        break;
    case 3: // �Ʒ���
        pacman_pos[1] -= 0.02f;
        break;
    default:
        break;
    }
    if (move_cnt == 10) {
        SetPacmanPosRel();
        DoCollide();
        move_cnt = 0;
        pacman_dir = pacman_next_dir;
        EatItems();
    }
}
void OpenPacmanMouth(void) {
    if (move_cnt == 5) {
        pacman_opmouth_flag = 0;
    }
    if (move_cnt == 0) {
        pacman_opmouth_flag = 1;
    }
}


void DrawGhost(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.8f);
    glVertex2f(ghost_pos[0] - 0.05f, ghost_pos[1] - 0.05f);
    glVertex2f(ghost_pos[0] - 0.05f, ghost_pos[1] + 0.05f);
    glVertex2f(ghost_pos[0] + 0.05f, ghost_pos[1] + 0.05f);
    glVertex2f(ghost_pos[0] + 0.05f, ghost_pos[1] - 0.05f);
    glEnd();
}

void MoveGhost(void) {
    if (ghost_move_stat == 0) return;
    ghost_move_cnt += 1;

    // ���ⱸ���� ���� �缳��
    if (ghost_move_cnt == 1) {
        if ((ghost_pos_rel[0] == 0) && (ghost_pos_rel[1] == 1)) {
            ghost_dir = 3;
        }
        else if ((ghost_pos_rel[0] == 9) && (ghost_pos_rel[1] == 8)) {
            ghost_dir = 2;
        }
    }

    // �浹�� ���ܼ� ���ο� ���� ������ ���ϴ� �÷��װ� Ȱ��ȭ�Ǹ� ���ο� ���� ����
    if (set_ghost_new_dir_flag == 1) {
        SetGhostDir();
    }

    // ��Ʈ�� ������ �̵� �������� �̵�
    if (ghost_dir == 0) {
        ghost_pos[0] -= 0.025f;
    }
    else if (ghost_dir == 1) {
        ghost_pos[0] += 0.025f;
    }
    else if (ghost_dir == 2) {
        ghost_pos[1] += 0.025f;
    }
    else if (ghost_dir == 3) {
        ghost_pos[1] -= 0.025f;
    }
    // ��Ʈ �����ġ �缳��
    SetGhostPosRel();

    // ������ �浹 ���� // �������ڸ��ڵ� ���߿� üũ�ؾ���. ������ ���� ���������� �����ϱ�
    if (ghost_move_cnt == 8) {
        DoGhostCollide();
    }

    // �Ѹǰ��� �浹 ����
    if (DoGhostPacmanCollide() == true) {
        pacman_move_stat = 0;
        ghost_move_stat = 0;
        lose_flag = 1;
    }

    // ��Ʈ ���� ī��Ʈ(Ÿ�� ƽ)�� 8�� �Ǹ� 0���� �缳��
    if (ghost_move_cnt == 8) {
        ghost_move_cnt = 0;
    }
}

void SetGhostPosRel(void) {
    if (ghost_move_cnt == 8) {
        switch (ghost_dir) {
        case 0: // ����
            ghost_pos_rel[1] -= 1;
            break;
        case 1: // ������
            ghost_pos_rel[1] += 1;
            break;
        case 2: // ����
            ghost_pos_rel[0] -= 1;
            break;
        case 3: // �Ʒ���
            ghost_pos_rel[0] += 1;
            break;
        default:
            break;
        }
    }
}

void SetGhostDir(void) {
    if ((ghost_pos_rel[0] == 0) && (ghost_pos_rel[1] == 1)) {
        ghost_dir = 3;
    }
    else if ((ghost_pos_rel[0] == 9) && (ghost_pos_rel[1] == 8)) {
        ghost_dir = 2;
    }
    else {
        // �� ���� �߿��� �� �� �ִ� ���� �߸���
        bool checkDir[4] = { 0,0,0,0 }; // ��, ��, ��, ��
        OutputDebugString((to_string(ghost_pos_rel[0]) + to_string(ghost_pos_rel[1]) + "���� ��ġ. � �������� �����ִ��� �߸��Ŵ�\n").c_str());
        // check left
        if (maze[ghost_pos_rel[0]][ghost_pos_rel[1] - 1] == 0) {
            checkDir[0] = 1;
        }
        // check right
        if (maze[ghost_pos_rel[0]][ghost_pos_rel[1] + 1] == 0) {
            checkDir[1] = 1;
        }
        // check up
        if (maze[ghost_pos_rel[0] - 1][ghost_pos_rel[1]] == 0) {
            checkDir[2] = 1;
        }
        // check down
        if (maze[ghost_pos_rel[0] + 1][ghost_pos_rel[1]] == 0) {
            checkDir[3] = 1;
        }
        OutputDebugString((to_string(checkDir[0]) + to_string(checkDir[1]) + to_string(checkDir[2]) + to_string(checkDir[3]) + "�����ִ¹���üũ\n").c_str());
        // �� ����� �� �������� �����ϰ� �ϱ�.
        int dir = -1;
        int temp = rand() % 4;
        while (dir == (-1)) {
            if (checkDir[temp] == 1) {
                dir = temp;
                break;
            }
            temp = (temp + 1) % 4;
            string xtest = to_string(temp) + "heyhey\n";
            OutputDebugString(xtest.c_str());
        }
        ghost_dir = dir;
    }
    set_ghost_new_dir_flag = 0;
}

//int cnt = 0;
bool DoGhostCollide(void) {
    int next_ghost_pos_rel[2] = { ghost_pos_rel[0],ghost_pos_rel[1] };

    switch (ghost_dir) {
    case 0: // ����
        next_ghost_pos_rel[1] -= 1;
        break;
    case 1: // ������
        next_ghost_pos_rel[1] += 1;
        break;
    case 2: // ����
        next_ghost_pos_rel[0] -= 1;
        break;
    case 3: // �Ʒ���
        next_ghost_pos_rel[0] += 1;
        break;
    default:
        break;
    }

    if (maze[next_ghost_pos_rel[0]][next_ghost_pos_rel[1]] == 1) {
        set_ghost_new_dir_flag = 1;
        OutputDebugString((to_string(next_ghost_pos_rel[0]) + to_string(next_ghost_pos_rel[1]) + "�浹�� �����. ���� ������ �̷��ؼ�\n").c_str());
        return true;
    }
    //else return false;
    //cnt = cnt % 4;
    //cnt += 1;
    //if (cnt == 3) {
    //    set_ghost_new_dir_flag = 1;
    //}
    int randVal1 = rand() % 5;
    int randVal2 = rand() % 5;
    if (randVal1 == randVal2) {
        set_ghost_new_dir_flag = 1;
    }

    return true;
}

bool DoGhostPacmanCollide(void) {
    float distanceSquared = (pacman_pos[0] - ghost_pos[0]) * (pacman_pos[0] - ghost_pos[0]) + (pacman_pos[1] - ghost_pos[1]) * (pacman_pos[1] - ghost_pos[1]);
    if (distanceSquared < 0.02f) {
        OutputDebugString((to_string(distanceSquared) + "����").c_str());
        return true;
    }
    else return false;
}

void InitStage(void) {
    //lose_flag = 0;
    //win_flag = 0;
    //answer_flag = 0;
    //centerPos[2][2] = { { 0.0f, 0.0f }, { 0.2f, 0.2f } };
    //moveDirection[2][2] = { { 0.05f, 0.0f }, { 0.0f, 0.05f } };
    //itemSettingFlag = 0;
    //pacman_pos[2] = { 0.1f, 0.1f };
    //pacman_pos_rel[2] = { 4, 5 };
    //pacman_dir = 0;
    //pacman_next_dir = 0;
    //pacman_move_stat = 0;
    //pacman_opmouth_flag = 1;
    //rotAngle[4] = { 180, 0, 90, 270 };
    //bRight = true;
    //move_cnt = 0;
    //ghost_move_cnt = 0;
    //ghost_dir = 0;
    //set_ghost_new_dir_flag = 0;
    //ghost_pos[2] = { 0.1f, 0.7f };
    //ghost_pos_rel[2] = { 1, 5 };
    //ghost_move = 0.05f;
    //ghost_move_stat = 1;

    lose_flag = 0;
    win_flag = 0;
    answer_flag = 0;

    //centerPos[2][2] = { { 0.0f, 0.0f }, { 0.2f, 0.2f } };
    //centerPos[0][0] = 0.0f;
    //centerPos[0][1] = 0.0f;
    //centerPos[1][0] = 0.2f;
    //centerPos[1][1] = 0.2f;

    //moveDirection[2][2] = { { 0.05f, 0.0f }, { 0.0f, 0.05f } };
    moveDirection[0][0] = 0.05f;
    moveDirection[0][1] = 0.0f;
    moveDirection[1][0] = 0.0f;
    moveDirection[1][1] = 0.05f;

    itemSettingFlag = 0;

    //pacman_pos[2] = { 0.1f, 0.1f };
    pacman_pos[0] = 0.1f;
    pacman_pos[1] = 0.1f;

    //pacman_pos_rel[2] = { 4, 5 };
    pacman_pos_rel[0] = 4;
    pacman_pos_rel[1] = 5;

    pacman_dir = 0;
    pacman_next_dir = 0;
    pacman_move_stat = 0;
    pacman_opmouth_flag = 1;

    //rotAngle[4] = { 180, 0, 90, 270 };
    rotAngle[0] = 180;
    rotAngle[1] = 0;
    rotAngle[2] = 90;
    rotAngle[3] = 270;

    bRight = true;

    move_cnt = 0;

    ghost_move_cnt = 0;
    ghost_dir = 0;
    set_ghost_new_dir_flag = 0;

    //ghost_pos[2] = { 0.1f, 0.7f };
    ghost_pos[0] = 0.1f;
    ghost_pos[1] = 0.7f;

    //ghost_pos_rel[2] = { 1, 5 };
    ghost_pos_rel[0] = 1;
    ghost_pos_rel[1] = 5;

    ghost_move = 0.05f;
    ghost_move_stat = 1;

    num_items = 0, total_items = 0;

    //
    ghost2_move_cnt = 0;
    ghost2_dir = 0;
    set_ghost2_new_dir_flag = 0;

    //ghost2_pos[2] = { -0.3f, -0.3f };
    ghost2_pos[0] = -0.3f;
    ghost2_pos[1] = -0.3f;

    //ghost2_pos_rel[2] = { 6, 3 }
    ghost2_pos_rel[0] = 6;
    ghost2_pos_rel[1] = 3;

    ghost2_move = 0.05f;
    ghost2_move_stat = 1;

    cnt2 = 0;

}





//
void DrawGhost2(void) {

    glPushMatrix();
    glTranslatef(ghost2_pos[0], ghost2_pos[1], 0);
    glRotatef(cnt2 * (90 / 4), 0, 0, 1);
    glTranslatef(-ghost2_pos[0], -ghost2_pos[1], 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 0.5f, 0.2f);
    glBegin(GL_TRIANGLES);

    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] - 0.03f);
    glVertex2f(ghost2_pos[0] - 0.05f, ghost2_pos[1] - 0.05f);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] - 0.02f);

    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] - 0.07f, ghost2_pos[1]);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] + 0.02f);

    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] - 0.05f, ghost2_pos[1] + 0.05f);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] + 0.03f);

    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0], ghost2_pos[1] + 0.07f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] + 0.03f);

    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0] + 0.05f, ghost2_pos[1] + 0.05f);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] + 0.02f);

    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] + 0.07f, ghost2_pos[1]);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] - 0.02f);

    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] + 0.05f, ghost2_pos[1] - 0.05f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] - 0.03f);

    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] - 0.03f);
    glVertex2f(ghost2_pos[0], ghost2_pos[1] - 0.07f);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] - 0.03f);

    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] - 0.03f);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] - 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] + 0.03f);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] + 0.02f);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] + 0.03f, ghost2_pos[1] - 0.02f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] - 0.03f);
    glVertex2f(ghost2_pos[0] + 0.02f, ghost2_pos[1] - 0.03f);
    glVertex2f(ghost2_pos[0] - 0.02f, ghost2_pos[1] - 0.03f);
    glEnd();

    glPopMatrix();
}

void MoveGhost2(void) {
    if (ghost2_move_stat == 0) return;
    ghost2_move_cnt += 1;

    // ���ⱸ���� ���� �缳��
    if (ghost2_move_cnt == 1) {
        if ((ghost2_pos_rel[0] == 0) && (ghost2_pos_rel[1] == 1)) {
            ghost2_dir = 3;
        }
        else if ((ghost2_pos_rel[0] == 9) && (ghost2_pos_rel[1] == 8)) {
            ghost2_dir = 2;
        }
    }

    // �浹�� ���ܼ� ���ο� ���� ������ ���ϴ� �÷��װ� Ȱ��ȭ�Ǹ� ���ο� ���� ����
    if (set_ghost2_new_dir_flag == 1) {
        SetGhost2Dir();
    }

    // ��Ʈ�� ������ �̵� �������� �̵�
    if (ghost2_dir == 0) {
        ghost2_pos[0] -= 0.025f;
    }
    else if (ghost2_dir == 1) {
        ghost2_pos[0] += 0.025f;
    }
    else if (ghost2_dir == 2) {
        ghost2_pos[1] += 0.025f;
    }
    else if (ghost2_dir == 3) {
        ghost2_pos[1] -= 0.025f;
    }
    // ��Ʈ �����ġ �缳��
    SetGhost2PosRel();

    // ������ �浹 ���� // �������ڸ��ڵ� ���߿� üũ�ؾ���. ������ ���� ���������� �����ϱ�
    if (ghost2_move_cnt == 8) {
        DoGhost2Collide();
    }

    // �Ѹǰ��� �浹 ����
    if (DoGhost2PacmanCollide() == true) {
        pacman_move_stat = 0;
        ghost2_move_stat = 0;
        lose_flag = 1;
    }

    // ��Ʈ ���� ī��Ʈ(Ÿ�� ƽ)�� 8�� �Ǹ� 0���� �缳��
    if (ghost2_move_cnt == 8) {
        ghost2_move_cnt = 0;
    }

    cnt2++;
    if (cnt2 == 4) cnt2 = 0;
}

void SetGhost2PosRel(void) {
    if (ghost2_move_cnt == 8) {
        switch (ghost2_dir) {
        case 0: // ����
            ghost2_pos_rel[1] -= 1;
            break;
        case 1: // ������
            ghost2_pos_rel[1] += 1;
            break;
        case 2: // ����
            ghost2_pos_rel[0] -= 1;
            break;
        case 3: // �Ʒ���
            ghost2_pos_rel[0] += 1;
            break;
        default:
            break;
        }
    }
}

void SetGhost2Dir(void) {
    if ((ghost2_pos_rel[0] == 0) && (ghost2_pos_rel[1] == 1)) {
        ghost2_dir = 3;
    }
    else if ((ghost2_pos_rel[0] == 9) && (ghost2_pos_rel[1] == 8)) {
        ghost2_dir = 2;
    }
    else {
        // �� ���� �߿��� �� �� �ִ� ���� �߸���
        bool checkDir[4] = { 0,0,0,0 }; // ��, ��, ��, ��
        OutputDebugString((to_string(ghost2_pos_rel[0]) + to_string(ghost2_pos_rel[1]) + "���� ��ġ. � �������� �����ִ��� �߸��Ŵ�\n").c_str());
        // check left
        if (maze[ghost2_pos_rel[0]][ghost2_pos_rel[1] - 1] == 0) {
            checkDir[0] = 1;
        }
        // check right
        if (maze[ghost2_pos_rel[0]][ghost2_pos_rel[1] + 1] == 0) {
            checkDir[1] = 1;
        }
        // check up
        if (maze[ghost2_pos_rel[0] - 1][ghost2_pos_rel[1]] == 0) {
            checkDir[2] = 1;
        }
        // check down
        if (maze[ghost2_pos_rel[0] + 1][ghost2_pos_rel[1]] == 0) {
            checkDir[3] = 1;
        }
        OutputDebugString((to_string(checkDir[0]) + to_string(checkDir[1]) + to_string(checkDir[2]) + to_string(checkDir[3]) + "�����ִ¹���üũ\n").c_str());
        // �� ����� �� �������� �����ϰ� �ϱ�.
        int dir = -1;
        int temp = rand() % 4;
        while (dir == (-1)) {
            if (checkDir[temp] == 1) {
                dir = temp;
                break;
            }
            temp = (temp + 1) % 4;
            string xtest = to_string(temp) + "heyhey\n";
            OutputDebugString(xtest.c_str());
        }
        ghost2_dir = dir;
    }
    set_ghost2_new_dir_flag = 0;
}

//int cnt = 0;
bool DoGhost2Collide(void) {
    int next_ghost2_pos_rel[2] = { ghost2_pos_rel[0],ghost2_pos_rel[1] };

    switch (ghost2_dir) {
    case 0: // ����
        next_ghost2_pos_rel[1] -= 1;
        break;
    case 1: // ������
        next_ghost2_pos_rel[1] += 1;
        break;
    case 2: // ����
        next_ghost2_pos_rel[0] -= 1;
        break;
    case 3: // �Ʒ���
        next_ghost2_pos_rel[0] += 1;
        break;
    default:
        break;
    }

    if (maze[next_ghost2_pos_rel[0]][next_ghost2_pos_rel[1]] == 1) {
        set_ghost2_new_dir_flag = 1;
        OutputDebugString((to_string(next_ghost2_pos_rel[0]) + to_string(next_ghost2_pos_rel[1]) + "�浹�� �����. ���� ������ �̷��ؼ�\n").c_str());
        return true;
    }
    //else return false;
    //cnt = cnt % 4;
    //cnt += 1;
    //if (cnt == 3) {
    //    set_ghost2_new_dir_flag = 1;
    //}
    int randVal1 = rand() % 5;
    int randVal2 = rand() % 5;
    if (randVal1 == randVal2) {
        set_ghost2_new_dir_flag = 1;
    }

    return true;
}

bool DoGhost2PacmanCollide(void) {
    float distanceSquared = (pacman_pos[0] - ghost2_pos[0]) * (pacman_pos[0] - ghost2_pos[0]) + (pacman_pos[1] - ghost2_pos[1]) * (pacman_pos[1] - ghost2_pos[1]);
    if (distanceSquared < 0.02f) {
        OutputDebugString((to_string(distanceSquared) + "����").c_str());
        return true;
    }
    else return false;
}