/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSQuickPluDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSQuickPluDlg, CDialog)
/**********************************************************************/

CPMSQuickPluDlg::CPMSQuickPluDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bTraining, CWnd* pParent)
	: CSSDialog(CPMSQuickPluDlg::IDD, pParent), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	m_nAction = PMS_QUICKPLU_CANCEL;
	m_bTraining = bTraining;

	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, m_infoRoom );
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, m_infoAccount );

	//ALL LINES DEFAULT TO ADHOC CHARGE WITH QTY
	for ( int n = 0; n < 10; n++ )
	{
		m_nPluIdx[n] = -1;
		m_nLineType[n] = QUICK_ACCESS_PLU_SALE_QTY;
		m_dQty[n] = 1.0;
		m_dValue[n] = 0.0;
	}

	int nLine = 0;
	m_nAllowances = 0;
	m_nCharges = 0;

	//ADD PREDEFINED ALLOWANCES
	for ( int n = 0; n < 10; n++ )
	{
		if ( PMSOptions.GetQuickSaleEnable(n) != 2 )
			continue;

		__int64 nPluNo = PMSOptions.GetQuickSalePlu(n);

		int nType = PMSOptions.GetQuickSaleType(n); 

		switch( nType )
		{
		case QUICK_ACCESS_PLU_ALLOW:
		case QUICK_ACCESS_PLU_ALLOW_QTY:
			break;

		default:
			continue;
		}

		if ( nPluNo != 0 )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				m_nPluIdx[ nLine ] = nPluIdx;
		}

		m_nLineType[ nLine ] = nType;
		nLine++;
		m_nAllowances++;
	}

	//ADD PREDEFINED CHARGES
	for ( int n = 0; n < 10; n++ )
	{
		if ( PMSOptions.GetQuickSaleEnable(n) != 2 )
			continue;

		__int64 nPluNo = PMSOptions.GetQuickSalePlu(n);

		int nType = PMSOptions.GetQuickSaleType(n); 

		switch( nType )
		{
		case QUICK_ACCESS_PLU_SALE:
		case QUICK_ACCESS_PLU_SALE_QTY:
			break;

		default:
			continue;
		}

		if ( nPluNo != 0 )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				m_nPluIdx[ nLine ] = nPluIdx;
		}

		m_nLineType[ nLine ] = nType;
		nLine++;
		m_nCharges++;
	}

	//SET DEFAULT LINE QUANTITIES
	for ( int n = 0; n < 10; n++ )
	{
		switch( m_nLineType[n] )
		{
		case QUICK_ACCESS_PLU_SALE_QTY:
		case QUICK_ACCESS_PLU_ALLOW_QTY:
			if ( m_nPluIdx[n] != -1 )
				m_dQty[n] = 0.0;
			break;
		}
	}

	m_nStaticTypeID[0] = IDC_STATIC_TYPE1;
	m_nStaticTypeID[1] = IDC_STATIC_TYPE2;
	
	m_nStaticQtyID[0] = IDC_STATIC_QTY1;
	m_nStaticQtyID[1] = IDC_STATIC_QTY2;
	
	m_nStaticValueID[0] = IDC_STATIC_VALUE1;
	m_nStaticValueID[1] = IDC_STATIC_VALUE2;
	
	m_nDescriptionID[0] = IDC_EDIT_DESCRIPTION1;
	m_nDescriptionID[1] = IDC_EDIT_DESCRIPTION2;
	m_nDescriptionID[2] = IDC_EDIT_DESCRIPTION3;
	m_nDescriptionID[3] = IDC_EDIT_DESCRIPTION4;
	m_nDescriptionID[4] = IDC_EDIT_DESCRIPTION5;
	m_nDescriptionID[5] = IDC_EDIT_DESCRIPTION6;
	m_nDescriptionID[6] = IDC_EDIT_DESCRIPTION7;
	m_nDescriptionID[7] = IDC_EDIT_DESCRIPTION8;
	m_nDescriptionID[8] = IDC_EDIT_DESCRIPTION9;
	m_nDescriptionID[9] = IDC_EDIT_DESCRIPTION10;
	
	m_nQtyID[0] = IDC_EDIT_QTY1;
	m_nQtyID[1] = IDC_EDIT_QTY2;
	m_nQtyID[2] = IDC_EDIT_QTY3;
	m_nQtyID[3] = IDC_EDIT_QTY4;
	m_nQtyID[4] = IDC_EDIT_QTY5;
	m_nQtyID[5] = IDC_EDIT_QTY6;
	m_nQtyID[6] = IDC_EDIT_QTY7;
	m_nQtyID[7] = IDC_EDIT_QTY8;
	m_nQtyID[8] = IDC_EDIT_QTY9;
	m_nQtyID[9] = IDC_EDIT_QTY10;
	
	m_nValueID[0] = IDC_EDIT_VALUE1;
	m_nValueID[1] = IDC_EDIT_VALUE2;
	m_nValueID[2] = IDC_EDIT_VALUE3;
	m_nValueID[3] = IDC_EDIT_VALUE4;
	m_nValueID[4] = IDC_EDIT_VALUE5;
	m_nValueID[5] = IDC_EDIT_VALUE6;
	m_nValueID[6] = IDC_EDIT_VALUE7;
	m_nValueID[7] = IDC_EDIT_VALUE8;
	m_nValueID[8] = IDC_EDIT_VALUE9;
	m_nValueID[9] = IDC_EDIT_VALUE10;
	
	m_nBrowseID[0] = IDC_BUTTON_BROWSE_PLU1;
	m_nBrowseID[1] = IDC_BUTTON_BROWSE_PLU2;
	m_nBrowseID[2] = IDC_BUTTON_BROWSE_PLU3;
	m_nBrowseID[3] = IDC_BUTTON_BROWSE_PLU4;
	m_nBrowseID[4] = IDC_BUTTON_BROWSE_PLU5;
	m_nBrowseID[5] = IDC_BUTTON_BROWSE_PLU6;
	m_nBrowseID[6] = IDC_BUTTON_BROWSE_PLU7;
	m_nBrowseID[7] = IDC_BUTTON_BROWSE_PLU8;
	m_nBrowseID[8] = IDC_BUTTON_BROWSE_PLU9;
	m_nBrowseID[9] = IDC_BUTTON_BROWSE_PLU10;
}

/**********************************************************************/

CPMSQuickPluDlg::~CPMSQuickPluDlg()
{
}

/**********************************************************************/

void CPMSQuickPluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for ( int n = 0; n < 2; n++ )
	{
		DDX_Control(pDX, m_nStaticTypeID[n], m_StaticType[n] );
		DDX_Control(pDX, m_nStaticQtyID[n], m_StaticQty[n] );
		DDX_Control(pDX, m_nStaticValueID[n], m_StaticValue[n] );
	}

	for ( int n = 0; n < 10; n++ )
	{
		DDX_Text(pDX, m_nQtyID[n], m_dQty[n] );
		DDX_Text(pDX, m_nValueID[n], m_dValue[n] );
		DDX_Control(pDX, m_nDescriptionID[n], m_editDescription[n] );
		DDX_Control(pDX, m_nBrowseID[n], m_buttonBrowse[n] );
	}

	DDX_Control(pDX, IDOK, m_buttonOK );
	DDX_Control(pDX, IDCANCEL, m_buttonCancel );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSQuickPluDlg, CDialog)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY1, OnKillFocusQty1)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY2, OnKillFocusQty2)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY3, OnKillFocusQty3)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY4, OnKillFocusQty4)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY5, OnKillFocusQty5)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY6, OnKillFocusQty6)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY7, OnKillFocusQty7)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY8, OnKillFocusQty8)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY9, OnKillFocusQty9)
	ON_EN_KILLFOCUS( IDC_EDIT_QTY10, OnKillFocusQty10)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU1, OnButtonBrowse1)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU2, OnButtonBrowse2)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU3, OnButtonBrowse3)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU4, OnButtonBrowse4)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU5, OnButtonBrowse5)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU6, OnButtonBrowse6)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU7, OnButtonBrowse7)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU8, OnButtonBrowse8)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU9, OnButtonBrowse9)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE_PLU10, OnButtonBrowse10)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPMSQuickPluDlg::GetEditQty( int n ){ return GetEdit( m_nQtyID[n] ); }
CEdit* CPMSQuickPluDlg::GetEditValue( int n ){ return GetEdit( m_nValueID[n] ); }
/**********************************************************************/

BOOL CPMSQuickPluDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strRoomText;
	::GetPMSRoomText( m_infoRoom.GetRoomNo(), m_infoRoom.GetRoomName(), strRoomText );

	CString strTitle;
	strTitle.Format ( "Manual Sales (%s, %s)",
		(const char*) strRoomText,
		 m_infoAccount.GetGuestName() );

	if ( TRUE == m_bTraining )
		strTitle += " (Training Mode)";

	SetWindowText( strTitle );

	for ( int n = 0; n < 10; n++ )
	{
		SubclassEdit ( m_nQtyID[n], SS_NUM_SDP, 8, "%.5f" );
		SubclassEdit ( m_nValueID[n], SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	}

	//SET HEADER TEXTS
	if ( m_nAllowances != 0 )
		m_StaticType[0].SetWindowText( "Allowance Type" );
	else
		m_StaticType[0].SetWindowText( "Charge Type" );

	//LOAD PREDEFINED PLU ITEMS
	for ( int n = 0; n < 10; n++ )
	{
		switch( m_nLineType[n] )
		{
		case QUICK_ACCESS_PLU_SALE:
		case QUICK_ACCESS_PLU_SALE_QTY:
		case QUICK_ACCESS_PLU_ALLOW:
		case QUICK_ACCESS_PLU_ALLOW_QTY:
			{
				CPluCSVRecord Plu;
				int nPluIdx = m_nPluIdx[n];

				if ( nPluIdx != -1 )
				{
					DataManager.Plu.GetAt( nPluIdx, Plu );
					m_editDescription[ n ].SetWindowText( Plu.GetReportText() );
				}
				else
				{
					GetEditQty(n) -> EnableWindow( FALSE );
					GetEditValue(n) -> EnableWindow( FALSE );
				}
			}
			break;
		}
	}

	//DISABLE QTY FIELDS FOR VALUE ONLY LINES
	for ( int n = 0; n < 10; n++ )
	{
		switch( m_nLineType[n] )
		{
		case QUICK_ACCESS_PLU_SALE_QTY:
		case QUICK_ACCESS_PLU_ALLOW_QTY:
			break;

		default:
			GetEditQty(n) -> EnableWindow( FALSE );
			break;
		}
	}

	//MOVE CONTROLS TO MAKE SPACE FOR SECOND HEADER IF REQUIRED
	if ( ( m_nAllowances != 0 ) && ( m_nCharges != 0 ) )
	{
		int nYPos = 34 + ( 20 * m_nAllowances );
		MoveControl( &m_StaticType[1], 11, nYPos );
		MoveControl( &m_StaticQty[1], 147, nYPos );
		MoveControl( &m_StaticValue[1], 195, nYPos );
		
		for ( int x = m_nAllowances; x < 10; x++ )
		{
			int nYPos = 48 + ( 20 * x );
			MoveControl( &m_editDescription[x], 9, nYPos );
			MoveControl( GetEditQty(x), 147, nYPos );
			MoveControl( GetEditValue(x), 195, nYPos );
			MoveControl( &m_buttonBrowse[x], 241, nYPos );
		}
	}
	else
	{
		m_StaticType[1].ShowWindow( FALSE );
		m_StaticQty[1].ShowWindow( FALSE );
		m_StaticValue[1].ShowWindow( FALSE );
	}

	for ( int n = m_nAllowances + m_nCharges; n < 10; n++ )
	{
		ShowAndEnableWindow( &m_editDescription[n], FALSE );
		ShowAndEnableWindow( GetEditQty(n), FALSE );
		ShowAndEnableWindow( GetEditValue(n), FALSE );
		ShowAndEnableWindow( &m_buttonBrowse[n], FALSE );
	}

	{
		int nLines = m_nAllowances + m_nCharges;
		if ( ( m_nAllowances != 0 ) && ( m_nCharges != 0 ) )
			nLines++;

		int nYPos = 44 + ( 20 * nLines );
		MoveControl( &m_buttonOK, 200, nYPos );
		MoveControl( &m_buttonCancel, 256, nYPos );
		ResizeDialog( 313, nYPos + 21 );
	}

	return TRUE;  
}

/**********************************************************************/
void CPMSQuickPluDlg::OnKillFocusQty1(){ KillFocusQty(0); }
void CPMSQuickPluDlg::OnKillFocusQty2(){ KillFocusQty(1); }
void CPMSQuickPluDlg::OnKillFocusQty3(){ KillFocusQty(2); }
void CPMSQuickPluDlg::OnKillFocusQty4(){ KillFocusQty(3); }
void CPMSQuickPluDlg::OnKillFocusQty5(){ KillFocusQty(4); }
void CPMSQuickPluDlg::OnKillFocusQty6(){ KillFocusQty(5); }
void CPMSQuickPluDlg::OnKillFocusQty7(){ KillFocusQty(6); }
void CPMSQuickPluDlg::OnKillFocusQty8(){ KillFocusQty(7); }
void CPMSQuickPluDlg::OnKillFocusQty9(){ KillFocusQty(8); }
void CPMSQuickPluDlg::OnKillFocusQty10(){ KillFocusQty(9); }
/**********************************************************************/

void CPMSQuickPluDlg::KillFocusQty( int n ) 
{
	if ( m_nPluIdx[n] != -1 )
	{
		UpdateData( TRUE, FALSE );
	
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( m_nPluIdx[n], PluRecord );
		
		m_dValue[n] = m_dQty[n] * PluRecord.GetPrice( 0 );

		UpdateData ( FALSE );
		GetEditValue(n) -> SetSel ( 0, -1 );
	}
}

/**********************************************************************/
void CPMSQuickPluDlg::OnButtonBrowse1(){ BrowsePlu(0); }
void CPMSQuickPluDlg::OnButtonBrowse2(){ BrowsePlu(1); }
void CPMSQuickPluDlg::OnButtonBrowse3(){ BrowsePlu(2); }
void CPMSQuickPluDlg::OnButtonBrowse4(){ BrowsePlu(3); }
void CPMSQuickPluDlg::OnButtonBrowse5(){ BrowsePlu(4); }
void CPMSQuickPluDlg::OnButtonBrowse6(){ BrowsePlu(5); }
void CPMSQuickPluDlg::OnButtonBrowse7(){ BrowsePlu(6); }
void CPMSQuickPluDlg::OnButtonBrowse8(){ BrowsePlu(7); }
void CPMSQuickPluDlg::OnButtonBrowse9(){ BrowsePlu(8); }
void CPMSQuickPluDlg::OnButtonBrowse10(){ BrowsePlu(9); }
/**********************************************************************/

void CPMSQuickPluDlg::BrowsePlu( int n )
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;

	m_nPluIdx[n] = nPluIdx;

	switch( m_nLineType[n] )
	{
	case QUICK_ACCESS_PLU_SALE:
		m_nLineType[n] = QUICK_ACCESS_PLU_SALE_QTY;
		break;

	case QUICK_ACCESS_PLU_ALLOW:
		m_nLineType[n] = QUICK_ACCESS_PLU_ALLOW_QTY;
		break;
	}
		
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	
	m_editDescription[n].SetWindowTextA( PluRecord.GetReportText() );
	
	GetEditValue(n) -> EnableWindow( TRUE );

	GetEditQty(n) -> EnableWindow( TRUE );
	GetEditQty(n) -> SetFocus();
	GetEditQty(n) -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPMSQuickPluDlg::GetPMSInvoiceSales( CPMSInvoiceArray& arraySales )
{
	CPMSInvoiceLineInfo infoSale;
	infoSale.m_dateSale = COleDateTime::GetCurrentTime();
	infoSale.m_nLineType = PMS_LINE_TRAN;
	infoSale.m_nTransactionNo = 0;

	arraySales.Reset();

	for ( int n = 0; n < 10; n++ )
	{
		bool bAccept = FALSE;

		if ( m_nPluIdx[n] != -1 )
		{
			switch( m_nLineType[n] )
			{
			case QUICK_ACCESS_PLU_SALE_QTY:
			case QUICK_ACCESS_PLU_ALLOW_QTY:
				bAccept = ( CPriceHelpers::CompareDoubles( m_dQty[n], 0.0, 3 ) != 0 );
				break;

			default:
				bAccept = ( CPriceHelpers::CompareDoubles( m_dValue[n], 0.0, 3 ) != 0 );
				break;
			}
		}

		if ( TRUE == bAccept )
		{
			CPluCSVRecord Plu;
			int nPluIdx = m_nPluIdx[n];
			DataManager.Plu.GetAt( nPluIdx, Plu );

			int nGroupNo = 0;

			int nDeptIdx;
			int nDeptNo = Plu.GetBaseDeptNo();
			if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord Dept;
				DataManager.Department.GetAt( nDeptIdx, Dept );
				nGroupNo = Dept.GetEposGroup();
			}

			if ( arraySales.GetInvoiceItemCount() == 0 )
				arraySales.AddInvoiceItem( infoSale );

			switch( m_nLineType[n] )
			{
			case QUICK_ACCESS_PLU_ALLOW:
			case QUICK_ACCESS_PLU_ALLOW_QTY:
				infoSale.m_nLineType = PMS_LINE_ALLOWANCE;
				infoSale.m_dValue = 0.0;
				infoSale.m_dAllowance = m_dValue[n];
				break;

			case QUICK_ACCESS_PLU_SALE:
			case QUICK_ACCESS_PLU_SALE_QTY:
			default:
				infoSale.m_nLineType = PMS_LINE_SALE;
				infoSale.m_dValue = m_dValue[n];
				infoSale.m_dAllowance = 0.0;
				break;
			}
			
			infoSale.m_nBasePluNo = Plu.GetPluNoInt();
			infoSale.m_strDescription = Plu.GetReportText();
			infoSale.m_nDeptNo = nDeptNo;
			infoSale.m_nGroupNo = nGroupNo;
			infoSale.m_nModifier = 0;
			infoSale.m_nLevel = 0;
			infoSale.m_dEposPluQty = m_dQty[n];
			infoSale.m_dBasePluQty = m_dQty[n];
			infoSale.m_dDiscount = 0.0;
			infoSale.m_nTaxBand = Plu.GetBaseTaxBandInt();

			arraySales.AddInvoiceItem( infoSale );
		}
	}
}

/**********************************************************************/

void CPMSQuickPluDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_nAction = PMS_QUICKPLU_EDIT;
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSQuickPluDlg::OnCancel()
{
	m_nAction = PMS_QUICKPLU_CANCEL;
	EndDialog( IDOK );
}

/**********************************************************************/
#endif
/**********************************************************************/
