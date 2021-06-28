
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include<list>
using namespace std;

class MyCurve {
public:
	list<CPoint> m_pts;  //창크기 안쓸때 이거쓴다
	list<double> xx;		//xx,yy 창크기 변할때..
	list<double> yy;
};



// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	list<MyCurve> m_curve;

	bool m_Lbutton;
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

