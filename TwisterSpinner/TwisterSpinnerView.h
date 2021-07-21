
// TwisterSpinnerView.h : interface of the CTwisterSpinnerView class
//
#include <random>
#pragma once
#include "TwisterSpinnerDoc.h"
#include "FlickerFree.h"
#include "resource.h"

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
	bool arrowSpun = false;
	double spinAmount;
	double spinAngle = 0;
	bool listBoxMoved = false;
	bool gameStarted = false;
	double r = 200;
	int currentIndex = 0;
public:
	void Reset();
	

// Implementation
public:
	virtual ~CTwisterSpinnerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString fetchResString(int resId);
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in TwisterSpinnerView.cpp
inline CTwisterSpinnerDoc* CTwisterSpinnerView::GetDocument() const
   { return reinterpret_cast<CTwisterSpinnerDoc*>(m_pDocument); }
#endif

