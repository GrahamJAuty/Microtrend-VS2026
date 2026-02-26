/**********************************************************************/
#include "ClearHistoryEposSales.h"
#include "..\SPosEcrManager5\ClearHistoryPluDelivery.h"
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "NodeTypes.h"
#include "Password.h" 
/**********************************************************************/
#include "ClearHistoryDlgEcrman.h"
/**********************************************************************/
static const int HISTORY_TYPE_NONE = 0;
static const int HISTORY_TYPE_EPOS_PLU = 1;
static const int HISTORY_TYPE_EPOS_SUMMARY = 2;
static const int HISTORY_TYPE_EPOS_DETAIL = 3;
static const int HISTORY_TYPE_PLU_DELIVERY = 4;
/**********************************************************************/

CClearHistoryDlgEcrman::CClearHistoryDlgEcrman( CWnd* pParent)
	: CDialog(CClearHistoryDlgEcrman::IDD, pParent),
	m_DateSelectorSingle( m_comboDates, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CClearHistoryDlgEcrman)
	//}}AFX_DATA_INIT
	m_checkHistoryID[0] = IDC_CHECK_HISTORY1;
	m_checkHistoryID[1] = IDC_CHECK_HISTORY2;
	m_checkHistoryID[2] = IDC_CHECK_HISTORY3;
	m_checkHistoryID[3] = IDC_CHECK_HISTORY4;
	m_nHistoryType[0] = HISTORY_TYPE_NONE;
	m_nHistoryType[1] = HISTORY_TYPE_NONE;
	m_nHistoryType[2] = HISTORY_TYPE_NONE;
	m_nHistoryType[3] = HISTORY_TYPE_NONE;
}

/**********************************************************************/

void CClearHistoryDlgEcrman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearHistoryDlgEcrman)
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	//}}AFX_DATA_MAP
	for ( int nIndex = 0; nIndex < 4; nIndex++ )
		DDX_Control(pDX, m_checkHistoryID[nIndex], m_checkHistory[nIndex]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CClearHistoryDlgEcrman, CDialog)
	//{{AFX_MSG_MAP(CClearHistoryDlgEcrman)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CClearHistoryDlgEcrman::OnInitDialog() 
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

#ifndef STOCKMAN_SYSTEM

	if ( ( EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE ) && ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE ) && ( PasswordArray.GetEnable( PasswordTicks::AdminPluDelivery ) == TRUE ) )
	{
		m_checkHistory[nPos].SetWindowText ( "Delete Plu delivery audit" );
		m_nHistoryType[nPos] = HISTORY_TYPE_PLU_DELIVERY;
		nPos++;
	}

#endif

	for ( ; nPos < 4; nPos++ )
	{
		m_checkHistory[nPos].EnableWindow(FALSE);
		m_checkHistory[nPos].ShowWindow(SW_HIDE);
		m_nHistoryType[nPos] = HISTORY_TYPE_NONE;
	}

	return TRUE;  
}

/**********************************************************************/

void CClearHistoryDlgEcrman::OnSelectLocation() 
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_LocationSelector.FillEposCombo();	

#ifndef STOCKMAN_SYSTEM

	for ( int nPos = 0; nPos < 4; nPos++ )
	{
		if ( m_nHistoryType[nPos] == HISTORY_TYPE_PLU_DELIVERY )
		{
			switch( LocSelEntity.GetConType() )
			{
			case NODE_DATABASE:
			case NODE_SYSTEM:
				m_checkHistory[ nPos ].EnableWindow( TRUE );
				break;

			default:
				m_checkHistory[ nPos ].SetCheck( FALSE );
				m_checkHistory[ nPos ].EnableWindow( FALSE );
				break;
			}
		}
	}

#endif
}

/**********************************************************************/

void CClearHistoryDlgEcrman::OnSelectDate() 
{
	m_DateSelectorSingle.UpdateDateControl();
}

/**********************************************************************/

void CClearHistoryDlgEcrman::OnOK() 
{
	bool bGotWork = FALSE;
	for ( int nIndex = 0; nIndex < 4; nIndex++ )
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
	for ( int nIndex = 0; nIndex < 4; nIndex++ )
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


#ifndef STOCKMAN_SYSTEM

		case HISTORY_TYPE_PLU_DELIVERY:
			if ( bSpace == TRUE ) arrayWarning.Add ( "" );
			arrayWarning.Add ( "Plu delivery audits before this date will be deleted." );
			bSpace = TRUE;
			break;

#endif
		}
	}

	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CEposSelectArray SelectArray;
	LocSelEntity.GetEPOSSelectArray( SelectArray );

	bool bDeletePlu = FALSE;
	bool bDeleteSummary = FALSE;
	bool bDeleteDetail = FALSE;

	for ( int nIndex = 0; nIndex < 4; nIndex++ )
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

#ifndef STOCKMAN_SYSTEM

		case HISTORY_TYPE_PLU_DELIVERY:
			{
				CClearHistoryPluDelivery ( nYear, nMonth, nDay, LocSelEntity );
			}
			break;

#endif
		}
	}

	if ( ( TRUE == bDeletePlu ) || ( TRUE == bDeleteSummary ) || ( TRUE == bDeleteDetail ) )
	{
		COleDateTime dateTo = COleDateTime( nYear, nMonth, nDay, 0, 0, 0 );
		CClearHistoryEposSales( dateTo, LocSelEntity, SelectArray, bDeletePlu, bDeleteSummary, bDeleteDetail, FALSE );
	}
		
	CString strMsg = "The selected report types were deleted for dates before ";
	strMsg += strDate;
	Prompter.Info( strMsg );
}

/**********************************************************************/

bool CClearHistoryDlgEcrman::IsAvailable()
{
	if ( PasswordArray.GetEnable( PasswordTicks::AdminEposSales ) == TRUE )
		return TRUE;

#ifndef STOCKMAN_SYSTEM

	if ( ( EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE ) && ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE ) && ( PasswordArray.GetEnable( PasswordTicks::AdminPluDelivery ) == TRUE ) )
		return TRUE;

#endif

	return FALSE;
}

/**********************************************************************/

void CClearHistoryDlgEcrman::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorSingle.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/