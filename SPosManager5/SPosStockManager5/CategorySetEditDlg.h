#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "DefTextCategory.h"
/**********************************************************************/

class CCategorySetEditDlg : public CDialog
{
public:
	CCategorySetEditDlg( bool bSystem, CCategorySetCSVRecord& BatchRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCategorySetEditDlg)
	enum { IDD = IDD_CATEGORYSET_EDIT };
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCategorySetEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCategorySetEditDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listCategory;
	CCategorySetCSVRecord& m_CategorySetRecord;

private:
	bool m_bSystem;
	CDefTextCategory* m_pCatArray;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
