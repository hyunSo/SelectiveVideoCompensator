
// SelectiveVideoCompensator.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSelectiveVideoCompensatorApp:
// �� Ŭ������ ������ ���ؼ��� SelectiveVideoCompensator.cpp�� �����Ͻʽÿ�.
//

class CSelectiveVideoCompensatorApp : public CWinApp
{
public:
	CSelectiveVideoCompensatorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSelectiveVideoCompensatorApp theApp;