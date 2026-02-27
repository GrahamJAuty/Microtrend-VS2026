//**********************************************************************
// Export database records
//**********************************************************************
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "DbReportFields.h"
#include "DbReportHandler.h"
//**********************************************************************
#include "DatabaseExport.h"
/**********************************************************************/

CExportDatabase::CExportDatabase(const char* szLabel, CWnd* pParent)
{
	m_strLabel = szLabel;
	m_pParent = pParent;
	m_strExportFilename = "";					// get filled out whne report  file created
	m_strTitle = "";							// get filled out whne report  file created
	m_nExportCount = 0;							// not required for auto export
}

/**********************************************************************/

int CExportDatabase::CreateFile(bool bShowProgress)
{
	CDbReportHandler dbReporter{};
	if (dbReporter.Read(m_strLabel) == FALSE)
	{
		return nREPORT_NOERROR;					// no export format set up
	}

	m_strExportFilename = dbReporter.m_strFilename;
	m_strTitle = dbReporter.m_strTitle;

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount RepoAccount;
	RepoAccount.PrepareExportWhere(dbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
	RecordSet.m_strSort = dbReporter.GetSQLOrderBy();		
	
	CSSFile file;
	bool bWrittenHeader = FALSE;
	CSQLRowAccountFull RowAccount{};

	/*
	CReportConsolidationArray<CSortedInt64Item> arrayBioUserIDs;
	
	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
	{
		int nError = 0;
		SQLRepositoryBioRegister.GetSortedUserIDList(nError, arrayBioUserIDs);
	}
	break;
	}
	*/

	while ( RecordSet.StepSelectAll(RowAccount) == TRUE )
	{ 
		if (dbReporter.NonSQLFilter(RowAccount) == TRUE)
		{
			if (FALSE == bWrittenHeader)
			{
				if (file.Open(m_strExportFilename, "wb") == FALSE)
				{
					return nREPORT_CREATEFAIL;
				}
				else
				{
					file.WriteLine(dbReporter.m_strHeader);
					DbReportFields.CreateColumnListFromHeader(dbReporter.m_strHeader);
					bWrittenHeader = TRUE;
				}
			}

			CCSV csv{};
			dbReporter.AppendDbFieldsToCSVLine(RowAccount, &csv);
			if (file.WriteLine(csv.GetLine()) == TRUE)
			{
				++m_nExportCount;
			}
		}
	}
	
	if (FALSE == bWrittenHeader)
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
	if ( dbReporter.Read ( m_strLabel ) == TRUE )
	{
		bool bExists = ::FileExists( dbReporter.m_strFilename );
		CString strMode = ( bExists == TRUE ) ? "ab" : "wb";

		CSSFile file;
		if ( file.Open ( dbReporter.m_strFilename, strMode ) == TRUE )
		{
			if ( bExists == FALSE )									// see if export file exists
			{
				if (dbReporter.m_strLabel != "DEL2")				// epos biodelete file needs no header
				{
					file.WriteLine(dbReporter.m_strHeader);		
				}
			}

			//HEADER DEFINES REPORT LINE FORMAT EVEN IF NOT WRITTEN TO FILE
			DbReportFields.CreateColumnListFromHeader(dbReporter.m_strHeader);

			CCSV csv;
			dbReporter.AppendDbFieldsToCSVLine ( RowAccount, &csv );
			file.WriteLine ( csv.GetLine() );
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

//*******************************************************************
