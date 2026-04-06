
// MFCCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCCircle.h"
#include "MFCCircleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCCircleDlg 대화 상자



CMFCCircleDlg::CMFCCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCIRCLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCCircleDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCCircleDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDCLEAR, &CMFCCircleDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDRANDOM, &CMFCCircleDlg::OnBnClickedRandom)
END_MESSAGE_MAP()


// CMFCCircleDlg 메시지 처리기

BOOL CMFCCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetClientRect(&rect);
	m_image.Create(rect.Width(), -rect.Height(), 8);

	// 컬러 테이블 설정
	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
		rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
	m_image.SetColorTable(0, 256, rgb);

	// 인덱스 초기화
	m_nPointWriteIndex = 0;

	// 설정값 초기화
	SetDlgItemInt(IDC_EDIT1, m_nPointRadius);
	SetDlgItemInt(IDC_EDIT2, m_nCircleThickness);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCCircleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		Render();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCCircleDlg::ClearImage()
{
	memset((unsigned char*)m_image.GetBits(), 0xff, sizeof(unsigned char) * m_image.GetWidth() * m_image.GetHeight());
}

void CMFCCircleDlg::Render()
{
	// 클리어
	ClearImage();

	unsigned char* pfm = (unsigned char*)m_image.GetBits();

	// 포인트 그리기
	for (int i = 0; i < m_nPointWriteIndex; ++i)
		DrawPoint(pfm, m_arrPoint[i], m_nPointRadius);

	// 원 그리기
	if (m_nPointWriteIndex >= POINT_COUNT)
		DrawCircle(pfm, m_nCircleThickness);

	CClientDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	m_image.Draw(dc, 0, 0);
}

void CMFCCircleDlg::DrawPoint(unsigned char* pfm, stPoint& center, int radius)
{
	int nStartX = center.x - radius;
	int nStartY = center.y - radius;
	int nPitch = m_image.GetPitch();

	for (int i = nStartY; i < nStartY + radius * 2; ++i)
	{
		// 영역 체크
		if (i < 0 || i >= m_image.GetHeight())
			continue;

		for(int j = nStartX; j < nStartX + radius * 2; ++j)
		{
			// 영역 체크
			if (j < 0 || j >= m_image.GetWidth())
				continue;

			int dx = j - center.x;
			int dy = i - center.y;
			if (dx * dx + dy * dy <= radius * radius)
				pfm[i * nPitch + j] = 0x00;
		}
	}

	// 글자
	TextCoordinate(center);
}

void CMFCCircleDlg::DrawCircle(unsigned char* pfm, int thickness)
{
	// 원 계산
	double x1 = m_arrPoint[0].x;
	double y1 = m_arrPoint[0].y;
	double x2 = m_arrPoint[1].x;
	double y2 = m_arrPoint[1].y;
	double x3 = m_arrPoint[2].x;
	double y3 = m_arrPoint[2].y;

	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

	// 매우 작아지는 D값 예외처리
	if (fabs(D) < 0.000001)
		return;

	double dCenterX = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	double dCenterY = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

	double dx = dCenterX - m_arrPoint[0].x;
	double dy = dCenterY - m_arrPoint[0].y;

	double dRadius = sqrt(dx * dx + dy * dy);
	TRACE(_T("반지름: %f\n"), dRadius);


	// 그리기
	int nPitch = m_image.GetPitch();

	// 반지름에 따른 원 그리기 간격
	double dAngleInterval = 0.01 / max(1.0, 10 * ((int)dRadius / 50));
	//TRACE(_T("각간격: %f\n"), dAngleInterval);

	for (int t = 0; t < thickness; ++t)
	{
		double rr = (dRadius - (thickness * 0.5)) + t;

		for (double angle = 0; angle < 2 * 3.141592; angle += dAngleInterval)
		{
			int nX = dCenterX + rr * cos(angle);
			int nY = dCenterY + rr * sin(angle);

			// 화면 영역 예외처리
			if(nX < 0 || nY < 0 || nX >= m_image.GetWidth() || nY >= m_image.GetHeight())
				continue;

			pfm[nY * nPitch + nX] = 0x00;
		}
	}
}

void CMFCCircleDlg::TextCoordinate(stPoint center)
{
	// 2. DC 가져오기
	HDC hDC = m_image.GetDC();

	// 3. 글자 그리기 설정
	CDC* pDC = CDC::FromHandle(hDC);
	pDC->SetTextColor(RGB(0, 255, 0)); // 글자 색상
	pDC->SetBkMode(TRANSPARENT);      // 배경 투명

	// 4. 글자 쓰기
	CString str;
	pDC->TextOut(center.x + 7, center.y + 7, (str.Format(_T("(%d, %d)"), center.x, center.y), str));

	// 5. DC 해제
	m_image.ReleaseDC();
}

UINT CMFCCircleDlg::RandomThreadProcess(LPVOID pParam)
{
	CMFCCircleDlg* pDlg = (CMFCCircleDlg*)pParam;

	pDlg->SetRandomRunning(true);

	for (int i = 0; i < 10; i++)
	{
		Sleep(500);

		pDlg->OnRandomPoint();
	}

	pDlg->SetRandomRunning(false);

	return 0;
}

void CMFCCircleDlg::OnRandomPoint()
{
	// 포인터 랜덤 설정
	for (int i = 0; i < POINT_COUNT; i++)
	{
		m_arrPoint[i].x = rand() % m_image.GetWidth();
		m_arrPoint[i].y = rand() % m_image.GetHeight();
	}

	m_nPointWriteIndex = POINT_COUNT;

	Invalidate(FALSE);
}

void CMFCCircleDlg::OnOK()
{
	// 아무것도 안 함 → 종료 방지
}

void CMFCCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);

	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	if (m_nPointWriteIndex < POINT_COUNT)
	{
		//TRACE(_T("좌표: x=%d, y=%d\n"), point.x, point.y);

		if (m_nPointWriteIndex == POINT_COUNT)
			return;

		// 셋팅
		m_arrPoint[m_nPointWriteIndex].x = point.x;
		m_arrPoint[m_nPointWriteIndex++].y = point.y;

		// 화면 갱신
		Invalidate(FALSE); // 배경 지우지 않음 (깜빡임 감소)
	}
	else
	{
		// 포인트 선택
		for (int i = 0; i < POINT_COUNT; i++)
		{
			double dx = point.x - m_arrPoint[i].x;
			double dy = point.y - m_arrPoint[i].y;

			if (sqrt(dx * dx + dy * dy) < m_nPointRadius)
			{
				m_bDragging = true;
				m_nDraggingPointIndex = i;
				break;
			}
		}
	}
}

void CMFCCircleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);

	// 예외처리
	if (m_bDragging == false || m_nDraggingPointIndex == -1 || m_bRandomRunning == true)
		return;

	m_arrPoint[m_nDraggingPointIndex] = { (int)point.x, (int)point.y };

	Invalidate(FALSE);
}

void CMFCCircleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);

	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	m_bDragging = false;
	m_nDraggingPointIndex = -1;
}

void CMFCCircleDlg::OnBnClickedClear()
{
	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	memset(m_arrPoint, 0, sizeof(stPoint) * POINT_COUNT);

	m_nPointWriteIndex = 0;

	// 화면 갱신
	Invalidate(FALSE); // 배경 지우지 않음 (깜빡임 감소)
}

void CMFCCircleDlg::OnBnClickedRandom()
{
	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	AfxBeginThread(RandomThreadProcess, this);
}

void CMFCCircleDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	m_nPointRadius = GetDlgItemInt(IDC_EDIT1);

	// 화면 갱신
	Invalidate(FALSE); // 배경 지우지 않음 (깜빡임 감소)
}

void CMFCCircleDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 랜덤 동작이면 패스
	if (m_bRandomRunning)
		return;

	m_nCircleThickness = GetDlgItemInt(IDC_EDIT2);

	// 화면 갱신
	Invalidate(FALSE); // 배경 지우지 않음 (깜빡임 감소)
}
