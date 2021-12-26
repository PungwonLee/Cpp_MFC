
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication3-2.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_Lbutton = false;
	m_select = false;

}
CChildView::~CChildView()
{
}
BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
//	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
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
		CBrush brush2(RGB(255, 255, 255));
		memDC.SelectObject(brush2);
		memDC.Rectangle((*it).m_st.x, (*it).m_st.y, (*it).m_en.x, (*it).m_en.y);

		if ((*it).m_select == true) {
			CBrush brush(RGB(255, 255, 0));
			memDC.SelectObject(brush);
			memDC.Rectangle((*it).m_st.x, (*it).m_st.y, (*it).m_en.x, (*it).m_en.y);
		}
		it++;
	}

	dc.BitBlt(0, 0, w, h, &memDC, 0, 0,SRCCOPY);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Lbutton = true;
	m_prev = point;
	auto it = m_rect.rbegin();
	while (it != m_rect.rend()) {
		CRect rect((*it).m_st.x, (*it).m_st.y, (*it).m_en.x, (*it).m_en.y);
		if (rect.PtInRect(point)) {
			(*it).m_select = true;
			m_select = true;
			break;
		}
		it++;
		Invalidate();
	}

	MyRect rect;
	rect.m_st = point;
	rect.m_en = point;
	if (!m_select)
		m_rect.push_back(rect);

	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Lbutton = false;
	
	if (!m_select) {
		m_rect.back().m_en = point;
		if (m_rect.back().m_st.x > m_rect.back().m_en.x) {
			int tmp = m_rect.back().m_st.x;
			m_rect.back().m_st.x = m_rect.back().m_en.x;
			m_rect.back().m_en.x = tmp;
		}
		if (m_rect.back().m_st.y > m_rect.back().m_en.y) {
			int tmp = m_rect.back().m_st.y;
			m_rect.back().m_st.y = m_rect.back().m_en.y;
			m_rect.back().m_en.y = tmp;
		}

	}
	auto it = m_rect.rbegin();
	while (it != m_rect.rend()) {
		(*it).m_select = 0;
		it++;
		Invalidate();
	}

	m_select = false;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Lbutton&& !m_select) {
		m_rect.back().m_en = point;
		Invalidate();
	}
	auto it = m_rect.begin();
	CPoint move = point;
	move -= m_prev;
	while (it != m_rect.end()) {
		if ((*it).m_select == true) {
			(*it).m_st += move;
			(*it).m_en += move;
		}
		it++;
		Invalidate();
		m_prev = point;
	}
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}
