#pragma once
/**********************************************************************/
#include "CascadingMacroSelector.h"
#include "SPOSKeyboardPluFilterArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditKeyType : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditKeyType();
	~CSPOSKeyboardPropPageEditKeyType();
	void SetParentSheet( void* pParentSheet ){ m_pParentSheet = pParentSheet; }

	//{{AFX_DATA(CSPOSKeyboardPropPageEditKeyType)
	enum { IDD = IDD_PROPPAGE_EDITKEY_TYPE };
	CEdit	m_editText;
	CButton	m_buttonBrowse;
	CButton m_buttonKeyText;
	CButton m_buttonHide;
	CButton m_buttonDefaultTextAlways;
	CButton m_buttonNoSort;
	CSSComboBox	m_comboType;
	CSSComboBox	m_comboValue;
	CSSComboBox	m_comboMacro;
	CEdit m_editMacro;
	CString m_strValue;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditKeyType)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
public:
	void SetKeyRecord( CSPOSKeyboardKeyRecord* pRecord );
	void SetDefaultTextMode() { m_bDefaultTextMode = TRUE; }

	bool SelectComboItemType( DWORD wItemData );
	bool SelectComboItemValue();

protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditKeyType)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectType();
	afx_msg void OnSelectValue();
	afx_msg void OnKillFocusValue();
	afx_msg void OnButtonKeyText();
	afx_msg void OnButtonBrowse();
	afx_msg void OnKillFocusMacro();
	afx_msg void OnSelectMacro();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateKeyTypeAndValue();
	void RedrawKey();
	void SetupEditBox ( const char* szMinValue, const char* szMaxValue, int nWidth );
	void FillValueCombo( bool bReset );
	void SaveRecord();

private:
	CEdit* GetEditValue();

private:
	CSPOSKeyboardKeyRecord* m_pKeyRecord;
	int m_nType;

	CStringArray m_arrayComboValues;
	CString m_strMinValue;
	CString m_strMaxValue;

private:
	bool m_bDefaultTextMode;
	CSPOSKeyboardPluFilterArray m_PluFilterArray;
	
private:
	CCascadingMacroSelector m_MacroSelector;
	int m_nMacroNo;

private:
	void* m_pParentSheet;
};

/**********************************************************************/

