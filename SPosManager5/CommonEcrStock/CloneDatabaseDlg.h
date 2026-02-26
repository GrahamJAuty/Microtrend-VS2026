#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "DataPageFilter.h"
/**********************************************************************/

class CCloneDatabaseDlg : public CDialog
{
public:
	CCloneDatabaseDlg( int nDbIdx, CDataPageFilter& Filter, CWnd* pParent = NULL);   
	bool IsRelevant();

	//{{AFX_DATA(CCloneDatabaseDlg)
	enum { IDD = IDD_CLONE_DATABASE };
	CSSComboBoxEx	m_comboDestination;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCloneDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCloneDatabaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddListItem( const char* sz, int nItemData );
	void AddWarning( CStringArray& arrayWarning, const char* sz );
	void CopyOrRemove ( const char* szSource, const char* szDest );

private:
	CLocationSelector m_LocationSelector;
	int m_nSourceDbIdx;
	CDataPageFilter& m_Filter;
	CSSListTaggedSelectCtrl m_listType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/

