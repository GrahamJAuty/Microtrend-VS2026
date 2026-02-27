#pragma once

//******************************************************************
#ifndef SYSTEMTYPE_MANAGER
//******************************************************************

class CWaitDlg
{
public:
	CWaitDlg(CWnd* pParent = NULL){}	

public:
	void SetMessageText( const char* sz ){}
};

//******************************************************************
#else
//******************************************************************

#include "..\SmartPay4Manager\resource.h"

class CWaitDlg : public CDialog
{
public:
	CWaitDlg(CWnd* pParent = NULL);							 // standard constructor
	CWaitDlg(bool bBackgroundMode, CWnd* pParent = NULL );   // background - no show
	virtual ~CWaitDlg();

// Dialog Data
	enum { IDD = IDD_WAIT };

public:
	void SetMessageText( const char* sz ){ SetDlgItemText( IDC_STATIC_MESSAGE, sz ); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void DisplayWindow ( CWnd* pParent );

private:
	bool m_bBackgroundMode;
};

//******************************************************************
#endif
//******************************************************************
