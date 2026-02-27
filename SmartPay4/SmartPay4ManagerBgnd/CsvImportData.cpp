//*******************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "CsvImportData.h"
//*******************************************************************

CCsvImportData::CCsvImportData()
{
	m_strImportFilename = "";
	m_bEnableImport = TRUE;
	m_nImportMethod = nIMPORT_METHOD_ALL;
	m_nLocateOn = 0;					// 0=No check, 1=MemberID,2-9 = Info1-8
	m_bDeleteFile = FALSE;
	m_strStandardNextNewUserID = "1";

	m_bEnableTopupImport = TRUE;
	m_nTopupLocateOn = 0;					// 0=no check, 1=MemberID,2-9 = Info1-8;	
	m_strTopupFilename = "";
	m_bTopupAddUnknown = FALSE;
	m_strTopupNextNewUserID = "1";
	m_bDeleteTopupFile = FALSE;
	m_nImportTopupMethod = nIMPORT_METHOD_EXISTONLY;

	m_bEnableCustomImport = FALSE;
	m_nCustomLocateOn = 0;					// 0=no check, 1=MemberID,2-9 = Info1-8
	m_strCustomNextNewUserID = "1";
	m_strCustomMenuText = "Custom File";
	m_strCustomFilename = "";
	m_bDeleteCustomFile = FALSE;
	m_nImportCustomMethod = nIMPORT_METHOD_ALL;
	m_bIgnoreCustomCase = TRUE;
	m_nCustomFieldCount = 0;					// not saved
	m_nCustomMatchCount = 0;					// not saved

	m_bEnableDbaseAuditImport = TRUE;
	m_strDbaseAuditFilename = "";
	m_nDbaseImportMethod = nIMPORT_METHOD_ALL;

	m_strMandatoryHeaderStandard = "";
	m_strMandatoryHeaderTopUp = "";
	m_nFilenameCacheHoursStandard = 0;
	m_nFilenameCacheHoursTopUp = 0;

	m_strFilename = Filenames.GetCSVImportDataFilename();
	m_strCustomDbFilename = Filenames.GetCustomImportFieldsFilename();
}

//*******************************************************************

bool CCsvImportData::Read()
{
	CSSIniFile file;
	bool bReply = file.Read(m_strFilename);

	m_bEnableImport = file.GetBool("EnableImport", m_bEnableImport);
	m_strImportFilename = file.GetString("ImportFile", m_strImportFilename);
	m_nImportMethod = file.GetInt("ImportMethod", m_nImportMethod);
	m_nLocateOn = file.GetInt("LocateOn", m_nLocateOn);
	m_strStandardNextNewUserID = file.GetString("StartUserID", m_strStandardNextNewUserID);

	m_bDeleteFile = file.GetBool("DeleteFile", m_bDeleteFile);

	m_bEnableTopupImport = file.GetBool("EnableTopup", m_bEnableTopupImport);
	m_nTopupLocateOn = file.GetInt("TopupLocate", m_nTopupLocateOn);
	m_strTopupFilename = file.GetString("TopupFile", m_strTopupFilename);
	m_bTopupAddUnknown = file.GetBool("TopupUnknown", m_bTopupAddUnknown);
	m_strTopupNextNewUserID = file.GetString("TopupStart", m_strTopupNextNewUserID);
	m_bDeleteTopupFile = file.GetBool("DeleteTopup", m_bDeleteTopupFile);

	m_bEnableCustomImport = file.GetBool("EnableCustom", m_bEnableCustomImport);
	m_nCustomLocateOn = file.GetInt("CustomLocate", m_nCustomLocateOn);
	m_strCustomNextNewUserID = file.GetString("CustomStart", m_strCustomNextNewUserID);
	m_strCustomMenuText = file.GetString("CustomMenu", m_strCustomMenuText);
	m_strCustomFilename = file.GetString("CustomFile", m_strCustomFilename);
	m_bDeleteCustomFile = file.GetBool("DeleteCustom", m_bDeleteCustomFile);
	m_nImportCustomMethod = file.GetInt("CustomImport", m_nImportCustomMethod);
	m_bIgnoreCustomCase = file.GetBool("CustomCase", m_bIgnoreCustomCase);

	m_bEnableDbaseAuditImport = file.GetBool("EnableDbase", m_bEnableDbaseAuditImport);
	m_strDbaseAuditFilename = file.GetString("DbaseFile", m_strDbaseAuditFilename);
	m_nDbaseImportMethod = file.GetInt("DbaseImport", m_nDbaseImportMethod);

	m_strMandatoryHeaderStandard = file.GetString("MandatoryHeaderStandard", m_strMandatoryHeaderStandard);
	m_strMandatoryHeaderTopUp = file.GetString("MandatoryHeaderTopUp", m_strMandatoryHeaderTopUp);
	m_nFilenameCacheHoursStandard = file.GetInt("FilenameCacheHoursStandard", m_nFilenameCacheHoursStandard);
	m_nFilenameCacheHoursTopUp = file.GetInt("FilenameCacheHoursTopUp", m_nFilenameCacheHoursTopUp);

	if (System.GetInterfaceType() == nINTERFACE_BIOMETRIC)
	{
		m_nLocateOn = 0;
	}

	return bReply;
}

//*********************************************************************

bool CCsvImportData::Save()
{
	CSSIniFile file;

	file.Set("EnableImport", m_bEnableImport);
	file.Set("ImportFile", m_strImportFilename);
	file.Set("ImportMethod", m_nImportMethod);
	file.Set("DeleteFile", m_bDeleteFile);

	file.Set("EnableTopup", m_bEnableTopupImport);
	file.Set("TopupLocate", m_nTopupLocateOn);
	file.Set("TopupFile", m_strTopupFilename);
	file.Set("TopupUnknown", m_bTopupAddUnknown);
	file.Set("TopupStart", m_strTopupNextNewUserID);
	file.Set("DeleteTopup", m_bDeleteTopupFile);

	file.Set("EnableCustom", m_bEnableCustomImport);
	file.Set("CustomLocate", m_nCustomLocateOn);
	file.Set("CustomStart", m_strCustomNextNewUserID);
	file.Set("CustomMenu", m_strCustomMenuText);
	file.Set("CustomFile", m_strCustomFilename);
	file.Set("DeleteCustom", m_bDeleteCustomFile);
	file.Set("CustomImport", m_nImportCustomMethod);
	file.Set("CustomCase", m_bIgnoreCustomCase);

	file.Set("EnableDbase", m_bEnableDbaseAuditImport);
	file.Set("DbaseFile", m_strDbaseAuditFilename);
	file.Set("DbaseImport", m_nDbaseImportMethod);

	file.Set("StartUserID", m_strStandardNextNewUserID);

	file.Set("MandatoryHeaderStandard", m_strMandatoryHeaderStandard);
	file.Set("MandatoryHeaderTopUp", m_strMandatoryHeaderTopUp);
	file.Set("FilenameCacheHoursStandard", m_nFilenameCacheHoursStandard);
	file.Set("FilenameCacheHoursTopUp", m_nFilenameCacheHoursTopUp);

	if (System.GetInterfaceType() != nINTERFACE_BIOMETRIC)
	{
		file.Set("LocateOn", m_nLocateOn);
	}

	return file.Write(m_strFilename);
}

//*********************************************************************

const char* CCsvImportData::GetLocateOnLabel(int nLocateOn)
{
	switch (nLocateOn)
	{
	case 1: 
		return Account::MemberID.Label;

	case 2: 
		return Account::Info1.Label;

	case 3: 
		return Account::Info2.Label;

	case 4: 
		return Account::Info3.Label;

	case 5: 
		return Account::Info4.Label;

	case 6: 
		return Account::Info5.Label;

	case 7:
		return Account::Info6.Label;

	case 8: 
		return Account::Info7.Label;

	case 9: 
		return Account::Info8.Label;

	case 10: 
		return Account::ExternalRef1.Label;

	case 11: 
		return Account::ExternalRef2.Label;
	
	default:
		return "";
	}
}

//******************************************************************
// convert custom header line to database field names

CString CCsvImportData::ConvertCustomHeader ( const char* szCsvFile )
{
	CString strError = "";
	m_nCustomFieldCount	= 0;
	m_nCustomMatchCount	= 0;

	CMemberDatabase dbCustomList(m_strCustomDbFilename);
	if ( dbCustomList.OpenForSharing ( DB_READONLY ) == TRUE )
	{
		CFileStringArray file;
		if ( file.Read ( szCsvFile ) == TRUE )
		{
			CCSV csvHeader(file.GetAt(0));							// get import header line;
			m_nCustomFieldCount	= csvHeader.GetSize();

			for ( int i = 0 ; i < m_nCustomFieldCount ; i++ )
			{
				CString strCustomLabel = csvHeader.GetString(i);
				if (m_bIgnoreCustomCase == TRUE)
				{
					strCustomLabel.MakeUpper();
				}

				int nCount = 0;
				for ( int n = 0 ; n < dbCustomList.GetRecordCount() ; n++ )
				{
					if (dbCustomList.MoveTo(n) == TRUE)
					{
						CString strLabel = dbCustomList.GetUserID();
						if (m_bIgnoreCustomCase == TRUE)
						{
							strLabel.MakeUpper();
						}

						if ( strLabel == strCustomLabel )
						{
							csvHeader.SetAt ( i, dbCustomList.GetMemberID() );
							++nCount;
						}
					}
				}

				if (nCount > 0)
				{
					++m_nCustomMatchCount;
				}
			}

			if ( m_nCustomMatchCount > 0 )
			{
				file.SetAt ( 0, csvHeader.GetLine() );					// write new header back to import file
				file.Write ( szCsvFile );
			}
			else
			{
				strError = "No custom field names found in import header line";
			}
		}
		else
		{
			strError.Format ( "Unable to open import file %s", szCsvFile );
		}

		dbCustomList.Close();
	}
	else
	{
		strError = "Unable to open custom field list database";
	}

	return strError;
}

//******************************************************************
