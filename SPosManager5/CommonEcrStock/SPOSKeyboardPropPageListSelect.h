#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageListSelect : public CPropertyPage
{
public:
	CSPOSKeyboardPropPageListSelect();
	~CSPOSKeyboardPropPageListSelect();

public:
	//{{AFX_DATA(CSPOSKeyboardPropPageListSelect)
	enum { IDD = IDD_PROPPAGE_LIST_SELECT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageListSelect)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();

protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageListSelect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	CSSListTaggedSelectCtrl m_listSelected;
	CArray<__int64,__int64> m_arrayListNo;

public:
	int m_nListType;
};

/**********************************************************************/

