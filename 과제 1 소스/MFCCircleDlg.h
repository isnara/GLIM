
// MFCCircleDlg.h: 헤더 파일
//

#pragma once

#define POINT_COUNT				3
#define POINT_RADIUS			10
#define CIRCLE_THICKNESS		6


struct stPoint
{
	int x;
	int y;
};


// CMFCCircleDlg 대화 상자
class CMFCCircleDlg : public CDialogEx
{
private:
	CImage		m_image;

	stPoint		m_arrPoint[POINT_COUNT] = {};
	int			m_nPointWriteIndex = 0;

	// 포인트 이동
	bool		m_bDragging = false;
	int			m_nDraggingPointIndex = -1;

	// 랜덤
	bool 		m_bRandomRunning = false;

	// 설정값
	int			m_nPointRadius = POINT_RADIUS;
	int			m_nCircleThickness = CIRCLE_THICKNESS;

// 생성입니다.
public:
	CMFCCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	void ClearImage();

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void Render();
	void DrawPoint(unsigned char* pfm, stPoint& center, int radius);
	void DrawCircle(unsigned char* pfm, int thickness);
	void TextCoordinate(stPoint center);

	static UINT RandomThreadProcess(LPVOID pParam);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedRandom();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void OnOK();

public:
	void OnRandomPoint();

	void SetRandomRunning(bool bRunning) { m_bRandomRunning = bRunning; }
};
