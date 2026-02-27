#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "RecordXferData.h"
//$$******************************************************************

class CRecordXferPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CRecordXferPropertyPage2(CSQLRowAccountFull& RowAccountFrom, CRecordXferData& RecordXferData );
	void SetPointer ( bool bHotlistReplace );
	virtual ~CRecordXferPropertyPage2();

	enum { IDD = IDD_PROPPAGE_XFER2 };

	CButton m_checkDOB;
	CButton m_checkEmail;
	CButton m_checkGender;
	CButton m_checkInfo[MAXINFOTICK + 1];
	CButton m_checkDietary;
	CButton m_checkAllergies;
	CButton m_checkAlert;
	CButton m_checkTick[MAXRECORDTICK + 1];
	
	CEdit m_editEmail;
	CSSComboBox m_comboGender;
	CEdit m_editInfo[MAXINFOTICK + 1];
	CEdit m_editDietary;
	CButton m_buttonAllergies;
	CEdit m_editAllergies;
	CButton m_buttonAlert;
	CEdit m_editAlert;
	CButton m_checkRecordTick[MAXRECORDTICK + 1];
	
	CButton m_buttonSelectAll;

private:
	CEdit* GetEditDOB();

private:
	bool m_bTransferGender;
	int m_nAllergyCodes;
	int m_nAlertCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnToggleEmail();
	afx_msg void OnToggleInfo1();
	afx_msg void OnToggleInfo2();
	afx_msg void OnToggleInfo3();
	afx_msg void OnToggleInfo4();
	afx_msg void OnToggleInfo5();
	afx_msg void OnToggleInfo6();
	afx_msg void OnToggleInfo7();
	afx_msg void OnToggleInfo8();
	afx_msg void OnToggleTick1();
	afx_msg void OnToggleTick2();
	afx_msg void OnToggleTick3();
	afx_msg void OnToggleTick4();
	afx_msg void OnToggleDOB();
	afx_msg void OnToggleAllergies();
	afx_msg void OnButtonAllergies();
	afx_msg void OnToggleAlert();
	afx_msg void OnButtonAlert();
	afx_msg void OnToggleDietary();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	bool StartAllowed();

private:
	void SetDisplay();
	void SetDisplayAllergy();

private:
	CSQLRowAccountFull& m_RowAccountFrom;
	CRecordXferData& m_RecordXferData;
	bool m_bHotlistReplace;
	bool m_bSelectAllDetails;
	CAlertText m_alert;

private:
	bool m_bInfoHide[MAXINFOTICK + 1];
};

//$$******************************************************************
