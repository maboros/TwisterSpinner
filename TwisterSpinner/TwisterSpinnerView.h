
// TwisterSpinnerView.h : interface of the CTwisterSpinnerView class
//

#pragma once
#include "TwisterSpinnerDoc.h"
#define	IDT_TIMER_0	WM_USER + 200
constexpr UINT IDC_SPINBUTTON(101);
constexpr UINT IDC_ADDBUTTON(IDC_SPINBUTTON + 1);
constexpr UINT IDC_REMOVEBUTTON(IDC_SPINBUTTON + 2);
constexpr UINT IDC_NEWGAMEBUTTON(IDC_SPINBUTTON + 3);
constexpr UINT IDC_LISTBOX(IDC_SPINBUTTON + 4);
constexpr UINT IDC_TEXTBOX(IDC_SPINBUTTON + 5);




class CTwisterSpinnerView : public CView
{
protected: // create from serialization only
	CTwisterSpinnerView() noexcept;
	DECLARE_DYNCREATE(CTwisterSpinnerView)

	// Attributes
public:
	CTwisterSpinnerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DisplayLimbAndColor();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	UINT_PTR timerVal;
	bool timerLive;
	bool arrowSpun = false;
	double spinAmount;
	double spinAngle = 0;
	bool listBoxMoved = false;
	double pi = 3.14159;
public:
	void Reset();
	double r = 200;

// Implementation
public:
	virtual ~CTwisterSpinnerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSpinButtonClicked();
	afx_msg void OnAddButtonClicked();
	afx_msg void OnRemoveButtonClicked();
	afx_msg void OnNewGameButtonClicked();
	afx_msg void OnTimer(UINT_PTR TimerVal);
	DECLARE_MESSAGE_MAP()
private:
	CButton spinButton;
	CButton addButton;
	CButton removeButton;
	CButton newGameButton;
	CEdit textBox;
	CListBox listBox;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

#ifndef _DEBUG  // debug version in TwisterSpinnerView.cpp
inline CTwisterSpinnerDoc* CTwisterSpinnerView::GetDocument() const
   { return reinterpret_cast<CTwisterSpinnerDoc*>(m_pDocument); }
#endif

