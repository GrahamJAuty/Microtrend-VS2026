//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRowOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
//$$******************************************************************

class CEditStampsDlg : public CSSDialog
{
public:
	CEditStampsDlg( CSQLRowOfferStamp& RowStampOld, CSQLRowOfferStamp& RowStampNew, CSQLRowStampOfferInfo& RowOffer, bool bAdd, CWnd* pParent = NULL);   
	
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
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToggleNoExpiry();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditStamps() { return GetEdit(IDC_EDIT_STAMPS); }

private:
	CSQLRowOfferStamp& m_RowStampOld;
	CSQLRowOfferStamp& m_RowStampNew;
	CSQLRowStampOfferInfo& m_RowOffer;
	bool m_bAdd;
};

//$$******************************************************************
