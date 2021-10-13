
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "HomeworkFinal.h"
#include "ChildView.h"
#include<list> //리스트
#define _USE_MATH_DEFINES
#include<cmath>
#include <typeinfo>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
using namespace std;
CChildView::CChildView()
{
	drawShape = 0;
	x = y = 0;
	m_start = CPoint(0, 0);
	m_end = CPoint(0, 0);
	m_RButton = false;
	m_LButton = false;
	b_select = false;
	sel_cnt = 0;
	m_prev = CPoint(0, 0);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DRAW_RECTANGLE, &CChildView::OnDrawRectangle)
	ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CURVE, &CChildView::OnDrawCurve)
	ON_COMMAND(ID_ACTION_SELECT, &CChildView::OnActionSelect)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CChildView::OnUpdateDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CChildView::OnUpdateDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE, &CChildView::OnUpdateDrawCurve)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SELECT, &CChildView::OnUpdateActionSelect)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DRAW_STAR32786, &CChildView::OnDrawStar32786)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STAR32786, &CChildView::OnUpdateDrawStar32786)
	ON_COMMAND(ID_ALIGN_BRINGFRONT, &CChildView::OnAlignBringfront)
	ON_COMMAND(ID_ALIGN_BRINGBACK, &CChildView::OnAlignBringback)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINGBACK, &CChildView::OnUpdateAlignBringback)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINGFRONT, &CChildView::OnUpdateAlignBringfront)
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
	memDC.Rectangle(rect); //흰화면으로 칠한다.

	CFont font;
	font.CreatePointFont(400, _T("궁서체"));
	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(255, 200, 200));
	CString str3;
	str3.Format(_T("C++ Homework final"));
	memDC.DrawText(str3, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	CPen pen2(PS_ALTERNATE, 3, RGB(55, 55, 55));
	memDC.SelectObject(pen2);

	auto it = m_shape.begin();					//그리기
	while (m_shape.end() != it) {
		(*it)->Draw(&memDC);
		if (drawShape == 4 && !m_shape.empty())
			(*it)->SelectDraw(&memDC);
		else (*it)->m_select = false;
		it++;
	}


	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	memDC.SelectObject(pen);
	memDC.SelectStockObject(NULL_BRUSH);		///영역 지정

	if ((sel_cnt <= 0 && m_LButton == true)) {

		memDC.Rectangle(m_start.x, m_start.y, m_end.x, m_end.y);
	}

	CFont font2;
	font2.CreatePointFont(120, _T("보통"));
	memDC.SelectObject(&font2);
	memDC.SetTextColor(RGB(0, 0, 0));

	CString str;
	str.Format(_T("Number of Shapes: %d"), m_shape.size());
	memDC.TextOutW(3, 2, str);
	CString str2;
	if (drawShape != 4)sel_cnt = 0;
	str2.Format(_T("Number of Selected: %d"), sel_cnt);
	memDC.TextOutW(3, 28, str2);


	dc.BitBlt(0, 0, w, h, &memDC, 0, 0, SRCCOPY);

}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_LButton = true;
	m_start = point;
	m_end = point;
	
	COLORREF collor = RGB(rand() % 100 + 150, rand() % 100 + 150, rand() % 100 + 150);

	if (drawShape == 0) {		///////////사각형	
		MyRect rect;
		rect.m_en = point;
		rect.m_st = point;
		rect.m_collor = collor;
		m_shape.push_back(new MyRect(rect));

	}
	else if (drawShape == 1) {		//원
		MyCircle circle;
		circle.m_en = point;
		circle.m_st = point;
		circle.m_collor = collor;
		m_shape.push_back(new MyCircle(circle));
	}

	else if (drawShape == 2) {		//선
		MyCurve curve;
		curve.m_pts.push_back(point);
		curve.m_collor = collor;
		m_shape.push_back(new MyCurve(curve));
	}
	else if (drawShape == 3) {		//별
		MyStar star;
		star.m_en = point;
		star.m_st = point;
		star.m_collor = collor;
		m_shape.push_back(new MyStar(star));
	}
	else if (drawShape == 4 && !m_shape.empty()) {
		b_select = true;
		m_prev = point;	//이동하기전 좌표.
		auto it = m_shape.rbegin();
		int ch = 0;
		while (it != m_shape.rend()) {
			CRect rect((*it)->m_st.x, (*it)->m_st.y, (*it)->m_en.x, (*it)->m_en.y);
			
			if (rect.PtInRect(point) && (*it)->m_nType != 2 || ((*it)->m_nType == 3 && (*it)->PointSearch(point)) 
				|| ((*it)->m_nType == 2 && (*it)->PointSearch(point))) {
				if ((*it)->m_select == true && *it == m_shape.back()) { it++; ch = 1; continue; } //마지막 객체를 다시 눌렀을때 반례
				if ((*it)->m_select == true || ch) { ch = 0; break; }
				if (sel_cnt != 0 && (nFlags & MK_SHIFT) == 0) { // 쉬프트 미입력 상태
					tmp->m_select = false;
					sel_cnt--;
				}
				(*it)->m_select = true;
				tmp = *it;
				sel_cnt++;
				break;

			}
			else {
				if ((nFlags & MK_SHIFT) == 0) {
					auto it2 = m_shape.rbegin();
					while (it2 != m_shape.rend()) {
						if (ch && *it2 == m_shape.back()) { it2++; continue; }
						if ((*it2)->m_select == true) {
							(*it2)->m_select = false;
							sel_cnt--;
						}
						it2++;
						Invalidate();
					}
				}
			}
			it++;
			Invalidate();
		}
		Invalidate();
	}

	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);

}
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LButton = false;
	m_end = point;
	b_select = false;

	if (drawShape == 0 || drawShape == 1 || drawShape == 3 || drawShape == 2) { //선 사각형
		m_shape.back()->m_en = point;
		if (drawShape != 3)
			m_shape.back()->Swap();
	}

	else if ((drawShape == 4 && sel_cnt == 0) || (nFlags & MK_SHIFT) != 0) {
		CPoint tmp;
		if (m_start.x > m_end.x) {
			tmp.x = m_start.x;
			m_start.x = m_end.x;
			m_end.x = tmp.x;
		}
		if (m_start.y > m_end.y) {
			tmp.y = m_start.y;
			m_start.y = m_end.y;
			m_end.y = tmp.y;
		}
		auto it = m_shape.rbegin();
		while (it != m_shape.rend()) {

			CRect rect(m_start.x, m_start.y, m_end.x, m_end.y);
			CPoint xx((*it)->m_st);
			CPoint yy((*it)->m_en);
			if ((*it)->m_nType == 2) {
				xx = (*(*it)->m_pts.begin());
				yy = (*--(*it)->m_pts.end());
			}
			if (rect.PtInRect(xx) && rect.PtInRect(yy)) {
				(*it)->m_select = true;
				sel_cnt++;
			}
			it++;
			Invalidate();
		}
		Invalidate();
	}
	Invalidate();
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nFlags & MK_LBUTTON) != 0) {   //마우스가 움직이면서 왼쪽버튼


		if (drawShape == 0 || drawShape == 1 || drawShape == 3) 		////// 원,별
			m_shape.back()->m_en = point;
		
		else if (drawShape == 2) 	////// 선
			m_shape.back()->m_pts.push_back(point);
		
		else if (drawShape == 4) {//꾹누른상태에서 이동 끝점만 이동
			m_end = point;
			Invalidate();
		}
		Invalidate();


	}
	if (sel_cnt > 0 && b_select == true) {
		CPoint move;
		move.x = point.x - m_prev.x;
		move.y = point.y - m_prev.y;
		auto it = m_shape.begin();
		while (m_shape.end() != it) {
			if ((*it)->m_select == true) {
				
				(*it)->m_st += move;
				(*it)->m_en += move;
				if ((*it)->m_nType == 2) {
					auto it2 = (*it)->m_pts.begin();
					while (it2 != (*it)->m_pts.end()) {
						(*it2) += move;
						Invalidate();
						it2++;
					}
				}
				Invalidate();
			}
			it++;
		}
		m_prev = point;
	}

	Invalidate();


	CWnd::OnMouseMove(nFlags, point);
}





BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{

	return true;
	//CWnd::OnEraseBkgnd(pDC);
}
void MyRect::Draw(CDC* pDC) {			//사각형 그리기
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(pen);

	CBrush brush(m_collor);
	pDC->SelectObject(brush);
	pDC->Rectangle(m_st.x, m_st.y, m_en.x, m_en.y);

}
void MyCircle::Draw(CDC* pDC) {			//원 그리기
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(pen);

	CBrush brush(m_collor);
	pDC->SelectObject(brush);
	pDC->Ellipse(m_st.x, m_st.y, m_en.x, m_en.y);


}
void MyCurve::Draw(CDC* pDC) {/////////////////curver draw
	if (m_pts.size() < 1) return;
	CPen pen2(PS_ALTERNATE, 4, m_collor);
	pDC->SelectObject(pen2);
	auto it = m_pts.begin();
	C_start = (*it);
	pDC->MoveTo(*it);
	while (it != m_pts.end()) {
		pDC->LineTo(*it);

		C_end = (*it);
		it++;
	}
}
void MyStar::Draw(CDC* pDC) {/////////////////별 그리기
	CPoint ps[6];
	double r = sqrt((m_st.x - m_en.x) * (m_st.x - m_en.x) + (m_st.y - m_en.y) * (m_st.y - m_en.y));
	double Theta = 0;
	int cnt = 0;
	const double PI = 3.141592653589793;

	for (double i = 0; i <= 720.0; i += 144.0)
	{
		Theta = i * PI / 180 + 2.19;
		ps[cnt] = CPoint(m_st.x + (float)(r * 1.0 * cos(Theta)),
			m_st.y + (float)(r * 1.0 * sin(Theta)));
		cnt++;
	}
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(pen);
	CBrush brush(m_collor);
	pDC->SelectObject(brush);
	CRgn rgn;
	rgn.CreatePolygonRgn(ps, 6, WINDING);

	pDC->Polygon(ps, 6);
	pDC->PaintRgn(&rgn);

}
void CMyShape::SelectDraw(CDC* pDC) {

	if (m_select == true) {
		CPen pen(PS_DOT, 1, m_Select_collor);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectObject(pen);

		CRect rect(m_st.x, m_st.y, m_en.x, m_en.y);
		rect.InflateRect(5, 5);
		pDC->Rectangle(rect);
	}
}
void MyCurve::SelectDraw(CDC* pDC) {
	if (m_select == true) {

		CPoint Coordinates[4] = { CPoint(10000,0),CPoint(0,10000),CPoint(0,-10000),CPoint(0,-10000) };
		auto it = m_pts.begin();
		C_start = (*it);
		while (it != m_pts.end()) {
			if ((*it).x < Coordinates[0].x)
				Coordinates[0] = (*it);
			if ((*it).x > Coordinates[2].x)
				Coordinates[2] = (*it);
			if ((*it).y < Coordinates[1].y)
				Coordinates[1] = (*it);
			if ((*it).y > Coordinates[3].y)
				Coordinates[3] = (*it);
			it++;
		}
		CPoint top(Coordinates[0].x < Coordinates[1].x ? Coordinates[0].x : Coordinates[1].x, Coordinates[0].y < Coordinates[1].y ? Coordinates[0].y : Coordinates[1].y);
		CPoint down(Coordinates[2].x > Coordinates[3].x ? Coordinates[2].x : Coordinates[3].x, Coordinates[2].y > Coordinates[3].y ? Coordinates[2].y : Coordinates[3].y);
		CRect rect(top, down);
		CPen pen(PS_DOT, 1, m_Select_collor);
		pDC->SelectStockObject(NULL_BRUSH);

		pDC->SelectObject(pen);
		pDC->Rectangle(rect);


	}
}
void MyStar::SelectDraw(CDC* pDC) {
	if (m_select == true) {
		CPoint ps[6];
		double r = sqrt((m_st.x - m_en.x) * (m_st.x - m_en.x) + (m_st.y - m_en.y) * (m_st.y - m_en.y));
		double Theta = 0;
		int cnt = 0;
		const double PI = 3.141592653589793;
		CPoint Coordinates[4] = { CPoint(10000,0),CPoint(0,10000),CPoint(0,-10000),CPoint(0,-10000) };
		for (double i = 0; i <= 720.0; i += 144.0)
		{
			Theta = i * PI / 180 + 2.19;
			ps[cnt] = CPoint(m_st.x + (float)(r * 1.0 * cos(Theta)),
				m_st.y + (float)(r * 1.0 * sin(Theta)));
			if (ps[cnt].x < Coordinates[0].x)
				Coordinates[0] = ps[cnt];
			if (ps[cnt].x > Coordinates[2].x)
				Coordinates[2] = ps[cnt];

			if (ps[cnt].y < Coordinates[1].y)
				Coordinates[1] = ps[cnt];
			if (ps[cnt].y > Coordinates[3].y)
				Coordinates[3] = ps[cnt];
			cnt++;
		}
		CPoint top(Coordinates[0].x < Coordinates[1].x ? Coordinates[0].x : Coordinates[1].x, Coordinates[0].y < Coordinates[1].y ? Coordinates[0].y : Coordinates[1].y);
		CPoint down(Coordinates[2].x > Coordinates[3].x ? Coordinates[2].x : Coordinates[3].x, Coordinates[2].y > Coordinates[3].y ? Coordinates[2].y : Coordinates[3].y);
		CRect rect(top, down);

		CPen pen(PS_DOT, 1, m_Select_collor);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectObject(pen);
		pDC->Rectangle(rect);
	}

}


void CMyShape::Swap() {
	CPoint tmp;
	if (m_st.x > m_en.x) {
		tmp.x = m_st.x;
		m_st.x = m_en.x;
		m_en.x = tmp.x;
	}

	if (m_st.y > m_en.y) {
		tmp.y = m_st.y;
		m_st.y = m_en.y;
		m_en.y = tmp.y;
	}
}
bool CMyShape::PointSearch(CPoint point) {
	return false;
}
bool MyCurve::PointSearch(CPoint point) {
	bool check = false;
	auto it = m_pts.begin();
	
	CRect rect(point, point);
	rect.InflateRect(8, 8);
	while (it != m_pts.end()) {
		if (rect.PtInRect((*it)))
			check = true;
		it++;
	}
	return check;

}

bool MyStar::PointSearch(CPoint point) {
	CPoint ps[6];
	double r = sqrt((m_st.x - m_en.x) * (m_st.x - m_en.x) + (m_st.y - m_en.y) * (m_st.y - m_en.y));
	double Theta = 0;
	int cnt = 0;
	bool check = false;
	const double PI = 3.141592653589793;
	CPoint Coordinates[4] = { CPoint(10000,0),CPoint(0,10000),CPoint(0,-10000),CPoint(0,-10000) };
	for (double i = 0; i <= 720.0; i += 144.0)
	{
		Theta = i * PI / 180 + 2.19;
		ps[cnt] = CPoint(m_st.x + (float)(r * 1.0 * cos(Theta)),
			m_st.y + (float)(r * 1.0 * sin(Theta)));
		if (ps[cnt].x < Coordinates[0].x)
			Coordinates[0] = ps[cnt];
		if (ps[cnt].x > Coordinates[2].x)
			Coordinates[2] = ps[cnt];
		if (ps[cnt].y < Coordinates[1].y)
			Coordinates[1] = ps[cnt];
		if (ps[cnt].y > Coordinates[3].y)
			Coordinates[3] = ps[cnt];
		cnt++;
	}
	CPoint top(Coordinates[0].x < Coordinates[1].x ? Coordinates[0].x : Coordinates[1].x, Coordinates[0].y < Coordinates[1].y ? Coordinates[0].y : Coordinates[1].y);
	CPoint down(Coordinates[2].x > Coordinates[3].x ? Coordinates[2].x : Coordinates[3].x, Coordinates[2].y > Coordinates[3].y ? Coordinates[2].y : Coordinates[3].y);
	CRect rect(top, down);
	rect.InflateRect(-(m_en.x - m_st.x) / 4, -(m_en.y - m_st.y) / 4);
	check = rect.PtInRect(point);

	int cross = 0;
	for (int i = 0; i < 6; i++) {
		int j = (i + 1) % 6;
		if ((ps[i].y > point.y) != (ps[j].y > point.y)) {
			double atX = (((ps[j].x - ps[i].x) / (ps[j].y - ps[i].y)) * (point.y - ps[i].y)) + ps[i].x;
			if (point.x < atX)
				cross++;
		}
	}//홀수 내부  , 짝수 외부
	if (cross % 2 == 1 || check)
		return true;
	else
		return false;


}



void CChildView::OnDrawRectangle()
{
	drawShape = 0;

	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CChildView::OnDrawCircle()
{
	drawShape = 1;
	Invalidate();

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CChildView::OnDrawCurve()
{

	drawShape = 2;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
void CChildView::OnDrawStar32786()
{
	drawShape = 3;
	Invalidate();
}


void CChildView::OnActionSelect()
{
	drawShape = 4;
	Invalidate();

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}





void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu menu;
	menu.LoadMenuW(IDR_MAINFRAME);
	CMenu* pMenu = menu.GetSubMenu(3);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd());
}


void CChildView::OnAlignBringfront()
{

	auto it = m_shape.begin();
	int cnt = 0;								
	while (m_shape.end() != it && m_shape.size() != cnt) {

		if ((*it)->m_select == true) {
			auto* tmp = *it;
			it = m_shape.erase(it);
			m_shape.push_back(tmp);
		}
		else
			it++;
		cnt++;
		Invalidate();
	}
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CChildView::OnAlignBringback()
{
	auto it = m_shape.rbegin();					
	int cnt = 0;
	while (m_shape.rend() != it && cnt != m_shape.size()) {

		if ((*it)->m_select == true) {
			auto* tmp = *it;
			m_shape.erase(--it.base());
			m_shape.push_front(tmp);

		}
		else it++;
		cnt++;

		Invalidate();
	}

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CChildView::OnUpdateDrawRectangle(CCmdUI* pCmdUI)
{
	if (drawShape == 0)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateDrawCircle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawShape == 1)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CChildView::OnUpdateDrawCurve(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawShape == 2)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateDrawStar32786(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawShape == 3)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CChildView::OnUpdateActionSelect(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawShape == 4)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CChildView::OnUpdateAlignBringback(CCmdUI* pCmdUI)
{
	if (sel_cnt > 0 && drawShape == 4)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CChildView::OnUpdateAlignBringfront(CCmdUI* pCmdUI)
{
	if (sel_cnt > 0 && drawShape == 4)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nChar == VK_DELETE) && drawShape == 4) {
		auto it = m_shape.begin();
		while (m_shape.end() != it) {

			if ((*it)->m_select == true) {
				it = m_shape.erase(it);
				sel_cnt--;
			}
			else
				it++;
			Invalidate();
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}






