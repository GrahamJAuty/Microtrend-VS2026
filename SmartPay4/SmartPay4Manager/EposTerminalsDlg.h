#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
/**********************************************************************/
#include "SQLRowSetEposTerminal.h"
/**********************************************************************/

class CEposTerminalsDlg : public CSSAutoShutdownDialog
{
public:
	CEposTerminalsDlg(CWnd* pParent = NULL);
	virtual ~CEposTerminalsDlg();

	enum { IDD = IDD_EPOS_TERMINALS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDisable();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();

private:
	void DisplayTerminals();

private:
	CSSListCtrl m_list;

private:
	CSQLRowSetEposTerminal m_RowSetEP;
	CArray<int, int> m_arrayDeletedEP;
	bool m_bModified;
};

/**********************************************************************/
