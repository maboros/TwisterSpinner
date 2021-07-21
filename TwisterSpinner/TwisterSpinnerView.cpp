
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
#include "resource.h"
//#define M_PI   3.14159265358979323846264338327950288
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>


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
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTwisterSpinnerView construction/destruction


CTwisterSpinnerView::CTwisterSpinnerView() noexcept
{
	spinAmount = 0;
}

void CTwisterSpinnerView::Reset()
{
	spinButton.EnableWindow(false);
	removeButton.EnableWindow(false);
	OnNewGameButtonClicked();
}

CTwisterSpinnerView::~CTwisterSpinnerView()
{
}

BOOL CTwisterSpinnerView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

COLORREF giveColor(int i){
	switch (i % 4) {
	case 0:
		return  RGB(255, 0, 0);
		break;
	case 1:
		return  RGB(0, 255, 0);
		break;
	case 2:
		return  RGB(0, 0, 255);
		break;
	default:
		return  RGB(255, 255, 0);
		break;
	}
}
CString CTwisterSpinnerView::fetchResString(int resId)
{
	CString str;
	str.LoadString(resId);
	return str;
}
void CTwisterSpinnerView::OnDraw(CDC* pDC)
{
	//Init of variables and dc

	CRect screen;
	GetClientRect(screen);
	CPoint center = CPoint((screen.right - screen.left) / 2, (screen.bottom - screen.top) / 2);
	CPoint previous = CPoint(center.x, center.y+r);
	pDC->SetBkColor(RGB(200, 200, 200));
	FlickerFreeDC::CMemDC dc{ pDC };

	//Drawing labels

	CFont font;
	int iHeight = -MulDiv(9, ::GetDeviceCaps(dc, LOGPIXELSY), 72);
	font.CreateFont(iHeight, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Comic Sans"));
	dc.SelectObject(&font);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING), CRect(center.x - r - 320, center.y - r, center.x - r - 200, center.y - r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+1), CRect(center.x - r-20, center.y - r, center.x - r + 150, center.x - r + 20), DT_SINGLELINE|DT_LEFT);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+2), CRect(center.x + r - 20, center.y - r, center.x + r + 150, center.x - r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+3), CRect(center.x - r - 20, center.y + r, center.x - r + 150, center.x + r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+4), CRect(center.x + r - 20, center.y + r, center.x + r + 150, center.x + r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+5), CRect(center.x - r - 380, center.y - r - 60, center.x - r - 250, center.y - r - 40), DT_SINGLELINE | DT_LEFT);
	if (gameStarted) {
		if (currentIndex == listBox.GetCount()) {
			currentIndex = 0;
		}
		CString playerTurn;
		listBox.GetText(currentIndex,playerTurn);
		playerTurn.Append(fetchResString(IDS_TURN));
		dc.DrawText(playerTurn, CRect(center.x - 200, center.y - r - 30, center.x + 200, center.x - r - 10), DT_SINGLELINE | DT_CENTER);
	}
	CFont fontBig;
	iHeight = -MulDiv(18, ::GetDeviceCaps(dc, LOGPIXELSY), 72);
	fontBig.CreateFont(iHeight,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN,_T("Comic Sans"));
	dc.SelectObject(&fontBig);
	dc.DrawText(fetchResString(IDS_PLAYERSTRING+6), CRect(center.x - 200, center.y - r - 70, center.x + 200, center.x - r - 50), DT_SINGLELINE | DT_CENTER);

	//Drawing the wheel

	dc.MoveTo(previous);
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
			COLORREF myCol=giveColor(i);
			CBrush coloredBrush(myCol);
			dc.SelectObject(coloredBrush);
			double x =(double)center.x+ cos(2.0 * (1.0+i) * M_PI / size)*r;
			double y =(double)center.y+ sin(2.0 * (1.0+i) * M_PI / size)*r;	
			dc.Pie(CRect(center.x-r,center.y-r,center.x+r,center.y+r), CPoint(x, y),previous);
			previous=CPoint(x,y);
		}

		//Draw the arrow

		CPen drawPen(PS_SOLID, 3, RGB(0, 0, 0));
		double arrowR = r * 0.9;
		dc.SelectObject(&drawPen);
		dc.MoveTo(center);
		dc.LineTo(center.x + cos(spinAngle) * arrowR, center.y + sin(spinAngle) * arrowR);
		dc.Ellipse(center.x + cos(spinAngle) * arrowR - 2, center.y + sin(spinAngle) * arrowR - 2, center.x + cos(spinAngle) * arrowR + 2, center.y + sin(spinAngle) * arrowR + 2);
		
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

	
void CTwisterSpinnerView::OnSpinButtonClicked()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	timerVal = SetTimer(IDT_TIMER_0, 1, NULL);
	if (timerVal == 0)
	{
		MessageBox(fetchResString(IDS_TIMERMSG), fetchResString(IDS_TIMERFAIL), MB_OK | MB_SYSTEMMODAL);
	}
	while (spinAngle > 2*M_PI) {
		spinAmount -= 2 * M_PI;
		spinAngle -= 2 * M_PI;
	}
	std::uniform_real_distribution<double> dist(5*M_PI, 7*M_PI);	
	spinAmount +=dist(mt);
	arrowSpun = true;
	newGameButton.EnableWindow(false);
	addButton.EnableWindow(false);
	removeButton.EnableWindow(false);
	spinButton.EnableWindow(false);
	return;
}
void CTwisterSpinnerView::OnAddButtonClicked()
{
	CString name;
	textBox.GetWindowText(name);
	if (name.IsEmpty()) {
		return;
	}
	textBox.SetWindowText(L"");
	listBox.AddString(name);
	removeButton.EnableWindow(true);
}
void CTwisterSpinnerView::OnRemoveButtonClicked()
{
	int index = listBox.GetCurSel();
	if ( index< currentIndex&&gameStarted&&currentIndex!=0) {
		currentIndex-= 1;
	}
	listBox.DeleteString(index);
	if (listBox.GetCount() == 1&&gameStarted) {
		CString winner;
		listBox.GetText(0, winner);
		winner.Append(fetchResString(IDS_WINNER));
		MessageBox(winner, fetchResString(IDS_GRATS), MB_OK);
		Reset();
	}
	if (listBox.GetCount() == 0) {
		removeButton.EnableWindow(false);
	}
	Invalidate();
}
void CTwisterSpinnerView::OnNewGameButtonClicked()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	if (gameStarted)
	{
		if (MessageBox(fetchResString(IDS_NEWGAMEQ), fetchResString(IDS_GAMEOVER), MB_YESNO) == IDYES)
		{
			while (listBox.GetCount() > 0) {
				listBox.DeleteString(0);
			}
			std::uniform_real_distribution<double> dist(0, 2 * M_PI);
			spinAmount = dist(mt);
			spinAngle = dist(mt);
			currentIndex = 0;
			gameStarted = false;
			addButton.EnableWindow(true);
			spinButton.EnableWindow(false);
			removeButton.EnableWindow(true);
		}	
	}
	else {
		if (listBox.GetCount() >= 2) {
			gameStarted = true;
			spinButton.EnableWindow(true);
			addButton.EnableWindow(false);
			removeButton.EnableWindow(false);
		}
	}
	Invalidate();
	
}
void CTwisterSpinnerView::DisplayLimbAndColor() {
	CString limbAndColor;
	double halfCircle = 180;
	if (cos(spinAngle) < 0) {
		limbAndColor.Append(fetchResString(IDS_LEFT));
	}
	else {
		limbAndColor.Append(fetchResString(IDS_RIGHT));
	}
	if (sin(spinAngle) <= 0) {
		limbAndColor.Append(fetchResString(IDS_HAND));
	}
	else {
		limbAndColor.Append(fetchResString(IDS_LEG));
	}
	double angleDeg = (spinAngle* halfCircle) / M_PI;
	while (angleDeg > halfCircle/2) {
		angleDeg -= halfCircle/2;
	}
	if (angleDeg >= 3 * halfCircle/8)
	{
		limbAndColor.Append(fetchResString(IDS_YELLOW));
	}
	else if (angleDeg >= 2 * halfCircle/8)
	{
		limbAndColor.Append(fetchResString(IDS_BLUE));
	}
	else if (angleDeg >= halfCircle/8)
	{
		limbAndColor.Append(fetchResString(IDS_GREEN));
	}
	else
	{
		limbAndColor.Append(fetchResString(IDS_RED));
	}
	MessageBox(limbAndColor, fetchResString(IDS_SPUN), MB_OK);
	currentIndex += 1;
	arrowSpun = false;
	newGameButton.EnableWindow(true);
	removeButton.EnableWindow(true);
	spinButton.EnableWindow(true);
	Invalidate();
}
void CTwisterSpinnerView::OnTimer(UINT_PTR TimerVal)
{
	double angleIncr = 0.025;
	double third = 1.0 / 3.0;
	if (spinAngle /spinAmount < 2*third&&spinAngle/spinAmount>third)
	{
		spinAngle += angleIncr*2;
	}
	else if (spinAngle / spinAmount < third)
	{
		spinAngle += angleIncr*4;
	}
	else 
	{
		spinAngle += angleIncr;
	}
	if (spinAngle > spinAmount)
	{
		if (arrowSpun) {
			arrowSpun = false;
			DisplayLimbAndColor();
		}
		KillTimer(timerVal);
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
int CTwisterSpinnerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	srand(time(NULL));
	spinAmount=(rand()) / (RAND_MAX / (2 * M_PI));
	spinAngle = (rand()) / (RAND_MAX / (2 * M_PI));


	spinButton.Create(fetchResString(IDS_SPIN), BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150),this,IDC_SPINBUTTON);
	spinButton.EnableWindow(false);

	addButton.Create(fetchResString(IDS_ADDPLAYER), BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_ADDBUTTON);
	
	removeButton.Create(fetchResString(IDS_REMOVEPLAYER), BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_REMOVEBUTTON);
	removeButton.EnableWindow(false);
	
	newGameButton.Create(fetchResString(IDS_STARTNEWGAMEB), BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_NEWGAMEBUTTON);

	textBox.Create(WS_CHILD | WS_VISIBLE|WS_BORDER, CRect(100, 100, 200, 500), this, IDC_TEXTBOX);
	listBox.Create(WS_CHILD | WS_VISIBLE|WS_BORDER,CRect(100,100,200,500),this,IDC_LISTBOX);
	return 0;
}


BOOL CTwisterSpinnerView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}


void CTwisterSpinnerView::OnSize(UINT nType, int cx, int cy)
{

	CRect screen;
	GetClientRect(screen);
	CPoint center = CPoint((screen.right - screen.left) / 2, (screen.bottom - screen.top) / 2);
	GetDlgItem(IDC_SPINBUTTON)->MoveWindow(center.x - 50, center.y + r * 1.1, 100, 50);
	GetDlgItem(IDC_NEWGAMEBUTTON)->MoveWindow(center.x + r + 50, center.y - 25, 150, 50);
	if (!listBoxMoved) {
		GetDlgItem(IDC_LISTBOX)->MoveWindow(center.x - r - 250, center.y - r, 220, 400, false);
		listBoxMoved = true;
	}
	GetDlgItem(IDC_TEXTBOX)->MoveWindow(center.x - r - 250, center.y - r - 60, 220, 50);
	GetDlgItem(IDC_ADDBUTTON)->MoveWindow(center.x - r - 250, center.y + r + 20, 110, 50);
	GetDlgItem(IDC_REMOVEBUTTON)->MoveWindow(center.x - r - 140, center.y + r + 20, 110, 50);
	listBoxMoved = false;

	CView::OnSize(nType, cx, cy);
}
