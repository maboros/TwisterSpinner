
// TwisterSpinnerView.cpp : implementation of the CTwisterSpinnerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TwisterSpinner.h"
#endif

#include "TwisterSpinnerDoc.h"
#include "TwisterSpinnerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTwisterSpinnerView

IMPLEMENT_DYNCREATE(CTwisterSpinnerView, CView)

BEGIN_MESSAGE_MAP(CTwisterSpinnerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SPINBUTTON,OnSpinButtonClicked)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTwisterSpinnerView construction/destruction

class myBrush {
	HDC hdc;
	HBRUSH brush;
public:
	myBrush(HDC hdc, COLORREF color) {
		this->hdc = hdc;
		this->brush = CreateSolidBrush(color);
		SelectObject(hdc, brush);
	}
	~myBrush() {
		DeleteObject(SelectObject(hdc, brush));
	}
};

CTwisterSpinnerView::CTwisterSpinnerView() noexcept
{
	// TODO: add construction code here

}

CTwisterSpinnerView::~CTwisterSpinnerView()
{
}

BOOL CTwisterSpinnerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTwisterSpinnerView drawing
double spinAngle = 0;

void CTwisterSpinnerView::OnDraw(CDC* pDC)
{
	CPoint previous = CPoint(150, 200);
	int midX = 150;
	int midY = 150;
	CTwisterSpinnerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: add draw code for native data here
		
		double r=120;
		double pi = 3.14159;
		int size=16;
		pDC->MoveTo(previous);
		//HGDIOBJ brush = pDC->SelectObject(GetStockObject(DC_BRUSH));
		for (int i = 0; i <= size; i++) {
			COLORREF myCol;
			switch (i % 4) {
			case 0:
				myCol =RGB(255, 0, 0);
				break;
			case 1:
				myCol = RGB(0, 255, 0);
				break;
			case 2:
				myCol = RGB(0, 0, 255);
				break;
			default:
				myCol = RGB(255, 255, 0);
				break;
			}
			myBrush brush((HDC)*pDC, myCol);
			double x =(double)midX+ cos(2.0 * (1.0+i) * pi / size+spinAngle)*r;
			double y =(double)midY+ sin(2.0 * (1.0+i) * pi / size+spinAngle)*r;
			pDC->Pie(CRect(midX-r,midY-r,midX+r,midY+r), CPoint(x, y),previous);
			previous=CPoint(x,y);
		}
}

void CTwisterSpinnerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	
}

void CTwisterSpinnerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

UINT_PTR timerVal;
bool timerLive;
void CTwisterSpinnerView::OnSpinButtonClicked()
{
	timerVal = SetTimer(IDT_TIMER_0, 100, NULL);
	if (timerVal == 0)
	{
		MessageBox(L"Timer dead", L"IDT_TIMER_0", MB_OK | MB_SYSTEMMODAL);
	}
	timerLive = true;
	return;
}

void CTwisterSpinnerView::OnTimer(UINT_PTR TimerVal)
{
	spinAngle += 0.1;
	if (spinAngle > 6.3)
	{
		spinAngle = 0;
		KillTimer(timerVal);
		timerLive = false;
	}
	spinButton.Invalidate();
	Invalidate();
	return;
}


// CTwisterSpinnerView diagnostics

#ifdef _DEBUG
void CTwisterSpinnerView::AssertValid() const
{
	CView::AssertValid();
}

void CTwisterSpinnerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTwisterSpinnerDoc* CTwisterSpinnerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTwisterSpinnerDoc)));
	return (CTwisterSpinnerDoc*)m_pDocument;
}
#endif //_DEBUG


// CTwisterSpinnerView message handlers


int CTwisterSpinnerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	spinButton.Create(_T("Spin!"), BS_PUSHBUTTON, CRect(100, 100, 150, 150),this,IDC_SPINBUTTON);
	spinButton.ShowWindow(SW_SHOW);
	return 0;
}
