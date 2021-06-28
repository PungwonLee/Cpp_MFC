
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_Lbutton = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
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
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

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

	auto it = m_curve.begin();
	while (it != m_curve.end()) {

		auto it2 = (*it).xx.begin(); 
		auto it3 = (*it).yy.begin();
		
		CPoint x = CPoint((*it2) * w, ((*it3) * h));
		memDC.MoveTo(x);
		while (it2 != (*it).xx.end()) {
			x = CPoint((*it2) * w, ((*it3) * h));
			memDC.LineTo(x);
			it2++;
			it3++;
		}
		it++;
	}
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Lbutton = true;

	CRect rect;
	GetClientRect(rect);
	MyCurve curve;
	curve.xx.push_back(point.x / (double)rect.Width());
	curve.yy.push_back(point.y / (double)rect.Height());  //변환된 좌표 저장.
	m_curve.push_back(curve);

	Invalidate();
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	if (m_Lbutton) {
		m_curve.back().xx.push_back(point.x / (double)rect.Width());
		m_curve.back().yy.push_back(point.y / (double)rect.Height());
	}
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Lbutton = false;
	Invalidate();
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return false;
	//return CWnd::OnEraseBkgnd(pDC);
}
