#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

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
	void ResetData();
	bool ReadDefault();
	void ValidateSmartcardList();
	void ExtractCSVLine ( CCSV* csv );
	const char* GetFieldData (CSQLRowAccountFull& RowAccount, int nFieldNo );
	const char* GetTestFieldData ( int nFieldNo );
	CString FormatCardNo (__int64 nUserID, bool bFullCardNo );

private:
	CString m_strFilename;
	CString m_strBarmanFilename;
	CString m_strFieldData;
};

//*******************************************************************
