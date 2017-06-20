// SelectiveVideoCompensatorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SelectiveVideoCompensator.h"
#include "SelectiveVideoCompensatorDlg.h"
#include "afxdialogex.h"
#include "opencv\cv.h"
#include "opencv2\opencv.hpp"
#include "opencv\cxcore.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <iostream>

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PIM1 0
#define MP42 1
#define I420 2
#define FLV1 3
#define MJPG 4

Mat matFrame;
Mat matFrameCrop;
Mat matFrameAdjust;
int LTx, LTy, RBx, RBy;
int posSm;
double posR, posG, posB, posH, posBr;
bool histoON;
CSelectiveVideoCompensatorDlg* Master;

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
	, m_Title(_T(""))
	, m_checkHistoOn(FALSE)
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
	DDX_Check(pDX, IDC_CHECK_HISTO, m_checkHistoOn);
	DDX_Control(pDX, IDC_CHECK_HISTO, m_checkHisto);
	DDX_Control(pDX, IDC_SLIDER_SM, m_sliderSmoothing);
	DDX_Control(pDX, IDC_COMBO1, m_comboCodec);
	DDX_Control(pDX, IDC_START, m_Codec);
	DDX_Control(pDX, IDC_TITLE, m_videoTitle);
	DDX_Text(pDX, IDC_TITLE, m_Title);
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
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SM, &CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderSmoothing)
	ON_EN_CHANGE(IDC_EDIT3, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CSelectiveVideoCompensatorDlg::OnEnChangeEdit7)
	ON_COMMAND(ID_EDIT_BOUNDARY, &CSelectiveVideoCompensatorDlg::OnEditBoundary)
	ON_COMMAND(ID_EDIT_RESET, &CSelectiveVideoCompensatorDlg::OnEditReset)
	ON_BN_CLICKED(IDC_CHECK_HISTO, &CSelectiveVideoCompensatorDlg::OnBnClickedCheckHisto)
	ON_BN_CLICKED(IDC_START, &CSelectiveVideoCompensatorDlg::OnBnClickedStart)
END_MESSAGE_MAP()

void CSelectiveVideoCompensatorDlg::Initialize_GUIs(){
	m_sliderRed.SetPos(100);
	m_sliderGreen.SetPos(100);
	m_sliderBlue.SetPos(100);
	m_sliderHistogram.SetPos(100);
	m_sliderBrightness.SetPos(100);
	m_sliderSmoothing.SetPos(0);
	posSm = 0;
	posR = posG = posB = posH = posBr = 1.0;

	str.Format(_T("%.2lf"), 1.0); //Format�� �̿��Ͽ� int���� ����
	m_strR.SetString(str);
	m_strG.SetString(str);
	m_strB.SetString(str);
	m_strBr.SetString(str);
	m_strH.SetString(str);
	m_Title.SetString(_T("compensate"));
	UpdateData(false); //set�� ���ֱ� ���ؼ� UpdateData(false); �� ���

	histoON = false;
	CheckDlgButton(IDC_CHECK_HISTO, false);
}

// CSelectiveVideoCompensatorDlg �޽��� ó����

BOOL CSelectiveVideoCompensatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_sliderRed.SetRange(0, 200);
	m_sliderGreen.SetRange(0, 200);
	m_sliderBlue.SetRange(0, 200);
	m_sliderHistogram.SetRange(0, 200);
	m_sliderBrightness.SetRange(0, 200);
	m_sliderSmoothing.SetRange(0, 2);

	m_comboCodec.AddString(_T("MPEG1"));			//PIM1
	m_comboCodec.AddString(_T("MPEG4"));			//MP42
	m_comboCodec.AddString(_T("YUV 4:2:0"));		//I420
	m_comboCodec.AddString(_T("Flash Video"));		//FLV1
	m_comboCodec.AddString(_T("Motion JPEG"));		//MJPG
	m_comboCodec.AddString(_T("Other Codecs"));		
	m_comboCodec.SetCurSel(0);

	Initialize_GUIs();

	Master = this;

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

void CSelectiveVideoCompensatorDlg::DisplayImage(int IDC_PICTURE_TARGET, Mat targetMat)
{
	IplImage* targetIplImage = new IplImage(targetMat);
	if(targetIplImage!=nullptr){
		CWnd* pWnd_ImageTraget = GetDlgItem(IDC_PICTURE_TARGET);
		CClientDC dcImageTraget(pWnd_ImageTraget);
		RECT rcImageTraget;
		pWnd_ImageTraget->GetClientRect(&rcImageTraget);

		BITMAPINFO bitmapInfo;
		memset(&bitmapInfo, 0, sizeof(bitmapInfo));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = targetIplImage->width;
		bitmapInfo.bmiHeader.biHeight = -targetIplImage->height;

		IplImage *tempImage = nullptr;

		if (targetIplImage->nChannels == 1)
		{
			tempImage = cvCreateImage(cvSize(targetIplImage->width, targetIplImage->height), IPL_DEPTH_8U, 3);
			cvCvtColor(targetIplImage, tempImage, CV_GRAY2BGR);
		}
		else if (targetIplImage->nChannels == 3)
		{
			tempImage = cvCloneImage(targetIplImage);
		}

		bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

		dcImageTraget.SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(dcImageTraget.GetSafeHdc(), rcImageTraget.left, rcImageTraget.top, rcImageTraget.right, rcImageTraget.bottom,	
			0, 0, tempImage->width, tempImage->height, tempImage->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&tempImage);
	}
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
		capture.open(path);
		if (!capture.isOpened()){
			MessageBox(CString("Open Failure"));
			return;
		}
		
		Initialize_GUIs();
		capture.read(matFrame);
		imshow("ROI", matFrame);
		LTx = LTy = 0;
		RBx = matFrame.cols;
		RBy = matFrame.rows;
		DisplayImage(IDC_FRAME_ORIGIN, matFrame);
		matFrameCrop = matFrame.clone();
		matFrameAdjust = matFrame.clone();
		Rect rect(LTx, LTy, RBx, RBy);
		rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 
		DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
		ColorHistogram(IDC_HIST_COLOR, matFrame);
		GreyHistogram(IDC_HIST_COLOR, matFrame);
	}
	ColorHistogram(IDC_HIST_COLOR, matFrame);
}

void CSelectiveVideoCompensatorDlg::GreyHistogram(int IDC_PICTURE_TARGET, Mat targetMat){
	Mat greyMat;
	cvtColor(targetMat, greyMat, CV_BGR2GRAY);

	Mat histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	calcHist(&greyMat, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	// Plot the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < number_bins; i++)
	{
		line(histImage, Point(bin_w*(i), hist_h),
			Point(bin_w*(i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}
	DisplayImage(IDC_HIST_GREY, histImage);
}

void CSelectiveVideoCompensatorDlg::ColorHistogram(int IDC_PICTURE_TARGET, Mat targetMat){
	Mat histogramB, histogramG, histogramR;
	const int channel_numbersB[] = { 0 };  // Blue
	const int channel_numbersG[] = { 1 };  // Green
	const int channel_numbersR[] = { 2 };  // Red
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	// R, G, B���� ���� ������׷��� ����Ѵ�.
	calcHist(&targetMat, 1, channel_numbersB, Mat(), histogramB, 1, &number_bins, &channel_ranges);
	calcHist(&targetMat, 1, channel_numbersG, Mat(), histogramG, 1, &number_bins, &channel_ranges);
	calcHist(&targetMat, 1, channel_numbersR, Mat(), histogramR, 1, &number_bins, &channel_ranges);

	// Plot the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(histogramB, histogramB, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histogramG, histogramG, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histogramR, histogramR, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < number_bins; i++)
	{
		line(histImage, Point(bin_w*(i), hist_h),
			Point(bin_w*(i), hist_h - cvRound(histogramB.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i), hist_h),
			Point(bin_w*(i), hist_h - cvRound(histogramG.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i), hist_h),
			Point(bin_w*(i), hist_h - cvRound(histogramR.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	DisplayImage(IDC_HIST_COLOR, histImage);
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


Mat ImageCompensate(){
	Rect rect(LTx, LTy, RBx - LTx, RBy - LTy);
	Mat tmp = matFrame.clone();

	if (histoON){
		matFrameCrop = Mat(matFrame.clone(), rect);
		Mat ycrcb;

		cvtColor(matFrameCrop,ycrcb,CV_BGR2YCrCb);
        vector<Mat> channels;
        split(ycrcb,channels);
		equalizeHist(channels[0], channels[0]);
		merge(channels, ycrcb);
		cvtColor(ycrcb, matFrameCrop, CV_YCrCb2BGR);

		matFrameCrop.copyTo(tmp(rect));
	}
	matFrameAdjust = tmp.clone();	

	int minY = MAX(LTy - rect.height / 5, 0);
	int minX = MAX(LTx - rect.width / 5, 0);
	int maxY = MAX(LTy + rect.height / 5, matFrame.rows);
	int maxX = MAX(LTx + rect.width / 5, matFrame.cols);

	for(int y = LTy; y < RBy; y++) {
		for(int x = LTx; x < RBx; x++) {
			matFrameAdjust.at<Vec3b>(y,x)[0] =
				saturate_cast<uchar>((double)posH * (double)tmp.at<Vec3b>(y,x)[0] * (double)posBr * posB);
			matFrameAdjust.at<Vec3b>(y,x)[1] =
				saturate_cast<uchar>((double)posH * (double)tmp.at<Vec3b>(y,x)[1] * (double)posBr * posG);
			matFrameAdjust.at<Vec3b>(y,x)[2] =
				saturate_cast<uchar>((double)posH * (double)tmp.at<Vec3b>(y,x)[2] * (double)posBr * posR);		
		}
	}

	Mat ROI(matFrameAdjust, rect);
	Mat result(matFrameAdjust);
	ROI.copyTo(matFrameCrop);
	rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 
	
	imshow("ROI", matFrameCrop);

	Master->DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
	Master->ColorHistogram(IDC_HIST_COLOR, matFrameCrop);
	Master->GreyHistogram(IDC_HIST_COLOR, matFrameCrop);
	
	return result;
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posR = (double)m_sliderRed.GetPos()/100.0;
	str.Format(_T("%.2lf"), posR); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT3, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posG = (double)m_sliderGreen.GetPos()/100.0;
	str.Format(_T("%.2lf"), posG); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT4, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posB = (double)m_sliderBlue.GetPos()/100.0;
	str.Format(_T("%.2lf"), posB); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT5, str);
	ImageCompensate();
}



void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderHistogram(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posH = (double)m_sliderHistogram.GetPos()/100.0;
	str.Format(_T("%.2lf"), posH); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT6, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posBr = (double)m_sliderBrightness.GetPos()/100.0;
	str.Format(_T("%.2lf"), posBr); //Format�� �̿��Ͽ� int���� ����
	SetDlgItemText(IDC_EDIT7, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderSmoothing(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	posSm = m_sliderSmoothing.GetPos();
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnBnClickedCheckHisto()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
   if (m_checkHistoOn){
		histoON = true;
	}
   else{
		histoON = false;
	}
	ImageCompensate();
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	static int click = 0;
	if ( event == EVENT_LBUTTONDOWN ){
		LTx = x; LTy = y;
		click = 1;
	}
	else if ( event == EVENT_LBUTTONUP && click){
		RBx = x; RBy = y;
		click = 0;
		if (LTx > RBx){	int tmp = LTx; LTx = RBx; RBx = tmp;}
		if (LTy > RBy){	int tmp = LTy; LTy = RBy; RBy = tmp;}

		Master->Initialize_GUIs();

		matFrameAdjust = matFrame.clone();
		Rect rect(LTx, LTy, RBx - LTx, RBy - LTy);
		rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 
		Mat ROI(matFrame, rect);
		ROI.copyTo(matFrameCrop);

		cvDestroyWindow("ROI");
		namedWindow("ROI", CV_WINDOW_AUTOSIZE);
		resizeWindow("ROI", RBx - LTx, RBy - LTy);
		imshow("ROI", matFrameCrop);

		Master->DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
		Master->ColorHistogram(IDC_HIST_COLOR, matFrameCrop);
		Master->GreyHistogram(IDC_HIST_COLOR, matFrameCrop);
		
		cvDestroyWindow("BoundarySetting");
	}
}

void CSelectiveVideoCompensatorDlg::OnEditBoundary()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	namedWindow("BoundarySetting");
	setMouseCallback("BoundarySetting", CallBackFunc, NULL);
	imshow("BoundarySetting", matFrame);
}


void CSelectiveVideoCompensatorDlg::OnEditReset()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	LTx = LTy = 0;
	RBx = matFrame.cols;
	RBy = matFrame.rows;

	Initialize_GUIs();

	matFrameAdjust = matFrame.clone();
	Rect rect(LTx, LTy, RBx - LTx, RBy - LTy);
	rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 

	imshow("ROI", matFrameAdjust);
	Master->DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
	Master->ColorHistogram(IDC_HIST_COLOR, matFrameAdjust);
	Master->GreyHistogram(IDC_HIST_COLOR, matFrameAdjust);
}


void CSelectiveVideoCompensatorDlg::OnBnClickedStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CT2CA path_(m_Title.GetString());
	string newPath(path_);

	switch(m_comboCodec.GetCurSel()){
		case PIM1:
			writer.open(newPath + ".avi", CV_FOURCC('P','I','M','1'), capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
		case MP42:
			writer.open(newPath + ".avi", CV_FOURCC('M','P','4','2'), capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
		case I420:			
			writer.open(newPath + ".avi", CV_FOURCC('I','4','2','0'), capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
		case FLV1:
			writer.open(newPath + ".avi", CV_FOURCC('F','L','V','1'), capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
		case MJPG:
			writer.open(newPath + ".avi", CV_FOURCC('M','J','P','G'), capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
		default:
			writer.open(newPath + ".avi", -1, capture.get(CV_CAP_PROP_FPS), 
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
			break;
	}

	if (!writer.isOpened()){
		return;
	}
	while (1){
		if (!capture.read(matFrame))             
			break;
		writer.write(ImageCompensate());
	}
	capture.release();
	writer.release();
	cvDestroyWindow("ROI");

	capture.open(path);
	capture.read(matFrame);
	DisplayImage(IDC_FRAME_ORIGIN, matFrame);
	ImageCompensate();
	DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
	ColorHistogram(IDC_HIST_COLOR, matFrameAdjust);
	GreyHistogram(IDC_HIST_COLOR, matFrameAdjust);
}
