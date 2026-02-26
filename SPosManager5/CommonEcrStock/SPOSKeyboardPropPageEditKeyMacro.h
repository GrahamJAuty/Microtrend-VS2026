#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CascadingMacroCSVArray.h"

class CSPOSKeyboardPropPageEditKeyMacro : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditKeyMacro();
	~CSPOSKeyboardPropPageEditKeyMacro();

public:
	void SetMacroRecord( CCascadingMacroCSVRecord* pRecord ) { m_pMacroRecord = pRecord; }

	//{{AFX_DATA(CSPOSKeyboardPropPageEditKeyMacro)
	enum { IDD = IDD_PROPPAGE_EDITKEY_MACRO };
	CEdit m_editParentType;
	CEdit m_editParentName;
	CEdit m_editParentData;
	CStatic m_staticLocal;
	CButton m_checkLocal;
	CEdit m_editLocalName;
	CEdit m_editLocalData;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditKeyMacro)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg void OnToggleLocal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditKeyMacro)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	CCascadingMacroCSVRecord* m_pMacroRecord;
};

/**********************************************************************/

