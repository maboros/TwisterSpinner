
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
#define M_PI   3.14159265358979323846264338327950288
#include<random>
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
void CTwisterSpinnerView::OnDraw(CDC* pDC)
{
	//Init of variables and dc

	CRect screen;
	GetWindowRect(screen);
	CPoint center = CPoint((screen.right - screen.left) / 2, (screen.bottom - screen.top) / 2);
	CPoint previous = CPoint(center.x, center.y+r);
	pDC->SetBkColor(RGB(200, 200, 200));
	FlickerFreeDC::CMemDC dc{ pDC };

	//Drawing labels

	CString stringFetcher[7];
	for (int i = 0; i < 7; ++i)
	{
		stringFetcher[i].LoadString(IDS_PLAYERSTRING+i);
	}
	CFont font;
	int iHeight = -MulDiv(9, ::GetDeviceCaps(dc, LOGPIXELSY), 60);
	int iWidth = -MulDiv(7, ::GetDeviceCaps(dc, LOGPIXELSX), 90);
	font.CreateFont(iHeight, iWidth, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Comic Sans"));
	dc.SelectObject(&font);
	dc.DrawText(stringFetcher[0], CRect(center.x - r - 320, center.y - r, center.x - r - 250, center.y - r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(stringFetcher[1], CRect(center.x - r-20, center.y - r, center.x - r + 50, center.x - r + 20), DT_SINGLELINE|DT_LEFT);
	dc.DrawText(stringFetcher[2], CRect(center.x + r - 20, center.y - r, center.x + r + 50, center.x - r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(stringFetcher[3], CRect(center.x - r - 20, center.y + r, center.x - r + 50, center.x + r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(stringFetcher[4], CRect(center.x + r - 20, center.y + r, center.x + r + 50, center.x + r + 20), DT_SINGLELINE | DT_LEFT);
	dc.DrawText(stringFetcher[5], CRect(center.x - r - 370, center.y - r - 60, center.x - r - 250, center.y - r - 40), DT_SINGLELINE | DT_LEFT);
	if (gameStarted) {
		if (currentIndex == listBox.GetCount()) {
			currentIndex = 0;
		}
		CString playerTurn;
		listBox.GetText(currentIndex,playerTurn);
		CString turn;
		turn.LoadString(IDS_TURN);
		playerTurn.Append(turn);
		dc.DrawText(playerTurn, CRect(center.x - 200, center.y - r - 30, center.x + 200, center.x - r - 10), DT_SINGLELINE | DT_CENTER);
	}
	CFont fontBig;
	iHeight = -MulDiv(18, ::GetDeviceCaps(dc, LOGPIXELSY), 72);
	fontBig.CreateFont(iHeight,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN,_T("Comic Sans"));
	dc.SelectObject(&fontBig);
	dc.DrawText(stringFetcher[6], CRect(center.x - 200, center.y - r - 70, center.x + 200, center.x - r - 50), DT_SINGLELINE | DT_CENTER);

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

	std::random_device rd;
	std::mt19937 mt(rd());
void CTwisterSpinnerView::OnSpinButtonClicked()
{
	timerVal = SetTimer(IDT_TIMER_0, 1, NULL);
	if (timerVal == 0)
	{
		MessageBox(L"Timer failed to initialize", L"IDT_TIMER_0", MB_OK | MB_SYSTEMMODAL);
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
	textBox.GetWindowTextW(name);
	if (name.IsEmpty()) {
		return;
	}
	textBox.SetWindowTextW(L"");
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
		CString stringFetcher;
		stringFetcher.LoadString(IDS_WINNER);
		winner.Append(stringFetcher);
		stringFetcher.LoadString(IDS_GRATS);
		MessageBox(winner, stringFetcher, MB_OK);
		Reset();
	}
	Invalidate();
}
void CTwisterSpinnerView::OnNewGameButtonClicked()
{
	if (gameStarted)
	{
		CString stringFetcher;
		stringFetcher.LoadString(IDS_GAMEOVER);
		CString stringFetcher2;
		stringFetcher2.LoadString(IDS_NEWGAMEQ);
		if (MessageBox(stringFetcher2,stringFetcher, MB_YESNO) == IDYES)
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
	CString stringFetcher;
	
	if (cos(spinAngle) < 0) {
		stringFetcher.LoadString(IDS_LEFT);
		limbAndColor.Append(stringFetcher);
	}
	else {
		stringFetcher.LoadString(IDS_RIGHT);
		limbAndColor.Append(stringFetcher);
	}
	if (sin(spinAngle) <= 0) {
		stringFetcher.LoadString(IDS_HAND);
		limbAndColor.Append(stringFetcher);
	}
	else {
		stringFetcher.LoadString(IDS_LEG);
		limbAndColor.Append(stringFetcher);
	}
	double angleDeg = (spinAngle*180) / M_PI;
	while (angleDeg > 90) {
		angleDeg -= 90;
	}
	if (angleDeg >= 3 * 22.5)
	{
		stringFetcher.LoadString(IDS_YELLOW);
		limbAndColor.Append(stringFetcher);
	}
	else if (angleDeg >= 2 * 22.5)
	{
		stringFetcher.LoadString(IDS_BLUE);
		limbAndColor.Append(stringFetcher);
	}
	else if (angleDeg >= 22.5)
	{
		stringFetcher.LoadString(IDS_GREEN);
		limbAndColor.Append(stringFetcher);
	}
	else
	{
		stringFetcher.LoadString(IDS_RED);
		limbAndColor.Append(stringFetcher);
	}
	stringFetcher.LoadString(IDS_SPUN);
	MessageBox((LPCTSTR)limbAndColor, stringFetcher, MB_OK);
	currentIndex += 1;
	arrowSpun = false;
	newGameButton.EnableWindow(true);
	removeButton.EnableWindow(true);
	spinButton.EnableWindow(true);
	Invalidate();
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

	CString stringFetcher;

	stringFetcher.LoadString(IDS_SPIN);
	spinButton.Create(stringFetcher, BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150),this,IDC_SPINBUTTON);
	spinButton.EnableWindow(false);

	stringFetcher.LoadString(IDS_ADDPLAYER);
	addButton.Create(stringFetcher, BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_ADDBUTTON);
	
	stringFetcher.LoadString(IDS_REMOVEPLAYER);
	removeButton.Create(stringFetcher, BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_REMOVEBUTTON);
	removeButton.EnableWindow(false);
	
	stringFetcher.LoadString(IDS_STARTNEWGAMEB);
	newGameButton.Create(stringFetcher, BS_PUSHBUTTON | WS_VISIBLE, CRect(300, 100, 400, 150), this, IDC_NEWGAMEBUTTON);

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
		CRect screen;
		GetWindowRect(screen);
		CPoint center = CPoint((screen.right - screen.left) / 2, (screen.bottom - screen.top) / 2);
		GetDlgItem(IDC_SPINBUTTON)->MoveWindow(center.x - 50, center.y + r * 1.1, 100, 50);
		GetDlgItem(IDC_NEWGAMEBUTTON)->MoveWindow(center.x + r + 50, center.y - 25, 150, 50);
		if (!listBoxMoved) {
			GetDlgItem(IDC_LISTBOX)->MoveWindow(center.x - r - 250, center.y - r, 220, 400, false);
			listBoxMoved = true;
		}
		GetDlgItem(IDC_TEXTBOX)->MoveWindow(center.x - r - 250, center.y - r - 60, 220, 50);
		GetDlgItem(IDC_ADDBUTTON)->MoveWindow(center.x - r - 250, center.y + r + 20, 100, 50);
		GetDlgItem(IDC_REMOVEBUTTON)->MoveWindow(center.x - r - 140, center.y + r + 20, 110, 50);
		listBoxMoved = false;
	}

	return CView::OnWndMsg(message, wParam, lParam, pResult);
}
