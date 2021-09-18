
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication3-1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_Lbutton=false;
	m_color=0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CChildView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CChildView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CChildView::OnUpdateColorRed)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	CRect rect;
	GetClientRect(rect);
	int w = rect.Width();
	int h = rect.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBMP;
	memBMP.CreateCompatibleBitmap(&dc, w, h);
	memDC.SelectObject(memBMP);

	memDC.Rectangle(rect);

	auto it = m_rect.begin();
	while (it != m_rect.end()) {
		CBrush brush((*it).m_color);
		memDC.SelectObject(brush);
		memDC.Rectangle((*it).m_st.x, (*it).m_st.y, (*it).m_en.x, (*it).m_en.y);
		it++;
	}
	dc.BitBlt(0, 0, w, h, &memDC, 0,0, SRCCOPY);
}
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Lbutton = true;
	MyRect rect;
	rect.m_st = point;
	rect.m_en = point;

	if (m_color == 0) {
		rect.m_color = RGB(255, 0, 0);
	}
	if (m_color == 1) {
		rect.m_color = RGB(0, 255, 0);
	}
	if (m_color == 2) {
		rect.m_color = RGB(0, 0,255);
	}
	m_rect.push_back(rect);

	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Lbutton = false;
	//m_rect.back().m_en = point;
	Invalidate();

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Lbutton) {
		m_rect.back().m_en = point;
	}
	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnColorRed()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_color = 0;
}

void CChildView::OnColorGreen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_color = 1;
}

void CChildView::OnColorBlue()
{
	m_color = 2;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CChildView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	if (m_color == 2) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}

void CChildView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	if (m_color == 1) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}

void CChildView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	if (m_color == 0) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}
