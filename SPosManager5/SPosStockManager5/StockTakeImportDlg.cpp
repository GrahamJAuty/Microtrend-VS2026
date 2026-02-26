/**********************************************************************/
#include "CCSVReportLine.h"
#include "FileRemover.h"
#include "ReportHelpers.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StockAuditBaseReport.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockTakeImportDlg.h"
/**********************************************************************/

CStocktakeInfo::CStocktakeInfo()
{
	m_nStocktakeNo = 0;
	m_strType = "";
	m_strComment = "";
	m_nLineCount = 0;
}

/**********************************************************************/

CStocktakeItemInfo::CStocktakeItemInfo()
{
	m_nStocktakeNo = 0;
	m_nPluIdx = 0;
	m_nStockIdx = 0;
	m_dStockPerPlu;
	m_strNumber = "";
	m_strQty = "";
}

/**********************************************************************/

CExceptionInfo::CExceptionInfo()
{
	m_nType = 0;
	m_nPluIdx = 0;
	m_nStockIdx = 0;
	m_dStockPerPlu;
	m_strNumber = "";
	m_strQty = "";
}

/**********************************************************************/

CStocktakeImportDlg::CStocktakeImportDlg( int nSpIdx, const char* szFilename, CStocktakeCSVArray& StocktakeArray, CWnd* pParent)
	: CDialog(CStocktakeImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStocktakeImportDlg)
	//}}AFX_DATA_INIT

	m_nExceptionType[0] = 0;
	m_nExceptionType[1] = 0;

	m_nSpIdx = nSpIdx;
	m_strFilename = szFilename;
	m_bDirectStockCode = FALSE;

	for ( int n = 0; n < StocktakeArray.GetSize(); n++ )
	{
		CStocktakeCSVRecord Stocktake;
		StocktakeArray.GetAt( n, Stocktake );
		int nStocktakeNo = Stocktake.GetStocktakeNo();

		int nPos;
		if ( FindStocktakeInfo( nStocktakeNo, nPos ) == FALSE )
		{
			CStocktakeInfo info;
			info.m_nStocktakeNo = nStocktakeNo;
			info.m_strType = Stocktake.GetStocktakeTypeText();
			info.m_strComment = Stocktake.GetComment();
			info.m_nLineCount = 0;
			m_arrayStocktakeInfo.InsertAt( nPos, info );
		}
	}
}

/**********************************************************************/

void CStocktakeImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStocktakeImportDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO_ACTION, m_comboAction );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStocktakeImportDlg, CDialog)
	//{{AFX_MSG_MAP(CStocktakeImportDlg)
	ON_BN_CLICKED(IDC_BUTTON_ALLREPORT, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONEREPORT, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStocktakeImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	m_listStocktakes.SubclassDlgItem ( IDC_LIST, this );
	m_listStocktakes.InsertColumn ( 0, "", LVCFMT_LEFT, 26 );
	m_listStocktakes.InsertColumn ( 1, "Type", LVCFMT_LEFT, 70 );
	m_listStocktakes.InsertColumn ( 2, "Comment", LVCFMT_LEFT, 210 );
	m_listStocktakes.InsertColumn ( 3, "Import Lines", LVCFMT_RIGHT, 70 );
	m_listStocktakes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES /* | LVS_EX_GRIDLINES */ );

	m_listException.SubclassDlgItem ( IDC_LIST_EXCEPTION, this );
	m_listException.InsertColumn ( 0, "", LVCFMT_LEFT, 26 );
	m_listException.InsertColumn ( 1, "Exceptions", LVCFMT_LEFT, 280 );
	m_listException.InsertColumn ( 2, "Import Lines", LVCFMT_RIGHT, 70 );
	m_listException.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES /* | LVS_EX_GRIDLINES */ );

	m_comboAction.AddString ( "Add to closing stock level" );
	m_comboAction.AddString ( "Subtract from closing stock level" );
	m_comboAction.AddString ( "Set closing stock level" );
	m_comboAction.SetCurSel(0);
	
	AddList();
	return TRUE;  
}

/**********************************************************************/

bool CStocktakeImportDlg::FindStocktakeInfo( int nStocktakeNo, int& nPos )
{
	int nStart = 0;
	int nEnd = m_arrayStocktakeInfo.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayStocktakeInfo[nPos].m_nStocktakeNo == nStocktakeNo )
			return TRUE;

		else if ( m_arrayStocktakeInfo[nPos].m_nStocktakeNo < nStocktakeNo )
			nStart = nPos + 1;

		else
			nEnd = nPos - 1;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

bool CStocktakeImportDlg::FindStocktakeItemInfo( int nStocktakeNo, int nStockIdx, int& nPos )
{
	int nStart = 0;
	int nEnd = m_arrayStocktakeItemInfo.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayStocktakeItemInfo[nPos].m_nStocktakeNo < nStocktakeNo )
			nStart = nPos + 1;

		else if ( m_arrayStocktakeItemInfo[nPos].m_nStocktakeNo > nStocktakeNo )
			nEnd = nPos - 1;

		else if ( m_arrayStocktakeItemInfo[nPos].m_nStockIdx < nStockIdx )
			nStart = nPos + 1;

		else if ( m_arrayStocktakeItemInfo[nPos].m_nStockIdx > nStockIdx )
			nEnd = nPos - 1;

		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CStocktakeImportDlg::FindExceptionInfo( int nType, int& nPos )
{
	int nStart = 0;
	int nEnd = m_arrayExceptionInfo.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayExceptionInfo[nPos].m_nType < nType )
			nStart = nPos + 1;
		else
			nEnd = nPos - 1;
	}

	nPos = nStart;
}

/**********************************************************************/

void CStocktakeImportDlg::AddList()
{
	for ( int n = 0; n < m_arrayStocktakeInfo.GetSize(); n++ )
	{
		CStocktakeInfo info = m_arrayStocktakeInfo.GetAt(n);

		CString strLines;
		strLines.Format( "%d", info.m_nLineCount );
	
		int nPos = m_listStocktakes.GetItemCount();
		m_listStocktakes.InsertItem ( nPos, "" );
		m_listStocktakes.SetItem ( nPos, 1, LVIF_TEXT, info.m_strType, 0, 0, 0, NULL );	
		m_listStocktakes.SetItem ( nPos, 2, LVIF_TEXT, info.m_strComment, 0, 0, 0, NULL );
		m_listStocktakes.SetItem ( nPos, 3, LVIF_TEXT, strLines, 0, 0, 0, NULL );
		m_listStocktakes.SetCheck ( nPos, FALSE );
	}

	int nStartIdx, nEndIdx;
	FindExceptionInfo( 1, nStartIdx );
	FindExceptionInfo( 5, nEndIdx );
	
	if ( nStartIdx < nEndIdx )
	{
		CString strLines;
		strLines.Format( "%d", nEndIdx - nStartIdx );
	
		int nPos = m_listException.GetItemCount();
		m_listException.InsertItem ( nPos, "" );
		m_listException.SetItem ( nPos, 1, LVIF_TEXT, "Discard lines that do not match a stock item", 0, 0, 0, NULL );	
		m_listException.SetItem ( nPos, 2, LVIF_TEXT, strLines, 0, 0, 0, NULL );
		m_listException.SetCheck ( nPos, FALSE );
		m_nExceptionType[nPos] = 1;
	}

	FindExceptionInfo( 5, nStartIdx );
	FindExceptionInfo( 7, nEndIdx );
	
	if ( nStartIdx < nEndIdx )
	{
		CString strLines;
		strLines.Format( "%d", nEndIdx - nStartIdx );
	
		int nPos = m_listException.GetItemCount();
		m_listException.InsertItem ( nPos, "" );
		m_listException.SetItem ( nPos, 1, LVIF_TEXT, "Discard lines that are not in a stocktake", 0, 0, 0, NULL );	
		m_listException.SetItem ( nPos, 2, LVIF_TEXT, strLines, 0, 0, 0, NULL );
		m_listException.SetCheck ( nPos, FALSE );
		m_nExceptionType[nPos] = 2;
	}
}

/**********************************************************************/

void CStocktakeImportDlg::OnButtonAll() 
{	
	for (int nIndex = 0; nIndex < m_listStocktakes.GetItemCount(); nIndex++)
	{
		m_listStocktakes.SetCheck(nIndex, TRUE);
	}
}

/**********************************************************************/

void CStocktakeImportDlg::OnButtonNone() 
{	
	for (int nIndex = 0; nIndex < m_listStocktakes.GetItemCount(); nIndex++)
	{
		m_listStocktakes.SetCheck(nIndex, FALSE);
	}
}

/**********************************************************************/

void CStocktakeImportDlg::AddException( int nType, CExceptionInfo& info )
{
	info.m_nType = nType;

	int nPos;
	FindExceptionInfo( nType + 1, nPos );
	m_arrayExceptionInfo.InsertAt( nPos, info );
}

/**********************************************************************/

bool CStocktakeImportDlg::CheckImportFile()
{
	CSSFile fileImportSource;
	if ( fileImportSource.Open( m_strFilename, "rb" ) == FALSE )
		return 0;

	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	CString strBuffer;
	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{				
		CStringArray StockDetails;
		CCSV csvImport ( strBuffer );

		CString strNumber = csvImport.GetString(0);
		CString strQty = csvImport.GetString(1);

		CStocktakeItemInfo infoItem;
		infoItem.m_strNumber = strNumber;
		infoItem.m_strQty = strQty;

		CExceptionInfo infoException;
		infoException.m_strNumber = strNumber;
		infoException.m_strQty = strQty;

		__int64 nPluNo = 0;
		CString strStockCode = "";

		if (TRUE == m_bDirectStockCode)
		{
			strStockCode = strNumber;
		}
		else
		{
			if (DealerFlags.GetImportBarcodeFlag() == TRUE)
				nPluNo = DataManager.Barcode.GetPluFromBarcode(_atoi64(strNumber));
			else
				nPluNo = _atoi64(strNumber);

			//CHECK THAT PLU ITEM EXISTS
			int nPluIdx;
			if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == FALSE)
			{
				AddException(1, infoException);
				continue;
			}

			infoItem.m_nPluIdx = nPluIdx;
			infoException.m_nPluIdx = nPluIdx;

			//CHECK THAT PLU ITEM IS LINKED TO SINGLE STOCK ITEM
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt(nPluIdx, PluRecord);

			CRecipeCSVRecord RecipeRecord;
			DataManager.Plu.GetRecipe(nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord);

			if (RecipeRecord.GetSize() == 0)
			{
				AddException(2, infoException);
				continue;
			}

			if (RecipeRecord.GetSize() != 1)
			{
				AddException(3, infoException);
				continue;
			}

			strStockCode = RecipeRecord.GetStockCode(0);
		}
		
		//CHECK THAT LINKED STOCK ITEM EXISTS
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
		{
			AddException( 4, infoException );
			continue;
		}	

		infoItem.m_nStockIdx = nStockIdx;
		infoException.m_nStockIdx = nStockIdx;

		//CHECK THAT LINKED STOCK ITEM IS IN A STOCKTAKE
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		int nStocktakeNo = StockLevels.GetStocktakeNo();
		if ( nStocktakeNo <= 0 )
		{
			AddException( 5, infoException );
			continue;
		}	

		int nPos;
		if ( FindStocktakeInfo( nStocktakeNo, nPos ) == FALSE )
		{
			AddException( 6, infoException );
			continue;
		}
		else
		{
			m_arrayStocktakeInfo[nPos].m_nLineCount++;

			if ( FindStocktakeItemInfo( nStocktakeNo, nStockIdx, nPos ) == TRUE )
			{
				if (m_arrayStocktakeItemInfo[nPos].m_strQty != "")
				{
					m_arrayStocktakeItemInfo[nPos].m_strQty += ",";
				}

				m_arrayStocktakeItemInfo[nPos].m_strQty += strQty;
			}
			else
			{
				CStocktakeItemInfo info;
				
				if (TRUE == m_bDirectStockCode)
				{
					info.m_dStockPerPlu = 1;
				}
				else
				{
					DataManager.Plu.GetPluStockQty(nPluNo, info.m_dStockPerPlu);
				}

				info.m_nStocktakeNo = nStocktakeNo;
				info.m_nStockIdx = nStockIdx;
				info.m_strNumber = strNumber;
				info.m_strQty = strQty;
				m_arrayStocktakeItemInfo.InsertAt( nPos, info );
			}
		}
	}

	for ( int n = m_arrayStocktakeInfo.GetSize() - 1; n >= 0; n-- )
		if ( m_arrayStocktakeInfo[n].m_nLineCount == 0 )
			m_arrayStocktakeInfo.RemoveAt( n );

	return ( ( m_arrayStocktakeInfo.GetSize() != 0 ) || ( m_arrayExceptionInfo.GetSize() != 0 ) );
}

/**********************************************************************/

void CStocktakeImportDlg::OnButtonPreview() 
{
	CArray<int, int> arrayTickedItems;
	CArray<int, int> arrayExceptions;

	for ( int nIndex = 0; nIndex < m_listStocktakes.GetItemCount(); nIndex++ )
		if ( m_listStocktakes.GetCheck( nIndex ) != 0 )
			arrayTickedItems.Add( nIndex );

	for ( int nException = 1; nException <= 6; nException++ )
	{
		int nStartIdx, nEndIdx;
		FindExceptionInfo( nException, nStartIdx );
		FindExceptionInfo( nException + 1, nEndIdx );
		nEndIdx--;

		if ( nEndIdx >= nStartIdx )
			arrayExceptions.Add( nException );
	}

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;
	
	ReportFile.SetStyle1 ( "Closing Stock Import Preview" );

	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 450 );
	ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Action", TA_LEFT, 200 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "New Qty", TA_RIGHT, 200 );

	CString strType = "";

	int nAction = m_comboAction.GetCurSel();
	switch( nAction )
	{
	case 1:		strType = "Subtract";		break;
	case 2:		strType = "Set";			break;
	case 0:
	default:	strType = "Add";			break;
	}

	bool bGotLine = FALSE;

	for ( int n = 0; n < arrayTickedItems.GetSize(); n++ )
	{
		int nSel = arrayTickedItems.GetAt(n);
		CStocktakeInfo info = m_arrayStocktakeInfo.GetAt(nSel);

		int nStocktakeNo = info.m_nStocktakeNo;

		int nStartIdx, nEndIdx;
		FindStocktakeItemInfo( nStocktakeNo, -1, nStartIdx );
		FindStocktakeItemInfo( nStocktakeNo + 1, -1, nEndIdx );
		nEndIdx--;

		if ( nEndIdx < nStartIdx )
			continue;

		if ( TRUE == bGotLine )
			ReportFile.WriteLine( "" );

		CString str;
		str.Format( "<..>Stocktake %s (%s)",
			(const char*) info.m_strType,
			(const char*) info.m_strComment );

		ReportFile.WriteLine( str );
		ReportFile.WriteLine( "<LI>" );

		bGotLine = TRUE;

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CStocktakeItemInfo infoItem = m_arrayStocktakeItemInfo.GetAt(nIdx);

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( infoItem.m_nStockIdx, StockRecord );
			CString strStockCode = StockRecord.GetStockCode();

			CString strLineStockCode = ReportHelpers.GetDisplayStockCode( strStockCode );
			CString strLineDescription = StockRecord.GetDescription();
			CString strLineCategory = DataManager.Category.GetText( StockRecord.GetCategory() );

			CStockLevelsCSVRecord StockLevels;
			int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
			DataManager.Stock.GetStockpointTotals ( infoItem.m_nStockIdx, StockRecord, nSpNo, StockLevels );

			int nSubUnits = StockRecord.GetSubUnits( StockOptions.GetStockSubUnitsFlag() );

			double dClosingQty = StockLevels.GetStocktakeClosingQty();

			CCSV csvChanges( infoItem.m_strQty );

			for ( int x = 0; x < csvChanges.GetSize(); x++ )
			{
				double dChange = csvChanges.GetDouble(x) * infoItem.m_dStockPerPlu;

				CStockReportLine csvOut;

				if ( 0 == x )
				{
					csvOut.AppendString ( strLineStockCode );
					csvOut.AppendString ( strLineDescription );
					csvOut.AppendString ( strLineCategory );
					csvOut.AppendString ( strType );
				}
				else
				{
					csvOut.AppendString ( "" );
					csvOut.AppendString ( "" );
					csvOut.AppendString ( "" );
					csvOut.AppendString ( "" );
				}

				csvOut.AppendStockQty( dChange, nSubUnits );
			
				switch( nAction )
				{
				case 1:		dClosingQty -= dChange;		break;
				case 2:		dClosingQty = dChange;		break;
				case 0:
				default:	dClosingQty += dChange;		break;
				}
				
				csvOut.AppendStockQty( dClosingQty, nSubUnits );

				ReportFile.WriteLine( csvOut.GetLine() );
			}
		}
	}

	ReportFile.WriteLine( "" );

	for ( int x = 0; x < arrayExceptions.GetSize(); x++ )
	{
		int nExceptionType = arrayExceptions.GetAt(x);

		CString strType = "<..>Exceptions : ";

		switch( nExceptionType )
		{
		case 1:		strType += "Plu items not found in database";			break;
		case 2:		strType += "Plu items not linked to stock items";		break;
		case 3:		strType += "Plu items linked to unknown stock items";	break;
		case 4:		strType += "Plu items linked to multiple stock items";	break;
		case 5:		strType += "Stock items not in stocktake";				break;
		case 6:		strType += "Stock items in unknown stocktake";			break;
		default:	strType += "";											break;
		}

		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( strType );
		ReportFile.WriteLine( "<LI>" );

		int nStartIdx, nEndIdx;
		FindExceptionInfo( nExceptionType, nStartIdx );
		FindExceptionInfo( nExceptionType + 1, nEndIdx );
		nEndIdx--;

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CExceptionInfo info = m_arrayExceptionInfo[nIdx];

			CStockReportLine csvOut;
			
			switch(  nExceptionType )
			{
			case 1:
				csvOut.AppendString( info.m_strNumber );
				csvOut.AppendString( "" );
				csvOut.AppendString( "" );
				csvOut.AppendString( "" );
				csvOut.AppendString( info.m_strQty );
				ReportFile.WriteLine( csvOut.GetLine() );
				break;

			case 2:
			case 3:
			case 4:
				AppendPluExceptions( info, csvOut );
				ReportFile.WriteLine( csvOut.GetLine() );
				break;

			case 5:
			case 6:
				AppendStockExceptions( info, csvOut );
				ReportFile.WriteLine( csvOut.GetLine() );
				break;
			}
		}
	}
			
	ReportFile.WriteLine ( "" );
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::AutoClosingPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CStocktakeImportDlg::AppendPluExceptions( CExceptionInfo& info, CStockReportLine& csvOut ) 
{
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( info.m_nPluIdx, PluRecord );
					
	csvOut.AppendString( PluRecord.GetPluNoString() );
	csvOut.AppendString( ReportHelpers.GetReportText( PluRecord ) );
	csvOut.AppendString( "" );
	csvOut.AppendString( "" );
	csvOut.AppendString( info.m_strQty );
}		

/**********************************************************************/

void CStocktakeImportDlg::AppendStockExceptions( CExceptionInfo& info, CStockReportLine& csvOut ) 
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( info.m_nStockIdx, StockRecord );
	CString strStockCode = StockRecord.GetStockCode();				

	CString strLineStockCode = ReportHelpers.GetDisplayStockCode( strStockCode );
	CString strLineDescription = StockRecord.GetDescription();
	CString strLineCategory = DataManager.Category.GetText( StockRecord.GetCategory() );

	csvOut.AppendString( strLineStockCode );
	csvOut.AppendString( strLineDescription );
	csvOut.AppendString( strLineCategory );
	csvOut.AppendString( "" );
	csvOut.AppendString( info.m_strQty );
}		

/**********************************************************************/

void CStocktakeImportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CArray<int,int> arrayItemsToProcess;
		CArray<int,int> arrayItemsToKeep;
		CArray<int,int> arrayExceptionsToDiscard;
		CArray<int,int> arrayExceptionsToKeep;
	
		for ( int nIndex = 0; nIndex < m_listStocktakes.GetItemCount(); nIndex++ )
		{
			if ( m_listStocktakes.GetCheck( nIndex ) != 0 )
				arrayItemsToProcess.Add( nIndex );
			else
				arrayItemsToKeep.Add( nIndex );
		}

		for ( int nIndex = 0; nIndex < m_listException.GetItemCount(); nIndex++ )
		{
			if ( m_listException.GetCheck( nIndex ) != 0 )
				arrayExceptionsToDiscard.Add( nIndex );
			else
				arrayExceptionsToKeep.Add( nIndex );
		}

		if ( arrayItemsToProcess.GetSize() > 0 )
			UpdateStockLevels( arrayItemsToProcess );

		if ( ( arrayItemsToKeep.GetSize() == 0 ) && ( arrayExceptionsToKeep.GetSize() == 0 ) )
			CFileRemover FileRemover( m_strFilename );
		else if ( ( arrayItemsToProcess.GetSize() != 0 ) || ( arrayExceptionsToDiscard.GetSize() != 0 ) )
			UpdatePendingFile( arrayItemsToKeep, arrayExceptionsToKeep );

		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CStocktakeImportDlg::UpdateStockLevels( CArray<int,int>& arrayItemsToProcess )
{
	int nType = 0;
	switch( m_comboAction.GetCurSel() )
	{
	case 1:		nType = SUBTRACT_STOCK_LEVELS;		break;
	case 2:		nType = OVERWRITE_STOCK_LEVELS;		break;
	case 0:
	default:	nType = ADD_STOCK_LEVELS;			break;
	}
	
	for ( int n = 0; n < arrayItemsToProcess.GetSize(); n++ )
	{
		int nSel = arrayItemsToProcess.GetAt(n);
		CStocktakeInfo info = m_arrayStocktakeInfo.GetAt(nSel);

		int nStocktakeNo = info.m_nStocktakeNo;

		int nStartIdx, nEndIdx;
		FindStocktakeItemInfo( nStocktakeNo, -1, nStartIdx );
		FindStocktakeItemInfo( nStocktakeNo + 1, -1, nEndIdx );
		nEndIdx--;

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CStocktakeItemInfo infoItem = m_arrayStocktakeItemInfo.GetAt(nIdx);

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( infoItem.m_nStockIdx, StockRecord );
			CString strStockCode = StockRecord.GetStockCode();

			CStockLevelsCSVRecord StockLevels;
			int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
			DataManager.Stock.GetStockpointTotals ( infoItem.m_nStockIdx, StockRecord, nSpNo, StockLevels );

			double dClosingQty = StockLevels.GetStocktakeClosingQty();

			CCSV csvChanges( infoItem.m_strQty );
	
			for ( int x = 0; x < csvChanges.GetSize(); x++ )
			{
				double dChange = csvChanges.GetDouble(x) * infoItem.m_dStockPerPlu;

				switch( nType )
				{
				case SUBTRACT_STOCK_LEVELS:		dClosingQty -= dChange;		break;
				case OVERWRITE_STOCK_LEVELS:	dClosingQty = dChange;		break;
				case ADD_STOCK_LEVELS:
				default:						dClosingQty += dChange;		break;
				}
			}

			StockLevels.SetStocktakeClosingQty( dClosingQty );
			DataManager.Stock.SetStockpointTotals ( infoItem.m_nStockIdx, StockRecord, nSpNo, StockLevels );
		}
	}

	CDataManagerInfo info;
	DataManager.WriteStockLevels( m_nSpIdx, info );
}

/**********************************************************************/

void CStocktakeImportDlg::UpdatePendingFile( CArray<int,int>& arrayItemsToKeep, CArray<int,int>& arrayExceptionsToKeep  )
{
	CSSFile filePending;
	if ( filePending.Open( m_strFilename, "wb" ) == FALSE )
		return;
	
	for ( int n = 0; n < arrayItemsToKeep.GetSize(); n++ )
	{
		int nSel = arrayItemsToKeep.GetAt(n);
		int nStocktakeNo = m_arrayStocktakeInfo[ nSel ].m_nStocktakeNo;

		int nStartIdx, nEndIdx;
		FindStocktakeItemInfo( nStocktakeNo, -1, nStartIdx );
		FindStocktakeItemInfo( nStocktakeNo + 1, -1, nEndIdx );
		nEndIdx--;

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CStocktakeItemInfo infoItem = m_arrayStocktakeItemInfo.GetAt(nIdx);

			CCSV csvChanges( infoItem.m_strQty );
		
			for ( int x = 0; x < csvChanges.GetSize(); x++ )
			{
				CCSV csvOut;
				csvOut.Add( infoItem.m_strNumber );
				csvOut.Add( csvChanges.GetString(x) );
				filePending.WriteLine( csvOut.GetLine() );
			}
		}
	}

	for ( int n = 0; n < arrayExceptionsToKeep.GetSize(); n++ )
	{
		int nSel = arrayExceptionsToKeep.GetAt(n);
		int nType = m_nExceptionType[ nSel ];

		int nStartIdx = -1;
		int nEndIdx = -1;

		switch( nType )
		{
		case 1:
			FindExceptionInfo( 1, nStartIdx );
			FindExceptionInfo( 5, nEndIdx );
			break;

		case 2:
			FindExceptionInfo( 5, nStartIdx );
			FindExceptionInfo( 7, nEndIdx );
			break;
		}

		if ( nStartIdx != -1 )
		{
			for ( int nIdx = nStartIdx; nIdx < nEndIdx; nIdx++ )
			{
				CExceptionInfo info = m_arrayExceptionInfo.GetAt(nIdx);

				CCSV csvOut;
				csvOut.Add( info.m_strNumber );
				csvOut.Add( info.m_strQty );
				filePending.WriteLine( csvOut.GetLine() );
			}
		}
	}
}

/**********************************************************************/
