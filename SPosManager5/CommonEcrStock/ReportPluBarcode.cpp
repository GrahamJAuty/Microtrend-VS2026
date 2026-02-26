/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "PluPrtBarcodeDlg.h" 
#include "ReportHelpers.h"
#include "Repspawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportPluBarcode.h"
/**********************************************************************/
static const int REPORT_OKAY = 0;
static const int REPORT_ERROR_RANGE = 1;
static const int REPORT_ERROR_FILE = 2;
/**********************************************************************/
static const int LEN_PLUNO = 350;
static const int LEN_REPTEXT = 400;
static const int LEN_DEPTNO	= 100;
static const int LEN_BARCODE = 380;
static const int LEN_PRINT = 300;
/**********************************************************************/

CReportPluBarcode::CReportPluBarcode( CPluFilterArray& FilterArray, const char* szKey ) : m_FilterArray ( FilterArray )
{
	CFilenameUpdater FnUp( SysFiles::BarLinkPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_strKey = szKey;
}

/**********************************************************************/

void CReportPluBarcode::CreateReport()
{
	DataManager.Barcode.CreatePluBarcodeList();
	StatusProgress.Lock( TRUE, "Creating report" );
	int nResult = CreatePluBarcodeReport();
	StatusProgress.Unlock();

	switch( nResult )
	{
	case 0:
		{
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, m_strKey, NULL );
		}
		break;

	case 1:
		Prompter.Error ( "There are no Plus in the selected range" );
		break;

	case 2:
		Prompter.Error ( "Unable to create Plu report file" );
		break;
	}
}

/**********************************************************************/

int CReportPluBarcode::CreatePluBarcodeReport()
{
	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( m_nPluFrom, m_nPluTo, nStartIdx, nRange );

	if ( nRange == 0 )
		return REPORT_ERROR_RANGE;				//no plus in range

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return REPORT_ERROR_FILE;				//unable to create file
	
	m_ReportFile.SetStyle1 ( "Plu Barcode Links" );
	CreatePluBarcodeReportHeader ();
	
	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_nDepartmentFilter );

	CCSVPluPrtBarcodeKey csvKey ( m_strKey );
	
	if ( TRUE == m_bSortByDept )
	{
		CReportConsolidationArray<CSortedIntAndInt> ConsolArray;

		for ( int nStep = 0; nStep < nRange; nStep++ )	
		{ 
			StatusProgress.SetPos ( nStep, nRange, 0, 50 );

			int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nStep );
			
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );

			__int64 nPluNo = PluRecord.GetPluNoInt(); 
			if ( ( nPluNo < m_nPluFrom ) || ( nPluNo > m_nPluTo ) )
				continue;
			
			if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
				continue;

			if ( ( csvKey.PrintZeroPriced() == FALSE ) && ( PluRecord.GetPrice ( 0 ) == 0.0 ) )
				continue;

			CSortedIntAndInt item;
			item.m_nInt1 = PluRecord.GetBaseDeptNo();
			item.m_nInt2 = nStartIdx + nStep;
			ConsolArray.Consolidate( item );
		}

		nRange = ConsolArray.GetSize();
		int nCurrentDeptNo = -1;
	
		for ( int nStep = 0; nStep < nRange; nStep++ )
		{
			StatusProgress.SetPos ( nStep, nRange, 50, 50 );
			
			CSortedIntAndInt item;
			ConsolArray.GetAt( nStep, item );
			int nDeptNo = item.m_nInt1;
			int nPluIdx = m_FilterArray.GetPluItemIndexAt( item.m_nInt2 );

			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );

			CStringArray arrayLines;
			CreatePluBarcodeReportLines( PluRecord, arrayLines );

			int nLines = arrayLines.GetSize();
			if ( nLines > 0 )
			{
				if ( nDeptNo != nCurrentDeptNo )
				{
					if ( nCurrentDeptNo != -1 )
						m_ReportFile.WriteLine( "" );

					nCurrentDeptNo = nDeptNo;

					CString strHeader;
					strHeader.Format ( "<..>D%2.2d %s", nCurrentDeptNo, DataManager.Department.GetReportTextByDeptNo ( nCurrentDeptNo ) );
					m_ReportFile.WriteLine( strHeader );

					if ( TRUE == csvKey.PrintSeparate() )
						m_ReportFile.WriteLine( "<LI>" );
				}

				for ( int n = 0; n < nLines; n++ )
					m_ReportFile.WriteLine( arrayLines.GetAt(n) );
					
				if ( TRUE == csvKey.PrintSeparate() )
					m_ReportFile.WriteLine( "<LI>" );
			}
		}
	}
	else
	{
		for ( int nStep = 0; nStep < nRange; nStep++ )	
		{ 
			StatusProgress.SetPos ( nStep, nRange );
		
			int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nStep );
			
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );

			__int64 nPluNo = PluRecord.GetPluNoInt(); 
			if ( ( nPluNo < m_nPluFrom ) || ( nPluNo > m_nPluTo ) )
				continue;
					
			if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
				continue;

			if ( ( csvKey.PrintZeroPriced() == FALSE ) && ( PluRecord.GetPrice ( 0 ) == 0.0 ) )
				continue;

			CStringArray arrayLines;
			CreatePluBarcodeReportLines ( PluRecord, arrayLines );

			int nLines = arrayLines.GetSize();
			if ( nLines > 0 )
			{
				for ( int n = 0; n < nLines; n++ )
					m_ReportFile.WriteLine( arrayLines.GetAt(n) );
				
				if ( TRUE == csvKey.PrintSeparate() )
					m_ReportFile.WriteLine( "<LI>" );
			}
		}
	}

	m_ReportFile.WriteLine ( "" );	
	m_ReportFile.Close();
	
	return 0;			//function completed properly
}

/**********************************************************************/

void CReportPluBarcode::CreatePluBarcodeReportHeader()
{
	CCSVPluPrtBarcodeKey csv ( m_strKey );

	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, LEN_PLUNO );
	m_ReportFile.AddColumn ( "Plu Description", TA_LEFT, LEN_REPTEXT );
	m_ReportFile.AddColumn ( "Dp", TA_RIGHT, LEN_DEPTNO );
	m_ReportFile.AddColumn ( "Barcode", TA_LEFT, LEN_BARCODE );

	if ( Sysset.IsBarmanSystem() == TRUE )
		m_ReportFile.AddColumn ( "Print", TA_LEFT, LEN_PRINT );
}

/**********************************************************************/

void CReportPluBarcode::CreatePluBarcodeReportLines ( CPluCSVRecord& PluRecord, CStringArray& arrayLines )
{
	arrayLines.RemoveAll();

	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );		
	
	if ( TRUE == m_bDirect )
	{
		__int64 nPluNo = PluRecord.GetPluNoInt();

		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == FALSE )
		{
			bool bShelf = PluRecord.GetPrintShelfFlag();
			bool bProduct = PluRecord.GetPrintProductFlag();
			bProduct &= BarcodePluNoTable.IsValidEan( nPluNo );

			if ( ( TRUE == bShelf ) || ( TRUE == bProduct ) )
			{
				CBarcodeCSVRecord Dummy;
				Dummy.SetPrintShelfFlag( bShelf );
				Dummy.SetPrintProductFlag( bProduct );

				csvOut.Add ( ReportHelpers.GetDisplayPluNo ( nPluNo ) );
				csvOut.Add ( PluRecord.GetReportText() );
				csvOut.Add ( PluRecord.GetBaseDeptNo() );
				csvOut.Add ( "Direct Label" );
				csvOut.Add ( Dummy.GetPrintFlagText() );

				arrayLines.Add( csvOut.GetLine() );
			}
		}
	}
	
	CArray<int,int> BarcodeIdxArray;
	DataManager.Barcode.GetBarcodeIdxArray( PluRecord.GetPluNoInt(), BarcodeIdxArray );
	
	for ( int nIndex = 0; nIndex < BarcodeIdxArray.GetSize(); nIndex++ )
	{
		csvOut.RemoveAll();

		if ( arrayLines.GetSize() == 0 )
		{
			csvOut.Add ( ReportHelpers.GetDisplayPluNo ( PluRecord.GetPluNoInt() ) );
			csvOut.Add ( PluRecord.GetReportText() );
			csvOut.Add ( PluRecord.GetBaseDeptNo() );
		}
		else
		{
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "" );
		}
	
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( BarcodeIdxArray.GetAt( nIndex ), BarcodeRecord );
		csvOut.Add (BarcodeRecord.GetBarcodeNoString() );

		if ( Sysset.IsBarmanSystem() == TRUE )
			csvOut.Add (BarcodeRecord.GetPrintFlagText() );

		arrayLines.Add( csvOut.GetLine() );
	}
}

/**********************************************************************/
