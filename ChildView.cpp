
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_st=CPoint(0,0);
	m_en=CPoint(0, 0);

	m_Lbutton = false;
	m_Rbutton=false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
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
	memDC.CreateCompatibleDC(&dc);		// 메모리 dc 만들기
	CBitmap memBMP;
	memBMP.CreateCompatibleBitmap(&dc, w, h);	//그림그릴 메모리를 만든다.
	memDC.SelectObject(memBMP);					//DC와 Bitmap을 연결한다.
	memDC.Rectangle(rect);					//흰화면으로 칠한다.	

	CBrush brush(RGB(0, 0, 0));
	memDC.SelectObject(&brush);
	
	auto it = m_point.begin();
	while (it != m_point.end()) {
		memDC.Ellipse((*it).x, (*it).y, (*it).x+5, (*it).y+5);
		it++;
	}

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0)); // 방법 1
	memDC.SelectObject(&pen); //펜 선택
	memDC.SelectStockObject(NULL_BRUSH);
	if(m_Rbutton)
		memDC.Rectangle(m_st.x,m_st.y, m_en.x,m_en.y);

	CString str;
	str.Format(_T("Number of Shapes: %d"), m_point.size());
	memDC.SetTextAlign(TA_CENTER);
	memDC.TextOutW(3, 2, str); //x=3, y=3에 위치에 str출력

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);//그린 그림을 화면 DC로 Bitblt

}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Lbutton = true;
	m_point.push_back(point);
	Invalidate();
	SetCapture();

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_Rbutton = true;
	m_st = point;
	m_en = point;
	SetCapture();
	CWnd::OnRButtonDown(nFlags, point);
}
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Lbutton) {
		m_point.push_back(point);
	}
	if (m_Rbutton) {
		m_en = point;
	}
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return false;
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Lbutton = false;	
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_Rbutton = false;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	auto it = m_point.begin();
	while (it != m_point.end()) {
		if (m_st.x > m_en.x) {
			int tmp = m_st.x;
			m_st.x = m_en.x;
			m_en.x = tmp;
		}
		if (m_st.y > m_en.y) {
			int tmp = m_st.y;
			m_st.y = m_en.y;
			m_en.y = tmp;
		}
			
		if ((*it).x > m_st.x && (*it).y > m_st.y && (*it).x < m_en.x && (*it).y < m_en.y) {
			it = m_point.erase(it);
		}
		else it++;
		Invalidate();
	}
	ReleaseCapture();
	Invalidate();
	

	CWnd::OnRButtonUp(nFlags, point);
}
