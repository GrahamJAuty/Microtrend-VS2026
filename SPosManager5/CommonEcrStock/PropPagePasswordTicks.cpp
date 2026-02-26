/**********************************************************************/
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePasswordTicks.h"
/**********************************************************************/

CPropPagePasswordTicks::CPropPagePasswordTicks() : CPropertyPage(CPropPagePasswordTicks::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePasswordTicks)
	//}}AFX_DATA_INIT
	m_nPageType = PASSWORDTICKS_PAGE_FILE;
}

/**********************************************************************/

CPropPagePasswordTicks::~CPropPagePasswordTicks()
{
}

/**********************************************************************/

void CPropPagePasswordTicks::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePasswordTicks)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePasswordTicks, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePasswordTicks)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnToggleTick)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnToggleTick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnToggleTick)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST, OnToggleTick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePasswordTicks::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	switch ( m_nPageType )
	{
	case PASSWORDTICKS_PAGE_FILE:
		m_arrayTicks.Add( PasswordTicks::FileCommsLog );
		m_arrayTicks.Add( PasswordTicks::FileImportWizard );
		m_arrayTicks.Add( PasswordTicks::FileBackup );
		m_arrayTicks.Add( PasswordTicks::FileRestore );
		m_arrayTicks.Add( PasswordTicks::FileBackupLog );
		m_arrayTicks.Add( PasswordTicks::FileBackupStatus );
		break;


	case PASSWORDTICKS_PAGE_SYSTEM:
		m_arrayTicks.Add( PasswordTicks::DatabaseMenu );
		m_arrayTicks.Add( PasswordTicks::EditKeyboards );
		
#ifdef STOCKMAN_SYSTEM
		m_arrayTicks.Add( PasswordTicks::DatabaseOrderProcess );
#endif

		m_arrayTicks.Add( PasswordTicks::EditEposData );
		m_arrayTicks.Add( PasswordTicks::EditPluRecords );

#ifndef STOCKMAN_SYSTEM
		{
			m_arrayTicks.Add( PasswordTicks::PluAddStock );
			m_arrayTicks.Add( PasswordTicks::PluClearStock );
			m_arrayTicks.Add( PasswordTicks::PluDeliverStock );
		}
#endif

		m_arrayTicks.Add( PasswordTicks::EditDepartment );
		m_arrayTicks.Add( PasswordTicks::EditGroup );
		m_arrayTicks.Add( PasswordTicks::EditTax );

		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			m_arrayTicks.Add(PasswordTicks::EditMixMatch);
			m_arrayTicks.Add(PasswordTicks::EditTimePlan);
			m_arrayTicks.Add(PasswordTicks::EditWalletStampRule);
		}

		m_arrayTicks.Add( PasswordTicks::EditAllergen );
		m_arrayTicks.Add( PasswordTicks::EditPayment );
		
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			m_arrayTicks.Add(PasswordTicks::EditModifier);
		}
		
		m_arrayTicks.Add( PasswordTicks::EditAnalysisCategory );
		m_arrayTicks.Add( PasswordTicks::EditPaidInOutText );
		
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			m_arrayTicks.Add( PasswordTicks::EditServerNames );
			m_arrayTicks.Add( PasswordTicks::EditServerDetails );
			m_arrayTicks.Add( PasswordTicks::EditServerPresets );
			m_arrayTicks.Add( PasswordTicks::EditCustomers );
			m_arrayTicks.Add( PasswordTicks::EditCustomersDelDay );
		}

#ifdef STOCKMAN_SYSTEM
		m_arrayTicks.Add( PasswordTicks::EditStockData );
		m_arrayTicks.Add( PasswordTicks::EditStockRecords );
		m_arrayTicks.Add( PasswordTicks::EditDeliveries );
		m_arrayTicks.Add( PasswordTicks::EditTransfers );
		m_arrayTicks.Add( PasswordTicks::EditAdjustments );
		m_arrayTicks.Add( PasswordTicks::EditReturns );
		m_arrayTicks.Add( PasswordTicks::EditManualSales );
		m_arrayTicks.Add( PasswordTicks::EditOpening );
		m_arrayTicks.Add( PasswordTicks::EditReorder );
		m_arrayTicks.Add( PasswordTicks::EditActiveStock );
		m_arrayTicks.Add( PasswordTicks::UnitechEnable );
		m_arrayTicks.Add( PasswordTicks::UnitechPluDownload );
		//m_arrayTicks.Add( PasswordTicks::UnitechPluUpload );
		m_arrayTicks.Add( PasswordTicks::UnitechStockUpload );
#endif
		break;

	case PASSWORDTICKS_PAGE_STOCKTAKE:
		m_arrayTicks.Add( PasswordTicks::Stocktaking );
		m_arrayTicks.Add( PasswordTicks::StocktakingNew );
		m_arrayTicks.Add( PasswordTicks::StocktakingEdit);
		m_arrayTicks.Add( PasswordTicks::StocktakingLevels );
		m_arrayTicks.Add( PasswordTicks::StocktakingReports );
		m_arrayTicks.Add( PasswordTicks::StocktakingFinalise );
		m_arrayTicks.Add( PasswordTicks::StocktakingAbandon );
		break;

	case PASSWORDTICKS_PAGE_TASKS:
		m_arrayTicks.Add( PasswordTicks::TasksMenu );
		m_arrayTicks.Add( PasswordTicks::TasksMenuAdhoc );
		m_arrayTicks.Add( PasswordTicks::TasksMenuPreset1 );
		m_arrayTicks.Add( PasswordTicks::TasksMenuPreset2 );
		
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset3 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset4 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset5 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset6 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset7 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset8 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset9 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset10 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset11 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset12 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset13 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset14 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset15 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset16 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset17 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset18 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset19 );
			m_arrayTicks.Add( PasswordTicks::TasksMenuPreset20 );
		}

		m_arrayTicks.Add( PasswordTicks::PluChangesTask );
		m_arrayTicks.Add( PasswordTicks::CommsBatchSales );
		m_arrayTicks.Add( PasswordTicks::CommsDownloads );
		break;

	case PASSWORDTICKS_PAGE_REPORTS:
		m_arrayTicks.Add( PasswordTicks::ReportsMenu );
		m_arrayTicks.Add( PasswordTicks::ReportsEpos );

		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			m_arrayTicks.Add( PasswordTicks::ReportsWorking );
			m_arrayTicks.Add( PasswordTicks::ReportsCustomer );
			m_arrayTicks.Add( PasswordTicks::ReportsStatement );
		}

		m_arrayTicks.Add( PasswordTicks::ReportsPluHistory );

#ifdef STOCKMAN_SYSTEM
		m_arrayTicks.Add( PasswordTicks::ReportsStock );
		m_arrayTicks.Add( PasswordTicks::ReportsAudit );
#else
		m_arrayTicks.Add( PasswordTicks::ReportsPluApparent );
		m_arrayTicks.Add( PasswordTicks::HousekeepingPluApparent );
		m_arrayTicks.Add( PasswordTicks::ReportsPluDelivery );
#endif
		
		break;	

	case PASSWORDTICKS_PAGE_ADMIN:
		m_arrayTicks.Add( PasswordTicks::AdminMenu );
		m_arrayTicks.Add( PasswordTicks::AdminEposSales );

#ifdef STOCKMAN_SYSTEM
		m_arrayTicks.Add( PasswordTicks::AdminStockSales );
		m_arrayTicks.Add( PasswordTicks::AdminOrders );
		m_arrayTicks.Add( PasswordTicks::AdminOrderAudits );
		m_arrayTicks.Add( PasswordTicks::AdminPluStock );
		m_arrayTicks.Add( PasswordTicks::AdminBarcode );
		m_arrayTicks.Add( PasswordTicks::AdminSuppliers );
		m_arrayTicks.Add( PasswordTicks::AdminStockAudits );
		m_arrayTicks.Add( PasswordTicks::AdminStockChange );
#else
		m_arrayTicks.Add( PasswordTicks::AdminBarcode );
		m_arrayTicks.Add( PasswordTicks::AdminPluStock );
		m_arrayTicks.Add( PasswordTicks::AdminPluDelivery );
#endif
		break;

	case PASSWORDTICKS_PAGE_SETUP:
		m_arrayTicks.Add( PasswordTicks::SetupMenu );
		m_arrayTicks.Add( PasswordTicks::SetupMenuSystem );
		m_arrayTicks.Add( PasswordTicks::SetupMenuComms );
		m_arrayTicks.Add( PasswordTicks::SetupMenuBarcodeEntry );
		m_arrayTicks.Add( PasswordTicks::SetupMenuLabels );
		m_arrayTicks.Add( PasswordTicks::SetupMenuTexts );

		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			m_arrayTicks.Add( PasswordTicks::SetupMenuBillTexts );
			m_arrayTicks.Add( PasswordTicks::SetupMenuFunction );
			m_arrayTicks.Add( PasswordTicks::SetupMenuOPGroup );
			m_arrayTicks.Add( PasswordTicks::SetupMenuTableNames );
		}

		m_arrayTicks.Add( PasswordTicks::SetupMenuMacros );
		m_arrayTicks.Add( PasswordTicks::SetupMenuReasons );
		m_arrayTicks.Add( PasswordTicks::SetupMenuNetworkResources );
		m_arrayTicks.Add( PasswordTicks::SetupMenuOptions );

#ifdef STOCKMAN_SYSTEM
		m_arrayTicks.Add( PasswordTicks::UnitechSetup );
#endif

		break;
		
	case PASSWORDTICKS_PAGE_SMARTCARD:
		if ( SysInfo.GotSmartPayLink() )
		{
			m_arrayTicks.Add( PasswordTicks::Smartpay );
		}
		else
		{
			m_arrayTicks.Add( PasswordTicks::Loyalty );
		}
		break;

	case PASSWORDTICKS_PAGE_PMS:
		m_arrayTicks.Add( PasswordTicks::PMS );
		m_arrayTicks.Add( PasswordTicks::PMSOptions );
		m_arrayTicks.Add( PasswordTicks::PMSManualSales );
		m_arrayTicks.Add( PasswordTicks::PMSManualDelete );
		m_arrayTicks.Add( PasswordTicks::PMSPayments );
		m_arrayTicks.Add( PasswordTicks::PMSInvoices );
		m_arrayTicks.Add( PasswordTicks::PMSCheckIn );
		m_arrayTicks.Add( PasswordTicks::PMSCheckOut );
		m_arrayTicks.Add( PasswordTicks::PMSChangeRoomRate );
		break;
	}

	m_listTick.SubclassDlgItem ( IDC_LIST, this );
	m_listTick.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listTick.InsertColumn ( 0, "", LVCFMT_LEFT, 410 );
	m_listTick.SetLockItemZeroFlag( FALSE );

	m_listTick.ClearList();
	for ( int n = 0; n < m_arrayTicks.GetSize(); n++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = PasswordArray.GetTickName( m_arrayTicks[n] );
		item.m_nData = 0;
		item.SetTagState(0);
		m_listTick.AddItemToList( item );
	}

	SetTicks();
	SetTickEnables();

	return TRUE;  
}

/**********************************************************************/

void CPropPagePasswordTicks::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listTick.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePasswordTicks::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveTicks();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePasswordTicks::OnSetActive() 
{
	SetTicks();
	SetTickEnables();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePasswordTicks::OnToggleTick( NMHDR* pNMHDR, LRESULT* pResult)
{
	if ( m_listTick.GetTagStateChangedFlag() == TRUE )
	{
		SetTickEnables();
		m_listTick.Invalidate();
	}	
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePasswordTicks::SetTicks()
{
	CPasswordTickInfo info;
	for ( int n = 0; n < PASSWORD_FLAGS; n++ )
	{
		info.m_nTickNo = n;
		m_PasswordTemp.SetEnable( info, m_pPasswordToEdit -> GetEnable( info ) );
	}

	for ( int n = 0; n < m_arrayTicks.GetSize(); n++ )
	{
		m_listTick.SetItemTagState( n, m_PasswordTemp.GetEnable ( m_arrayTicks[n] ) ? 1 : 0 );
	}
}

/**********************************************************************/

void CPropPagePasswordTicks::SetTickEnables()
{
	for ( int n = 0; n < m_arrayTicks.GetSize(); n++ )
	{
		m_PasswordTemp.SetEnable( m_arrayTicks[n], ( m_listTick.GetItemTagState(n) == 1 ) );
	}

	for ( int n = 0; n < m_arrayTicks.GetSize(); n++ )
	{
		bool bEnable = TRUE;

		CPasswordTickInfo infoLink1, infoLink2;
		infoLink1.m_nTickNo = m_arrayTicks[n].m_nLink1;
		infoLink2.m_nTickNo = m_arrayTicks[n].m_nLink2;
		
		if ( infoLink1.m_nTickNo != -1 )
			if ( m_PasswordTemp.GetEnable( infoLink1 ) == FALSE )
				bEnable = FALSE;

		if ( infoLink2.m_nTickNo != -1 )
			if ( m_PasswordTemp.GetEnable( infoLink2 ) == FALSE )
				bEnable = FALSE;

		CSSListTaggedSelectItem item;
		m_listTick.GetListItem( n, item );
		item.m_bLocked = !bEnable;
		m_listTick.SetListItem( n, item );
	}
}

/**********************************************************************/

bool CPropPagePasswordTicks::UpdateRecord()
{
	SaveTicks();	
	return TRUE;
}

/**********************************************************************/

void CPropPagePasswordTicks::SaveTicks()
{
	for ( int n = 0; n < m_arrayTicks.GetSize(); n++ )
	{
		bool b = ( m_listTick.GetItemTagState( n ) == 1 );
		m_pPasswordToEdit -> SetEnable( m_arrayTicks[n], b );
	}
}

/**********************************************************************/

void CPropPagePasswordTicks::Refresh()
{
	SetTicks();
	SetTickEnables();
}

/**********************************************************************/

