//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
//$$******************************************************************

class CEditStampsDlg : public CSSAutoShutdownDialog
{
public:
	CEditStampsDlg( CSQLRowPurchaseControlStamp& RowStampOld, CSQLRowPurchaseControlStamp& RowStampNew, CSQLRowPurchaseControlRule& RowRule, bool bAdd, CWnd* pParent = NULL);   
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	enum { IDD = IDD_EDIT_STAMPS };
	CDateTimeCtrl m_DatePickerAward;
	CDateTimeCtrl m_DatePickerExpiry;
	CEdit m_editNever;
	CButton m_checkNoExpiry;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CStatic m_staticStamps;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	afx_msg void OnToggleNoExpiry();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditStamps() { return GetEdit(IDC_EDIT_STAMPS); }

private:
	CSQLRowPurchaseControlStamp& m_RowStampOld;
	CSQLRowPurchaseControlStamp& m_RowStampNew;
	CSQLRowPurchaseControlRule& m_RowRule;
	bool m_bAdd;
};

//$$******************************************************************
