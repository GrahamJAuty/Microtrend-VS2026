/**********************************************************************/
#include "Consolidation.h"
#include "EposReportCustomDepositDlg.h"
#include "EposReportCustomDiscountDlg.h"
#include "EposReportCustomGeneralDlg.h"
#include "EposReportCustomHourlyDlg.h"
#include "EposReportCustomLoyaltyReconDlg.h"
#include "EposReportCustomMixMatchDlg.h"
#include "EposReportCustomNewDlg.h"
#include "EposReportCustomPaymentDetailDlg.h"
#include "EposReportCustomPluPriceBandDlg.h"
#include "EposReportCustomTransactionDlg.h"
#include "EposReports.h"
#include "EposReportSelect.h"
#include "FileRemover.h"
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportSelectNewDlg.h"
/**********************************************************************/

CEposReportSelectNewDlg::CEposReportSelectNewDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CEposReportSelectNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEposReportSelectNewDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CEposReportSelectNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEposReportSelectNewDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportSelectNewDlg, CDialog)
	//{{AFX_MSG_MAP(CEposReportSelectNewDlg)
	ON_BN_CLICKED( IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED( IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED( IDC_BUTTON_RESETORDER, OnButtonResetOrder)
	ON_BN_CLICKED( IDC_BUTTON_CREATE, OnButtonCreate )
	ON_BN_CLICKED( IDC_BUTTON_EDIT, OnButtonEdit )
	ON_BN_CLICKED( IDC_BUTTON_ADD, OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_REMOVE, OnButtonRemove )
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SELECTED, OnGetDispInfoListSelected)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_AVAILABLE, OnGetDispInfoListAvailable)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECTED, OnDoubleClickListSelected)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE, OnDoubleClickListAvailable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportSelectNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//READ LIST OF SELECTED REPORTS
	m_EposReportSelect.LoadReportSelection();

	m_arraySelected.RemoveAll();
	for ( int n = 0; n < m_EposReportSelect.GetSelectedReportCount(); n++ )
	{
		CSortedIntWithFlag item;
		item.m_nInt = m_EposReportSelect.GetSelectedReportType(n);
		item.m_nFlag = 0;
		m_arraySelected.Add( item );
	}

	m_arrayAvailable.RemoveAll();
	for ( int n = 0; n < m_EposReportSelect.GetAvailableReportCount(); n++ )
	{
		CSortedIntWithFlag item;
		item.m_nInt = m_EposReportSelect.GetAvailableReportType(n);
		item.m_nFlag = 0;
		m_arrayAvailable.Add( item );
	}

	m_listSelected.SubclassDlgItem ( IDC_LIST_SELECTED, this );
	m_listSelected.InsertColumn ( 0, "Type", LVCFMT_LEFT, 80 );
	m_listSelected.InsertColumn ( 1, "Report Name", LVCFMT_LEFT, 200 );
	m_listSelected.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listSelected.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSelected.SetItemCountEx( m_arraySelected.GetSize() );
	
	m_listAvailable.SubclassDlgItem ( IDC_LIST_AVAILABLE, this );
	m_listAvailable.InsertColumn ( 0, "Report Name", LVCFMT_LEFT, 200 );
	m_listAvailable.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listAvailable.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listAvailable.SetItemCountEx( m_arrayAvailable.GetSize() );

	SelectLine( FALSE, 0 );
	SelectLine( TRUE, 0 );

	return TRUE;
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( ( IDC_LIST_SELECTED == nIDCtl ) || ( IDC_LIST_AVAILABLE == nIDCtl ) ) 
      lpMeasureItemStruct->itemHeight += 10;
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );

		int nIndex = pDispInfo->item.iItem;
		if ( ( nIndex >= 0 ) && ( nIndex < m_arraySelected.GetSize() ) )
		{
			CSortedIntWithFlag item = m_arraySelected.GetAt( nIndex );
			int nReportType = item.m_nInt;

			CString strLine = "";
			if ( nReportType < EPOS_CUSTOM_FIRST_REPORT )
			{
				csv.Add( "Standard" );
				csv.Add( CEposReport::GetReportNameExternal( nReportType ) );
			}
			else
			{
				csv.Add( "Custom" );
				csv.Add( m_EposReportSelect.GetCustomReportName( nReportType ) );
			}
		}

		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );
		
		int nIndex = pDispInfo->item.iItem;
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayAvailable.GetSize() ) )
		{
			CSortedIntWithFlag item = m_arrayAvailable.GetAt( nIndex );
			int nReportType = item.m_nInt;

			csv.Add ( CEposReport::GetReportNameExternal( nReportType ) );
		}

		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

CSSListCtrlVirtual* CEposReportSelectNewDlg::GetReportList( bool bAvailable )
{
	return ( bAvailable ) ? &m_listAvailable : &m_listSelected;
}

/**********************************************************************/

CArray<CSortedIntWithFlag,CSortedIntWithFlag>* CEposReportSelectNewDlg::GetReportArray( bool bAvailable )
{
	return ( bAvailable ) ? &m_arrayAvailable : & m_arraySelected;
}

/**********************************************************************/

void CEposReportSelectNewDlg::SelectSingleItem( bool bAvailable )
{
	SelectLine( bAvailable, GetReportList( bAvailable ) -> GetCurSel() );
}

/**********************************************************************/

void CEposReportSelectNewDlg::SelectLine( bool bAvailable, int nIndex )
{
	GetReportList( bAvailable ) -> Invalidate();

	for ( int n = 0; n < GetReportList( bAvailable ) -> GetItemCount(); n++ )
	{
		if ( n != nIndex )
			GetReportList( bAvailable ) -> SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			GetReportList( bAvailable ) -> SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			GetReportList( bAvailable ) -> SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::GetSelectedItemArray( bool bAvailable, CReportConsolidationArray<CSortedIntItem>& arrayItems )
{
	arrayItems.RemoveAll();

	POSITION pos = GetReportList( bAvailable ) -> GetFirstSelectedItemPosition();
		
	while (pos)
	{
		CSortedIntItem indexItem;
		indexItem.m_nItem = GetReportList( bAvailable ) -> GetNextSelectedItem(pos);
		arrayItems.Consolidate( indexItem );
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::ClearSelection( bool bAvailable )
{
	if ( GetReportArray( bAvailable ) -> GetSize() == GetReportList( bAvailable ) -> GetItemCount() )
	{
		for ( int n = 0; n < GetReportArray( bAvailable ) -> GetSize(); n++ )
		{
			CSortedIntWithFlag item = GetReportArray( bAvailable ) -> GetAt(n);
			item.m_nFlag = 0;
			GetReportArray( bAvailable ) -> SetAt( n, item );
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::MarkSelection( bool bAvailable )
{
	if ( GetReportArray( bAvailable ) -> GetSize() == GetReportList( bAvailable ) -> GetItemCount() )
	{
		ClearSelection( bAvailable );

		POSITION pos = GetReportList( bAvailable ) -> GetFirstSelectedItemPosition();
		
		while (pos)
		{
			int nIdx = GetReportList( bAvailable ) -> GetNextSelectedItem(pos);
			CSortedIntWithFlag item = GetReportArray( bAvailable ) -> GetAt( nIdx );
			item.m_nFlag = 1;
			GetReportArray( bAvailable ) -> SetAt( nIdx, item );
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::RestoreSelection( bool bAvailable )
{
	if ( GetReportList( bAvailable ) -> GetItemCount() == GetReportArray( bAvailable ) -> GetSize() )
	{
		GetReportList( bAvailable ) -> Invalidate();

		bool bGotMark = FALSE;
		for ( int n = 0; n < GetReportList( bAvailable ) -> GetItemCount(); n++ )
		{
			CSortedIntWithFlag item = GetReportArray( bAvailable ) -> GetAt(n);

			if ( item.m_nFlag != 1 )
				GetReportList( bAvailable ) -> SetItemState(n, 0, LVIS_SELECTED);
			else
			{
				GetReportList( bAvailable ) -> SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
				
				if ( FALSE == bGotMark )
				{
					GetReportList( bAvailable ) -> SetSelectionMark(n);
					bGotMark = TRUE;
				}
			}
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonAdd()
{
	MoveReports( TRUE );
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonRemove()
{
	MoveReports( FALSE );
}

/**********************************************************************/

void CEposReportSelectNewDlg::MoveReports( bool bFromAvailable )
{
	ClearSelection( FALSE );
	ClearSelection( TRUE );

	CSSListCtrlVirtual* pSourceList = GetReportList( bFromAvailable );
	CSSListCtrlVirtual* pDestList = GetReportList( FALSE == bFromAvailable );
	CArray<CSortedIntWithFlag,CSortedIntWithFlag>* pSourceArray = GetReportArray( bFromAvailable );
	CArray<CSortedIntWithFlag,CSortedIntWithFlag>* pDestArray = GetReportArray( FALSE == bFromAvailable );

	CReportConsolidationArray<CSortedIntItem> arrayIndex;
	GetSelectedItemArray( bFromAvailable, arrayIndex );

	int nInsertPos = 0;
	int nDestSel = pDestList -> GetCurSel();
	if ( ( nDestSel >= 0 ) && ( nDestSel < pDestArray -> GetSize() ) )
		nInsertPos = nDestSel + 1;
	else
		nInsertPos = pDestArray -> GetSize();

	if ( arrayIndex.GetSize() != 0 )
	{
		for ( int n = arrayIndex.GetSize() - 1; n >= 0; n-- )
		{
			CSortedIntItem indexItem;
			arrayIndex.GetAt( n, indexItem );

			CSortedIntWithFlag arrayItem = pSourceArray -> GetAt( indexItem.m_nItem );
			pSourceArray -> RemoveAt( indexItem.m_nItem );

			bool bMoveToDest = TRUE;

			if ( FALSE == bFromAvailable )
			{
				int nReportType = arrayItem.m_nInt;

				if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
				{
					CEposReportSelectInfo infoReport;
					infoReport.SetReportType( nReportType );
					m_EposReportSelect.RemoveCustomReport( infoReport );

					CFilenameUpdater FnUp( SysFiles::EcrCustomReportPrm, 1, nReportType + 1 - EPOS_CUSTOM_FIRST_REPORT );
					CString strParamsFilename = FnUp.GetFilenameToUse();
					CFileRemover FileRemover( strParamsFilename );
					
					bMoveToDest = FALSE;
				}
			}

			if ( TRUE == bMoveToDest )
			{
				arrayItem.m_nFlag = 1;
				pDestArray -> InsertAt( nInsertPos++, arrayItem );
			}
		}

		pSourceList -> SetItemCountEx( pSourceArray -> GetSize() );
		pDestList -> SetItemCountEx( pDestArray -> GetSize() );

		if ( FALSE == bFromAvailable )
			m_EposReportSelect.ApplyDefaultSortOrder( m_arrayAvailable );

		//SELECT MOVED ITEMS IN DESTINATION LIST
		RestoreSelection( FALSE == bFromAvailable );

		//SELECT INDEX ITEM IN SOURCE LIST
		CSortedIntItem indexItem;
		arrayIndex.GetAt( 0, indexItem );
		
		int nSourceSel = indexItem.m_nItem;
		if ( nSourceSel >= pSourceArray -> GetSize() )
			nSourceSel--;

		SelectLine( bFromAvailable, nSourceSel );

		pSourceList -> Invalidate();
		pDestList -> Invalidate();
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonCreate()
{
	CEposReportCustomNewDlg dlgNewReport( this );
	if (dlgNewReport.DoModal() != IDOK)
	{
		return;
	}

	int nFamily = 0;

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		switch( dlgNewReport.m_nSel )
		{
		case 1:		
			nFamily = EPOS_CUSTOM_FAMILY_HOURLYGRP;
			break;

		case 2:		
			nFamily = EPOS_CUSTOM_FAMILY_HOURLYDPT;
			break;

		case 3:		
			nFamily = EPOS_CUSTOM_FAMILY_COVERS_HOURLY;	
			break;

		case 4:		
			nFamily = EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP;
			break;

		case 5:		
			nFamily = EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT; 
			break;

		case 6:
			nFamily = EPOS_CUSTOM_FAMILY_PLU_PRICEBAND;
			break;

		case 7:		
			nFamily = EPOS_CUSTOM_FAMILY_GENERAL;
			break;

		case 8:		
			nFamily = EPOS_CUSTOM_FAMILY_DEPOSIT;
			break;

		case 9:		
			nFamily = EPOS_CUSTOM_FAMILY_MIXMATCH; 
			break;

		case 10:		
			nFamily = EPOS_CUSTOM_FAMILY_DISCOUNT;
			break;

		case 11:
			nFamily = EPOS_CUSTOM_FAMILY_PAYMENTDETAIL;
			break;

		case 12:	
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_LOY_V3:
			case LOYALTY_TYPE_LOY_V4:
			case LOYALTY_TYPE_SMP_V2:
			case LOYALTY_TYPE_SMP_V4:
				nFamily = EPOS_CUSTOM_FAMILY_LOYALTYRECON;
				break;

			default:
				nFamily = EPOS_CUSTOM_FAMILY_TRANSACTION;
				break;
			}
			break;

		case 13:
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_LOY_V3:
			case LOYALTY_TYPE_LOY_V4:
			case LOYALTY_TYPE_SMP_V2:
			case LOYALTY_TYPE_SMP_V4:
				nFamily = EPOS_CUSTOM_FAMILY_TRANSACTION;
				break;

			default:
				nFamily = EPOS_CUSTOM_FAMILY_HOURLY;
				break;
			}
			break;
			

		case 0:
		default: 
			nFamily = EPOS_CUSTOM_FAMILY_HOURLY;
			break;
		}
	}
	else
	{
		switch( dlgNewReport.m_nSel )
		{
		case 1: 
			nFamily = EPOS_CUSTOM_FAMILY_TRANSACTION; 
			break;

		case 0:
		default: 
			nFamily = EPOS_CUSTOM_FAMILY_HOURLY;
			break;
		}
	}
	
	CEposReportSelectInfo infoReport;
	if ( m_EposReportSelect.AddCustomReport( infoReport, nFamily ) == FALSE )
	{
		Prompter.Error( "You have already created the maximum\nnumber of custom Epos reports." );
		return;
	}

	int nSel = m_listSelected.GetCurSel();
	if ((nSel < 0) || (nSel >= m_arraySelected.GetSize()))
	{
		nSel = -1;
	}

	SelectSingleItem( FALSE );

	CSortedIntWithFlag item;
	item.m_nInt = infoReport.GetReportType();
	item.m_nFlag = 0;

	m_arraySelected.InsertAt( nSel + 1, item );
	m_listSelected.SetItemCountEx( m_arraySelected.GetSize() );
	m_listSelected.SetCurSel( nSel + 1 );
	OnButtonEdit();
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonEdit()
{
	int nSel = m_listSelected.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < m_arraySelected.GetSize() ) )
	{
		SelectSingleItem( FALSE );
		
		CSortedIntWithFlag item = m_arraySelected.GetAt( nSel );
		int nReportType = item.m_nInt;
		
		if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
		{
			CEposReportSelectInfo infoReport;
			infoReport.SetReportType( nReportType );

			bool bSave = FALSE;
			if ( m_EposReportSelect.GetReportInfo( infoReport ) == TRUE )
			{
				int nFamily = infoReport.GetCustomReportFamily(); 
				switch( nFamily )
				{
				case EPOS_CUSTOM_FAMILY_HOURLY:
				case EPOS_CUSTOM_FAMILY_HOURLYGRP:
				case EPOS_CUSTOM_FAMILY_HOURLYDPT:
				case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
				case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
				case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
					{
						CEposReportCustomHourlyDlg dlg( nFamily, m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_GENERAL:
					{
						CEposReportCustomGeneralDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_TRANSACTION:
					{
						CEposReportCustomTransactionDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_DEPOSIT:
					{
						CEposReportCustomDepositDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_MIXMATCH:
					{
						CEposReportCustomMixMatchDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_DISCOUNT:
					{
						CEposReportCustomDiscountDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
						bSave = ( dlg.DoModal() == IDOK );
					}
					break;

				case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
				{
					CEposReportCustomLoyaltyReconDlg dlg( m_EposReportSelect, infoReport, FALSE, this);
					bSave = (dlg.DoModal() == IDOK);
				}
				break;

				case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
				{
					CEposReportCustomPaymentDetailDlg dlg( m_EposReportSelect, infoReport, FALSE, this );
					bSave = ( dlg.DoModal() == IDOK );
				}
				break;

				case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
				{
					CDataManagerInfo info;
					DataManager.OpenDatabaseQuickNoFiles(0, TRUE);
					DataManager.OpenPriceText(DB_READONLY, info);

					CEposReportCustomPluPriceBandDlg dlg(m_EposReportSelect, infoReport, FALSE, this);
					bSave = (dlg.DoModal() == IDOK);

					DataManager.ClosePriceText(info);
				}
				break;
				}

				if ( TRUE == bSave )
				{
					m_EposReportSelect.SetReportInfo( infoReport );
					m_listSelected.Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_listSelected.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listSelected.GetItemCount() ) )
	{
		SelectSingleItem( FALSE );

		CSortedIntWithFlag item = m_arraySelected.GetAt( nSel );

		if ( item.m_nInt >= EPOS_CUSTOM_FIRST_REPORT )
			OnButtonEdit();
		else
			OnButtonRemove();
	}
	*pResult = 0;
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_listAvailable.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listAvailable.GetItemCount() ) )
	{
		SelectSingleItem( TRUE );
		OnButtonAdd();
	}
	*pResult = 0;
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonUp() 
{
	int nSel = m_listSelected.GetCurSel();
	if ( ( nSel > 0 ) && ( nSel < m_listSelected.GetItemCount() ) )
	{
		if ( m_listSelected.GetItemCount() == m_arraySelected.GetSize() )
		{
			CSortedIntWithFlag item1 = m_arraySelected.GetAt( nSel );
			CSortedIntWithFlag item2 = m_arraySelected.GetAt( nSel - 1 );

			m_arraySelected.SetAt( nSel, item2 );
			m_arraySelected.SetAt( nSel - 1, item1 );

			SelectLine( FALSE, nSel - 1 );
			m_listSelected.EnsureVisible( nSel - 1, FALSE );
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonDown() 
{
	int nSel = m_listSelected.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listSelected.GetItemCount() - 1 ) )
	{
		if ( m_listSelected.GetItemCount() == m_arraySelected.GetSize() )
		{
			CSortedIntWithFlag item1 = m_arraySelected.GetAt( nSel );
			CSortedIntWithFlag item2 = m_arraySelected.GetAt( nSel + 1 );

			m_arraySelected.SetAt( nSel, item2 );
			m_arraySelected.SetAt( nSel + 1, item1 );

			SelectLine( FALSE, nSel + 1 );
			m_listSelected.EnsureVisible( nSel + 1, FALSE );
		}
	}
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnButtonResetOrder() 
{
	RestoreSelectedSortOrder();
}

/**********************************************************************/

void CEposReportSelectNewDlg::RestoreSelectedSortOrder()
{
	MarkSelection( FALSE );
	m_EposReportSelect.ApplyDefaultSortOrder( m_arraySelected );
	RestoreSelection( FALSE );
}

/**********************************************************************/

void CEposReportSelectNewDlg::OnOK() 
{
	m_EposReportSelect.UpdateReportStatus( m_arraySelected, m_arrayAvailable );
	m_EposReportSelect.SaveReportSelection();
	EndDialog( IDOK );
}

/**********************************************************************/


