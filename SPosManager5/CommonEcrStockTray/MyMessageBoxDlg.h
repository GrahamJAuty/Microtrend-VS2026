#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "MyMessageBoxInfo.h"
#include "MyMessageBoxStatic.h"
/**********************************************************************/
//include "MySSDialog.h"
/**********************************************************************/

class CMyMessageBoxDlg : public CSSDialog
{
public:
	CMyMessageBoxDlg( CMyMessageBoxInfo& MessageBoxInfo, CWnd* pParent = NULL);
	CMyMessageBoxDlg( CMyMessageBoxDlg** pParentToThis, CMyMessageBoxInfo& MessageBoxInfo, CWnd* pParent = NULL);

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
	int PixelsToDBUs( int nPixels, bool bHeight );

protected:
	//{{AFX_MSG(CMyMessageBoxDlg)
	virtual BOOL OnInitDialog();
	virtual void OnButtonAction1();
	virtual void OnButtonAction2();
	virtual void OnButtonAction3();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMyMessageBoxInfo& m_MessageBoxInfo;
	CMyMessageBoxDlg** m_pParentToThis;
	bool m_bIsModeless;
};

/**********************************************************************/
