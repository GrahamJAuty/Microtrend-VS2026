#pragma once
/**********************************************************************/
#include "SPOSKeyboardScreenRecord.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditScreenTrading : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditScreenTrading();
	~CSPOSKeyboardPropPageEditScreenTrading();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditScreenTrading)
	enum { IDD = IDD_PROPPAGE_EDITSCREEN_TRADING };
	CSSComboBox m_comboTopLevel;
	CSSComboBox m_comboFunction;
	CString m_strName;
	int m_nScreenStyle;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditScreenTrading)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
public:
	void SetScreenRecord( CSPOSKeyboardScreenRecord* pScreen );
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditScreenTrading)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEditTopLevel();
	afx_msg void OnButtonEditFunction();
	afx_msg void OnRadioStyle1();
	afx_msg void OnRadioStyle2();
	afx_msg void OnRadioStyle3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void EditList( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, int nListType );
	void RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType );
	__int64 GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo );

private:
	void SaveRecord();

private:
	CSPOSKeyboardScreenRecord* m_pScreenRecord;
	void* m_pPropertySheet;

	CArray<__int64,__int64> m_arrayListNoTopLevel;
	CArray<__int64,__int64> m_arrayListNoFunction;
};

/**********************************************************************/

