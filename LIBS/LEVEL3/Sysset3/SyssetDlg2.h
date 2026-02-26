/**********************************************************************/
#pragma once
/**********************************************************************/

#include "sysset3.h"
#include "SyssetEdit.h"

/**********************************************************************/

class CSyssetDlg2 : public CDialog
{
public:
	CSyssetDlg2 ( CWnd* pParent );

	enum { IDD = IDD_SYSSET_DLG2 };
	CStatic	m_staticPhone;
	CStatic	m_staticExpiry;
	CButton	m_buttonClearExpiry;
	CStatic	m_staticLine6;
	CStatic	m_staticLine5;
	CStatic	m_staticLine4;
	CStatic	m_staticLine3;
	CStatic	m_staticLine2;
	CStatic	m_staticLine1;
	CString	m_strDealerPhone;
	COleDateTime	m_timeExpiry;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClearExpiry();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strExpiry;

private:
	CUserEdit m_editDealerPhone;
};

/**********************************************************************/
