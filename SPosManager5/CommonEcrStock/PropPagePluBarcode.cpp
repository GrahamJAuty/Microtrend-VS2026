 /**********************************************************************/
#include "BarcodeNumberDlg.h"
/**********************************************************************/
#include "PropPagePluBarcode.h"
/**********************************************************************/

CPropPagePluBarcode::CPropPagePluBarcode() : CSSPropertyPage(CPropPagePluBarcode::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePluBarcode)
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;
	m_bReadOnly = FALSE;
}

/**********************************************************************/

CPropPagePluBarcode::~CPropPagePluBarcode()
{
}

/**********************************************************************/

void CPropPagePluBarcode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluBarcode)
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluBarcode, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluBarcode)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluBarcode::OnInitDialog() 
{
	//MUST DO THIS BEFORE CALLING GET RECORD DATA 
	//FOR THE FIRST TIME IN ANY SESSION
	DataManager.Barcode.CreatePluBarcodeList();
	
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	m_listBarcodes.SubclassDlgItem ( IDC_LIST, this );
	m_listBarcodes.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listBarcodes.InsertColumn ( 1, "Barcode", LVCFMT_LEFT, 150 );

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
	{
		m_listBarcodes.InsertColumn ( 2, "Modifier", LVCFMT_LEFT, 220 );
		m_listBarcodes.InsertColumn ( 3, "Printable", LVCFMT_LEFT, 160 );
		ResizeControl( &m_listBarcodes, 420, 435 );
		MoveControl( &m_buttonAdd, 440, 7 );
		MoveControl( &m_buttonRemove, 440, 43 );
	}
	else
	{
		m_listBarcodes.InsertColumn ( 2, "Printable", LVCFMT_LEFT, 160 );
	}

	m_listBarcodes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	SetRecordControls();
	AddList(0);
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluBarcode::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listBarcodes.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePluBarcode::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluBarcode::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePluBarcode::GetRecordData()
{
	DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
}

/**********************************************************************/

void CPropPagePluBarcode::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;
}

/**********************************************************************/

bool CPropPagePluBarcode::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluBarcode::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluBarcode::SetRecordControls()
{
	if ( m_bReadOnly == TRUE )
	{
		m_buttonAdd.ShowWindow( SW_HIDE );
		m_buttonRemove.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/

void CPropPagePluBarcode::AddList( int nTargetBarcodeIdx )
{
	m_listBarcodes.DeleteAllItems();
	for ( int nIndex = 0; nIndex < m_BarcodeIdxArray.GetSize(); nIndex++ )
	{
		CBarcodeCSVRecord BarcodeRecord;
		int nBarcodeIdx = m_BarcodeIdxArray.GetAt( nIndex );
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		CString strNum;
		strNum.Format ( "%3.3d", nIndex + 1 );

		CString strMod = "";
		{
			int nMod = BarcodeRecord.GetModifier();
			if ( nMod != 0 )
				strMod = DataManager.Modifier.GetDisplayName( nMod );
		}
		
		CCSV csvOut ( '\t' );
		csvOut.Add ( strNum );
		csvOut.Add ( BarcodeRecord.GetBarcodeNoString() );

		if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
			csvOut.Add ( strMod );

		csvOut.Add ( BarcodeRecord.GetPrintFlagText() );
		
		m_listBarcodes.AddString ( csvOut.GetLine() );

		if ( nBarcodeIdx == nTargetBarcodeIdx )
			m_listBarcodes.SetCurSel( nIndex );
	}
}

/**********************************************************************/

void CPropPagePluBarcode::OnButtonAdd() 
{
	CBarcodeCSVRecord BarcodeRecord;
	BarcodeRecord.SetPluNo ( m_pPluRecord -> GetPluNoInt() );
	BarcodeRecord.SetBarcodeNo ( 0 );
	
	CBarcodeNumberDlg dlg ( BARCODE_PLUDB_ADD, BarcodeRecord, this );
	if ( dlg.DoModal() != IDOK )
		return;

	__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();

	int nBarcodeIdx;
	if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
	{
		m_pPluChangesBuffer -> LogBarcodeChange( BarcodeRecord.GetBarcodeNoString() );
		DataManager.Barcode.InsertRecord( nBarcodeIdx, BarcodeRecord );
		DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
		AddList( nBarcodeIdx );
	}
	else
	{
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
		CString strPluNo1 = BarcodeRecord.GetPluNoString();
		CString strPluNo2 = m_pPluRecord -> GetPluNoString();

		if ( strPluNo1 == strPluNo2 )
		{
			CString strMsg;
			strMsg.Format ( "Barcode %s is already linked to this Plu",
				BarcodeRecord.GetBarcodeNoString() );

			Prompter.Info ( strMsg );
			return;
		}
		else
		{
			int nPluIdx;
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
			{
				CString strMsg;
				strMsg.Format ( "Barcode %s is already linked to Plu %s\n\nWould you like to link it to this Plu instead",
					BarcodeRecord.GetBarcodeNoString(),
					BarcodeRecord.GetPluNoString() );

				if ( Prompter.YesNo ( strMsg ) != IDYES )
					return;
			}
			
			//REMOVE AND REINSERT THE RECORD TO UPDATE THE
			//INTERNAL PLU TO BARCODE TABLE 
			m_pPluChangesBuffer -> LogBarcodeChange( BarcodeRecord.GetBarcodeNoString() );
			DataManager.Barcode.RemoveRecord( nBarcodeIdx );
			BarcodeRecord.SetPluNo( _atoi64( strPluNo2 ) );
			BarcodeRecord.SetPrintShelfFlag( TRUE );
			BarcodeRecord.SetPrintProductFlag( TRUE );
			DataManager.Barcode.InsertRecord( nBarcodeIdx, BarcodeRecord );
			DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
			AddList( nBarcodeIdx );
		}
	}
}

/**********************************************************************/

void CPropPagePluBarcode::OnButtonRemove() 
{
	int nSel = m_listBarcodes.GetCurSel();
	int nListSize = m_BarcodeIdxArray.GetSize();
	if ( ( nSel >= 0 ) && ( nSel < nListSize ) )
	{
		int nBarcodeIdx = m_BarcodeIdxArray.GetAt( nSel );
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

		CString strMsg;
		strMsg.Format ( "Are you sure you wish to delete barcode no %s", BarcodeRecord.GetBarcodeNoString() );
	
		if ( Prompter.YesNo( strMsg ) == IDYES )
		{
			m_pPluChangesBuffer -> LogBarcodeChange ( BarcodeRecord.GetBarcodeNoString() );
			DataManager.Barcode.RemoveRecord( nBarcodeIdx );
			DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
	
			nListSize = m_BarcodeIdxArray.GetSize();
	
			AddList( -1 );

			if ( nSel < nListSize )
				m_listBarcodes.SetCurSel( nSel );
			else if ( nListSize > 0 )
				m_listBarcodes.SetCurSel ( nListSize - 1 );
		}
	}
}

/**********************************************************************/
