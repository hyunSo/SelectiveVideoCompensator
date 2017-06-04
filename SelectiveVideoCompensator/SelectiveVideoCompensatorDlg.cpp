// SelectiveVideoCompensatorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SelectiveVideoCompensator.h"
#include "SelectiveVideoCompensatorDlg.h"
#include "afxdialogex.h"
#include "opencv\cv.h"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv\cxcore.h"
#include "opencv2\highgui.hpp"

#include <iostream>

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSelectiveVideoCompensatorDlg ��ȭ ����



CSelectiveVideoCompensatorDlg::CSelectiveVideoCompensatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectiveVideoCompensatorDlg::IDD, pParent)
	, m_strR(_T(""))
	, m_strG(_T(""))
	, m_strB(_T(""))
	, m_strH(_T(""))
	, m_strBr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSelectiveVideoCompensatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_R, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_G, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_B, m_sliderBlue);
	DDX_Control(pDX, IDC_SLIDER_H, m_sliderHistogram);
	DDX_Control(pDX, IDC_SLIDER_BR, m_sliderBrightness);
	DDX_Text(pDX, IDC_EDIT3, m_strR);
	DDX_Text(pDX, IDC_EDIT4, m_strG);
	DDX_Text(pDX, IDC_EDIT5, m_strB);
	DDX_Text(pDX, IDC_EDIT6, m_strH);
	DDX_Text(pDX, IDC_EDIT7, m_strBr);
}

BEGIN_MESSAGE_MAP(CSelectiveVideoCompensatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN32771, &CSelectiveVideoCompensatorDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE32772, &CSelectiveVideoCompensatorDlg::OnFileSave)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_R, &CSelectiveVideoCompensatorDlg::OnNMCustomdrawSliderR)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_R, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderRed)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_G, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderGreen)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_B, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBlue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_H, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderHistogram)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BR, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBrightness)
	ON_EN_CHANGE(IDC_EDIT3, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit7)
END_MESSAGE_MAP()


// CSelectiveVideoCompensatorDlg �޽��� ó����

BOOL CSelectiveVideoCompensatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_sliderRed.SetRange(0, 100);
	m_sliderRed.SetPos(0);
	m_sliderGreen.SetRange(0, 100);
	m_sliderGreen.SetPos(0);
	m_sliderBlue.SetRange(0, 100);
	m_sliderBlue.SetPos(0);
	m_sliderHistogram.SetRange(0, 100);
	m_sliderHistogram.SetPos(0);
	m_sliderBrightness.SetRange(0, 100);
	m_sliderBrightness.SetPos(0);

	str.Format(_T("%d"), 0); //Format�� �̿��Ͽ� int���� ����
	m_strR.SetString(str);
	m_strG.SetString(str);
	m_strB.SetString(str);
	m_strH.SetString(str);
	m_strBr.SetString(str);
	UpdateData(false); //set�� ���ֱ� ���ؼ� UpdateData(false); �� ���

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelectiveVideoCompensatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSelectiveVideoCompensatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSelectiveVideoCompensatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSelectiveVideoCompensatorDlg::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString szFilter = _T("Video Files (*.mp4)|*.mp4|All Files (*.*)|*.*|");
    CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, szFilter);
 
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		CT2CA tempstr(pathName);
		path = string(tempstr);		
		VideoCapture capture(path);
		if (!capture.isOpened()){
			MessageBox(CString("Open Failure"));
			return;
		}

		Mat matFrame, temp;		
		IplImage *iplFrame;//, *mask;
		cvNamedWindow("main");
		capture.read(matFrame);
		temp = matFrame.clone();
		iplFrame=&IplImage(matFrame);
		int WIDTH=iplFrame->width;
		int HEIGHT=iplFrame->height;
		cvShowImage("main", iplFrame);
		MessageBox(pathName);
	}

}


void CSelectiveVideoCompensatorDlg::OnFileSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CSelectiveVideoCompensatorDlg::OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		*pResult = 0;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CSelectiveVideoCompensatorDlg::OnEnChangeEdit3()
{}
void CSelectiveVideoCompensatorDlg::OnEnChangeEdit4()
{}
void CSelectiveVideoCompensatorDlg::OnEnChangeEdit5()
{}
void CSelectiveVideoCompensatorDlg::OnEnChangeEdit6()
{}
void CSelectiveVideoCompensatorDlg::OnEnChangeEdit7()
{}

void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	int posR = m_sliderRed.GetPos();
	str.Format(_T("%d"), posR); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT3, str);
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	int posG = m_sliderGreen.GetPos();
	str.Format(_T("%d"), posG); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT4, str);
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	int posB = m_sliderBlue.GetPos();
	str.Format(_T("%d"), posB); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT5, str);
}



void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderHistogram(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	int posH = m_sliderHistogram.GetPos();
	str.Format(_T("%d"), posH); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT6, str);
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	int posBr = m_sliderBrightness.GetPos();
	str.Format(_T("%d"), posBr); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT7, str);
}
