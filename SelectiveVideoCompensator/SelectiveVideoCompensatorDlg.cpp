// SelectiveVideoCompensatorDlg.cpp : 구현 파일
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

#define R  0
#define G  1
#define B  2
#define H  3
#define Br 4

Mat matFrame;
Mat matFrameCrop;
Mat matFrameAdjust;
int LTx, LTy, RBx, RBy;
int posR, posG, posB, posBr;
double posH;
bool histoON;
CSelectiveVideoCompensatorDlg* Master;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSelectiveVideoCompensatorDlg 대화 상자



CSelectiveVideoCompensatorDlg::CSelectiveVideoCompensatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectiveVideoCompensatorDlg::IDD, pParent)
	, m_strR(_T(""))
	, m_strG(_T(""))
	, m_strB(_T(""))
	, m_strH(_T(""))
	, m_strBr(_T(""))
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
	ON_COMMAND(ID_EDIT_BOUNDARY, &CSelectiveVideoCompensatorDlg::OnEditBoundary)
	ON_COMMAND(ID_EDIT_RESET, &CSelectiveVideoCompensatorDlg::OnEditReset)
	ON_BN_CLICKED(IDC_CHECK_HISTO, &CSelectiveVideoCompensatorDlg::OnBnClickedCheckHisto)
END_MESSAGE_MAP()


// CSelectiveVideoCompensatorDlg 메시지 처리기

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
	m_sliderHistogram.SetRange(0, 200);
	m_sliderHistogram.SetPos(100);
	m_sliderBrightness.SetRange(0, 100);
	m_sliderBrightness.SetPos(0);
	posR = posG = posB = posBr = 0;
	posH = 1.0;

	str.Format(_T("%d"), 0); //Format을 이용하여 int값을 변경
	m_strR.SetString(str);
	m_strG.SetString(str);
	m_strB.SetString(str);
	m_strBr.SetString(str);
	str.Format(_T("%.2lf"), 1.0); //Format을 이용하여 int값을 변경
	m_strH.SetString(str);
	UpdateData(false); //set을 해주기 위해서 UpdateData(false); 를 사용

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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSelectiveVideoCompensatorDlg::OnPaint()
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
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
		capture.read(matFrame);
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
/*
* @Debugging
		Mat temp;		
		IplImage *iplFrame;//, *mask;
		cvNamedWindow("main");
		temp = matFrame.clone();
		iplFrame=&IplImage(matFrame);
		int WIDTH=iplFrame->width;
		int HEIGHT=iplFrame->height;
		cvShowImage("main", iplFrame);
		MessageBox(pathName);
*/	
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
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
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

	// R, G, B별로 각각 히스토그램을 계산한다.
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
		line(histImage, Point(bin_w*(i - 1), hist_h/* - cvRound(histogramB.at<float>(i - 1))*/),
			Point(bin_w*(i), hist_h - cvRound(histogramB.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h/* - cvRound(histogramG.at<float>(i - 1))*/),
			Point(bin_w*(i), hist_h - cvRound(histogramG.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h/* - cvRound(histogramR.at<float>(i - 1))*/),
			Point(bin_w*(i), hist_h - cvRound(histogramR.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	DisplayImage(IDC_HIST_COLOR, histImage);
}

void CSelectiveVideoCompensatorDlg::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CSelectiveVideoCompensatorDlg::OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		*pResult = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


void ImageCompensate(){
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

	for( int y = LTy; y < RBy; y++ ) {
		for( int x = LTx; x < RBx; x++ ) {
			matFrameAdjust.at<Vec3b>(y,x)[0] =
			saturate_cast<uchar>((double)posH * (double)(tmp.at<Vec3b>(y,x)[0]) + (double)posBr + posB);
			matFrameAdjust.at<Vec3b>(y,x)[1] =
			saturate_cast<uchar>((double)posH * (double)(tmp.at<Vec3b>(y,x)[1]) + (double)posBr + posG);
			matFrameAdjust.at<Vec3b>(y,x)[2] =
			saturate_cast<uchar>((double)posH * (double)(tmp.at<Vec3b>(y,x)[2]) + (double)posBr + posR);		
		}
	}

	Mat ROI(matFrameAdjust, rect);
	ROI.copyTo(matFrameCrop);
	rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 
	
	imshow("ROI", matFrameCrop);

	Master->DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
	Master->ColorHistogram(IDC_HIST_COLOR, matFrameCrop);
	Master->GreyHistogram(IDC_HIST_COLOR, matFrameCrop);
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	int tmp = posR;
	posR = m_sliderRed.GetPos();
	str.Format(_T("%d"), posR); //Format을 이용하여 int값을 변경
	SetDlgItemText(IDC_EDIT3, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	int tmp = posG;
	posG = m_sliderGreen.GetPos();
	str.Format(_T("%d"), posG); //Format을 이용하여 int값을 변경
	SetDlgItemText(IDC_EDIT4, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	posB = m_sliderBlue.GetPos();
	str.Format(_T("%d"), posB); //Format을 이용하여 int값을 변경
	SetDlgItemText(IDC_EDIT5, str);
	ImageCompensate();
}



void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderHistogram(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	posH = (double)m_sliderHistogram.GetPos()/100.0;
	str.Format(_T("%.2lf"), posH); //Format을 이용하여 int값을 변경
	SetDlgItemText(IDC_EDIT6, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnNMReleasedcaptureSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	int tmp = posBr;
	posBr = m_sliderBrightness.GetPos();
	str.Format(_T("%d"), posBr); //Format을 이용하여 int값을 변경
	SetDlgItemText(IDC_EDIT7, str);
	ImageCompensate();
}


void CSelectiveVideoCompensatorDlg::OnBnClickedCheckHisto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	histoON = false;
	CheckDlgButton(IDC_CHECK_HISTO, false);

	namedWindow("BoundarySetting");
	setMouseCallback("BoundarySetting", CallBackFunc, NULL);
	imshow("BoundarySetting", matFrame);
}


void CSelectiveVideoCompensatorDlg::OnEditReset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	LTx = LTy = 0;
	RBx = matFrame.cols;
	RBy = matFrame.rows;
	histoON = false;
	CheckDlgButton(IDC_CHECK_HISTO, false);

	matFrameAdjust = matFrame.clone();
	Rect rect(LTx, LTy, RBx - LTx, RBy - LTy);
	rectangle(matFrameAdjust, rect, CV_RGB(255, 0, 0), 2, 8, 0); 
		
	Master->DisplayImage(IDC_FRAME_ADJUSTED, matFrameAdjust);
	Master->ColorHistogram(IDC_HIST_COLOR, matFrameAdjust);
	Master->GreyHistogram(IDC_HIST_COLOR, matFrameAdjust);
}


