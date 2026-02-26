//**********************************************************************
// Export database records
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLBuilderBind.h"
#include "..\SPosLoyalty4Shared\SQLDB.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//**********************************************************************
#include "DbReportHandler.h"
#include "LoyaltyManager.h"
#include "WorkingDlg.h"
#include "RepmanHandler.h"
#include "RecSort.h"
/**********************************************************************/
#include "DatabaseExport.h"
/**********************************************************************/

CExportDatabase::CExportDatabase(const char* szLabel, CWnd* pParent)
{
	m_strLabel = szLabel;
	m_pParent = pParent;

	m_nErrorNo = nREPORT_NOERROR;
	m_strExportFilename = "";
	m_strTitle = "";							// get filled out by label type
}

/**********************************************************************/
// export file

void CExportDatabase::Execute ( bool bShowProgress )
{
	int nExportCount = 0;
	int nErrorNo = CreateFile ( bShowProgress, nExportCount );

	CRepmanHandler repman ( m_strTitle, m_pParent );
	
	if (TRUE == bShowProgress)
	{
		repman.DisplayError(nErrorNo);
	}

	if ( (nREPORT_NOERROR == nErrorNo) && ( TRUE == bShowProgress ) )
	{
		CString strMsg = "";
		strMsg.Format ( "Number of lines exported to '%s' = %d", (const char*) m_strExportFilename, nExportCount );
		repman.DisplayMessage ( strMsg, MB_ICONINFORMATION );
	}
}

//*******************************************************************

int CExportDatabase::CreateFile(bool bShowProgress, int& nExportCount )
{
	nExportCount = 0;

	CDbReportHandler dbReporter{};
	if (dbReporter.Read(m_strLabel) == FALSE)
	{
		return nREPORT_CANCEL;								// no export format set up
	}

	m_strExportFilename = dbReporter.m_strFilename;
	m_strTitle = dbReporter.m_strTitle;

	CSSFile file;
	if (file.Open(m_strExportFilename, "wb") == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg WorkingDlg("Reading database");
	if (bShowProgress == TRUE)
	{
		WorkingDlg.Create();
	}
	bool bCancelButton = FALSE;

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareExportWhere(dbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
	RecordSet.m_strSort = dbReporter.GetSQLOrderBy();
	
	int nChecked = 0;
	CSQLRowAccountFull RowAccount;
	CArray<int, int> arrayHeader;

	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (0 == nChecked++)
		{
			file.WriteLine(dbReporter.m_strHeader);					// write header line
			LoyaltyHeaderHelpers.CreateNumericHeader(dbReporter.m_strHeader, arrayHeader);
		}

		if (TRUE == bShowProgress)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(nChecked) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (dbReporter.NonSQLFilter(RowAccount) == TRUE)
		{
			CCSV csv{};
			dbReporter.CreateReportLine(arrayHeader, &csv,RowAccount);
			if (file.WriteLine(csv.GetLine()) == TRUE)
			{
				++nExportCount;
			}
		}
	}

	file.Close();

	if ( TRUE == bCancelButton )
	{
		return nREPORT_CANCEL;
	}

	if (0 == nExportCount)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

/**********************************************************************/
// export single record- used by auto export on delete

int CExportDatabase::ExportRecord(CSQLRowAccountFull& RowAccount)
{
	int nReply = nREPORT_NOERROR;

	CDbReportHandler dbReporter{};
	if (dbReporter.Read(m_strLabel) == TRUE)
	{
		bool bExists = ::FileExists(dbReporter.m_strFilename);
		CString strMode = (bExists == TRUE) ? "ab" : "wb";

		CSSFile file;
		if (file.Open(dbReporter.m_strFilename, strMode) == TRUE)
		{
			if (bExists == FALSE)
			{
				file.WriteLine(dbReporter.m_strHeader);				// write header line
			}

			CArray<int, int>arrayHeader;
			LoyaltyHeaderHelpers.CreateNumericHeader(dbReporter.m_strHeader, arrayHeader);

			CCSV csv;
			dbReporter.CreateReportLine(arrayHeader, &csv, RowAccount);
			file.WriteLine(csv.GetLine());
			file.Close();
		}
		else
		{
			nReply = nREPORT_CREATEFAIL;
		}
	}
	else
	{
		nReply = nREPORT_NOERROR;					// no export format set up
	}

	return nReply;
}

//**********************************************************************
