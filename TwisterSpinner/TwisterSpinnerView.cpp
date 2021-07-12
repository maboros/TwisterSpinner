
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
#include "FlickerFree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>


// CTwisterSpinnerView

IMPLEMENT_DYNCREATE(CTwisterSpinnerView, CView)

BEGIN_MESSAGE_MAP(CTwisterSpinnerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SPINBUTTON,OnSpinButtonClicked)
	ON_BN_CLICKED(IDC_ADDBUTTON,OnAddButtonClicked)
	ON_BN_CLICKED(IDC_REMOVEBUTTON, OnRemoveButtonClicked)
	ON_BN_CLICKED(IDC_NEWGAMEBUTTON, OnNewGameButtonClicked)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
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

}

CTwisterSpinnerView::~CTwisterSpinnerView()
{
}

BOOL CTwisterSpinnerView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

double spinAngle = 0;
bool listBoxMoved = false;
double pi = 3.14159;
void CTwisterSpinnerView::OnDraw(CDC* pDC)
{

	pDC->SetBkColor(RGB(200, 200, 200));
	CRect screen;
	GetWindowRect(screen);
	CPoint center = CPoint((screen.right - screen.left) / 2, (screen.bottom - screen.top) / 2);
	double r = 200;
	GetDlgItem(IDC_SPINBUTTON)->MoveWindow(center.x-50, center.y + r * 1.1, 100, 50);
	GetDlgItem(IDC_NEWGAMEBUTTON)->MoveWindow(center.x + r+50, center.y -25, 150,50);
	if (!listBoxMoved) {
		GetDlgItem(IDC_LISTBOX)->MoveWindow(center.x - r - 250, center.y - r, 220, 400, false);
		listBoxMoved = true;
	}
	GetDlgItem(IDC_TEXTBOX)->MoveWindow(center.x - r - 250, center.y - r-60,220,50);
	GetDlgItem(IDC_ADDBUTTON)->MoveWindow(center.x - r - 250, center.y +r+20, 100,50);
	GetDlgItem(IDC_REMOVEBUTTON)->MoveWindow(center.x - r - 140, center.y +r+20, 110, 50);

	CPoint previous = CPoint(center.x, center.y+r);
	pDC->MoveTo(previous);
	FlickerFreeDC::CMemDC dc{ pDC };
	dc.DrawText(L"Left hand", CRect(center.x - r-20, center.y - r, center.x - r + 50, center.x - r + 20), DT_SINGLELINE|DT_LEFT);
	dc.DrawText(L"Right hand", CRect(center.x + r - 20, center.y - r, center.x + r + 50, center.x - r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(L"Left leg", CRect(center.x - r - 20, center.y + r, center.x - r + 50, center.x + r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(L"Right leg", CRect(center.x + r - 20, center.y + r, center.x + r + 50, center.x + r + 20), DT_SINGLELINE | DT_LEFT);


	CTwisterSpinnerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;		
		
		
		int size=16;
		
		CPen thickPen(PS_SOLID, 5, RGB(0, 0, 0));
		dc.SelectObject(&thickPen);
		dc.Ellipse(center.x - r, center.y - r, center.x + r, center.y + r);
		CPen thinPen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&thinPen);
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
			myBrush brush((HDC)dc, myCol);
			double x =(double)center.x+ cos(2.0 * (1.0+i) * pi / size)*r;
			double y =(double)center.y+ sin(2.0 * (1.0+i) * pi / size)*r;
			
			
			dc.Pie(CRect(center.x-r,center.y-r,center.x+r,center.y+r), CPoint(x, y),previous);
			previous=CPoint(x,y);
		}
		{
			CPen drawPen(PS_SOLID, 3, RGB(0, 0, 0));
			r = r * 0.9;
			dc.SelectObject(&drawPen);
			dc.MoveTo(center);
			dc.LineTo(center.x + cos(spinAngle) * r, center.y + sin(spinAngle) * r);
			dc.Ellipse(center.x + cos(spinAngle) * r - 2, center.y + sin(spinAngle) * r - 2, center.x + cos(spinAngle) * r + 2, center.y + sin(spinAngle) * r + 2);
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
bool arrowSpun = false;
double spinAmount;
void CTwisterSpinnerView::OnSpinButtonClicked()
{
	timerVal = SetTimer(IDT_TIMER_0, 1, NULL);
	if (timerVal == 0)
	{
		MessageBox(L"Timer failed to initialize", L"IDT_TIMER_0", MB_OK | MB_SYSTEMMODAL);
	}
	while (spinAngle > 2*pi) {
		spinAmount -= 2 * pi;
		spinAngle -= 2 * pi;
	}
	//spinAmount += (1 + (int)(500.0 * rand() / (RAND_MAX + 1.0)))/10+ 2 * pi;
	srand(time(NULL));
	spinAmount += 4*pi +(rand()) / (RAND_MAX / (2*pi*6 - 4*pi));
	timerLive = true;
	arrowSpun = true;
	newGameButton.EnableWindow(false);
	addButton.EnableWindow(false);
	removeButton.EnableWindow(false);
	spinButton.EnableWindow(false);
	return;
}
void CTwisterSpinnerView::OnAddButtonClicked()
{
	//textBox.gettext
	listBox.AddString(L"Someting");
}
void CTwisterSpinnerView::OnRemoveButtonClicked()
{
	listBox.DeleteString(0);
}
void CTwisterSpinnerView::OnNewGameButtonClicked()
{
	while (listBox.GetCount() > 0) {
		listBox.DeleteString(0);
	}
	
}
void CTwisterSpinnerView::DisplayLimbAndColor() {
	CString limbAndColor;
	if (cos(spinAngle) < 0) {
		limbAndColor.Append(L"Left ");
	}
	else {
		limbAndColor.Append(L"Right ");
	}
	if (sin(spinAngle) <= 0) {
		limbAndColor.Append(L"hand ");
	}
	else {
		limbAndColor.Append(L"leg ");
	}
	double angleDeg = (spinAngle*180) / pi;
	while (angleDeg > 90) {
		angleDeg -= 90;
	}
	if (angleDeg >= 3 * 22.5)
	{
		limbAndColor.Append(L"yellow");
	}
	else if (angleDeg >= 2 * 22.5)
	{
		limbAndColor.Append(L"blue");
	}
	else if (angleDeg >= 22.5)
	{
		limbAndColor.Append(L"green");
	}
	else
	{
		limbAndColor.Append(L"red");
	}
	MessageBox((LPCTSTR)limbAndColor, L"You spun", MB_OK);
	newGameButton.EnableWindow(true);
	addButton.EnableWindow(true);
	removeButton.EnableWindow(true);
	spinButton.EnableWindow(true);
	arrowSpun = false;
}
void CTwisterSpinnerView::OnTimer(UINT_PTR TimerVal)
{
	if (spinAngle /spinAmount < 0.66&&spinAngle/spinAmount>0.33)
	{
		spinAngle += 0.05;
	}
	else if (spinAngle / spinAmount < 0.33)
	{
		spinAngle += 0.1;
	}
	else 
	{
		spinAngle += 0.025;
	}
	if (spinAngle > spinAmount)
	{
		if (arrowSpun) {
			arrowSpun = false;
			DisplayLimbAndColor();
		}
		KillTimer(timerVal);
		timerLive = false;
	}
	spinButton.Invalidate();
	Invalidate();
	return;
}



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

	srand(time(NULL));
	spinAmount=(rand()) / (RAND_MAX / (2 * pi));
	spinAngle = (rand()) / (RAND_MAX / (2 * pi));


	spinButton.Create(_T("Spin!"), BS_PUSHBUTTON, CRect(300, 100, 400, 150),this,IDC_SPINBUTTON);
	spinButton.ShowWindow(SW_SHOW);
	addButton.Create(_T("Add player"), BS_PUSHBUTTON, CRect(300, 100, 400, 150), this, IDC_ADDBUTTON);
	addButton.ShowWindow(SW_SHOW);
	removeButton.Create(_T("Remove player"), BS_PUSHBUTTON, CRect(300, 100, 400, 150), this, IDC_REMOVEBUTTON);
	removeButton.ShowWindow(SW_SHOW);
	newGameButton.Create(_T("Start new game"), BS_PUSHBUTTON, CRect(300, 100, 400, 150), this, IDC_NEWGAMEBUTTON);
	newGameButton.ShowWindow(SW_SHOW);

	textBox.Create(WS_CHILD | WS_VISIBLE|WS_BORDER, CRect(100, 100, 200, 500), this, IDC_TEXTBOX);
	listBox.Create(WS_CHILD | WS_VISIBLE|WS_BORDER,CRect(100,100,200,500),this,IDC_LISTBOX);
	return 0;
}


BOOL CTwisterSpinnerView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}


BOOL CTwisterSpinnerView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SIZE) {
		listBoxMoved = false;
	}
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}
