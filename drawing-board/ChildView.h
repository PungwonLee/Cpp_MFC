
// ChildView.h: CChildView 클래스의 인터페이스
//
#include<list> //리스트
#include<vector>
using namespace std;

#pragma once
//template <typename T>

class CMyShape {
public:
	int m_nType;
	bool m_select;
	CPoint m_st;
	CPoint m_en;
	COLORREF m_collor;
	COLORREF m_Select_collor;

	list<CPoint>m_pts;

	CMyShape() {
		Swap();
		m_select = false;
		m_Select_collor = RGB(255, 0, 0);
	}
	
	void Swap();
	virtual void SelectDraw(CDC* pDC);
	virtual void Draw(CDC* pDC) = 0;
	virtual bool PointSearch(CPoint point);

};
class MyRect :public CMyShape {
public:

	virtual void Draw(CDC* pDC);


};
class MyCircle :public CMyShape {
public:
	virtual void Draw(CDC* pDC);


};
class MyCurve : public CMyShape{
public:
	MyCurve(){}
	MyCurve(const MyCurve &in)
	{	
		
		m_select = in.m_select;
		m_pts = in.m_pts;
		m_collor = in.m_collor;
		m_Select_collor = in.m_Select_collor;
		m_nType = 2;
	}
	CPoint C_start;
	CPoint C_end;
	virtual void Draw(CDC* pDC);
	virtual bool PointSearch(CPoint point);

	virtual void SelectDraw(CDC* pDC);

};

class MyStar : public CMyShape {
public:

	MyStar() {}
	MyStar(const MyStar& in)
	{
		m_st = in.m_st;
		m_en = in.m_en;
		m_select = in.m_select;
		m_pts = in.m_pts;
		m_collor = in.m_collor;
		m_Select_collor = in.m_Select_collor;
		m_nType = 3;
	}
	virtual void Draw(CDC* pDC);
	virtual bool PointSearch(CPoint point);
	virtual void SelectDraw(CDC* pDC);

};

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();
	
	
	

// 특성입니다.
public:
	list< CMyShape*> m_shape;

	CMyShape* tmp;
	
	CPoint m_start;
	CPoint m_end;

	CPoint m_prev;
	
	bool m_RButton;
	bool m_LButton;
	bool b_select;
	int drawShape;
	int sel_cnt;
	int x, y;
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDrawCurve();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnActionSelect();
	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCurve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionSelect(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDrawStar32786();
	afx_msg void OnUpdateDrawStar32786(CCmdUI* pCmdUI);
	afx_msg void OnAlignBringfront();
	afx_msg void OnAlignBringback();
	afx_msg void OnUpdateAlignBringback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignBringfront(CCmdUI* pCmdUI);
};



