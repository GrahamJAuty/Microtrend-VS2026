#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
/**********************************************************************/
#include "SingleListHelpers.h"
/**********************************************************************/

class CListDataDeleteDlg : public CSSAutoShutdownDialog
{
public:
	CListDataDeleteDlg( CSingleListContentHelpers* pContentHelpers, CSingleListDisplayHelpers* pDisplayHelpers, CWnd* pParent = NULL );  

	//{{AFX_DATA(CListDataDeleteDlg)
	enum { IDD = IDD_LIST_DATA_DELETE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListDataDeleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CStatic m_staticPrompt;

protected:
	//{{AFX_MSG(CListDataDeleteDlg)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	int GetArraySize();
		
private:
	CSSListCtrlVirtual m_listData;
	bool m_bEnableColour;
	
private:
	CSingleListContentHelpers* m_pContentHelpers;
	CSingleListDisplayHelpers* m_pDisplayHelpers;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
