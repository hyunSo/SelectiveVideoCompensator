
// SelectiveVideoCompensatorDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include <iostream>

// CSelectiveVideoCompensatorDlg ��ȭ ����
class CSelectiveVideoCompensatorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSelectiveVideoCompensatorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SELECTIVEVIDEOCOMPENSATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	CString str;
	std::string path;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;
	CString m_strR;
	CString m_strG;
	CString m_strB;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnNMReleasedcaptureSliderRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderBlue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	CString m_strBr;
	CString m_strH;
	CSliderCtrl m_sliderHistogram;
	CSliderCtrl m_sliderBrightness;
	afx_msg void OnNMReleasedcaptureSliderHistogram(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult);
};
