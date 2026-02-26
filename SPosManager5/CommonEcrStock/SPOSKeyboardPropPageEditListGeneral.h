#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditListGeneral : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditListGeneral();
	~CSPOSKeyboardPropPageEditListGeneral();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditListGeneral)
	enum { IDD = IDD_PROPPAGE_EDITLIST_GENERAL };
	CStatic m_staticTableNoLo;
	CStatic m_staticTableNoHi;
	CStatic m_staticTableArea;
	CStatic m_staticPriority;
	CStatic m_staticStackMode;
	CEdit	m_editKeys;
	CEdit	m_editPages;
	CSSComboBox m_comboType;
	CSSComboBox m_comboStackMode;
	CSSComboBox	m_comboStaydown;
	CSSComboBox	m_comboPriority;
	CSSComboBox m_comboPluLink;
	CSSTabbedComboBox m_comboFormat;
	CString	m_strName;
	int		m_nMinItems;
	int		m_nMaxItems;
	int		m_nTableNoLo;
	int		m_nTableNoHi;
	int		m_nTableArea;
	int		m_nNextList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditListGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
public:
	void SetListRecord( CSPOSKeyboardListRecord* pRecord );
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditListGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectFormat();
	afx_msg void OnSelectType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditTableNoLo();
	CEdit* GetEditTableNoHi();
	CEdit* GetEditTableArea();

private:
	void SaveRecord();

private:
	CSPOSKeyboardListRecord* m_pListRecord;
	void* m_pPropertySheet;
	int m_nListType;

private:
	bool m_bWantPriority;
	bool m_bWantStackMode;
	bool m_bWantTableArea;
};

/**********************************************************************/

