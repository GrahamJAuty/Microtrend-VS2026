#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CustomFieldSetCSVArray.h"
/**********************************************************************/

class CCustomFieldSetEditDlg : public CDialog
{
public:
	CCustomFieldSetEditDlg( CCustomFieldSetCSVRecord& BatchRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomFieldSetEditDlg)
	enum { IDD = IDD_CUSTOM_FIELD_SET_EDIT };
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomFieldSetEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCustomFieldSetEditDlg)
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
	CSSListTaggedSelectCtrl m_listCustomFields;
	CCustomFieldSetCSVRecord& m_CustomFieldSetRecord;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
