/**********************************************************************/
#include "JobDownloadDlg.h"
#include "..\SPosEcrManager5\JobDownloadDlgEcrLite.h"
#include "JobEODDlg.h"
#include "JobListCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SelectMultipleLocationDlg.h"
#include "SelectMultipleMasterDlg.h"
#include "SelectMultipleNetworkDlg.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "JobListDlg.h"
/**********************************************************************/

CJobListDlg::CJobListDlg( CEposTaskDatabase& TaskDatabase, bool bIsAdhocTask, CWnd* pParent )
	: CSSDialog(CJobListDlg::IDD, pParent), m_TaskDatabase ( TaskDatabase )
{
	//{{AFX_DATA_INIT(CJobListDlg)
	m_bTime = FALSE;
	m_nImportType = -1;
	//}}AFX_DATA_INIT
	m_bIsAdhocTask = bIsAdhocTask;
	
	CEposTaskHeader TaskHeader;
	m_TaskDatabase.GetEditableTaskHeader( TaskHeader );
	
	m_strName = TaskHeader.GetName();
	
	switch( TaskHeader.GetImportType() )
	{
	case IMPORT_IMMEDIATE:
	case IMPORT_STARTOFDAY:
	case IMPORT_SPECIFIED:
		m_nImportType = TaskHeader.GetImportType();
		break;

	default:
		m_nImportType = IMPORT_IMMEDIATE;
		break;
	}

	m_TaskDatabase.TidyEditableTaskJobs();
}

/**********************************************************************/

void CJobListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobListDlg)
	DDX_Control(pDX, IDC_BUTTON_JOB_UPLOAD, m_buttonBatchSales);
	DDX_Control(pDX, IDC_CHECK_TIME, m_checkTime);
	DDX_Control(pDX, IDC_TIMEPICKER, m_TimePicker);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_BUTTON_JOB_SELECT_LOC, m_buttonSelectLoc);
	DDX_Control(pDX, IDC_BUTTON_JOB_DOWNLOAD, m_buttonDownload);
	DDX_Control(pDX, IDC_BUTTON_JOB_EOD, m_buttonEOD);
	DDX_Control(pDX, IDC_CHECK_SELECTWHENRUN, m_checkSelectWhenRun);
	DDX_Check(pDX, IDC_CHECK_TIME, m_bTime);
	DDX_Radio(pDX, IDC_RADIO_DOWNLOAD, m_nImportType);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName); 
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CJobListDlg, CDialog)
	//{{AFX_MSG_MAP(CJobListDlg)
	ON_BN_CLICKED(IDC_BUTTON_JOB_SELECT_LOC, OnButtonJobSelectLoc)
	ON_BN_CLICKED(IDC_BUTTON_JOB_DOWNLOAD, OnButtonJobDownload)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDeleteJob)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	ON_BN_CLICKED(IDC_BUTTON_JOB_UPLOAD, OnButtonJobUpload)
	ON_BN_CLICKED(IDC_BUTTON_JOB_EOD, OnButtonJobEOD)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CJobListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listJobs.SubclassDlgItem ( IDC_LIST, this );

	if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC )
		m_listJobs.InsertColumn ( 0, "Job", LVCFMT_LEFT, 530 );
	else
	{
		m_listJobs.InsertColumn ( 0, "Job", LVCFMT_LEFT, 400 );
		m_listJobs.InsertColumn ( 1, "Locs", LVCFMT_RIGHT, 130 );
	}

	AddList();
	
	if ( m_listJobs.GetItemCount() > 0 )
		m_listJobs.SetCurSel ( 0 );

	CEposTaskHeader TaskHeader;
	m_TaskDatabase.GetEditableTaskHeader( TaskHeader );
	
	if ( m_TaskDatabase.GetPluChangesMode() == TRUE )
	{
		m_editName.EnableWindow( FALSE );
		ShowAndEnableWindow( &m_checkSelectWhenRun, FALSE );
		SetWindowText( "Setup Plu Changes Task" );
	}
	else if ( TRUE == m_bIsAdhocTask )
	{
		m_editName.EnableWindow( FALSE );
		SetWindowText ( "Setup Adhoc Task" );
	}
	else
	{
		m_editName.EnableWindow( TRUE );
		SetWindowText ( "Setup Preset Task" );
	}

	m_checkSelectWhenRun.SetCheck( TaskHeader.GetSelectLocationsWhenRunFlag() );

	if ( TaskHeader.GetHours() == 24 )
	{
		CTime time ( 1999, 1, 1, 0, 0, 0, -1 );
		m_TimePicker.SetTime ( &time );
		m_TimePicker.SetFormat ( "' -- : --'" );
		m_TimePicker.EnableWindow ( FALSE );
		m_checkTime.SetCheck ( FALSE );
	}
	else
	{
		CTime time ( 1999, 1, 1, TaskHeader.GetHours(), TaskHeader.GetMinutes(), 0, -1 );
		m_TimePicker.SetTime ( &time );
		m_TimePicker.SetFormat ( "HH:mm" );
		m_checkTime.SetCheck ( TRUE );
	}
	
	m_buttonBatchSales.EnableWindow ( PasswordArray.GetEnable( PasswordTicks::CommsBatchSales ) );
	m_buttonDownload.EnableWindow ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) ); 

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		m_buttonEOD.ShowWindow( SW_HIDE );
		m_buttonEOD.EnableWindow( FALSE );
	}

	if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC )
	{
		m_buttonSelectLoc.ShowWindow( SW_HIDE );
		m_buttonSelectLoc.EnableWindow( FALSE );
	}

	m_bCanDoEODProduct = FALSE;
	if ( EcrmanOptions.GetPluProdCSVImportType() != 0 )
	{
		for ( int n = 0; ( n < dbNetwork.GetSize() ) && ( FALSE == m_bCanDoEODProduct ); n++ )
		{
			switch( dbNetwork.GetConnectionType(n) )
			{
			case CONNECTION_TYPE_STANDARD_REALTIME:
				m_bCanDoEODProduct = ( EcrmanOptions.GetPluProdCSVImportType() == 1 );
				break;

			case CONNECTION_TYPE_STANDARD_BATCH:
				m_bCanDoEODProduct = TRUE;
				break;
			}
		}
	}

	m_bCanDoEODCustomer = FALSE;
	if ( EcrmanOptions.GetFeaturesCustomerNameImportType() != 0 )
	{
		for ( int n = 0; ( n < dbNetwork.GetSize() ) && ( FALSE == m_bCanDoEODCustomer ); n++ )
		{
			switch( dbNetwork.GetConnectionType(n) )
			{
			case CONNECTION_TYPE_STANDARD_REALTIME:
				m_bCanDoEODCustomer = ( EcrmanOptions.GetFeaturesCustomerNameImportType() == 1 );
				break;

			case CONNECTION_TYPE_STANDARD_BATCH:
				m_bCanDoEODCustomer = TRUE;
				break;
			}
		}
	}

	if ( ( FALSE == m_bCanDoEODProduct ) && ( FALSE == m_bCanDoEODCustomer) && ( EcrmanOptions.GetReportsEODReasonsFlag() == FALSE ) )
	{
		m_buttonEOD.ShowWindow( SW_HIDE );
		m_buttonEOD.EnableWindow( FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

void CJobListDlg::OnButtonJobSelectLoc() 
{
	int nJobIdx = m_listJobs.GetCurSel();
	if ( nJobIdx < 0 || nJobIdx >= m_listJobs.GetItemCount() )
		return;

	CEposTaskJob TaskJob;
	m_TaskDatabase.GetEditableTaskJob( nJobIdx, TaskJob );

	switch( TaskJob.GetJobNo() )
	{
	case RX_SALES:
	case RX_REASON:
		{
			CReportConsolidationArray<CEposJobLocation> arrayNetworks;
			TaskJob.GetLocationArray( arrayNetworks );

			CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> Set;
			for ( int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++ )
			{
				CSelectMultipleEntityInfo infoSelect;
				infoSelect.m_nNwkIdx = nNwkIdx;
				
				CEposJobLocation Location;
				Location.m_nNetWkLocNo = dbNetwork.GetNetworkNo( nNwkIdx );
				Location.m_nTerminalNo = 0;
				
				int nPos;
				infoSelect.m_bSelected = TaskJob.GetAllFlag() || arrayNetworks.Find( Location, nPos );

				Set.Add( infoSelect );
			}

			if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_DB )
			{
				for ( int nNwkIdx = Set.GetSize() - 1; nNwkIdx >= 0; nNwkIdx-- )
				{
					int nStartIdx, nEndIdx;
					dbLocation.GetNetworkLocIdxRange( nNwkIdx, nStartIdx, nEndIdx );

					for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
					{
						if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == FALSE )
						{
							Set.RemoveAt( nNwkIdx );
							break;
						}
					}
				}

				if ( Set.GetSize() == 0 )
				{
					Prompter.Error ( "You are not authorised to request\nthis job for any network" );
					return;
				}
			}
			
			int nOldSize = Set.GetSize();
			CSelectMultipleNetworkDlg dlg( "Select Networks", Set, this );
			if ( dlg.DoModal() != IDOK )
				return;

			{
				for ( int n = Set.GetSize() - 1; n >= 0; n-- )
					if ( FALSE == Set[n].m_bSelected )
						Set.RemoveAt(n);
			}

			if ( Set.GetSize() == nOldSize )
			{
				TaskJob.SetAllFlag( TRUE );
				TaskJob.SetLocations( "" );
				m_TaskDatabase.SetEditableTaskJob( nJobIdx, TaskJob );
			}
			else
			{
				CReportConsolidationArray<CEposJobLocation> arrayNetworks;

				for ( int n = 0; n < Set.GetSize(); n++ )
				{
					CEposJobLocation Network;
					Network.m_nNetWkLocNo = dbNetwork.GetNetworkNo( Set[n].m_nNwkIdx );
					Network.m_nTerminalNo = 0;
					arrayNetworks.Consolidate( Network );
				}

				TaskJob.SetAllFlag( FALSE );
				TaskJob.SetLocationArray( arrayNetworks );
				m_TaskDatabase.SetEditableTaskJob( nJobIdx, TaskJob );
			}
		}
		break;

	default:
		{
			CReportConsolidationArray<CEposJobLocation> arrayLocations;
			TaskJob.GetLocationArray( arrayLocations );
			
			CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> Set;
			for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			{
				CArray<CLocationMasterInfo,CLocationMasterInfo> arrayMasters;
				LocationMasterMap.GetLocationMasterList( nLocIdx, arrayMasters, TRUE, FALSE );

				for ( int nIdx = 0; nIdx < arrayMasters.GetCount(); nIdx++ )
				{
					CLocationMasterInfo infoMaster = arrayMasters.GetAt(nIdx);
					
					CSelectMultipleEntityInfo infoSelect;

					if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), infoSelect.m_nNwkIdx ) == FALSE )
						infoSelect.m_nNwkIdx = -1;

					infoSelect.m_nLocIdx = nLocIdx;
					infoSelect.m_nTNo = infoMaster.GetTerminalNo();
					
					CEposJobLocation Location;
					Location.m_nNetWkLocNo = dbLocation.GetNetworkLocNo( nLocIdx );
					Location.m_nTerminalNo = infoMaster.GetTerminalNo();
				
					int nPos;
					infoSelect.m_bSelected = TaskJob.GetAllFlag() || arrayLocations.Find( Location, nPos );

					Set.Add( infoSelect );
				}
			}

			if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_DB )
			{
				for ( int nSetIdx = Set.GetSize() - 1; nSetIdx >= 0; nSetIdx-- )
					if ( PasswordArray.CheckLocationAccessLocIdx( Set[nSetIdx].m_nLocIdx ) == FALSE )
						Set.RemoveAt( nSetIdx );

				if ( Set.GetSize() == 0 )
				{
					Prompter.Error ( "You are not authorised to request\nthis job for any location" );
					return;
				}
			}
			
			int nOldSize = Set.GetSize();
			CSelectMultipleMasterDlg dlg( Set, this );
			if ( dlg.DoModal() != IDOK )
				return;

			{
				for ( int n = Set.GetSize() - 1; n >= 0; n-- )
					if ( FALSE == Set[n].m_bSelected )
						Set.RemoveAt(n);
			}

			if ( Set.GetSize() == nOldSize )
			{
				TaskJob.SetAllFlag( TRUE );
				TaskJob.SetLocations( "" );
				m_TaskDatabase.SetEditableTaskJob( nJobIdx, TaskJob );
			}
			else
			{
				TaskJob.SetAllFlag( FALSE );

				CReportConsolidationArray<CEposJobLocation> arrayLocations;

				for ( int n = 0; n < Set.GetSize(); n++ )
				{
					CEposJobLocation Location;
					Location.m_nNetWkLocNo = dbLocation.GetNetworkLocNo( Set[n].m_nLocIdx );
					Location.m_nTerminalNo = Set[n].m_nTNo;
					arrayLocations.Consolidate( Location );
				}

				TaskJob.SetLocationArray( arrayLocations );
				m_TaskDatabase.SetEditableTaskJob( nJobIdx, TaskJob );
			}
		}
		break;
	}

	m_listJobs.DeleteAllItems();
	AddList();
	m_listJobs.SetCurSel(nJobIdx);
}

/**********************************************************************/

void CJobListDlg::OnButtonJobDownload() 
{
#ifdef STOCKMAN_SYSTEM

	CJobDownloadDlg dlg ( m_TaskDatabase, this );
	dlg.DoModal();
	m_listJobs.DeleteAllItems();
	AddList();

#else

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CJobDownloadDlg dlg ( m_TaskDatabase, this );
		dlg.DoModal();
		m_listJobs.DeleteAllItems();
		AddList();
	}
	else
	{
		CJobDownloadDlgEcrLite dlg ( m_TaskDatabase, this );
		dlg.DoModal();
		m_listJobs.DeleteAllItems();
		AddList();
	}

#endif
}

/**********************************************************************/

void CJobListDlg::OnButtonJobUpload() 
{
	m_TaskDatabase.AddEditableTaskJob( RX_SALES );
	m_TaskDatabase.TidyEditableTaskJobs();
	m_listJobs.DeleteAllItems();
	AddList();
}

/**********************************************************************/

void CJobListDlg::OnButtonJobEOD() 
{
	bool bWantDialog = TRUE;
	
	if ( EcrmanOptions.GetReportsEODReasonsFlag() == FALSE )
	{
		if ( FALSE == m_bCanDoEODCustomer )
		{
			m_TaskDatabase.AddEditableTaskJob( RX_EODPLU );
			m_TaskDatabase.TidyEditableTaskJobs();
			m_listJobs.DeleteAllItems();
			AddList();
			bWantDialog = FALSE;
		}
		else if ( FALSE == m_bCanDoEODProduct )
		{
			m_TaskDatabase.AddEditableTaskJob( RX_EODCUSTOMER );
			m_TaskDatabase.TidyEditableTaskJobs();
			m_listJobs.DeleteAllItems();
			AddList();
			bWantDialog = FALSE;
		}
	}
	
	if ( TRUE == bWantDialog )
	{
		CJobEODDlg dlg( this );

		if ( dlg.DoModal() == IDOK )
		{
			if ( EcrmanOptions.GetReportsEODReasonsFlag() == TRUE )
			{
				if ( dlg.m_bReason != 0 )
				{
					int nParam1 = dlg.GetDateType();
					m_TaskDatabase.AddEditableTaskJob( RX_REASON, nParam1, TRUE );
				}
			}

			if ( TRUE == m_bCanDoEODProduct )
			{
				if ( dlg.m_bPlu != 0 )
					m_TaskDatabase.AddEditableTaskJob( RX_EODPLU );
			}

			if ( TRUE == m_bCanDoEODCustomer )
			{
				if ( dlg.m_bCustomer != 0 )
					m_TaskDatabase.AddEditableTaskJob( RX_EODCUSTOMER );
			}
		
			if ( ( dlg.m_bReason != 0 ) || ( dlg.m_bPlu != 0 ) || ( dlg.m_bCustomer != 0 ) )
			{
				m_TaskDatabase.TidyEditableTaskJobs();
				m_listJobs.DeleteAllItems();
				AddList();
			}
		}
	}
}

/**********************************************************************/

void CJobListDlg::OnButtonDeleteJob() 
{
	int nIndex = m_listJobs.GetCurSel();

	if ( nIndex >= 0 )
	{
		m_TaskDatabase.RemoveEditableTaskJob( nIndex );
		m_listJobs.DeleteAllItems();
		AddList();

		if ( nIndex < m_listJobs.GetItemCount() ) 
			m_listJobs.SetCurSel ( nIndex );
		else
			m_listJobs.SetCurSel ( nIndex - 1 );
	}
}

/**********************************************************************/

void CJobListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonJobSelectLoc();
	*pResult = 0;
}

/**********************************************************************/

void CJobListDlg::AddList()
{
	CString strMsg;

	CJobListCSVRecord JobRecord;
	for ( int nJobIdx = 0; nJobIdx < m_TaskDatabase.GetEditableTaskJobCount(); nJobIdx++ )
	{
		CEposTaskJob TaskJob;
		m_TaskDatabase.GetEditableTaskJob( nJobIdx, TaskJob );

		if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC )
		{
			strMsg = JobRecord.GetJobName( TaskJob.GetJobNo(), TaskJob.GetParam1BaseJob(), TaskJob.GetParam1ExtraJob() );
		}
		else
		{
			strMsg.Format ( "%s\t%s",
				JobRecord.GetJobName( TaskJob.GetJobNo(), TaskJob.GetParam1BaseJob(), TaskJob.GetParam1ExtraJob() ),
				TaskJob.GetLocText() );
		}

		m_listJobs.AddString ( strMsg );
	}
}
	
/**********************************************************************/

void CJobListDlg::OnCheckTime() 
{
	UpdateData( TRUE, FALSE );

	if ( m_bTime == FALSE )
	{
		m_TimePicker.SetFormat ( "' -- : --'" );
		m_TimePicker.EnableWindow ( FALSE );
	}
	else
	{
		m_TimePicker.SetFormat ( "HH:mm" );
		m_TimePicker.EnableWindow ( TRUE );
	}
}

/**********************************************************************/

void CJobListDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nHours = 24;
		int nMinutes = 0;
	
		if ( m_bTime != 0 )
		{
			CTime time;
			m_TimePicker.GetTime ( time );
			nHours = time.GetHour();
			nMinutes = time.GetMinute();
		}

		CEposTaskHeader TaskHeader;
		m_TaskDatabase.GetEditableTaskHeader( TaskHeader );
		TaskHeader.SetHours ( nHours );
		TaskHeader.SetMinutes ( nMinutes );

		::TrimSpaces ( m_strName, FALSE );
		
		if ( m_strName != "" )
			TaskHeader.SetName ( m_strName );
		else if ( m_bIsAdhocTask == FALSE )
		{
			CString strName;
			strName.Format ( "Preset %d", TaskHeader.GetTaskNo() - 128 );
			TaskHeader.SetName ( strName );
		}

		TaskHeader.SetSelectLocationsWhenRunFlag( m_checkSelectWhenRun.GetCheck() != 0 );
	
		TaskHeader.SetImportType( m_nImportType );
		m_TaskDatabase.SetEditableTaskHeader( TaskHeader );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CJobListDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDOK );	
	*pResult = 0;
}

/**********************************************************************/

void CJobListDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON_JOB_SELECT_LOC );
	*pResult = 0;
}

/**********************************************************************/
