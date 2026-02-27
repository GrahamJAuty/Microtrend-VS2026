#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\UserTypeText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CPhotoIDListData
{
public:
	CPhotoIDListData();
	bool SaveDefault();
	bool Load ( const char* szLabel );
	bool RemoveLabel ( const char* szLabel );
	bool MakeDefault ( const char* szLabel );

	const char* GetReportData (CSQLRowAccountFull& RowAccount, const char* szImageFilename );
	const char* GetTestReportData();
	int CompileLabelCombo ( CSSComboBox* pCombo, bool bFullList = TRUE );

private:
	void ResetData();
	bool ReadDefault();
	void ValidateSmartcardList();
	void ExtractCSVLine(CCSV* csv);
	const char* GetFieldData(int nFieldNo, CSQLRowAccountFull& RowAccount);
	const char* GetTestFieldData(int nFieldNo);
	CString FormatCardNo(CSQLRowAccountFull& RowAccount, bool bFull);

public:
	CString m_strLabelName;

	CString m_strField1;
	CString m_strField2;
	CString m_strField3;
	CString m_strField4;
	CString m_strField5;
	CString m_strField6;
	CString m_strFieldText1;	
	CString m_strFieldText2;	
	CString m_strFieldText3;	
	CString m_strFieldText4;	
	CString m_strFieldText5;	
	CString m_strFieldText6;	

private:
	CAllergyText m_AllergyText;
	CAlertText m_AlertText;
	CUserTypeText m_UserText;

	CString m_strFilename;
	CString m_strBarmanFilename;
	CString m_strFieldData;
};
