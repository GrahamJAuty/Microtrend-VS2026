/**********************************************************************/
 
/**********************************************************************/
#include <process.h>
/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "DateSelectorRange.h"
//#include "EcrmanOptionsIni.h"
#include "Password.h"
#include "PosTrayOptions.h"
 
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayBatchExportVoidDlg.h"
/**********************************************************************/

CPosTrayBatchExportVoidDlg::CPosTrayBatchExportVoidDlg( CPosTrayTask& BatchEmail, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPosTrayBatchExportVoidDlg::IDD, pParent), 
		m_BatchEmail( BatchEmail )
{
	//{{AFX_DATA_INIT(CPosTrayBatchExportVoidDlg)
	//}}AFX_DATA_INIT
	m_pFieldsDlg = NULL;
}

/**********************************************************************/

CPosTrayBatchExportVoidDlg::~CPosTrayBatchExportVoidDlg()
{
	if ( m_pFieldsDlg != NULL )
		delete m_pFieldsDlg;
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayBatchExportVoidDlg)
	
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_DATE1, m_staticDate1);
	DDX_Control(pDX, IDC_STATIC_DATE2, m_staticDate2);
	DDX_Control(pDX, IDC_STATIC_DATE3, m_staticDate3);
	DDX_Control(pDX, IDC_STATIC_DATE4, m_staticDate4);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_TIMESLICE, m_comboTimeSlice);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);

	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_STATIC_DATABASE1, m_staticDatabase1);
	DDX_Control(pDX, IDC_STATIC_DATABASE2, m_staticDatabase2);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayBatchExportVoidDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayBatchExportVoidDlg)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)

	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_ADD, OnButtonFieldAdd)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_CHANGE, OnButtonFieldChange)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_REMOVE, OnButtonFieldRemove)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_TIDY, OnButtonFieldTidy)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_UP, OnButtonFieldMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_DOWN, OnButtonFieldMoveDown)

	ON_MESSAGE( WM_APP, OnAddField)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayBatchExportVoidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_BatchEmail.GetExportFieldListVoid( m_arrayFieldTypes );

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.InsertColumn ( 1, "No", LVCFMT_LEFT, 40 );
	m_listFields.InsertColumn ( 2, "Type", LVCFMT_LEFT, 210 );
	m_listFields.InsertColumn ( 3, "Sub Type", LVCFMT_LEFT, 110 );
	m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.SetCurSel(0);
	
	FillTimeSliceCombo();
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_LOCATION );
		m_LocationSelector.SetSingleLocIdx( PasswordArray.GetLocationAccessLocIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );
	
	if ( PosTrayOptions.GetBusinessDateFlag() == FALSE )
	{
		m_comboDates.AddString ( szRANGE_TODAY );
		m_comboDates.AddString ( szRANGE_YESTERDAY );
	}
	else
	{
		m_comboDates.AddString ( szRANGE_THIS_BUSDAY );
		m_comboDates.AddString ( szRANGE_LAST_BUSDAY );
	}
	CDateSelectorRange::AddSingleDayRanges ( m_comboDates );
	CDateSelectorRange::AddPreviousDayRanges( m_comboDates );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	CDateSelectorRange::AddThisTaxQuarterRange( m_comboDates );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_THIS_FINANCIAL_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_WEEK );
	m_comboDates.AddString ( szRANGE_LAST_MONTH );
	CDateSelectorRange::AddLastTaxQuarterRange( m_comboDates );
	m_comboDates.AddString ( szRANGE_LAST_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_FINANCIAL_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDates.SetCurSel ( 0 );

	for ( int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++ )
		m_comboSession.AddString( DataManagerNonDb.ReportSession.GetName( nIndex ) );

	if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
		m_comboSession.AddString( "EOD Business Day" );

	if ( m_comboSession.GetCount() == 0 )
		m_comboSession.InsertString ( 0, "00:00 to 23:59" );
	
	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	//PLU SALE TIME
	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Item void time" );

	LoadEmailBatchSettings();

	return TRUE;  
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPosTrayBatchExportVoidDlg::PrepareLine( int nIndex )
{
	bool bResult = FALSE;
	m_strDisplayLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayFieldTypes.GetSize() ) )
	{
		CPosTrayExportFieldName Name;
		Name.m_nFieldType = m_arrayFieldTypes.GetAt( nIndex );
		DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );
		CreateFieldDisplayLine( nIndex, Name );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::CreateFieldDisplayLine( int nIndex, CPosTrayExportFieldName& Name )
{
	CCSV csv( '\t' );
	csv.Add( nIndex + 1 );
	csv.Add( Name.m_strDisplayName1 );
	csv.Add( Name.m_strDisplayName2 );
	m_strDisplayLine = csv.GetLine();
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonFieldChange();
	*pResult = 0;
}

/**********************************************************************/

bool CPosTrayBatchExportVoidDlg::SelectExistingItem( int nFieldType )
{
	bool bResult = FALSE;

	for ( int nFieldIdx = 0; nFieldIdx < m_arrayFieldTypes.GetSize(); nFieldIdx++ )
	{
		if ( m_arrayFieldTypes.GetAt( nFieldIdx ) == nFieldType )
		{
			m_listFields.SetCurSel( nFieldIdx );
			m_listFields.Invalidate();
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldAdd()
{
	if ( m_arrayFieldTypes.GetSize() >= POSTRAY_MAX_FIELDS_VOID )
	{
		Prompter.Error( "You have already added the maximum\nnumber of fields." );
	}
	else
	{
		if ( m_pFieldsDlg == NULL )
		{
			m_pFieldsDlg = new CPosTrayExportFieldDlg( POSTRAY_TASKFILE_EXPORT_VOID_TASK, &m_pFieldsDlg, this );
			m_pFieldsDlg -> Create( IDD_POSTRAY_EXPORT_FIELDS, this );
		}
	
		m_pFieldsDlg -> SetFocus();
	}
}

/**********************************************************************/

long CPosTrayBatchExportVoidDlg::OnAddField( WPARAM wIndex, LPARAM lParam )
{
	int nListSize = m_arrayFieldTypes.GetSize();
	
	if ( nListSize < POSTRAY_MAX_FIELDS_VOID )
	{
		int nInsertPos = 0;

		if ( nListSize != 0 )
		{
			nInsertPos = m_listFields.GetCurSel() + 1;
			if ( ( nInsertPos < 1 ) || ( nInsertPos > nListSize ) )
				nInsertPos = nListSize;
		}
				
		if ( SelectExistingItem( lParam ) == FALSE )
		{
			m_arrayFieldTypes.InsertAt( nInsertPos, lParam );
			m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
			m_listFields.SetCurSel( nInsertPos );
			m_listFields.Invalidate();
		}
	}	
	
	return 0l;
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldChange()
{
	int nSel = m_listFields.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		if ( m_pFieldsDlg != NULL )
		{
			 delete m_pFieldsDlg;
			 m_pFieldsDlg = NULL;
		}
		  
		CPosTrayExportFieldDlg dlg( POSTRAY_TASKFILE_EXPORT_VOID_TASK, m_arrayFieldTypes.GetAt( nSel ), this );

		if ( dlg.DoModal() == IDOK )
		{
			if ( SelectExistingItem( dlg.m_nFieldType ) == FALSE )
			{
				m_arrayFieldTypes.SetAt( nSel, dlg.m_nFieldType );
				m_listFields.Invalidate();
			}
		}
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldRemove()
{
	int nSel = m_listFields.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		m_arrayFieldTypes.RemoveAt( nSel );

		if ( nSel >= m_arrayFieldTypes.GetSize() )
			nSel--;

		m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
		m_listFields.SetCurSel( nSel );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldTidy()
{
	int nSel = m_listFields.GetCurSel();
	
	int nFieldType = 0;
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
		nFieldType = m_arrayFieldTypes.GetAt( nSel );

	m_BatchEmail.TidyExportFieldListPayment( m_arrayFieldTypes );
	m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
	
	if ( SelectExistingItem( nFieldType ) == FALSE )
	{
		m_listFields.SetCurSel(0);
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldMoveUp()
{
	int nSel = m_listFields.GetCurSel();
	
	if ( ( nSel >= 1 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		int nTemp = m_arrayFieldTypes.GetAt( nSel );
		m_arrayFieldTypes.SetAt( nSel, m_arrayFieldTypes.GetAt( nSel - 1 ) );
		m_arrayFieldTypes.SetAt( nSel - 1, nTemp );
		m_listFields.SetCurSel( nSel - 1 );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnButtonFieldMoveDown()
{
	int nSel = m_listFields.GetCurSel();
	
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() - 1 ) )
	{
		int nTemp = m_arrayFieldTypes.GetAt( nSel );
		m_arrayFieldTypes.SetAt( nSel, m_arrayFieldTypes.GetAt( nSel + 1 ) );
		m_arrayFieldTypes.SetAt( nSel + 1, nTemp );
		m_listFields.SetCurSel( nSel + 1 );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::LoadEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorLookup LocSelLookup ( m_BatchEmail );
		
		if ( NODE_LOCATION_TERMINAL == LocSelLookup.GetConType() )
		{
			LocSelLookup.SetConTypeToLocation();

			CLocationSelectorEntity LocSelEntity( LocSelLookup );
			m_LocationSelector.ForceSelection( LocSelEntity );
		
			int nTermIdx = LocSelEntity.GetTermIdx();
			if ( ( nTermIdx >= 0 ) && ( nTermIdx < m_comboTerminal.GetCount() ) )
				m_comboTerminal.SetCurSel( nTermIdx );
		}
		else	
		{
			CLocationSelectorEntity LocSelEntity( LocSelLookup );
			m_LocationSelector.ForceSelection( LocSelEntity );
		}
	}

	//BATCH NAME
	m_editName.SetWindowText( m_BatchEmail.GetBatchName() );

	//DATE TYPE
	int nResult = m_comboDates.FindStringExact( 0, m_BatchEmail.GetBatchDateType() );
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );

	//DATE ROUNDING
	int nRoundTo = m_BatchEmail.GetBatchDateRoundTo();
	if ( ( nRoundTo < 0 ) || ( nRoundTo > 2 ) ) nRoundTo = 0;
	m_comboRound.SetCurSel( nRoundTo );

	//REPORT SESSION
	{
		int nSel = 0;
		CString strSession = m_BatchEmail.GetBatchSessionEntity();
		
		if ( strSession == "" )
			strSession = m_BatchEmail.GetBatchSessionLegacy();

		if ( strSession == "EOD" )
		{
			if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
				nSel = DataManagerNonDb.ReportSession.GetSize();
		}
		else
		{
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord Session;
				DataManagerNonDb.ReportSession.GetAt( n, Session );

				if ( strSession == Session.GetEntityID() )
				{
					nSel = n;
					break;
				}
			}
		}

		if ( nSel >= m_comboSession.GetCount() )
			nSel = 0;

		m_comboSession.SetCurSel( nSel );
	}
	
	//TIME SLICE
	{
		int nSel = 0;
		CString strTimeSlice = m_BatchEmail.GetBatchTimeSlice();

		for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
		{
			CTimeSliceMapCSVRecord TimeSlice;
			DataManagerNonDb.TimeSliceList.GetAt( n, TimeSlice );

			if ( strTimeSlice == TimeSlice.GetEntityID() )
			{
				nSel = n;
				break;
			}
		}

		if ( nSel >= m_comboTimeSlice.GetCount() )
			nSel = 0;

		m_comboTimeSlice.SetCurSel( nSel );
	}

	nResult = m_comboTimeSlice.FindStringExact( 0, m_BatchEmail.GetBatchTimeSlice() );
	if ( nResult != CB_ERR )
		m_comboTimeSlice.SetCurSel( nResult );

	//PLU TIME
	m_comboPluTime.SetCurSel( ( m_BatchEmail.GetBatchSaleTimeFlag() == 1 ) ? 1 : 0 );

	//REPORT SELECTION
	//REPORT TYPES
	CCSV csvReports( m_BatchEmail.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayTemp;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayTemp.Consolidate( item );
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::SaveEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_BatchEmail.SetBatchConType( LocSelEntity.GetConType() );
		m_BatchEmail.SetBatchConDbNo( LocSelEntity.GetDbNo() );
		m_BatchEmail.SetBatchConNwkNo( LocSelEntity.GetNwkNo() );
		m_BatchEmail.SetBatchConEntityNo( LocSelEntity.GetEntityNo() );
		m_BatchEmail.SetBatchConTermIdx( LocSelEntity.GetTermIdx() );
	}

	//BATCH NAME
	CString strName;
	m_editName.GetWindowText( strName );
	m_BatchEmail.SetBatchName( strName );

	//DATE TYPE
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	m_BatchEmail.SetBatchDateType( strDateType );

	//DATE ROUND
	m_BatchEmail.SetBatchDateRoundTo( m_comboRound.GetCurSel() );

	//REPORT SESSION
	{
		int nSel = m_comboSession.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.ReportSession.GetSize() ) )
		{
			CSessionCSVRecord Session;
			DataManagerNonDb.ReportSession.GetAt( nSel, Session );
			m_BatchEmail.SetBatchSessionEntity( Session.GetEntityID() );
		}
		else if ( ( nSel == DataManagerNonDb.ReportSession.GetSize() ) && ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE ) )
			m_BatchEmail.SetBatchSessionEntity( "EOD" );
		else
			m_BatchEmail.SetBatchSessionEntity( "" );

		m_BatchEmail.SetBatchSessionLegacy( "" );
	}

	//TIME SLICE
	{
		int nSel = m_comboTimeSlice.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.TimeSliceList.GetSize() ) )
		{
			CTimeSliceMapCSVRecord TimeSlice;
			DataManagerNonDb.TimeSliceList.GetAt( nSel, TimeSlice );
			m_BatchEmail.SetBatchTimeSlice( TimeSlice.GetEntityID() );
		}
		else
			m_BatchEmail.SetBatchTimeSlice( "" );
	}

	//PLU TIME
	m_BatchEmail.SetBatchSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::FillTimeSliceCombo()
{
	m_comboTimeSlice.ResetContent();

	for ( int nPos = 0; nPos < DataManagerNonDb.TimeSliceList.GetSize(); nPos++ )
	{
		CTimeSliceMapCSVRecord ReportRecord;
		DataManagerNonDb.TimeSliceList.GetAt( nPos, ReportRecord );

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			break;

		case SH_TIMESLICE_SECOND:
		case SH_TIMESLICE_MINUTE:
		case SH_TIMESLICE_5MIN:
		case SH_TIMESLICE_10MIN:
		case SH_TIMESLICE_15MIN:
		case SH_TIMESLICE_30MIN:
		case SH_TIMESLICE_60MIN:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			break;
		}
	}

	m_comboTimeSlice.SetCurSel( 0 );
	if ( m_comboTimeSlice.GetCount() <= 1 )
		m_comboTimeSlice.EnableWindow( FALSE );
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnSelectLocation() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_LocationSelector.FillEposCombo();	

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
}

/**********************************************************************/

void CPosTrayBatchExportVoidDlg::OnOK()
{
	SaveEmailBatchSettings();
	m_BatchEmail.SetExportFieldListVoid( m_arrayFieldTypes );
	EndDialog( IDOK );
}

/**********************************************************************/
