#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "ColumnManager.h"
/**********************************************************************/

class CColumnListDlg : public CSSDialog
{
public:
	CColumnListDlg( const char* szType, CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CColumnListDlg)
	enum { IDD = IDD_COLUMN_LIST };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CColumnListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CColumnListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
private:
	CArray<CColumnStatusInfo,CColumnStatusInfo>& m_arrayPos;
	CSSListTaggedSelectCtrl m_listColumn;
	int m_nProtectPos;
	CString m_strType;
};

/**********************************************************************/
