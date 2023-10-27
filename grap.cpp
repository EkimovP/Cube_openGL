#define WINVER 0x601
#include <afxwin.h>
#include <gl/GL.h>


//класс приложени€
class MyApp : public CWinApp
{
	virtual BOOL InitInstance();
};

MyApp app; //глобальный объект приложени€
//класс окна
class Win : public CFrameWnd
{
	DECLARE_MESSAGE_MAP()
	HGLRC glRC;
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	Win() : glRC(NULL) { Create(AfxRegisterWndClass(CS_OWNDC, LoadCursor(NULL, IDC_ARROW)), L"»спользование OpenGL"); 
	//первый параметр NULL, если не на вертуальной машине
	SetTimer(1, 50, NULL);
	angle = 0;}
	~Win() { wglDeleteContext(glRC); KillTimer(1); }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	float angle;
};

BEGIN_MESSAGE_MAP(Win, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL MyApp::InitInstance()
{
	m_pMainWnd = new Win;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

void Win::OnSize(UINT nType, int cx, int cy)
{
	//вызов метода базового класса
	CFrameWnd::OnSize(nType, cx, cy);

	if (glRC != NULL) wglDeleteContext(glRC);
	CDC* dc = GetDC();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32 };

	int pixelFormat = ChoosePixelFormat(*dc, &pfd);
	SetPixelFormat(*dc, pixelFormat, &pfd);

	glRC = wglCreateContext(*dc);
	ReleaseDC(dc);

	Invalidate(FALSE); // перерисовка окна
}

void Win::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	wglMakeCurrent(dc, glRC); //назначение контекста OpenGL

	glClearColor(0, 0, 0, 0); //очистка окна
	glClear(GL_COLOR_BUFFER_BIT);
	
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glPushMatrix();
	glFrustum(-1, 1, -1, 1, 2, 8);
	glTranslatef(0, 0, -6);
	glRotatef(angle, 1, 1, 0);

	if (rect.Height() >= rect.Width())
	{
		glViewport(0, rect.Height() / 2 - rect.Width() / 2, rect.Width(), rect.Width());
	}
	else
	{
		glViewport(rect.Width() / 2 - rect.Height() / 2, 0, rect.Height(), rect.Height());
	}
	
	//-----------вершины граней куба---------------
	//передн€€ грань
	glBegin(GL_QUADS);//четырЄхугольный полигон
	glColor3d(0, 0.4, 0.4); glVertex3d(1, 1, 1);
	glColor3d(1, 1, 1); glVertex3d(1, -1, 1);
	glColor3d(0, 0.4, 0.4); glVertex3d(-1, -1, 1);
	glColor3d(0.7, 0.7, 0.9); glVertex3d(-1, 1, 1);
	//задн€€ грань
	glColor3d(0.2, 0.2, 0.2); glVertex3d(-1, 1, -1);
	glColor3d(0.1, 0.8, 0.1); glVertex3d(-1, -1, -1);
	glColor3d(0.2, 0.2, 0.2); glVertex3d(1, -1, -1);
	glColor3d(0.1, 0.8, 0.1); glVertex3d(1, 1, -1);
	//бокова€ грань (повернуть направо)
	glColor3d(1, 0, 0); glVertex3d(-1, 1, -1);
	glColor3d(1, 1, 1); glVertex3d(-1, 1, 1);
	glColor3d(1, 0, 0); glVertex3d(-1, -1, 1);
	glColor3d(1, 1, 1); glVertex3d(-1, -1, -1);
	//бокова€ грань (повернуть налево)
	glColor3d(1, 0.2, 0); glVertex3d(1, -1, -1);
	glColor3d(0, 0, 0); glVertex3d(1, -1, 1);
	glColor3d(1, 0.2, 0); glVertex3d(1, 1, 1);
	glColor3d(0, 0, 0); glVertex3d(1, 1, -1);
	//нижн€€ грань
	glColor3d(0.1, 0.1, 0.1); glVertex3d(1, -1, -1);
	glColor3d(0, 0.2, 0.8);	glVertex3d(-1, -1, -1);
	glColor3d(0.1, 0.1, 0.1); glVertex3d(-1, -1, 1);
	glColor3d(0, 0.2, 0.8); glVertex3d(1, -1, 1);
	//верхн€€ грань
	glColor3d(0.8, 0.2, 0); glVertex3d(1, 1, -1);
	glColor3d(1, 0.8, 0);	glVertex3d(1, 1, 1);
	glColor3d(0.8, 0.2, 0); glVertex3d(-1, 1, 1);
	glColor3d(1, 0.8, 0); glVertex3d(-1, 1, -1);

	glEnd();

	glFlush(); //использование команд OpenGL
	SwapBuffers(dc); //переключение буферов

	glPopMatrix();
	wglMakeCurrent(NULL, NULL); //сброс текущего контекста
}

void Win::OnTimer(UINT_PTR nIDEvent)
{
	angle += 2;
	Invalidate(0);
	CFrameWnd::OnTimer(nIDEvent);
}
