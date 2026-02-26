/**********************************************************************/
#include "Password.h"
/**********************************************************************/
#include "CustomerAutoDeleter.h"
#include "MessageLogger.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "AutoDeleteCustomerDlg.h"
/**********************************************************************/

CAutoDeleteCustomerDlg::CAutoDeleteCustomerDlg( CWnd* pParent)
	: CSSDialog(CAutoDeleteCustomerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoDeleteCustomerDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoDeleteCustomerDlg)
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_CHECK_DAYS1, m_checkDays1);
	DDX_Control(pDX, IDC_CHECK_DAYS2, m_checkDays2);
	DDX_Control(pDX, IDC_STATIC_ONEOFF, m_staticOneOff);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT, m_staticAccount);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAutoDeleteCustomerDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoDeleteCustomerDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_CHECK_DAYS1, OnToggleDays1 )
	ON_BN_CLICKED( IDC_CHECK_DAYS2, OnToggleDays2 )
	ON_BN_CLICKED( IDC_BUTTON_LOG, OnButtonLog )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAutoDeleteCustomerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	SubclassEdit( IDC_EDIT_DAYS1, SS_NUM, 2, "%d" );
	SubclassEdit( IDC_EDIT_DAYS2, SS_NUM, 2, "%d" );

	m_checkDays1.EnableWindow( TRUE );
	m_checkDays2.EnableWindow( TRUE );
	m_checkDays1.SetCheck( FALSE );
	m_checkDays2.SetCheck( FALSE );
	GetEditDays1() -> EnableWindow( FALSE );
	GetEditDays2() -> EnableWindow( FALSE );
	m_staticOneOff.EnableWindow( FALSE );
	m_staticAccount.EnableWindow( FALSE );
	GetEditDays1() -> SetWindowText( "" );
	GetEditDays2() -> SetWindowText( "" );

	if ( EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == FALSE )
	{
		m_checkDays1.EnableWindow( FALSE );	
	}
	
	if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == FALSE )
	{
		m_checkDays2.EnableWindow( FALSE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::EditCustomersDelDay ) == FALSE )
	{
		m_checkDays1.EnableWindow( FALSE );
		m_checkDays2.EnableWindow( FALSE );
	}
	
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
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );

	return TRUE;  
}

/**********************************************************************/
CEdit* CAutoDeleteCustomerDlg::GetEditDays1(){ return GetEdit( IDC_EDIT_DAYS1 ); }
CEdit* CAutoDeleteCustomerDlg::GetEditDays2(){ return GetEdit( IDC_EDIT_DAYS2 ); }
/**********************************************************************/

void CAutoDeleteCustomerDlg::OnToggleDays1()
{
	if ( m_checkDays1.GetCheck() != 0 )
	{
		GetEditDays1() -> EnableWindow( TRUE );
		m_staticOneOff.EnableWindow( TRUE );

		CString strNum;
		strNum.Format( "%d", EcrmanOptions.GetFeaturesCustomerOneOffDelDay() );
		GetEditDays1() -> SetWindowText( strNum );
	}
	else
	{
		GetEditDays1() -> EnableWindow( FALSE );
		m_staticOneOff.EnableWindow( FALSE );
		GetEditDays1() -> SetWindowText( "" );
	}
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::OnToggleDays2()
{
	if ( m_checkDays2.GetCheck() != 0 )
	{
		GetEditDays2() -> EnableWindow( TRUE );
		m_staticAccount.EnableWindow( TRUE );

		CString strNum;
		strNum.Format( "%d", EcrmanOptions.GetFeaturesCustomerAccountDelDay() );
		GetEditDays2() -> SetWindowText( strNum );
	}
	else
	{
		GetEditDays2() -> EnableWindow( FALSE );
		m_staticAccount.EnableWindow( FALSE );
		GetEditDays2() -> SetWindowText( "" );
	}
}

/**********************************************************************/

int CAutoDeleteCustomerDlg::CheckDeletionDates( CCustomerCSVArray& Customers, int nRetainDays )
{
	int nChangeCount = 0;
	for ( int nCustIdx = 0; nCustIdx < Customers.GetSize(); nCustIdx++ )
	{
		CCustomerCSVRecord CustRecord;
		Customers.GetAt( nCustIdx, CustRecord );

		if ( CustRecord.GetAutoDeleteFlag() == TRUE )
		{
			int nCreateDay = CustRecord.GetLatestCreationDay();
			if ( nCreateDay != 0 )
			{
				int nNewAutoDeleteDay = nCreateDay + nRetainDays;

				if ( nNewAutoDeleteDay != CustRecord.GetAutoDeleteDay() )
				{
					CustRecord.SetAutoDeleteDay( nNewAutoDeleteDay );
					Customers.SetAt( nCustIdx, CustRecord );
					nChangeCount++;
				}
			}
		}
	}

	return nChangeCount;
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::OnButtonLog() 
{
	CFilenameUpdater FnUp ( SysFiles::AutoDeleteCustLogPrm );

	if ( ::FileExists ( Super.AutoDeleteCustLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.AutoDeleteCustLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Auto Delete Customer Log file was not found." );
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::LogMessage( const char* szMsg )
{
	LogMessage( -1, FALSE, szMsg );
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::LogMessage( int nLocIdx, bool bOneOff, const char* szMsg )
{
	CAutoDeleteCustLogInfo infoLog;
	infoLog.m_bOneOff = bOneOff;
	infoLog.m_nHideLevel  = 0;
	infoLog.m_nLocIdx = nLocIdx;
	infoLog.m_strInfo = szMsg; 	
	MessageLogger.LogAutoDeleteCustomer( infoLog );
}

/**********************************************************************/

void CAutoDeleteCustomerDlg::OnOK() 
{	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	bool bChangeOneOff = FALSE;
	int nOneOffRetainDays = 0;

	if ( m_checkDays1.GetCheck() != 0 )
	{
		bChangeOneOff = TRUE;

		CString strNum;
		GetEditDays1() -> GetWindowText( strNum );
		nOneOffRetainDays = atoi( strNum );

		if ( nOneOffRetainDays <= 0 )
		{
			Prompter.Error( "Please enter a number between 1 snd 99" );
			GetEditDays1() -> SetSel( 0, -1 );
			GetEditDays1() -> SetFocus();
			return;
		}
	}

	bool bChangeAccount = FALSE;
	int nAccountRetainDays = 0;

	if ( m_checkDays2.GetCheck() != 0 )
	{
		bChangeAccount = TRUE;

		CString strNum;
		GetEditDays2() -> GetWindowText( strNum );
		nAccountRetainDays = atoi( strNum );

		if ( nAccountRetainDays <= 0 )
		{
			Prompter.Error( "Please enter a number between 1 snd 99" );
			GetEditDays2() -> SetSel( 0, -1 );
			GetEditDays2() -> SetFocus();
			return;
		}
	}

	{
		CStringArray arrayWarning;

		CString strTitle;
		strTitle.Format( "Auto Delete Customers (%s)", LocSelEntity.GetEntityName() );
		arrayWarning.Add( strTitle );
		
		if  ( ( TRUE == bChangeOneOff ) || ( TRUE == bChangeAccount ) )
		{
			arrayWarning.Add( "The auto-deletion date for existing customers will be changed as below:" );

			if ( TRUE == bChangeOneOff )
			{
				CString str;
				str.Format( "+One Off Customers => %d day%s after their creation date.",
					nOneOffRetainDays, ( 1 == nOneOffRetainDays ) ? "" : "s" ); 

				arrayWarning.Add( str );
			}

			if ( TRUE == bChangeAccount )
			{
				CString str;
				str.Format( "+Account Customers => %d day%s after their creation date.",
					nAccountRetainDays, ( 1 == nAccountRetainDays ) ? "" : "s" ); 

				arrayWarning.Add( str );
			}

			arrayWarning.Add( "" );
		}

		arrayWarning.Add ( "Customer records which have reached or passed their auto-deletion date will be deleted." );

		if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
			return;
	}

	CEposSelectArray SelectArray;
	LocSelEntity.GetEPOSSelectArray( SelectArray );
	SelectArray.MakeList( FALSE, TRUE );

	LogMessage( "<LI>" );
	LogMessage( "User requested deletion check started" );

	if ( TRUE == bChangeOneOff )
	{
		CString strMsg;
		strMsg.Format( "Set auto delete period for one-off customers to %d day", nOneOffRetainDays );
		if ( nOneOffRetainDays != 1 ) strMsg += "s";
		LogMessage( strMsg );
	}

	if ( TRUE == bChangeAccount )
	{
		CString strMsg;
		strMsg.Format( "Set auto delete period for account customers to %d day", nAccountRetainDays );
		if ( nAccountRetainDays!= 1 ) strMsg += "s";
		LogMessage( strMsg );
	}

	bool bGotError = FALSE;
	for ( int n = 0; n < SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem item;
		SelectArray.GetListItem( n, item );
		int nLocIdx = item.m_nLocIdx;

		if ( EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == TRUE )
		{
			CDataManagerInfo info;
			if ( DataManagerNonDb.OpenCustomerOneOff( DB_READWRITE, nLocIdx, info ) == FALSE )
			{
				LogMessage( nLocIdx, TRUE, "Unable to open customer file" );
				bGotError = TRUE;	
			}
			else
			{
				DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
				DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();

				if ( TRUE == bChangeOneOff )
				{
					int nChangeCount = CheckDeletionDates( DataManagerNonDb.CustomerOneOff, nOneOffRetainDays );
					if ( nChangeCount != 0 )
					{
						{
							CString str;
							str.Format( "Changed deletion date for %d customer", nChangeCount );
							if ( nChangeCount != 1 ) str += "s";
							LogMessage( nLocIdx, TRUE, str );
						}

						DataManagerNonDb.CustomerOneOff.SaveHeaderStartDateRecord();
						DataManagerNonDb.CustomerOneOff.MoveDeletionsToMainArray();
						DataManagerNonDb.WriteCustomerOneOff( info );
						DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
						DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();
					}
				}

				CustomerAutoDeleterOneOff.DeleteCustomerExternal( nLocIdx );
				DataManagerNonDb.CloseCustomerOneOff( info );
			}
		}

		if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
		{
			CDataManagerInfo info;
			if ( DataManagerNonDb.OpenCustomerAccount( DB_READWRITE, nLocIdx, info ) == FALSE )
			{
				LogMessage( nLocIdx, FALSE, "Unable to open customer file" );
				bGotError = TRUE;					
			}
			else
			{
				DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
				DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();

				if ( TRUE == bChangeAccount )
				{
					int nChangeCount = CheckDeletionDates( DataManagerNonDb.CustomerAccount, nAccountRetainDays ); 
					if ( nChangeCount != 0 )
					{
						{
							CString str;
							str.Format( "Changed deletion date for %d customer", nChangeCount );
							if ( nChangeCount != 1 ) str += "s";
							LogMessage( nLocIdx, FALSE, str );
						}

						DataManagerNonDb.CustomerAccount.SaveHeaderStartDateRecord();
						DataManagerNonDb.CustomerAccount.MoveDeletionsToMainArray();
						DataManagerNonDb.WriteCustomerAccount( info );
						DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
						DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
					}
				}

				CustomerAutoDeleterAccount.DeleteCustomerExternal( nLocIdx );
				DataManagerNonDb.CloseCustomerAccount( info );
			}
		}
	}

	LogMessage( "User requested deletion check complete" );
	LogMessage( "<LI>" );

	CString strInfo = "";

	if ( TRUE == bGotError )
	{
		strInfo += "Customer updates have been attempted as requested";
		strInfo += "\nOne of more customer files could not be opened";
	}
	else
	{
		strInfo += "Customer updates have been processed as requested";
		
	}

	strInfo += "\nPlease see the log for more details";


	Prompter.Info( strInfo );
}

/**********************************************************************/
