//*******************************************************************
#include "resource.h"
//*******************************************************************
#include <atlpath.h>
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DeleteRecord.h"
#include "..\SmartPay4ManagerBgnd\SQLTranImportDatabase.h"
//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_ImportFilenameCache\SQLRepositoryImportFilenameCache.h"
#include "..\SmartPay4Shared\SQLDefines.h"
//*******************************************************************
#include "ImportRecordsDlg.h"
#include "ImportStatsDlg.h"
#include "SQLTranImportTopup.h"
#include "SQLTranImportUpgrade.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DatabaseImport.h"
//*******************************************************************

void CImportDatabase::ShowStats(CSQLImportCounters& Counters)
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = Counters.GetRead();
	Config.m_nLinesInvalid = Counters.GetInvalid();
	Config.m_nLinesIgnored = Counters.GetIgnored();
	Config.m_nLinesUsed = Counters.GetUsed();;
	Config.m_nRecordsAdded = Counters.GetAdded();
	Config.m_nLinesReadPos = 0;
	Config.m_nLinesInvalidPos = 1;
	Config.m_nLinesIgnoredPos = 2;
	Config.m_nLinesUsedPos = 3;
	Config.m_nRecordsAddedPos = 5;
	Config.m_nRecordsDeletedPos = -1;
	Config.m_nLastLinePos = 5;
	Config.m_strCaption = m_strTitle;
	Config.m_strExceptionFilename = Filenames.GetDBImportExceptionsFilename();

	CImportStatsDlg dlg(Config);
	dlg.DoModal();
}

//******************************************************************

void CImportDatabase::DisplayMessage ( const char* szMsg )
{
	CString strMsg = szMsg;
	if ( strMsg != "" )
	{
		Prompter.Info( szMsg, m_strTitle );
	}
}

//**************************************************************************
//*** Import csv records selected from import menu *************************
//**************************************************************************

bool CImportDatabase::CheckCache( CString strImportFilename, CString& strCacheFilename, int nType )
{
	bool bResult = FALSE;
	int nHours = 0;
	strCacheFilename = "";

	switch (nType)
	{
	case 0:
		nHours = m_data.GetFilenameCacheHoursStandard();
		break;

	case 1:
		nHours = m_data.GetFilenameCacheHoursTopUp();
		break;
	}

	if (nHours > 0)
	{
		{
			CPath MyPath(strImportFilename);
			int n = MyPath.FindFileName();

			if (n >= 0)
			{
				strCacheFilename = strImportFilename.Mid(n);
				strCacheFilename.MakeUpper();
			}
		}

		CSQLRepositoryImportFilenameCache RepoCache;
		if (RepoCache.CheckCache(nType, strCacheFilename, nHours) == TRUE)
		{
			m_strError = "The selected file has already been imported!";
			bResult = TRUE;
		}
	}

	return bResult;
}

//**************************************************************************

bool CImportDatabase::UserImport(CSQLImportCounters& Counters)
{
	bool bReply = FALSE;

	m_strTitle = "Import CSV Records";

	CImportRecordsDlg dlg(m_strTitle, m_data.m_strImportFilename, m_pParent);
	dlg.m_nImportMethod = m_data.m_nImportMethod;
	dlg.m_bDeleteFile = m_data.m_bDeleteFile;

	if (dlg.DoModal() == IDOK)
	{
		CString strImportFilename = dlg.m_strImportFilename;
		m_nImportMethod = dlg.m_nImportMethod;
		bool bDeleteFile = (dlg.m_bDeleteFile != 0);
		
		CString strCacheFilename = "";
		if (CheckCache(strImportFilename, strCacheFilename, 0) == FALSE)
		{
			CString strTmpFile = Filenames.GetImportsPath(::GetUniqueTempFilename("$06"));
			::CopyFile(strImportFilename, strTmpFile, FALSE);			// make copy of file ( may change header )
			m_arrayDeleteList.Add(strTmpFile);							// add to list of temp file to delete on exit

			if (ValidateFile(strTmpFile, m_data.GetMandatoryHeaderStandard()) == TRUE)
			{
				CSSFile fileImport;
				if (fileImport.Open(strTmpFile, "rb") == TRUE)
				{
					CSQLTranImportDatabase Tran(0, strCacheFilename);
					Tran.BeginTrans();
					Tran.DoWork(m_nImportMethod, m_nImportAppNo, fileImport, m_arrayImportHeader, m_nIndexCardNo);;

					if (Tran.EndTrans() == SQLTRAN_STATE_COMMITOK)
					{
						Tran.AfterTransaction();
						Tran.GetImportCounters(Counters);
						bReply = TRUE;

						if (TRUE == bDeleteFile)
						{
							m_arrayDeleteList.Add(strImportFilename);
						}
					}
				}
			}

			if (Counters.GetAdded() != 0)
			{
				SaveStandardNextNewUserID();
			}
		}

		DisplayMessage(m_strError);			
	}

	return bReply;
}

//**************************************************************************
//*** Import topups selected from import menu ******************************
//**************************************************************************

bool CImportDatabase::ImportTopups(CSQLImportCounters& Counters)
{
	bool bReply = FALSE;

	m_strTitle = "Import Account Topups";

	CImportRecordsDlg dlg(m_strTitle, m_data.m_strTopupFilename, m_pParent);
	dlg.m_nImportMethod = nIMPORT_METHOD_EXISTONLY;;
	dlg.m_bDeleteFile = m_data.m_bDeleteTopupFile;
	dlg.m_bEnableSelective = FALSE;

	if (dlg.DoModal() == IDOK)
	{
		CString strImportFilename = dlg.m_strImportFilename;
		m_nLocateOn = m_data.m_nTopupLocateOn;
		bool bDeleteFile = (dlg.m_bDeleteFile != 0);

		CString strCacheFilename = "";
		if (CheckCache(strImportFilename, strCacheFilename, 1) == FALSE)
		{
			if (m_data.m_bTopupAddUnknown == TRUE)
			{
				m_strNextNewUserID = m_data.m_strTopupNextNewUserID;
				m_nImportMethod = nIMPORT_METHOD_ALL;
			}
			else
			{
				m_strNextNewUserID = "";
				m_nImportMethod = nIMPORT_METHOD_EXISTONLY;
			}

			CString strTmpFile = Filenames.GetImportsPath(::GetUniqueTempFilename("$07"));
			::CopyFile(strImportFilename, strTmpFile, FALSE);			// make copy of file ( may change header )
			m_arrayDeleteList.Add(strTmpFile);							// add to list of temp file to delete on exit

			if (ValidateFile(strTmpFile, m_data.GetMandatoryHeaderTopUp()) == TRUE)
			{
				CSSFile importFile;
				if (importFile.Open(strTmpFile, "rb") == TRUE)
				{
					CSQLTranImportTopup Tran(1, strCacheFilename);
					Tran.BeginTrans();
					Tran.DoWork(m_nImportAppNo, importFile, m_arrayImportHeader, m_nIndexLocateOn, m_strLocateOnLabel, m_nIndexCardNo);

					bool bDoneImport = FALSE;
					if (Tran.EndTrans() == SQLTRAN_STATE_COMMITOK)
					{
						Tran.AfterTransaction();
						Tran.GetImportCounters(Counters);
						bReply = TRUE;

						if (bDeleteFile == TRUE)
						{
							m_arrayDeleteList.Add(strImportFilename);
						}
					}
				}

				if (m_data.m_bTopupAddUnknown == TRUE)
				{
					SaveTopupNextNewUserID();							// save next start from if required
				}
			}
		}

		DisplayMessage(m_strError);									// display any error message if requured
	}

	return bReply;
}

//**************************************************************************
//*** Import custom csv records selected from import menu ******************
//**************************************************************************

bool CImportDatabase::CustomImport(CSQLImportCounters& Counters)
{
	bool bReply = FALSE;

	m_strTitle.Format("Import Records ( %s )", 
		(const char*) m_data.m_strCustomMenuText);

	m_nImportAppNo = APPNO_IMPORTCUSTOM;

	CImportRecordsDlg dlg(m_strTitle, m_data.m_strCustomFilename, m_pParent);
	dlg.m_nImportMethod = m_data.m_nImportCustomMethod;
	dlg.m_bDeleteFile = m_data.m_bDeleteCustomFile;

	while (dlg.DoModal() != IDCANCEL)
	{
		CString strCustomFile = dlg.m_strImportFilename;
		CString strTmpFile = Filenames.GetImportsPath(::GetUniqueTempFilename("$08"));
		::CopyFile(strCustomFile, strTmpFile, FALSE);				// make copy of file in which to change the header
		m_arrayDeleteList.Add(strTmpFile);							// add to list of temp file to delete on exit

		if ((m_strError = m_data.ConvertCustomHeader(strTmpFile)) == "")			// convert custom header line to database field names
		{
			int nOK = IDYES;
			int nMatchedCount = m_data.GetCustomMatchCount();

			if (m_data.GetCustomFieldCount() > nMatchedCount)
			{
				CString strMsg = "";
				strMsg.Format("Only %d of the %d import header fields were matched the database fields!\n\nDo you still wish to continue with the import?", nMatchedCount, m_data.GetCustomFieldCount());
				nOK = Prompter.YesNo(strMsg, m_strTitle);
			}

			if (nOK == IDYES)
			{
				m_nImportMethod = dlg.m_nImportMethod;
				bool bDeleteFile = (dlg.m_bDeleteFile != 0);
				m_nLocateOn = m_data.m_nCustomLocateOn;
				m_strNextNewUserID = m_data.m_strCustomNextNewUserID;				// overwrite default settings

				if (ValidateFile(strTmpFile, "") == TRUE)
				{
					CSSFile fileImport;
					if (fileImport.Open(strTmpFile, "rb") == TRUE)
					{
						CSQLTranImportDatabase Tran;
						Tran.BeginTrans();
						Tran.DoWork(m_nImportMethod, m_nImportAppNo, fileImport, m_arrayImportHeader, m_nIndexCardNo);;

						if (Tran.EndTrans() == SQLTRAN_STATE_COMMITOK)
						{
							Tran.AfterTransaction();
							Tran.GetImportCounters(Counters);
							bReply = TRUE;

							if (TRUE == bDeleteFile)
							{
								m_arrayDeleteList.Add(strCustomFile);
							}
						}
					}

					if ( Counters.GetAdded() != 0 )
					{ 
						SaveCustomNextNewUserID();			
						break;
					}
				}
			}
		}

		DisplayMessage(m_strError);												// display any error message if requured
	}

	return bReply;
}

//***************************************************************************

bool CImportDatabase::IsValidFileForUpgrade(CString strFilename)
{
	int nCount = ::GetFileLineCount(strFilename);
	if (nCount < 2)
	{
		return FALSE;
	}

	CSSFile file1;
	if (file1.Open(strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	bool bReply = file1.Read(csvHeader);					// read header line 
	file1.Close();

	if (bReply == FALSE)
	{
		return FALSE;
	}

	// scan header line
	m_nIndexCardNo = -1;

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);

		if (SmartPayHeaderHelpers.GetNumericField(strField) == SMARTPAYFIELD_USERID)
		{
			m_nIndexCardNo = n;
		}
	}

	return (m_nIndexCardNo != -1);
}

//********************************************************************

bool CImportDatabase::DirectImport(CString strFilename)
{
	CSSFile file;
	if (file.Open(strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	if (file.Read(csvHeader) == FALSE)					// read header line 
	{
		return FALSE;
	}

	CArray<int, int> arrayHeader;
	SmartPayHeaderHelpers.CreateNumericHeader(csvHeader, m_arrayImportHeader);

	CSQLTranImportUpgrade Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_nImportAppNo, file, m_arrayImportHeader, m_nIndexCardNo);

	bool bResult = FALSE;

	if (Tran.EndTrans() == SQLTRAN_STATE_COMMITOK)
	{
		Tran.AfterTransaction();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
