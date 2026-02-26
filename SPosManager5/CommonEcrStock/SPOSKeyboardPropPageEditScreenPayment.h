#pragma once
/**********************************************************************/
#include "SPOSKeyboardScreenRecord.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditScreenPayment : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditScreenPayment();
	~CSPOSKeyboardPropPageEditScreenPayment();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditScreenPayment)
	enum { IDD = IDD_PROPPAGE_EDITSCREEN_PAYMENT };
	CSSComboBox m_comboTopLevel;
	CSSComboBox m_comboFunction;
	CString m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditScreenPayment)
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
	//{{AFX_MSG(CSPOSKeyboardPropPageEditScreenPayment)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEditTopLevel();
	afx_msg void OnButtonEditFunction();
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

