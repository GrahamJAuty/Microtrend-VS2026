#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PropPageServerGeneral.h"
#include "PropPageServerLogon.h"
#include "PropPageServerRestriction.h"
#include "ServerCSVArray.h"
#include "ServerRestrictionCSVArray.h"
/**********************************************************************/

class CPropertySheetServer : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetServer)

public:
	CPropertySheetServer( WORD wSpinID, bool bFullEdit, int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, int nServerIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetServer();

public:
	CPropPageServerGeneral m_PageGeneral;
	CPropPageServerLogon m_PageLogon;
	CPropPageServerRestriction m_PageRestrict;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetServer)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	int GetIndex() { return m_nServerIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetServer)
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	int m_nServerIdx;
	CServerCSVRecord m_ServerRecord;
	CServerRestrictionCSVRecord m_RestrictionRecord;

private:
	CByteArray m_PageArray;
	int m_nServerDbIdx;
	int m_nServerLocIdx;
	int m_nTerminalLocIdx;
	bool m_bFullEdit;
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	
private:
	CRect m_PageRect;
};

/**********************************************************************/
#endif
/**********************************************************************/
