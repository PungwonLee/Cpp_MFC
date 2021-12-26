
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include<list>
using namespace std;

class MyRect {
public:
	CPoint m_st;
	CPoint m_en;
	bool m_select;//선택된 도형 체크용
};
// CChildView 창
class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	list<MyRect> m_rect;
	CPoint m_prev;   //클릭 좌표
	bool m_Lbutton;
	bool m_select;	//선택했는지?
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
//	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

