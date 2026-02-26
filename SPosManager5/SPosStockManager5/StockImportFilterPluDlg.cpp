/**********************************************************************/
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditExceptions.h"
#include "StockAuditOpeningClosing.h"
#include "StockImportFilterListDlg.h"
/**********************************************************************/
#include "StockImportFilterPluDlg.h"
/**********************************************************************/

CStockImportFilterPluDlg::CStockImportFilterPluDlg( int nSpIdx, bool bClosing, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent)
	: CSSDialog(CStockImportFilterPluDlg::IDD, pParent), m_arrayImport( arrayImport )
{
	//{{AFX_DATA_INIT(CStockImportFilterPluDlg)
	//}}AFX_DATA_INIT

	m_nSpIdx = nSpIdx;
	m_bClosing = bClosing;
	m_nStocktakeNo = -1;
	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_nTotalExceptions = 0;
	m_nPluExplodeType = 0;
	m_nAction = 0;
	m_bMultiSheet = FALSE;

	m_strHeaderImportPath = szPath;
	m_strHeaderReference1 = "";
	m_strHeaderReference2 = "";
	m_strHeaderSuppName = "";
	m_strHeaderSpName1 = dbStockpoint.GetName( m_nSpIdx );
	m_strHeaderSpName2 = "";

	m_ImportDate = COleDateTime::GetCurrentTime();

	if ( TRUE == m_bClosing )
	{
		m_strTitle = "Import Closing Stock";
		m_strRadio1 = "Add imported levels to closing stock";
		m_strRadio2 = "Subtract imported levels from closing stock";
		m_strRadio3 = "Overwrite closing stock with imported levels";
		m_nImportFileReportType = AUDIT_IMPORT_PLU_CLOSING;
		m_nExceptionReportType = AUDIT_EX_CLOSING;
		m_nAuditReportType = AUDIT_CLOSING;
		m_nPluExplodeType = AUDIT_CLOSING;
	}
	else
	{
		m_strTitle = "Import Opening Stock";
		m_strRadio1 = "Add imported levels to opening stock";
		m_strRadio2 = "Subtract imported levels from opening stock";
		m_strRadio3 = "Overwrite opening stock with imported levels";
		m_nImportFileReportType = AUDIT_IMPORT_PLU_OPENING;
		m_nExceptionReportType = AUDIT_EX_OPENING;
		m_nAuditReportType = AUDIT_OPENING;
		m_nPluExplodeType = AUDIT_OPENING;
	}
}

/**********************************************************************/

CStockImportFilterPluDlg::~CStockImportFilterPluDlg()
{
}

/**********************************************************************/

void CStockImportFilterPluDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportFilterPluDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_LINES, m_staticLines);
	DDX_Control(pDX, IDC_STATIC_EXCEPTIONS, m_staticExceptions);
	DDX_Control(pDX, IDC_STATIC_SELECTED, m_staticSelected);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTIONS, m_buttonExceptions);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_buttonPreview);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_STATIC_ACTION, m_staticAction);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Radio(pDX, IDC_RADIO1, m_nAction);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportFilterPluDlg, CSSDialog)
	//{{AFX_MSG_MAP(CStockImportFilterPluDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_IMPORT, OnButtonImport )
	ON_BN_CLICKED( IDC_BUTTON_EXCEPTIONS, OnButtonExceptions )
	ON_BN_CLICKED( IDC_BUTTON_PREVIEW, OnButtonPreview )
	ON_BN_CLICKED( IDC_BUTTON_SELECT, OnButtonSelect )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportFilterPluDlg::OnInitDialog() 
{
	CSSDialog::OnInitDialog();

	SetWindowText( m_strTitle );

	CString strLines;
	strLines.Format( "%d", m_nLinesFound );
	m_staticLines.SetWindowText( strLines );

	CString strExceptions;
	strExceptions.Format( "%d", m_nTotalExceptions );
	m_staticExceptions.SetWindowText( strExceptions );

	m_radio1.SetWindowText( m_strRadio1 );
	m_radio2.SetWindowText( m_strRadio2 );
	m_radio3.SetWindowText( m_strRadio3 );

	if ( m_arrayImport.GetSize() == 0 )
	{
		m_buttonOK.ShowWindow( SW_HIDE );
		m_buttonOK.EnableWindow( FALSE );
		m_buttonPreview.ShowWindow( SW_HIDE );
		m_buttonPreview.EnableWindow( FALSE );
		m_buttonSelect.ShowWindow( SW_HIDE );
		m_buttonSelect.EnableWindow( FALSE );
		m_buttonCancel.SetWindowText( "Close" );
		m_radio1.EnableWindow( FALSE );
		m_radio2.EnableWindow( FALSE );
		m_radio3.EnableWindow( FALSE );
	}

	if ( 0 == m_nTotalExceptions )
	{
		m_buttonExceptions.ShowWindow( SW_HIDE );
		m_buttonExceptions.EnableWindow( FALSE );
	}

	UpdateSelectionCount();

	AdjustControls();

	return TRUE;  
}

/**********************************************************************/

void CStockImportFilterPluDlg::ProcessImportData()
{
	m_arrayImport.RemoveAll();
	StatusProgress.Lock( TRUE, "Processing import data" );
	ProcessImportDataInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockImportFilterPluDlg::UpdateSelectionCount()
{
	CString str;
	str.Format( "%d of %d",
		m_arrayImport.GetSelectionCount(),
		m_arrayImport.GetSize() );

	m_staticSelected.SetWindowText( str );
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnButtonImport()
{
	PreviewImportedData();
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnButtonExceptions()
{
	CStockAuditExceptions Audit( m_nExceptionReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strReference1 = m_strHeaderReference1;
	info.m_strReference2 = m_strHeaderReference2;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportFilterPluDlg::UpdateImportArray()
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	StatusProgress.Lock( TRUE, "Checking Current Stock Levels" );

	for (int n = 0; n < m_arrayImport.GetSize(); n++)
	{
		StatusProgress.SetPos(n, m_arrayImport.GetSize());

		CStockImportFilterItem item;
		m_arrayImport.GetItem(n, item);

		item.m_bSelectBuffer = item.m_bSelected;

		if (TRUE == m_bMultiSheet)
		{
			item.m_dCurrentSUQty = 0.0;
		}
		else
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt(item.m_nStockIdx, StockRecord);

			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals(item.m_nStockIdx, StockRecord, nSpNo, StockLevels);

			if (TRUE == m_bClosing)
			{
				item.m_dCurrentSUQty = StockLevels.GetStocktakeClosingQty();
			}
			else
			{
				item.m_dCurrentSUQty = StockLevels.GetOpeningQty();
			}
		}

		m_arrayImport.SetItem( n, item );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnButtonPreview() 
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	CSSFile fileAudit;
	CString strAuditFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV );
	if ( fileAudit.Open( strAuditFilename, "wb" ) == FALSE )
	{
		Prompter.Error( "Unable to create audit file" );
		return;
	}

	UpdateImportArray();
	
	for ( int nIndex = 0; nIndex < m_arrayImport.GetSize(); nIndex++ )
	{
		CStockImportFilterItem item;
		m_arrayImport.GetItem( nIndex, item );

		if (FALSE == item.m_bSelected)
		{
			continue;
		}

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( item.m_nStockIdx, StockRecord );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( m_nAuditReportType );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( m_nAction );
		csvOut.AddStockAuditQty ( item.m_dImportDUQty );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( item.GetNewStockLevel( m_nAction ) );
		fileAudit.WriteLine ( csvOut.GetLine() );		
	}

	fileAudit.Close();

	CStockAuditOpeningClosing Audit;
	Audit.SetReportType ( m_nAuditReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );

	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}	
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnButtonSelect() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		UpdateImportArray();

		m_arrayImport.ClearSelectionModifiedFlag();
		CStockImportFilterListDlg dlgList( m_nSpIdx, m_bClosing ? NODE_CLOSING : NODE_OPENING, m_nAction, m_arrayImport, this );
		
		if ( dlgList.DoModal() != IDOK )
		{
			for ( int n = 0; n < m_arrayImport.GetSize(); n++ )
			{
				CStockImportFilterItem item;
				m_arrayImport.GetItem( n, item );
				item.m_bSelected = item.m_bSelectBuffer;
				m_arrayImport.SetItem( n, item );
			}
		}

		UpdateSelectionCount();
	}
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/

void CStockImportFilterPluDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

