/**********************************************************************/
#include "..\CommonEcrStock\ClearHistoryEposSales.h"
#include "ClearHistoryStockAudits.h"
#include "ClearHistoryStockSales.h"
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "NodeTypes.h"
#include "Password.h" 
/**********************************************************************/
#include "ClearHistoryDlgStockman.h"
/**********************************************************************/
static const int HISTORY_TYPE_NONE = 0;
static const int HISTORY_TYPE_EPOS_PLU = 1;
static const int HISTORY_TYPE_EPOS_SUMMARY = 2;
static const int HISTORY_TYPE_EPOS_DETAIL = 3;
static const int HISTORY_TYPE_STOCK_AUDITS = 4;
static const int HISTORY_TYPE_STOCK_SALES = 5;
/**********************************************************************/

CClearHistoryDlgStockman::CClearHistoryDlgStockman( CWnd* pParent)
	: CDialog(CClearHistoryDlgStockman::IDD, pParent),
	m_DateSelectorSingle( m_comboDates, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CClearHistoryDlgStockman)
	//}}AFX_DATA_INIT
	m_checkHistoryID[0] = IDC_CHECK_HISTORY1;
	m_checkHistoryID[1] = IDC_CHECK_HISTORY2;
	m_checkHistoryID[2] = IDC_CHECK_HISTORY3;
	m_checkHistoryID[3] = IDC_CHECK_HISTORY4;
	m_checkHistoryID[4] = IDC_CHECK_HISTORY5;
	m_nHistoryType[0] = HISTORY_TYPE_NONE;
	m_nHistoryType[1] = HISTORY_TYPE_NONE;
	m_nHistoryType[2] = HISTORY_TYPE_NONE;
	m_nHistoryType[3] = HISTORY_TYPE_NONE;
	m_nHistoryType[4] = HISTORY_TYPE_NONE;
}

/**********************************************************************/

void CClearHistoryDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearHistoryDlgStockman)
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	//}}AFX_DATA_MAP
	for ( int nIndex = 0; nIndex < 5; nIndex++ )
		DDX_Control(pDX, m_checkHistoryID[nIndex], m_checkHistory[nIndex]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CClearHistoryDlgStockman, CDialog)
	//{{AFX_MSG_MAP(CClearHistoryDlgStockman)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CClearHistoryDlgStockman::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_SYSTEM );
		m_LocationSelector.SetForceSystemFlag( TRUE );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_LOCATION );
		m_LocationSelector.SetSingleLocIdx( PasswordArray.GetLocationAccessLocIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );

	arrayTypes.RemoveAll();
	
	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add( NODE_STOCKPOINT );
		arrayTypes.Add( NODE_SITE );
		arrayTypes.Add( NODE_DATABASE );
		arrayTypes.Add ( NODE_SYSTEM );
		m_StockpointSelector.SetForceSystemFlag( TRUE );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add( NODE_STOCKPOINT );
		arrayTypes.Add( NODE_SITE );
		arrayTypes.Add( NODE_DATABASE );
		m_StockpointSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add( NODE_STOCKPOINT );
		m_StockpointSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_StockpointSelector.BuildList( arrayTypes );
	m_StockpointSelector.FillLocationCombo ( &m_comboStockpoint, NULL );

	m_DateSelectorSingle.InitialiseDateControl();
	m_DateSelectorSingle.AddStandardReportRanges( FALSE );
	
	CString strType;
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.GetEarliestReportDateString ( strType );
	m_DateSelectorSingle.SetComboSelection( 0, strType );

	int nPos = 0;
	if ( PasswordArray.GetEnable( PasswordTicks::AdminEposSales ) == TRUE ) 
	{
		m_checkHistory[nPos].SetWindowText ( "Delete Plu sales reports" );
		m_nHistoryType[nPos] = HISTORY_TYPE_EPOS_PLU;
		nPos++;

		m_checkHistory[nPos].SetWindowText ( "Delete transaction summary reports" );
		m_nHistoryType[nPos] = HISTORY_TYPE_EPOS_SUMMARY;
		nPos++;

		m_checkHistory[nPos].SetWindowText ( "Delete transaction detail reports" );
		m_nHistoryType[nPos] = HISTORY_TYPE_EPOS_DETAIL;
		nPos++;
	}

	for ( ; nPos < 3; nPos++ )
	{
		m_checkHistory[nPos].EnableWindow(FALSE);
		m_checkHistory[nPos].ShowWindow(SW_HIDE);
		m_nHistoryType[nPos] = HISTORY_TYPE_NONE;
	}

	nPos = 3;

	if ( PasswordArray.GetEnable( PasswordTicks::AdminStockSales ) == TRUE )
	{
		m_checkHistory[nPos].SetWindowText ( "Delete Stock sales history" );
		m_nHistoryType[nPos] = HISTORY_TYPE_STOCK_SALES;
		nPos++;
	}

	if ( PasswordArray.GetEnable( PasswordTicks::AdminStockAudits ) == TRUE )
	{
		m_checkHistory[nPos].SetWindowText ( "Delete Stock audits" );
		m_nHistoryType[nPos] = HISTORY_TYPE_STOCK_AUDITS;
		nPos++;
	}

	for ( ; nPos < 5; nPos++ )
	{
		m_checkHistory[nPos].EnableWindow(FALSE);
		m_checkHistory[nPos].ShowWindow(SW_HIDE);
		m_nHistoryType[nPos] = HISTORY_TYPE_NONE;
	}

	m_comboLocation.EnableWindow( ( IsEposAvailable() ) && ( m_comboLocation.GetCount() > 1 ) );
	m_comboStockpoint.EnableWindow( IsStockAvailable()&& ( m_comboStockpoint.GetCount() > 1 ) );

	return TRUE;  
}

/**********************************************************************/

void CClearHistoryDlgStockman::OnSelectLocation() 
{
	m_LocationSelector.FillEposCombo();	
}

/**********************************************************************/

void CClearHistoryDlgStockman::OnSelectDate() 
{
	m_DateSelectorSingle.UpdateDateControl();
}

/**********************************************************************/

void CClearHistoryDlgStockman::OnOK() 
{
	bool bGotWork = FALSE;
	for ( int nIndex = 0; nIndex < 5; nIndex++ )
	{
		if ( ( m_nHistoryType[ nIndex ] != HISTORY_TYPE_NONE ) && ( m_checkHistory[ nIndex ].GetCheck() != 0 ) )
		{
			bGotWork = TRUE;
			break;
		}
	}

	if ( bGotWork == FALSE )
	{
		Prompter.Error ( "You have not selected any report types to delete" );
		return;
	}

	COleDateTime time;
	m_DatePickerTo.GetTime ( time );
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDay = time.GetDay();

	CString strDate;
	m_comboDates.GetWindowText( strDate );
	
	if ( strDate.Find( "ustom", 0 ) != -1 )
		strDate.Format ( "%2.2d/%2.2d/%4.4d", nDay, nMonth, nYear );
	
	CString strTitle = "Delete reports before ";
	strTitle += strDate;
	
	CStringArray arrayWarning;
	arrayWarning.Add ( strTitle );

	bool bSpace = FALSE;
	for ( int nIndex = 0; nIndex < 5; nIndex++ )
	{
		if ( m_checkHistory[ nIndex ].GetCheck() == 0 )
			continue;
		
		switch ( m_nHistoryType [ nIndex ] )
		{
		case HISTORY_TYPE_EPOS_PLU:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Plu sales reports before this date will be deleted." );
			bSpace = TRUE;
			break;

		case HISTORY_TYPE_EPOS_SUMMARY:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Transaction summary reports before this date will be deleted." );
			bSpace = TRUE;
			break;

		case HISTORY_TYPE_EPOS_DETAIL:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Transaction detail reports before this date will be deleted." );
			bSpace = TRUE;
			break;

		case HISTORY_TYPE_STOCK_AUDITS:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Stock audits before this date will be deleted." );
			bSpace = TRUE;
			break;

		case HISTORY_TYPE_STOCK_SALES:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Stock sales history before this date will be deleted." );
			bSpace = TRUE;
			break;
		}
	}

	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;

	CLocationSelectorEntity LocSelEntityLoc;
	m_LocationSelector.GetSelectedEntity( LocSelEntityLoc );

	CEposSelectArray SelectArray;
	LocSelEntityLoc.GetEPOSSelectArray ( SelectArray );

	CLocationSelectorEntity LocSelEntitySp;
	m_StockpointSelector.GetSelectedEntity( LocSelEntitySp );
	
	bool bDeletePlu = FALSE;
	bool bDeleteSummary = FALSE;
	bool bDeleteDetail = FALSE;

	for ( int nIndex = 0; nIndex < 5; nIndex++ )
	{
		if ( m_checkHistory[ nIndex ].GetCheck() == 0 )
			continue;
		
		switch ( m_nHistoryType [ nIndex ] )
		{
		case HISTORY_TYPE_EPOS_PLU:
			bDeletePlu = TRUE;
			break;

		case HISTORY_TYPE_EPOS_SUMMARY:
			bDeleteSummary = TRUE;
			break;

		case HISTORY_TYPE_EPOS_DETAIL:
			bDeleteDetail = TRUE;
			break;

		case HISTORY_TYPE_STOCK_AUDITS:
			{
				CClearHistoryStockAudits( nYear, nMonth, nDay, LocSelEntitySp );
			}
			break;

		case HISTORY_TYPE_STOCK_SALES:
			{
				CClearHistoryStockSales( nYear, nMonth, nDay, LocSelEntitySp );
			}
			break;
		}
	}

	if ( ( TRUE == bDeletePlu ) || ( TRUE == bDeleteSummary ) || ( TRUE == bDeleteDetail ) )
	{
		COleDateTime dateTo = COleDateTime( nYear, nMonth, nDay, 0, 0, 0 );
		CClearHistoryEposSales( dateTo, LocSelEntityLoc, SelectArray, bDeletePlu, bDeleteSummary, bDeleteDetail, FALSE );
	}
	
	CString strMsg = "The selected report types were deleted for dates before ";
	strMsg += strDate;
	Prompter.Info( strMsg );
}

/**********************************************************************/

bool CClearHistoryDlgStockman::IsAvailable()
{
	return ( IsEposAvailable() || IsStockAvailable() );
}

/**********************************************************************/

bool CClearHistoryDlgStockman::IsEposAvailable()
{
	if ( PasswordArray.GetEnable( PasswordTicks::AdminEposSales ) == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CClearHistoryDlgStockman::IsStockAvailable()
{
	if ( PasswordArray.GetEnable( PasswordTicks::AdminStockSales ) == TRUE )
		return TRUE;

	if ( PasswordArray.GetEnable( PasswordTicks::AdminStockAudits ) == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

void CClearHistoryDlgStockman::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorSingle.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/