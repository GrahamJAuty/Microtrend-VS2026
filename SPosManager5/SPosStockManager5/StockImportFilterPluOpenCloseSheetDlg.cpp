/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "PriceHelpers.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditImportPlu.h"
#include "StockImportSheetHelpers.h"
/**********************************************************************/
#include "StockImportFilterPluOpenCloseSheetDlg.h"
/**********************************************************************/

CStockImportFilterPluOpenCloseSheetDlg::CStockImportFilterPluOpenCloseSheetDlg( int nSpIdx, bool bClosing, bool bMultiSheet, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent)
: CStockImportFilterPluDlg( nSpIdx, bClosing, szPath, arrayImport, pParent )
{
	if (FALSE == bMultiSheet)
	{
		m_strTitle += " from ImportSheet";
	}
	else
	{
		m_strTitle += " from Consolidated ImportSheets";
	}

	m_bClosing = bClosing;
	m_bMultiSheet = bMultiSheet;
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseSheetDlg::AdjustControls()
{
	if (TRUE == m_bMultiSheet)
	{
		m_radio1.SetCheck(FALSE);
		m_radio2.SetCheck(FALSE);
		m_radio3.SetCheck(TRUE);
		ShowAndEnableWindow(&m_staticAction, FALSE);
		ShowAndEnableWindow(&m_radio1, FALSE);
		ShowAndEnableWindow(&m_radio2, FALSE);
		ShowAndEnableWindow(&m_radio3, FALSE);
		MoveControl(&m_buttonOK, 219, 110);
		MoveControl(&m_buttonCancel, 278, 110);
		ResizeDialog(335,131);
	}
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseSheetDlg::ProcessImportDataInternal()
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_nTotalExceptions = 0;
			
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );
	strPath += "\\";		
	
	CString strExceptionsFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV_EX );
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( strExceptionsFilename );

	CSSFile fileImportSource;
	CSSFile fileExceptions;
	
	if ( fileImportSource.Open ( m_strHeaderImportPath, "rb" ) == FALSE )
		return;

	if ( fileExceptions.Open ( strExceptionsFilename, "wb" ) == FALSE )
		return;

	CString strHeader;
	if ( fileImportSource.ReadString( strHeader ) == FALSE )
		return;

	CStockImportSheetHelpers ImportSheetHelpers( m_nSpIdx, m_nStocktakeNo );
	if ( ImportSheetHelpers.ProcessHeader( strHeader ) == FALSE )
		return;
			
	CString strBuffer;
	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{				
		CString strStockCode = "";
		bool bGotSUQty = FALSE;
		double dSUQty = 0.0;
		bool bGotDUQty = FALSE;
		double dDUQty = 0.0;

		//FIRST CHECK ALLOWS EXPLICIT DU QTY OF ZERO, AS OPPOSED TO BLANK
		ImportSheetHelpers.ProcessLine( strBuffer, strStockCode, bGotSUQty, dSUQty, bGotDUQty, dDUQty );
		if ((FALSE == bGotSUQty) && (FALSE == bGotDUQty))
		{
			continue;
		}

		//SECOND CHECK AVOIDS VALIDATION OF DELIVERY UNIT IF DU QTY IS ZERO
		bool bGotNonZeroDUQty = bGotDUQty;
		if ( TRUE == bGotNonZeroDUQty )
		{
			if ( CPriceHelpers::CompareDoubles( dDUQty, 0.0, 5 ) == 0 )
			{
				bGotNonZeroDUQty = FALSE;
				dDUQty = 0.0;
			}
		}

		m_nLinesFound++;

		double dDUYield = 0.0;

		int nStockIdx;
		CString strException = "";
		
		if (DataManager.Stock.FindStockItemByCode(strStockCode, nStockIdx) == FALSE)
		{
			strException = "Stock code not found";
		}
		else
		{
			CStockCSVRecord StockRecord;
			CStockLevelsCSVRecord StockLevels;
			switch( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, nSpNo, m_nStocktakeNo ) )
			{
			case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:

				if (FALSE == m_bClosing)
				{
					strException = "Stock code in pending stock take";
				}
				else
				{
					strException = "Stock code not in this stock take";
				}

				break;

			case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
				strException = "Stock item not active at this stockpoint";
				break;
			}

			if ( strException == "" )
			{
				if ( TRUE == bGotNonZeroDUQty )
				{
					if ( StockRecord.GetSupplierCount() == 0 )
					{
						strException = "No delivery method for stock item";
						dSUQty = dDUQty;
					}
					else
					{
						dDUYield = StockRecord.GetYield(0);
					}
				}
			}
		}

		if ( strException == "" )
		{
			double dTotalSUQty = 0.0;
			
			if (TRUE == bGotSUQty)
			{
				dTotalSUQty += dSUQty;
			}

			if (TRUE == bGotNonZeroDUQty)
			{
				dTotalSUQty += (dDUQty * dDUYield);
			}
			else
			{
				dDUQty = 0.0;
			}

			m_arrayImport.AddItem( nStockIdx, 0, dDUQty, dTotalSUQty );
		}
		else
		{
			AuditExceptions( fileExceptions, strStockCode, dSUQty, strException );
			m_nTotalExceptions++;
			m_nExceptionLines++;
		}

		StatusProgress.SetPos ( fileImportSource );
	}
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseSheetDlg::AuditExceptions( CSSFile& fileExceptions, const char* szStockCode, double dSUQty, const char* szProblem )
{
	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( m_nExceptionReportType );
	csvOut.Add ( 2 );
	csvOut.Add ( "" );
	csvOut.AddStockAuditQty ( dSUQty );
	csvOut.Add ( szStockCode );
	csvOut.Add ( szProblem );
	csvOut.Add ( "" );		//IGNORE SALE DATE
	csvOut.Add ( "" );		//IGNORE SALE TIME
	csvOut.Add ( FALSE );	//NOT A WRAPPER
	fileExceptions.WriteLine( csvOut.GetLine() );
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseSheetDlg::PreviewImportedData()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
	{
		Prompter.Error( "Unable to create import preview file" );
		return;
	}

	CSSFile fileImportSource;
	if ( fileImportSource.Open ( m_strHeaderImportPath, "rb" ) == FALSE )
	{
		Prompter.Error( "Unable to access import file" );
		return;
	}

	ReportFile.SetStyle1 ( "Stock ImportSheet" );

	CString strHeader;
	if ( fileImportSource.ReadString( strHeader ) == FALSE )
	{
		Prompter.Error( "No data found in import file" );
		return;
	}

	CStockImportSheetHelpers ImportSheetHelpers( m_nSpIdx, m_nStocktakeNo );
	if ( ImportSheetHelpers.CreateReportHeader( strHeader, ReportFile ) == FALSE )
	{
		Prompter.Error( "Invalid header in import file" );
		return;
	}

	CString strBuffer;
	while( fileImportSource.ReadString( strBuffer ) == TRUE )
	{
		CCSV csvData( strBuffer );

		CCSV csvReport( '\t' );
		for (int n = 0; n < csvData.GetSize(); n++)
		{
			csvReport.Add(csvData.GetString(n));
		}

		ReportFile.WriteLine( csvReport.GetLine() );
	}

	ReportFile.Close();

	CFilenameUpdater FnUp( SysFiles::StockSheetImportPrm );
	CString strParamFile = FnUp.GetFilenameToUse();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), strParamFile, strHeader, this );
}

/**********************************************************************/

