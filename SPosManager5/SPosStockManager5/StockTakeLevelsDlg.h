#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
/**********************************************************************/

class CStocktakeLevelsDlg : public CDialog
{
public:
	CStocktakeLevelsDlg( int nSpIdx, CWnd* pParent = NULL); 

	//{{AFX_DATA(CStocktakeLevelsDlg)
	enum { IDD = IDD_STOCKTAKE_LEVELS };
	CSSListCtrl m_listAction;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStocktakeLevelsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStocktakeLevelsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddOption( int& nListPos, const char* szText, int nNodeType );

private:
	int m_nSpIdx;
	CArray<int,int> m_arrayNodeTypes;

public:
	int m_nNodeType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
