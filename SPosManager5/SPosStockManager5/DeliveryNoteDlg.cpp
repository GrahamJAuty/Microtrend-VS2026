/**********************************************************************/
#include "AdhocDeliveryDatabaseDlg.h"
#include "AdjustmentsDatabasePluDlg.h"
#include "AdjustmentsDatabaseStockDlg.h"
#include "Consolidation.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "ReportConsolidationArray.h"
#include "ReturnsDatabaseDlg.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "DeliveryNoteDlg.h"
/**********************************************************************/

CDeliveryNoteDlg::CDeliveryNoteDlg( int nSpIdx, int nType, CStocktakeCSVRecord* pStocktake, CWnd* pParent)
	: CDialog(CDeliveryNoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeliveryNoteDlg)
	//}}AFX_DATA_INIT

	m_pStocktake = pStocktake;

	m_nSpIdx = nSpIdx;
	m_nType = nType;
	m_strReference1 = "";
	m_strReference2 = "";
	m_nSession = -1;

	switch ( m_nType )
	{
	case NODE_DELIVERY:
		m_bAutoNum = StockOptions.GetAutoNumAdhocFlag();

		if ( m_bAutoNum == TRUE )
			m_strReference1.Format ( "%s%6.6d", 
				StockOptions.GetAutoNumAdhocPrefix(),
				StockOptions.GetAutoNumAdhoc() + 1 );
		break;

	case NODE_RETURN:
		m_bAutoNum = StockOptions.GetAutoNumReturnFlag();

		if ( m_bAutoNum == TRUE )
			m_strReference1.Format ( "%s%6.6d", 
				StockOptions.GetAutoNumReturnPrefix(),
				StockOptions.GetAutoNumReturn() + 1);
		break;

	case NODE_ADJUSTMENT_STOCK:
	case NODE_ADJUSTMENT_PLU:
	default:
		m_bAutoNum = StockOptions.GetAutoNumAdjustFlag();

		if ( m_bAutoNum == TRUE )
			m_strReference1.Format ( "%s%6.6d", 
				StockOptions.GetAutoNumAdjustPrefix(),
				StockOptions.GetAutoNumAdjust() + 1);
		break;
	}

	m_strNewReference1 = m_strReference1;
	m_strNewReference2 = m_strReference2;
}

/**********************************************************************/

void CDeliveryNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeliveryNoteDlg)
	DDX_Control(pDX, IDC_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC_EDIT_REFERENCE1, m_editReference1);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_REFERENCE1, m_strReference1);
	DDV_MaxChars(pDX, m_strReference1, MAX_LENGTH_GENERAL_REFERENCE);
	DDX_Text(pDX, IDC_EDIT_REFERENCE2, m_strReference2);
	DDV_MaxChars(pDX, m_strReference2, MAX_LENGTH_GENERAL_REFERENCE);
	 
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDeliveryNoteDlg, CDialog)
	//{{AFX_MSG_MAP(CDeliveryNoteDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_SESSION, &CDeliveryNoteDlg::OnSelectSession)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDeliveryNoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
			
	CString strTitle;

	if ( NULL == m_pStocktake )
	{
		FillSessionCombo();
		OnSelectSession();
	}
	else
	{
		m_comboSession.EnableWindow( FALSE );
		m_comboSession.ShowWindow( SW_HIDE );
	}

	switch( m_nType )
	{
	case NODE_DELIVERY:
		strTitle.Format ( "Adhoc Delivery (%s)", dbStockpoint.GetName( m_nSpIdx ) );
		break;

	case NODE_RETURN:
		strTitle.Format ( "Returns (%s)", dbStockpoint.GetName( m_nSpIdx ) );
		break;

	case NODE_ADJUSTMENT_STOCK:
	case NODE_ADJUSTMENT_PLU:
	default:
		strTitle.Format ( "Adjustments (%s)", dbStockpoint.GetName( m_nSpIdx ) );
		m_staticSupplier.ShowWindow( SW_HIDE );
		m_comboSupplier.ShowWindow( SW_HIDE );
		break;
	}

	SetWindowText ( strTitle );
		
	if ( m_bAutoNum == TRUE )
		m_editReference1.EnableWindow ( FALSE );

	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
	{
		int nPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nSuppIdx ) );
		m_comboSupplier.SetItemData ( nPos, nSuppIdx + 1);
	}

	m_comboSupplier.InsertString ( 0, "Unspecified" );
	m_comboSupplier.SetItemData ( 0, 0 );

	m_comboSupplier.SetCurSel ( 0 );

	if ( m_comboSupplier.GetCount() < 2 )
		m_comboSupplier.EnableWindow ( FALSE );

	return TRUE;	
}

/**********************************************************************/

void CDeliveryNoteDlg::FillSessionCombo()
{
	m_comboSession.ResetContent();
	m_comboSession.AddString( "New Session" );

	for ( int n = 0; n < m_arraySessions.GetSize(); n++ )
		m_comboSession.AddString( m_arraySessions[n].m_strComboText );

	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);
}

/**********************************************************************/

void CDeliveryNoteDlg::CreateSessionList()
{
	m_arraySessions.RemoveAll();

	CString strPath = "";
	
	switch( m_nType )
	{
	case NODE_DELIVERY:
	case NODE_RETURN:
	case NODE_ADJUSTMENT_STOCK:
	case NODE_ADJUSTMENT_PLU:
		strPath = dbStockpoint.GetFolderPathSession( m_nSpIdx, m_nType );
		strPath += "\\";
		break;
	}

	if ( strPath != "" )
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile ( strPath + "*.*" );

		CReportConsolidationArray<CSortedStringItem> arrayFiles;

		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();

			if ( strFilename.GetLength() != 15 )
				continue;

			if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
				continue;

			if ( ::TestNumeric( strFilename.Right(6) ) == FALSE )
				continue;

			CSortedStringItem item;
			item.m_strItem = strFilename;
			arrayFiles.Consolidate( item );
		}

		for ( int n = arrayFiles.GetSize() - 1; n >= 0; n-- )
		{
			CSortedStringItem item;
			arrayFiles.GetAt( n, item );

			CString strFilename = item.m_strItem;

			int nYear = atoi( strFilename.Left(4) );
			int nMonth = atoi( strFilename.Mid(4,2) );
			int nDay = atoi( strFilename.Mid(6,2) );

			int nHour = atoi( strFilename.Mid(9,2) );
			int nMinute = atoi( strFilename.Mid(11,2) );
			int nSecond = atoi( strFilename.Right(2) );

			COleDateTime time = COleDateTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
			if ( time.m_status != COleDateTime::valid )
				continue;

			CSSFile fileSession;
			if ( fileSession.Open( strPath + strFilename, "rb" ) == FALSE )
				continue;

			CString strBuffer;
			fileSession.ReadString( strBuffer );
			CCSV csv( strBuffer );

			if ( csv.GetInt(0) != 1 )
				continue;

			CSavedSessionInfo info;
			info.m_nSuppNo = csv.GetInt(1);
			info.m_strReference1 = csv.GetString(2);
			info.m_strReference2 = csv.GetString(3);
			info.m_strFilename = strFilename;
			
			CString strSupplier = "";

			switch( m_nType )
			{
			case NODE_DELIVERY:
			case NODE_RETURN:
				{
					if ( 0 != info.m_nSuppNo )
					{
						int nSuppIdx;
						if ( DataManager.Supplier.FindSupplierByNumber( info.m_nSuppNo, nSuppIdx ) == TRUE )
							strSupplier.Format ( ", %s", DataManager.Supplier.GetName( nSuppIdx ) );
					}
				}
				break;
			}

			CString strReference = "";
			::FormatAuditReference( info.m_strReference1, info.m_strReference2, strReference );

			CString str;
			str.Format( "%2.2d/%2.2d/%4.4d at %2.2d:%2.2d (%s%s)",
				nDay,
				nMonth,
				nYear,
				nHour,
				nMinute,
				(const char*) strReference,
				(const char*) strSupplier );

			info.m_strComboText = str;
			m_arraySessions.Add( info );
		}
	}
}

/**********************************************************************/

void CDeliveryNoteDlg::OnSelectSession()
{
	int nSession = m_comboSession.GetCurSel();
	if ( m_nSession == nSession )
		return;

	if ( 0 == nSession )
	{
		m_strReference1 = m_strNewReference1;
		m_strReference2 = m_strNewReference2;
		UpdateData( FALSE );
		m_comboSupplier.SetCurSel(0);
	}
	else
	{
		m_strReference1 = m_arraySessions[nSession - 1].m_strReference1;
		m_strReference2 = m_arraySessions[nSession - 1].m_strReference2;

		UpdateData( FALSE );

		switch( m_nType )
		{
		case NODE_DELIVERY:
		case NODE_RETURN:
			{
				int nSuppIdx = 0;
				int nSuppNo = m_arraySessions[nSession - 1].m_nSuppNo;
				
				if ( nSuppNo != 0 )
				{
					if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == TRUE )
						nSuppIdx++;
					else
						nSuppIdx = 0;
				}

				m_comboSupplier.SetCurSel(0);
				for ( int n = 1; n < m_comboSupplier.GetCount(); n++ )
				{
					if ( m_comboSupplier.GetItemData(n) == nSuppIdx )
					{
						m_comboSupplier.SetCurSel(n);
						break;
					}
				}
			}
			break;
		}
	}

	m_nSession = nSession;
}

/**********************************************************************/

void CDeliveryNoteDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CString strSupplier;
	m_comboSupplier.GetLBText ( m_comboSupplier.GetCurSel(), strSupplier );
	int nSuppIdx = m_comboSupplier.GetItemData ( m_comboSupplier.GetCurSel() ) - 1;

	int nSel = m_comboSession.GetCurSel();
	CString strPreviousSession = "";
	if ( ( nSel >= 1 ) && ( nSel <= m_arraySessions.GetSize() ) )
		strPreviousSession = m_arraySessions[nSel - 1].m_strFilename;
	
	switch ( m_nType )
	{
	case NODE_DELIVERY:
		{	
			CString strAutoImportPluFile = "";
			CString strAutoImportWrapperFile = "";
			
			if ( strPreviousSession == "" )
			{
				CString strFilename;
				strFilename.Format( "%s\\auto_delivery_plu.dat",
					dbStockpoint.GetFolderPath( m_nSpIdx ) );

				if ( ::FileExists( strFilename ) == TRUE )
					strAutoImportPluFile = strFilename;
				else
				{
					strFilename.Format( "%s\\auto_delivery_wrapper.dat",
					dbStockpoint.GetFolderPath( m_nSpIdx ) );

					if ( ::FileExists( strFilename ) == TRUE )
						strAutoImportWrapperFile = strFilename;
				}
			}
			
			DataManager.StockPlu.Invalidate();
			CAdhocDeliveryDatabaseDlg dlg ( m_strReference1, m_strReference2, nSuppIdx, m_nSpIdx, m_pStocktake, this );
			dlg.SetPreviousSession( strPreviousSession );
			dlg.SetAutoImportPluFile( strAutoImportPluFile );
			dlg.SetAutoImportWrapperFile( strAutoImportWrapperFile );
			dlg.DoModal();
		}
		break;

	case NODE_RETURN:
		{
			CString strAutoImportFile = "";
			
			if ( strPreviousSession == "" )
			{
				CString strFilename;
				strFilename.Format( "%s\\auto_returns_plu.dat",
					dbStockpoint.GetFolderPath( m_nSpIdx ) );

				if ( ::FileExists( strFilename ) == TRUE )
					strAutoImportFile = strFilename;
			}
		
			CReturnsDatabaseDlg dlg ( m_strReference1, m_strReference2, nSuppIdx, m_nSpIdx, m_pStocktake, this );
			dlg.SetPreviousSession( strPreviousSession );
			dlg.SetAutoImportFile( strAutoImportFile );
			dlg.DoModal();
		}
		break;

	case NODE_ADJUSTMENT_STOCK:
		{
			CString strAutoImportFile = "";
			
			if ( strPreviousSession == "" )
			{
				CString strFilename;
				strFilename.Format( "%s\\auto_adjust_plu.dat",
					dbStockpoint.GetFolderPath( m_nSpIdx ) );

				if ( ::FileExists( strFilename ) == TRUE )
					strAutoImportFile = strFilename;
			}

			CAdjustmentsDatabaseStockDlg dlg ( m_strReference1, m_strReference2, m_nSpIdx, m_pStocktake, this );
			dlg.SetPreviousSession( strPreviousSession );
			dlg.SetAutoImportFile( strAutoImportFile );
			dlg.DoModal();
		}
		break;

	case NODE_ADJUSTMENT_PLU:
	default:
		{
			CString strAutoImportFile = "";
			
			if ( strPreviousSession == "" )
			{
				CString strFilename;
				strFilename.Format( "%s\\auto_adjust_plu.dat",
					dbStockpoint.GetFolderPath( m_nSpIdx ) );

				if ( ::FileExists( strFilename ) == TRUE )
					strAutoImportFile = strFilename;
			}

			CAdjustmentsDatabasePluDlg dlg ( m_strReference1, m_strReference2, m_nSpIdx, m_pStocktake, this );
			dlg.SetPreviousSession( strPreviousSession );
			dlg.SetAutoImportFile( strAutoImportFile );
			dlg.DoModal();
		}
		break;
	}
		
	EndDialog ( IDOK );
}

/**********************************************************************/

