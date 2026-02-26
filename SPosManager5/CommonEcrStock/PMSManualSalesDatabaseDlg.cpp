/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "Password.h"
#include "PMSManualDiscountEditDlg.h"
#include "PMSManualPaymentEditDlg.h"
#include "PMSManualSalesCloseDlg.h"
#include "PMSOptions.h"
#include "PMSQuickPluDlg.h"
#include "PMSRoomStatusTable.h"
#include "ReportHelpers.h"
#include "SysInfo.h"
#include "WMNumbers.h"
#include "..\SPosEcrManager5\ZSalesImporterEcrman.h"
#include "..\SPosStockManager5\ZSalesImporterStockman.h"
/**********************************************************************/
#include "PMSManualSalesDatabaseDlg.h"
/**********************************************************************/

CPMSManualSalesDatabaseDlg::CPMSManualSalesDatabaseDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, int nActionType, bool bTraining, CWnd* pParent )
	: CDialog(CPMSManualSalesDatabaseDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CPMSManualSalesDatabaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nRoomGridRoomIdx = nRoomGridRoomIdx;
	m_nRoomGridAccountIdx = nRoomGridAccountIdx;

	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, m_infoRoom );
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, m_infoAccount );

	m_nActionType = nActionType;
	m_bTraining = bTraining;

	m_nCacheIndex = -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	CString strRoomText;
	::GetPMSRoomText( m_infoRoom.GetRoomNo(), m_infoRoom.GetRoomName(), strRoomText );

	CString strType = "";
	switch( m_nActionType )
	{
	case PMS_MANUAL_PAYMENT:	strType = "Payments";		break;
	case PMS_MANUAL_DEPOSIT:	strType = "Deposits";		break;
	case PMS_MANUAL_SALES:
	default:					strType = "Sales";			break;
	}

	m_strTitle.Format ( "Edit Guest %s (%s, %s)",
		(const char*) strType,
		(const char*) strRoomText,
		m_infoAccount.GetGuestName() );

	if ( TRUE == m_bTraining )
		m_strTitle += " (Training Mode)";
		
	m_bSaveChanges = FALSE;

	switch( m_nActionType )
	{
	case PMS_MANUAL_PAYMENT:
	case PMS_MANUAL_DEPOSIT:
		m_arraySalesInitial.BuildInvoiceArray( m_infoRoom.GetBookingId(), nRoomGridAccountIdx + 1, PMS_INVOICETYPE_EDIT_PAYMENTS );
		break;

	case PMS_MANUAL_SALES:
	default:
		m_arraySalesInitial.BuildInvoiceArray( m_infoRoom.GetBookingId(), nRoomGridAccountIdx + 1, PMS_INVOICETYPE_EDIT_SALES );
		break;
	}
		
	m_arraySalesCurrent.CopyInvoiceItems( m_arraySalesInitial );

	m_pManualSalesEditDlg = NULL;
}	
	
/**********************************************************************/

CPMSManualSalesDatabaseDlg::~CPMSManualSalesDatabaseDlg()
{
	if ( m_pManualSalesEditDlg != NULL )
		delete m_pManualSalesEditDlg;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSManualSalesDatabaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSManualSalesDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSManualSalesDatabaseDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_QUICK, OnQuickSales)
	ON_COMMAND(IDM_ADD_SALES, OnAddSales)
	ON_COMMAND(IDM_ADD_REFUNDS, OnAddRefunds)
	ON_COMMAND(IDM_ADD_ALLOWANCES, OnAddAllowances)
	ON_COMMAND(IDM_ADD_PAYMENTS, OnAddPayments)
	ON_COMMAND(IDM_ADD_SDISC, OnAddSDisc)
	ON_COMMAND(IDM_ADD_LDISC, OnAddLDisc)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM_FILE_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessSalesItem)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSManualSalesDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listSales.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::PMSManualSales );

	switch( m_nActionType )
	{
	case PMS_MANUAL_PAYMENT:
	case PMS_MANUAL_DEPOSIT:
		m_ColumnManager.AddColumn( ColumnNames::Date, "Date", LVCFMT_LEFT, 80, 80 );
		m_ColumnManager.AddColumn( ColumnNames::Time, "Time", LVCFMT_LEFT, 80, 80 );
		m_ColumnManager.AddColumn( ColumnNames::Type, "Type", LVCFMT_LEFT, 150, 150 );
		m_ColumnManager.AddColumn( ColumnNames::PluValue, "Value", LVCFMT_RIGHT, 80, 80 );
		break;

	case PMS_MANUAL_SALES:
		m_ColumnManager.AddColumn( ColumnNames::Date, "Date", LVCFMT_LEFT, 80, 80 );
		m_ColumnManager.AddColumn( ColumnNames::Time, "Time", LVCFMT_LEFT, 80, 80 );
		m_ColumnManager.AddColumn( ColumnNames::Type, "Type", LVCFMT_LEFT, 90, 90 );
		m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu No", LVCFMT_LEFT, 110, 110 );
		m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 160, 160 );
		m_ColumnManager.AddColumn( ColumnNames::PluQty, "Qty", LVCFMT_RIGHT, 80, 80 );
		m_ColumnManager.AddColumn( ColumnNames::PluValue, "Value", LVCFMT_RIGHT, 80, 80 );
		break;
	}
	
	m_ColumnManager.LoadSettings();
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listSales, this );
	CenterWindow();

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );

	if ( PasswordArray.GetEnable( PasswordTicks::PMSManualDelete ) == FALSE )
		MenuChanger.KillItem( IDM_DELETE );

	switch( m_nActionType )
	{
	case PMS_MANUAL_PAYMENT:
	case PMS_MANUAL_DEPOSIT:
		MenuChanger.KillItem( IDM_ADD_SALES );
		MenuChanger.KillItem( IDM_ADD_REFUNDS );
		MenuChanger.KillItem( IDM_ADD_ALLOWANCES );
		MenuChanger.KillItem( IDM_ADD_SDISC );
		MenuChanger.KillItem( IDM_ADD_LDISC );
		MenuChanger.KillItem( IDM_QUICK );
		break;

	case PMS_MANUAL_SALES:
	default:
		MenuChanger.KillItem( IDM_ADD_PAYMENTS );
		break;
	}

	if ( PMSOptions.AreAnyQuickSalesEnabled() == FALSE )
		MenuChanger.KillItem( IDM_QUICK );

	MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
	
	DisplayScreen ( 0 );

	return TRUE;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listSales.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_arraySalesCurrent.GetInvoiceItemCount();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listSales.SetCurSel ( nIndex );
	else
		m_listSales.SetCurSel ( nSize - 1 );

	SetWindowText( m_strTitle );
	m_listSales.Invalidate();
}

/**********************************************************************/

const char* CPMSManualSalesDatabaseDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CCSV csvOut ( '\t' );

		CPMSInvoiceLineInfo info;
		m_arraySalesCurrent.GetInvoiceItem( nIndex, info );
		
		switch( info.m_nLineType )
		{
		case PMS_LINE_TRAN:
			{
				CString strDate;
				strDate.Format( "%2.2d/%2.2d/%4.4d",
					info.m_dateSale.GetDay(),
					info.m_dateSale.GetMonth(),
					info.m_dateSale.GetYear() );

				CString strTime;
				strTime.Format( "%2.2d:%2.2d",
					info.m_dateSale.GetHour(),
					info.m_dateSale.GetMinute() );

				csvOut.Add( strDate );
				csvOut.Add( strTime );
			}
			break;

		case PMS_LINE_SALE:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( "SALE" );
				csvOut.Add ( ReportHelpers.GetDisplayPluNo( info.m_nBasePluNo ) );
				csvOut.Add ( info.m_strDescription );
				csvOut.Add ( info.m_dBasePluQty, SysInfo.GetDPQty() );
				csvOut.Add ( info.m_dValue - info.m_dDiscount , SysInfo.GetDPValue() );
			}
			break;

		case PMS_LINE_REFUND:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( "REFUND" );
				csvOut.Add ( ReportHelpers.GetDisplayPluNo( info.m_nBasePluNo ) );
				csvOut.Add ( info.m_strDescription );
				csvOut.Add ( info.m_dBasePluQty, SysInfo.GetDPQty() );
				csvOut.Add ( info.m_dValue, SysInfo.GetDPValue() );
			}
			break;

		case PMS_LINE_ALLOWANCE:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( "ALLOWANCE" );
				csvOut.Add ( ReportHelpers.GetDisplayPluNo( info.m_nBasePluNo ) );
				csvOut.Add ( info.m_strDescription );
				csvOut.Add ( info.m_dBasePluQty, SysInfo.GetDPQty() );
				csvOut.Add ( info.m_dAllowance, SysInfo.GetDPValue() );
			}
			break;

		case PMS_LINE_SDISC:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( "DISCOUNT" );
				csvOut.Add( "(Subtotal)" );
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( info.m_dDiscount, SysInfo.GetDPValue() );
			}
			break;

		case PMS_LINE_LDISC:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( "DISCOUNT" );
				csvOut.Add( "(Loyalty)" );
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( info.m_dDiscount, SysInfo.GetDPValue() );
			}
			break;

		case PMS_LINE_PAYMENT:
			{
				csvOut.Add( "" );
				csvOut.Add( "" );
				csvOut.Add( info.m_strDescription);
				csvOut.Add( info.m_dValue, SysInfo.GetDPValue() );
			}
			break;
		}

		m_strDisplayLine = csvOut.GetLine();
	}
	
	return m_strDisplayLine;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CPMSManualSalesDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CPMSManualSalesDatabaseDlg::OnProcessSalesItem( WPARAM wIndex, LPARAM lParam )
{
	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CPMSManualSalesEditDlg* pDlg = (CPMSManualSalesEditDlg*) lParam;

		CPMSInvoiceLineInfo infoSale1, infoSale2;
		m_arraySalesCurrent.GetInvoiceItem( nSel, infoSale1 );
		pDlg -> GetSalesInfo( infoSale2 );

		infoSale2.m_dateSale = infoSale1.m_dateSale;
		infoSale2.m_nTransactionNo = infoSale1.m_nTransactionNo;
		
		int nPos = m_arraySalesCurrent.AddInvoiceItem( infoSale2 );
		m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
		m_listSales.SetCurSel( nPos );
		m_listSales.Invalidate();
	}
		
	return 0L;
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listSales, this );
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listSales, this );
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listSales, this );
}


/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnQuickSales()
{
	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CPMSInvoiceLineInfo infoSale1;
		m_arraySalesCurrent.GetInvoiceItem( nSel, infoSale1 );

		if ( PMS_MANUAL_SALES == m_nActionType )
		{
			if ( m_pManualSalesEditDlg == NULL )
			{
				CPMSQuickPluDlg dlg( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, m_bTraining, this );
				if ( dlg.DoModal() == IDOK )
				{
					CPMSInvoiceArray arrayNewSales;
					dlg.GetPMSInvoiceSales( arrayNewSales );

					int nPos = -1;
					for ( int n = 0; n < arrayNewSales.GetInvoiceItemCount(); n++ )
					{
						CPMSInvoiceLineInfo infoSale2;
						arrayNewSales.GetInvoiceItem( n, infoSale2 );

						infoSale2.m_dateSale = infoSale1.m_dateSale;
						infoSale2.m_nTransactionNo = infoSale1.m_nTransactionNo;
		
						nPos = m_arraySalesCurrent.AddInvoiceItem( infoSale2 );
						m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );			
					}

					if ( nPos != -1 )
						m_listSales.SetCurSel( nPos );

					m_listSales.Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddSales() 
{
	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		if ( m_pManualSalesEditDlg == NULL )
		{
			m_pManualSalesEditDlg = new CPMSManualSalesEditDlg( &m_pManualSalesEditDlg, PMS_LINE_SALE, this );
			m_pManualSalesEditDlg -> Create( IDD_PMS_MANUAL_SALES_EDIT, this );
		}
	
		m_pManualSalesEditDlg -> SetFocus();
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddRefunds() 
{
	if ( PMS_MANUAL_SALES == m_nActionType )
	{
		int nSel = m_listSales.GetCurSel();
		if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
		{
			if ( m_pManualSalesEditDlg == NULL )
			{
				m_pManualSalesEditDlg = new CPMSManualSalesEditDlg( &m_pManualSalesEditDlg, PMS_LINE_REFUND, this );
				m_pManualSalesEditDlg -> Create( IDD_PMS_MANUAL_SALES_EDIT, this );
			}
		
			m_pManualSalesEditDlg -> SetFocus();
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddAllowances() 
{
	if ( PMS_MANUAL_SALES == m_nActionType )
	{
		int nSel = m_listSales.GetCurSel();
		if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
		{
			if ( m_pManualSalesEditDlg == NULL )
			{
				m_pManualSalesEditDlg = new CPMSManualSalesEditDlg( &m_pManualSalesEditDlg, PMS_LINE_ALLOWANCE, this );
				m_pManualSalesEditDlg -> Create( IDD_PMS_MANUAL_SALES_EDIT, this );
			}
		
			m_pManualSalesEditDlg -> SetFocus();
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddPayments() 
{
	if ( PMS_MANUAL_SALES != m_nActionType )
	{
		if ( m_pManualSalesEditDlg == NULL )
		{
			int nSel = m_listSales.GetCurSel();
			if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
			{
				CPMSInvoiceLineInfo info;
				m_arraySalesCurrent.GetInvoiceItem( nSel, info );
				
				info.m_nLineType = PMS_LINE_PAYMENT;
				info.m_nBasePluNo = 0;
				info.m_nModifier = 0;
				info.m_dEposPluQty = 0.0;
				info.m_dBasePluQty = 0.0;
				info.m_dValue = 0.0;
				info.m_dDiscount = 0.0;
				info.m_strDescription = "";

				CPMSManualPaymentEditDlg dlg ( TRUE, this );
				dlg.SetSalesInfo( info );

				if ( dlg.DoModal() == IDOK )
				{
					dlg.GetSalesInfo( info );

					int nPos = m_arraySalesCurrent.AddInvoiceItem( info );
					m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
					m_listSales.SetCurSel( nPos );
					m_listSales.Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddSDisc() 
{
	if ( PMS_MANUAL_SALES == m_nActionType )
	{
		if ( m_pManualSalesEditDlg == NULL )
		{
			int nSel = m_listSales.GetCurSel();
			if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
			{
				CPMSInvoiceLineInfo info;
				m_arraySalesCurrent.GetInvoiceItem( nSel, info );
				
				info.m_nLineType = PMS_LINE_SDISC;
				info.m_nBasePluNo = 0;
				info.m_nModifier = 0;
				info.m_dEposPluQty = 0.0;
				info.m_dBasePluQty = 0.0;
				info.m_dValue = 0.0;
				info.m_dDiscount = 0.0;
				info.m_strDescription = "";

				CPMSManualDiscountEditDlg dlg ( TRUE, this );
				dlg.SetSalesInfo( info );

				if ( dlg.DoModal() == IDOK )
				{
					dlg.GetSalesInfo( info );

					int nPos = m_arraySalesCurrent.AddInvoiceItem( info );
					m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
					m_listSales.SetCurSel( nPos );
					m_listSales.Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnAddLDisc() 
{
	if ( PMS_MANUAL_SALES == m_nActionType )
	{
		if ( m_pManualSalesEditDlg == NULL )
		{
			int nSel = m_listSales.GetCurSel();
			if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
			{
				CPMSInvoiceLineInfo info;
				m_arraySalesCurrent.GetInvoiceItem( nSel, info );
				
				info.m_nLineType = PMS_LINE_LDISC;
				info.m_nBasePluNo = 0;
				info.m_nModifier = 0;
				info.m_dEposPluQty = 0.0;
				info.m_dBasePluQty = 0.0;
				info.m_dValue = 0.0;
				info.m_dDiscount = 0.0;
				info.m_strDescription = "";

				CPMSManualDiscountEditDlg dlg ( TRUE, this );
				dlg.SetSalesInfo( info );

				if ( dlg.DoModal() == IDOK )
				{
					dlg.GetSalesInfo( info );

					int nPos = m_arraySalesCurrent.AddInvoiceItem( info );
					m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
					m_listSales.SetCurSel( nPos );
					m_listSales.Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnEdit() 
{
	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CPMSInvoiceLineInfo infoSale;
		m_arraySalesCurrent.GetInvoiceItem( nSel, infoSale );

		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
			{
				int nOldLineType = PMS_LINE_SALE;
				bool bFromAdd = FALSE;
				
				if ( m_pManualSalesEditDlg != NULL )
				{
					CPMSInvoiceLineInfo infoDlg;
					m_pManualSalesEditDlg -> GetSalesInfo( infoDlg );
					nOldLineType = infoDlg.m_nLineType;
					 
					delete m_pManualSalesEditDlg;
					m_pManualSalesEditDlg = NULL;
					bFromAdd = TRUE;
				}
			  
				CPMSManualSalesEditDlg dlg ( this );
				dlg.SetSalesInfo( infoSale );

				if ( dlg.DoModal() == IDOK )
				{
					m_arraySalesCurrent.RemoveInvoiceItem( nSel );

					CPMSInvoiceLineInfo infoDlg;
					dlg.GetSalesInfo( infoDlg );

					int nPos = m_arraySalesCurrent.AddInvoiceItem( infoDlg );
					m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );
					m_listSales.SetCurSel( nPos );
					m_listSales.Invalidate();
				}

				if ( bFromAdd == TRUE )
				{
					switch( nOldLineType )
					{
					case PMS_LINE_SALE:
						OnAddSales();
						break;

					case PMS_LINE_REFUND:
						OnAddRefunds();
						break;

					case PMS_LINE_ALLOWANCE:
						OnAddAllowances();
						break;
					}
				}
			}
			break;
	
		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			if ( m_pManualSalesEditDlg == NULL )
			{	
				CPMSManualDiscountEditDlg dlg (FALSE, this );
				dlg.SetSalesInfo( infoSale );

				if ( dlg.DoModal() == IDOK )
				{
					CPMSInvoiceLineInfo infoDlg;
					dlg.GetSalesInfo( infoDlg );
					m_arraySalesCurrent.SetInvoiceItem( nSel, infoDlg );
					m_listSales.SetCurSel( nSel );
					m_listSales.Invalidate();
				}
			}
			break;

		case PMS_LINE_PAYMENT:
			if ( m_pManualSalesEditDlg == NULL )
			{	
				CPMSManualPaymentEditDlg dlg ( FALSE, this );
				dlg.SetSalesInfo( infoSale );

				if ( dlg.DoModal() == IDOK )
				{
					CPMSInvoiceLineInfo infoDlg;
					dlg.GetSalesInfo( infoDlg );
					m_arraySalesCurrent.SetInvoiceItem( nSel, infoDlg );
					m_listSales.SetCurSel( nSel );
					m_listSales.Invalidate();
				}
			}
			break;
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnDelete() 
{
	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CPMSInvoiceLineInfo info;
		m_arraySalesCurrent.GetInvoiceItem( nSel, info );

		CString strMsg = "Are you sure you wish to delete this ";

		switch( info.m_nLineType )
		{
		case PMS_LINE_SALE:
			strMsg += "sale";
			break;

		case PMS_LINE_REFUND:
			strMsg += "refund";
			break;

		case PMS_LINE_ALLOWANCE:
			strMsg += "allowance";
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			strMsg += "discount";
			break;

		case PMS_LINE_PAYMENT:
			strMsg += "payment";
			break;

		default:
			return;
		}

		if ( Prompter.YesNo ( strMsg ) == IDYES )
		{
			m_arraySalesCurrent.RemoveInvoiceItem( nSel );
			m_listSales.SetItemCountEx( m_arraySalesCurrent.GetInvoiceItemCount() );

			if ( nSel < m_listSales.GetItemCount() )
				m_listSales.SetCurSel ( nSel );
			else
				m_listSales.SetCurSel ( nSel - 1 );

			m_listSales.Invalidate();
		}
	}
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if ( m_arraySalesCurrent.GetInvoiceItemCount() == 0 )
		return;

	bool bAccept = FALSE;

	int nSel = m_listSales.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySalesCurrent.GetInvoiceItemCount() )
	{
		CPMSInvoiceLineInfo infoSale;
		m_arraySalesCurrent.GetInvoiceItem( nSel, infoSale );

		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
		case PMS_LINE_PAYMENT:
			bAccept = TRUE;
			break;
		}
	}

	if ( FALSE == bAccept )
		return;

	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_STOCK_LIST));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuPopup( pPopup );

	if ( PasswordArray.GetEnable( PasswordTicks::PMSManualDelete ) == FALSE )
		MenuChanger.KillItem( IDM_DELETE );

	MenuChanger.BuildMenu( pPopup );
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnCancel()
{
	CPMSInvoiceArray arrayChanges;

	arrayChanges.GetInvoiceChanges( m_arraySalesCurrent, m_arraySalesInitial );

	bool bQuit = TRUE;

	if ( arrayChanges.GetInvoiceItemCount() != 0 )
	{
		CPMSManualSalesCloseDlg dlg( m_nActionType, this );
		dlg.DoModal();

		switch( dlg.m_nAction )
		{
		case PMSSALES_CONFIRM:
			{
				arrayChanges.WriteInvoice( m_infoRoom.GetBookingId(), m_nRoomGridAccountIdx + 1, TRUE );

				if ( PMS_MANUAL_SALES == m_nActionType )
				{
#ifndef STOCKMAN_SYSTEM
					CZSalesImporterEcrman ZSalesImporter;
					ZSalesImporter.LookForPMSSales();
#else
					CZSalesImporterStockman ZSalesImporter;
					ZSalesImporter.LookForPMSSales();
#endif
					CDataManagerInfo infoDb;
					DataManager.OpenDatabaseReadOnly( 0, infoDb );
				}
				bQuit = TRUE;
			}
			break;

		case PMSSALES_CANCEL:
			bQuit = TRUE;
			break;

		case PMSSALES_CONTINUE:
		default:
			bQuit = FALSE;
			break;
		}
	}

	if ( TRUE == bQuit )
		EndDialog (IDOK );
}

/**********************************************************************/

void CPMSManualSalesDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/
#endif
/**********************************************************************/
