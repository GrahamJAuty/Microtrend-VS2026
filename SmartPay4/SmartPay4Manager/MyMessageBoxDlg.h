#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\MyMessageBoxInfo.h"
#include "..\SmartPay4Shared\MyMessageBoxStatic.h"
/**********************************************************************/

class CMyMessageBoxDlg : public CSSAutoShutdownDialog
{
public:
	CMyMessageBoxDlg( CMyMessageBoxInfo& MessageBoxInfo, CWnd* pParent = NULL);

	//{{AFX_DATA(CMyMessageBoxDlg)
	enum { IDD = IDD_MESSAGE };
	//}}AFX_DATA
	CMyMessageBoxStatic m_staticMessage;
	CButton m_buttonAction1;
	CButton m_buttonAction2;
	CButton m_buttonAction3;

	//{{AFX_VIRTUAL(CMyMessageBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();

private:
	int PixelsToDBUs( int nPixels, bool bHeight );

protected:
	//{{AFX_MSG(CMyMessageBoxDlg)
	virtual void OnButtonAction1();
	virtual void OnButtonAction2();
	virtual void OnButtonAction3();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMyMessageBoxInfo& m_MessageBoxInfo;
};

/**********************************************************************/
