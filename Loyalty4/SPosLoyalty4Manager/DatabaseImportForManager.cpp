//*****************************************************************************************
// Import database records 
//*****************************************************************************************
// CAUTION: Any fields change in here MUST also be added to DatabaseImport in LoyaltyServer 
//*****************************************************************************************
#include "resource.h"
//*****************************************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTranImportDatabase.h"
//*****************************************************************************************
#include "FindSingleDlg.h"
#include "GroupComboHelpers.h"
#include "ImportRecordsDlg.h"
#include "ImportStatsDlg.h"
#include "LoyaltyManager.h"
#include "WorkingDlg.h"
//**********************************************************************
#include "DatabaseImportForManager.h"
//**********************************************************************

CImportDatabaseForManager::CImportDatabaseForManager(const char* szFilename, CWnd* pParent)
{
	m_strFilename = szFilename;
	m_pParent = pParent;
	m_nIgnored = 0;
	m_nRead = 0;
	m_nUsed = 0;
	m_nInvalid = 0;
	m_nAdded = 0;
	m_bBackgroundMode = FALSE;

	m_nIndexCardNo = -1;
	m_strTitle = "CSV Import";
}

/**********************************************************************/

CImportDatabaseForManager::~CImportDatabaseForManager()
{
}

/**********************************************************************/

bool CImportDatabaseForManager::IsValidFile()
{
	CString strError = "";

	int nCount = ::GetFileLineCount(m_strFilename);
	if (nCount < 2)
	{
		if (nCount != 0)
		{
			strError.Format("Insufficent data lines found in file ' %s '",
				(const char*)m_strFilename);

			DisplayMessage(strError, MB_ICONINFORMATION);
		}
		return FALSE;
	}

	CSSFile file1;
	if (file1.Open(m_strFilename, "rb") == FALSE)
	{
		strError.Format("Unable to open import file ' %s '",
			(const char*)m_strFilename);

		DisplayMessage(strError, MB_ICONSTOP);
		return FALSE;
	}

	CCSV csvHeader;
	bool bReply = file1.Read(csvHeader);					// read header line 
	file1.Close();

	if (bReply == FALSE)
	{
		strError.Format("No data found in the import file ' %s '",
			(const char*)m_strFilename);

		DisplayMessage(strError, MB_ICONSTOP);
		return FALSE;
	}

	// scan header line

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);

		if (LoyaltyHeaderHelpers.GetNumericField(strField) == LOYALTYFIELD_USERID)
		{
			m_nIndexCardNo = n;
		}
	}

	// ** see if have CardNo field in header.

	if (m_nIndexCardNo != -1)									// must have a CardNo field
	{
		bool bOk = TRUE;
		if (FALSE == m_bBackgroundMode)
		{
			CString strBuf = "";
			strBuf.Format("%d lines found in the import file - OK to continue", nCount - 1);	// ignore header line in count
			MessageBeep(MB_ICONQUESTION);
			bOk = (Prompter.OkCancel(strBuf, m_strTitle) == IDCANCEL) ? FALSE : TRUE;
		}
		return bOk;
	}

	// to get here don't have a CardNo field in header

	CString strMsg = "";
	strMsg.Format("The import file contains %d lines of data.\n\n", nCount - 1);	// ignore header line in count

	CString strMsg1 = "";
	strMsg1.Format("As it does not contain a CardNo Field, %d new database records will be created.\n", nCount - 1);
	strMsg1 += "You must specify the starting card number for these new records.\n\nDo you still wish to continue";
	strMsg += strMsg1;

	MessageBeep(MB_ICONQUESTION);
	if (Prompter.YesNo(strMsg, m_strTitle) == IDNO)
	{
		return FALSE;
	}

	// need to add new cards to database

	CFindSingleImportDlg dlg{};

	if (dlg.DoModal() == IDCANCEL)
	{
		MessageBeep(MB_ICONINFORMATION);
		Prompter.Info("Import cancelled", m_strTitle);
		return FALSE;
	}

	csvHeader.InsertAt(0, Account::UserID.Label);		// modify header to include cardno field
	m_nIndexCardNo = 0;										// index to cardno field

	CFileStringArray file;
	if (file.Read(m_strFilename) == TRUE)
	{
		file.SetAt(0, csvHeader.GetLine());
		int nNextNewCardNo = atoi(dlg.GetOutputCardNo());

		for (int nLine = 1; nLine < file.GetCount(); nLine++)
		{
			CCSV csvLine(file.GetAt(nLine));
			csvLine.InsertAt(0, nNextNewCardNo++);			// add card number
			file.SetAt(nLine, csvLine.GetLine());
		}

		file.Write(m_strFilename);
	}
	return TRUE;
}

//**********************************************************************

bool CImportDatabaseForManager::IsValidFileForUpgrade()
{
	int nCount = ::GetFileLineCount(m_strFilename);
	if (nCount < 2)
	{
		return FALSE;
	}

	CSSFile file1;
	if (file1.Open(m_strFilename, "rb") == FALSE)
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

		if ( LoyaltyHeaderHelpers.GetNumericField(strField) == LOYALTYFIELD_USERID )
		{ 
			m_nIndexCardNo = n;
		}
	}

	return (m_nIndexCardNo != -1 );
}

//**********************************************************************
// must call IsValidFile before calling Import as that validates file for use

void CImportDatabaseForManager::UserImport()
{
	CImportRecordsDlg dlg ( m_strFilename, m_pParent );
	if (dlg.DoModal() == IDOK)
	{
		DirectImport(dlg.m_nImportMethod);
	}
}

/**********************************************************************/
// also called by auto import

bool CImportDatabaseForManager::DirectImport(int nImportMethod, bool bForUpgrade)
{
	CSSFile file;
	if (file.Open(m_strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	if (file.Read(csvHeader) == FALSE)					// read header line 
	{
		return FALSE;
	}

	CArray<int, int> arrayHeader;
	LoyaltyHeaderHelpers.CreateNumericHeader(csvHeader, arrayHeader);

	bool bDoneImport = FALSE;

	CSQLTranImportDatabase Tran;
	Tran.BeginTrans();
	Tran.DoWorkManagerImport(m_bBackgroundMode, bForUpgrade, nImportMethod, file, arrayHeader, m_nIndexCardNo);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		Tran.AfterTransaction();
		bDoneImport = TRUE;
		break;

	default:
		Tran.ShowErrorMessage("Import Accounts");
		break;
	}

	file.Close();

	if ((FALSE == m_bBackgroundMode) && (FALSE == bForUpgrade) && (TRUE == bDoneImport))
	{
		CImportStatsDlg dlgStats;
		dlgStats.m_nLinesRead = Tran.GetRead();
		dlgStats.m_nLinesInvalid = Tran.GetInvalid();
		dlgStats.m_nLinesIgnored = Tran.GetIgnored();
		dlgStats.m_nLinesUsed = Tran.GetUsed();
		dlgStats.m_nRecordsAdded = Tran.GetAdded();
		dlgStats.m_strExceptionFilename = Filenames.GetDBImportExceptionsFilename();
		dlgStats.DoModal();
	}

	return TRUE;
}

/**********************************************************************/

void CImportDatabaseForManager::DisplayMessage ( const char* szMsg, UINT uType )
{
	if ( m_bBackgroundMode == FALSE )
	{
		MessageBeep ( uType );
		Prompter.DisplayMessage( szMsg, m_strTitle, uType );
	}
}

/**********************************************************************/

