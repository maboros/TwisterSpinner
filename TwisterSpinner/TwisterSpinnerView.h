
// TwisterSpinnerView.h : interface of the CTwisterSpinnerView class
//

#pragma once
#define	IDT_TIMER_0	WM_USER + 200
constexpr UINT IDC_SPINBUTTON(101);



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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTwisterSpinnerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSpinButtonClicked();
	afx_msg void OnTimer(UINT_PTR TimerVal);
	DECLARE_MESSAGE_MAP()
private:
	CButton spinButton;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in TwisterSpinnerView.cpp
inline CTwisterSpinnerDoc* CTwisterSpinnerView::GetDocument() const
   { return reinterpret_cast<CTwisterSpinnerDoc*>(m_pDocument); }
#endif

