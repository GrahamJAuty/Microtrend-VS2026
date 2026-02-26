/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
#include "PosTrayDateRange.h"
#include "PosTrayOptions.h"
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
#include "..\CommonEcrTray\ReportPluDelivery.h"
#include "..\CommonEcrTray\ReportPluStockLevel.h"
#include "SalesHistoryReportPlu.h"
/**********************************************************************/
#include "PosTrayReportCreatorStkLevelEcr.h"
/**********************************************************************/

CPosTrayReportCreatorStkLevelEcr::CPosTrayReportCreatorStkLevelEcr( CPosTrayTask& Task ) : m_Task( Task )
{
	//EMAIL FOLDER
	{
		m_strEmailFolder = "PosTray\\Email";
		::GetSyssetProgramPath( m_strEmailFolder );
		::CreateSubdirectory( m_strEmailFolder );
		m_strEmailFolder += "\\ECRMANSTOCK";
		::CreateSubdirectory( m_strEmailFolder );
	}

	//PRINT FOLDER
	{
		m_strPrintFolder = "PosTray\\Print";
		::GetSyssetProgramPath( m_strPrintFolder );
		::CreateSubdirectory( m_strPrintFolder );

		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_strPrintFilenamePrefix.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_2%3.3d_",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			m_Task.GetTaskNo() );

		m_nNextPrintFileNumber = 1;
	}

	//EXPORT FOLDER
	{
		m_strExportFolderTarget = m_Task.GetTaskExportFolder();
		::TrimSpaces( m_strExportFolderTarget, FALSE );
		
		if ( m_strExportFolderTarget == "" )
		{
			m_strExportFolderTarget = "PosTray\\Exports";
			::GetSyssetProgramPath( m_strExportFolderTarget );
		}
		::CreateSubdirectory( m_strExportFolderTarget );

		m_strExportFolderPending = "PosTray\\Pending";
		::GetSyssetProgramPath( m_strExportFolderPending );
		::CreateSubdirectory( m_strExportFolderPending );
		m_strExportFolderPending += "\\PluStock";
		::CreateSubdirectory( m_strExportFolderPending );
	}
}

/**********************************************************************/

CPosTrayReportCreatorStkLevelEcr::~CPosTrayReportCreatorStkLevelEcr(void)
{
}

/**********************************************************************/

bool CPosTrayReportCreatorStkLevelEcr::ProcessTask( COleDateTime& dateTask )
{
	m_dateTask = dateTask;
	bool bResult = ProcessTaskInternal();	
	return bResult;
}

/**********************************************************************/

bool CPosTrayReportCreatorStkLevelEcr::ProcessTaskInternal()
{
	COleDateTime oleDummy;
	CPosTrayDateRange dateRange;
	
	if ( PosTrayOptions.GetBusinessDateFlag() == TRUE )
		dateRange.SetDateToday( m_dateTask, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute() );
	else
		dateRange.SetDateToday( m_dateTask );
	
	dateRange.GetRange( m_Task.GetBatchDateType(), m_oleDateFrom, m_oleDateTo, oleDummy );
	
	CCSV csvReports( m_Task.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayEntityIDs;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayEntityIDs.Consolidate( item );
	}

	int nExtraReportID = 1;
	CReportConsolidationArray<CSortedStringItem> arrayExportLabels;

	CArray<int,int> arrayPossibleReports;
	arrayPossibleReports.Add( REPORT_PLUSTOCK_APPARENT );
	arrayPossibleReports.Add( REPORT_PLUSTOCK_VALUATION  );
	arrayPossibleReports.Add( REPORT_PLUSTOCK_REORDER );
	arrayPossibleReports.Add( REPORT_PLUSTOCK_DEL_AUDIT );
	arrayPossibleReports.Add( REPORT_PLUSTOCK_DEL_TOTAL );
	arrayPossibleReports.Add( REPORT_PLUSTOCK_DEL_VALUE );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	m_Task.CreateEmailReportMap(arrayEmailByReport);

	for ( int n = 0; n < arrayPossibleReports.GetSize(); n++ )
	{
		int nReportType = arrayPossibleReports.GetAt(n);

		{
			CSortedStringItem itemReport;
			itemReport.m_strItem.Format( "%4.4d", nReportType );
	
			int nIdx;
			if ( arrayEntityIDs.Find( itemReport, nIdx ) == FALSE )
				continue;
		}

		int nDbIdx = 0;
		int nDbNo = m_Task.GetBatchConDbNo();
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			nDbIdx = 0;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

		bool bGotReport = FALSE;
		CString strEmailSubject = "";
		CString strReportKey = "";
		CString strParamFilename = "";

		int nSuppFilter = m_Task.GetBatchSupplierFilter();
		
		if ( nSuppFilter < 0 )
			nSuppFilter = ECRMANSUPPLIER_ALL;

		if ( nReportType != REPORT_PLUSTOCK_DEL_AUDIT )
		{
			CReportPluStockLevel Report;
			Report.SetReportType( nReportType );
			Report.SetDbIdx( nDbIdx );
			Report.SetPluRangeFlag ( m_Task.GetBatchPluRangeFlag() );
			Report.SetPluFrom ( m_Task.GetBatchPluNoFrom() );
			Report.SetPluTo ( m_Task.GetBatchPluNoTo() );
			Report.SetDepartmentFilter(m_Task.GetBatchDeptFilter() );
			Report.SetDepartmentSortFlag( m_Task.GetBatchDeptSortFlag() );
			Report.SetSupplierFilter( nSuppFilter );
			Report.SetSupplierSortFlag( m_Task.GetBatchSupplierSortFlag() );
			Report.SetZeroSkipFlag( m_Task.GetBatchStockZeroSkipFlag() );

			switch ( nReportType )
			{
			case REPORT_PLUSTOCK_APPARENT:
				Report.SetReportName ( "Plu Apparent Stock" );
				Report.ConsolidateStockLevels();
				bGotReport = TRUE;
				break;

			case REPORT_PLUSTOCK_REORDER:
				Report.SetReportName ( "Plu Stock Reorder" );
				Report.ConsolidateStockLevels();
				bGotReport = TRUE;
				break;

			case REPORT_PLUSTOCK_VALUATION:
				Report.SetReportName ( "Plu Stock Valuation" );
				Report.ConsolidateStockLevels();
				bGotReport = TRUE;
				break;

			case REPORT_PLUSTOCK_DEL_TOTAL:
				Report.SetDateFrom( m_oleDateFrom );
				Report.SetDateTo( m_oleDateTo );
				Report.SetReportName ( "Plu Delivery Totals" );
				Report.ConsolidateDeliveryTotals();
				bGotReport = TRUE;
				break;

			case REPORT_PLUSTOCK_DEL_VALUE:
				Report.SetDateFrom( m_oleDateFrom );
				Report.SetDateTo( m_oleDateTo );
				Report.SetReportName ( "Plu Delivery Valuation" );
				Report.ConsolidateDeliveryTotals();
				bGotReport = TRUE;
				break;
			}

			if ( TRUE == bGotReport )
			{
				CFilenameUpdater FnUp( SysFiles::PluApparentPrm );
				strParamFilename = FnUp.GetFilenameToUse();
				strReportKey.Format ( "%d", nReportType );
				strReportKey += Report.GetReportKey();
				bGotReport = Report.CreateReport();
				strEmailSubject = Report.GetEmailSubject();
			}
		}
		else
		{
			CReportPluDelivery Report;
			Report.SetDbIdx( nDbIdx );
			Report.SetDateFrom( m_oleDateFrom );
			Report.SetDateTo( m_oleDateTo );
			Report.SetPluRangeFlag ( m_Task.GetBatchPluRangeFlag() );
			Report.SetPluFrom ( m_Task.GetBatchPluNoFrom() );
			Report.SetPluTo ( m_Task.GetBatchPluNoTo() );
			Report.SetDepartmentFilter ( m_Task.GetBatchDeptFilter() );
			Report.SetSupplierFilter( nSuppFilter );
			
			CFilenameUpdater FnUp ( SysFiles::PluDelAuditPrm );
			strParamFilename = FnUp.GetFilenameToUse();
			bGotReport = Report.CreateDeliveryAuditReport();
			strEmailSubject = Report.GetEmailSubject();
		}
	
		if ( TRUE == bGotReport )
		{
			CString strTaskFilename = m_Task.GetTaskExportFilename();
			::TrimSpaces( strTaskFilename, FALSE );

			CReportFileDoc ReportFileDoc ( Super.ReportFilename() );

			int nEmailFormat = m_Task.GetTaskFormatEmail();
			if ( ( 0 == nEmailFormat ) || ( 1 == nEmailFormat ) )
			{
				int nEmailIdx = 1;
				{
					int nPos = 0;
					CSortedIntByString item;
					item.m_strItem.Format("%4.4d", nReportType);
					if (arrayEmailByReport.Find(item, nPos) == TRUE)
					{
						arrayEmailByReport.GetAt(nPos, item);
						nEmailIdx = item.m_nItem;
					}
				}

				if ((nEmailIdx >= 1) && (nEmailIdx <= 5))
				{
					CString strEmailFilename;
					strEmailFilename.Format("\\%4.4d_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.%s",
						nReportType,
						timeNow.GetYear(),
						timeNow.GetMonth(),
						timeNow.GetDay(),
						timeNow.GetHour(),
						timeNow.GetMinute(),
						timeNow.GetSecond(),
						(1 == nEmailFormat) ? "htm" : "csv");

					CString strNKNFilename;
					strNKNFilename = strEmailFilename.Left(strEmailFilename.GetLength() - 3);
					strNKNFilename += "nkn";

					if (0 == nEmailFormat)
					{
						ReportFileDoc.SaveAsCSV(m_strEmailFolder + strEmailFilename);
					}
					else
					{
						CTabRuler TabRuler;
						CReportHelpers ReportHelpers;
						ReportHelpers.CreateTabRuler(ReportFileDoc, strParamFilename, strReportKey, TabRuler);
						ReportFileDoc.SaveAsHTML(m_strEmailFolder + strEmailFilename, TabRuler);
					}

					CSSFile fileNKN;
					if (fileNKN.Open(m_strEmailFolder + strNKNFilename, "wb") == TRUE)
					{
						fileNKN.WriteLine(m_Task.GetTaskEmailNickname(nEmailIdx - 1));
						fileNKN.WriteLine(strEmailSubject);
						fileNKN.Close();
					}
				}
			}

			int nExportFormat = m_Task.GetTaskFormatExport();
			if ( ( 0 == nExportFormat ) || ( 1 == nExportFormat ) )
			{
				CString strFilenameBody;
				strFilenameBody.Format( "Stock%4.4d", nReportType );

				CPosTrayTaskConsolInfo infoQuick{};
				infoQuick.m_nQuickType = 0;

				CString strExportFilenameTarget = "";
				strExportFilenameTarget += m_Task.GetTaskExportFilename();
				strExportFilenameTarget += strFilenameBody;
				strExportFilenameTarget += m_Task.GetExportFilenameSuffix( m_dateTask, infoQuick );

				CString strSlash = "\\";
				CString strExportFilenamePending = "";
				strExportFilenamePending += strSlash; 
				strExportFilenamePending += CSSMD5::CalcMD5( m_strExportFolderTarget + strSlash + strExportFilenameTarget );
				strExportFilenamePending += ( 1 == nExportFormat ) ? ".htm" : ".csv"; 

				CString str001Filename;
				str001Filename = strExportFilenamePending.Left( strExportFilenamePending.GetLength() - 3 );
				str001Filename += "001";
	
				CSSFile file001;
				if ( file001.Open( m_strExportFolderPending + str001Filename, "wb" ) == TRUE )
				{
					file001.WriteLine( strExportFilenameTarget + strExportFilenamePending.Right(4) );
					file001.WriteLine( m_Task.GetFilename( FALSE ) );
					file001.Close();
				}

				if ( 0 == nExportFormat )
					ReportFileDoc.SaveAsCSV( m_strExportFolderPending + strExportFilenamePending );
				else
				{
					CTabRuler TabRuler;
					CReportHelpers ReportHelpers;
					ReportHelpers.CreateTabRuler( ReportFileDoc, strParamFilename, strReportKey, TabRuler );
					ReportFileDoc.SaveAsHTML( m_strExportFolderPending + strExportFilenamePending  );
				}
			}

			if ( TRUE == m_Task.GetTaskAutoPrintFlag() )
			{
				CString strPrintFilename = "";
				strPrintFilename.Format( "%s\\%s%6.6d",
					(const char*) m_strPrintFolder,
					(const char*) m_strPrintFilenamePrefix,
					m_nNextPrintFileNumber++ );

				{
					CSSFile fileHelper;
					if ( fileHelper.Open( strPrintFilename + ".dat", "wb" ) == TRUE )
					{
						CFilenameUpdater FnUp( SysFiles::PluApparentPrm );
						fileHelper.WriteLine( FnUp.GetFilenameToUse() );
						fileHelper.WriteLine( strReportKey );
					}
				}

				::CopyFile( Super.ReportFilename(), strPrintFilename + ".rep", FALSE );
			}
		}
	}
	
	return TRUE;
}

/**********************************************************************/
