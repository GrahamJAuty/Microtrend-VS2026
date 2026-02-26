/**********************************************************************/
#include "BarcodeImportTypeDlg.h"
#include "BarcodeNumberDlg.h"
#include "BarcodeRangeDlg.h"
#include "CommsBatchDlg.h"
#include "CSVDBImportStatsDlg.h"
#include "DatabaseCSVArray.h"
#include "..\SPosEcrManager5\DatabaseImportHandlerEcrman.h"
#include "..\SPosStockManager5\DatabaseImportHandlerStockman.h"
#include "DownloadDateDlg.h"
#include "ImportDefines.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "RepSpawn.h"
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataBarcodeDlg.h"
/**********************************************************************/

CSingleListContentHelpersBarcode::CSingleListContentHelpersBarcode()
{
}

/**********************************************************************/

int CSingleListContentHelpersBarcode::GetArraySize()
{
	return DataManager.Barcode.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersBarcode::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nIndex, BarcodeRecord );

		int nPluIdx;
		__int64 nPluNo = BarcodeRecord.GetPluNoInt();

		CString strPrint = "";
		if ( Sysset.IsBarmanSystem() == TRUE )
		{
			strPrint.Format( "\t%s",
				BarcodeRecord.GetPrintFlagText( TRUE ) );
		}

		CString strMod = "";
		{
			int nMod = BarcodeRecord.GetModifier();
			if ( nMod != 0 )
				strMod = DataManager.Modifier.GetDisplayName( nMod );
		}

		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			CCSV csvLine( '\t' );
			csvLine.Add( BarcodeRecord.GetBarcodeNoString() );
			csvLine.Add( BarcodeRecord.GetPluNoString() );
			if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) csvLine.Add( strMod );
			csvLine.Add( PluRecord.GetReportText() );
			csvLine.Add( strPrint );

			strLine = csvLine.GetLine();
		}
		else
		{
			CCSV csvLine( '\t' );
			csvLine.Add( BarcodeRecord.GetBarcodeNoString() );
			csvLine.Add( BarcodeRecord.GetPluNoString() );
			if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) csvLine.Add( strMod );
			csvLine.Add( "Unknown" );
			csvLine.Add( strPrint );

			strLine = csvLine.GetLine();
		}

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersBarcode::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	list.InsertColumn ( 0, "Barcode No", LVCFMT_LEFT, 120 );
	list.InsertColumn ( 1, "Plu No", LVCFMT_LEFT, 120 );

	int nOffset = 2;
	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
		list.InsertColumn ( nOffset++, "Modifier", LVCFMT_LEFT, 100 );

	list.InsertColumn ( nOffset++, "Description", LVCFMT_LEFT, 190 );
	list.InsertColumn ( nOffset++, "Print", LVCFMT_LEFT, 50 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataBarcodeDlg::CListDataBarcodeDlg( CWnd* pParent ) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersBarcode();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataBarcodeDlg::~CListDataBarcodeDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataBarcodeDlg::SetTitle()
{
	CString strTitle = "Barcode Database ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), FALSE ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataBarcodeDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.SetWindowText ( "&Import" );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
	{
		m_button5.EnableWindow( FALSE );
		m_button5.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 137 );
	MoveControl( &m_button5, 368, 172 );
	MoveControl( &m_button6, 368, 207 );
}

/**********************************************************************/

void CListDataBarcodeDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
	
	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
	{
		ResizeDialog( 495, 348 );
		ResizeControl( &m_listData, 415, 334 );
		MoveControl( &m_button1, 438, 7 );
		MoveControl( &m_button2, 438, 42 );
		MoveControl( &m_button3, 438, 77 );
		MoveControl( &m_button4, 438, 137 );
		MoveControl( &m_button5, 438, 172 );
		MoveControl( &m_button6, 438, 207 );
		MoveControl( &m_buttonOK, 438, 311 );
		MoveControl( &m_buttonCancel, 438, 327 );
	}
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleButton1()
{
	CBarcodeCSVRecord BarcodeRecord;
	BarcodeRecord.SetBarcodeNo( 0 );
	BarcodeRecord.SetPluNo ( 0 );

	CBarcodeNumberDlg dlg ( BARCODE_LIST_ADD, BarcodeRecord, this );

	if ( dlg.DoModal() == IDOK )
	{
		int nBarcodeIdx;
		if ( DataManager.Barcode.FindBarcodeByNumber( BarcodeRecord.GetBarcodeNoInt(), nBarcodeIdx ) == FALSE )
		{
			m_WarnCancelAfterChange.SetFlag();
			DataManager.Barcode.InsertAt( nBarcodeIdx, BarcodeRecord );
			m_listData.SetItemCountEx ( GetArraySize() );
		}
		
		SelectLine ( nBarcodeIdx );
	}
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleButton2()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( nIndex, BarcodeRecord );

	CBarcodeNumberDlg dlg ( BARCODE_LIST_EDIT, BarcodeRecord, this );

	if ( dlg.DoModal() == IDOK )
	{
		m_WarnCancelAfterChange.SetFlag();
		DataManager.Barcode.SetAt( nIndex, BarcodeRecord );
		SelectLine( nIndex );
	}
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleButton3()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( nIndex, BarcodeRecord );

	CBarcodeRangeDlg dlgRange( "Delete Barcodes", this );
	dlgRange.SetBarcodeFrom( DataManager.Barcode.GetBarcodeNoInt(nIndex) );
	dlgRange.SetBarcodeTo( DataManager.Barcode.GetBarcodeNoInt(nIndex) );

	if ( dlgRange.DoModal() != IDOK )
		return;

	__int64 nBarcodeFrom = dlgRange.GetBarcodeFrom();
	__int64 nBarcodeTo = dlgRange.GetBarcodeTo();

	int nStartIdx, nEndIdx;
	DataManager.Barcode.FindBarcodeByNumber( nBarcodeFrom, nStartIdx );
	if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeTo, nEndIdx ) == FALSE )
	{
		nEndIdx -= 1;
	}

	for ( int nPos = nEndIdx; nPos >= nStartIdx; nPos-- )
	{
		DataManager.Barcode.RemoveAt( nPos );
	}
	
	m_WarnCancelAfterChange.SetFlag();
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nIndex );
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleButton4() 
{
	if ( DataManager.Barcode.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some barcodes\nbefore you can print this report" );
		return;
	}

	CBarcodeRangeDlg dlgRange( "Print Barcodes", this );
	dlgRange.SetBarcodeFrom ( DataManager.Barcode.GetBarcodeNoInt(0) );
	dlgRange.SetBarcodeTo ( DataManager.Barcode.GetBarcodeNoInt( DataManager.Barcode.GetSize() - 1 ) );
	if ( dlgRange.DoModal() != IDOK )
		return;

	__int64 nBarcodeFrom = dlgRange.GetBarcodeFrom();
	__int64 nBarcodeTo = dlgRange.GetBarcodeTo();

	int nStartIdx, nEndIdx;
	DataManager.Barcode.FindBarcodeByNumber( nBarcodeFrom, nStartIdx );
	if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeTo, nEndIdx ) == FALSE )
	{
		nEndIdx -= 1;
	}

	if ( nStartIdx > nEndIdx )
	{
		Prompter.Error ( "No barcodes were found\nin the specified range" );
		return;
	}

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Barcode List" );		
	ReportFile.AddColumn ( "Barcode No", TA_LEFT, 310 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) ReportFile.AddColumn ( "Modifier", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	if ( Sysset.IsBarmanSystem() == TRUE )	ReportFile.AddColumn ( "Print", TA_LEFT, 300 );
	
	for ( int nBarcodeIdx = nStartIdx; nBarcodeIdx <= nEndIdx; nBarcodeIdx++ )
	{
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		CString strName = "";
		{
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				strName = PluRecord.GetReportText();
			}
			else	
				strName = "Unknown";
		}

		CString strMod = "";
		{
			int nMod = BarcodeRecord.GetModifier();
			if ( nMod != 0 )
				strMod = DataManager.Modifier.GetDisplayName( nMod );
		}

		CCSV csvOut ( '\t' );
		csvOut.Add ( BarcodeRecord.GetBarcodeNoString() );
		csvOut.Add( BarcodeRecord.GetPluNoString() );
		if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) csvOut.Add( strMod );
		csvOut.Add( strName );
		if ( Sysset.IsBarmanSystem() == TRUE )	csvOut.Add( BarcodeRecord.GetPrintFlagText() );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();
		
	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::BarcodePrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataBarcodeDlg::HandleButton5() 
{
	int	nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Barcodes", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_BAR, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Barcodes" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/

void CListDataBarcodeDlg::HandleButton6()
{
	CString strTitle = "Import Barcode CSV File";
	CFileDialog dlgFilename ( TRUE, "csv", "barcode", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	
	if ( dlgFilename.DoModal() != IDOK )
		return;

	CString strFilename = dlgFilename.GetPathName();

	CBarcodeImportTypeDlg dlgMethod ( this );
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;
	if ( dlgMethod.DoModal() != IDOK )
		return;

#ifndef STOCKMAN_SYSTEM
	CDatabaseImportHandlerEcrman ImportHandler;
#else
	CDatabaseImportHandlerStockman ImportHandler;
#endif

	ImportHandler.SetBarcodeImportMethod( dlgMethod.m_nImportMethod );
	int nResult = ImportHandler.ImportBarcodeFile ( strFilename );
	
	if ( nResult != IMPORTERROR_NONE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to process import file\n(error code %d)", nResult );
		Prompter.Error ( strMsg );
		return;
	}

	m_WarnCancelAfterChange.SetFlag();
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( 0 );

	CCSVDBImportStatsDlg dlgInfo ( NODE_BARCODE, this );
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nRecordsAdded = ImportHandler.GetImportBarcodeAdded();
	dlgInfo.m_nRecordsModified = ImportHandler.GetImportBarcodeModified();
	dlgInfo.DoModal();
}

/**********************************************************************/

	
