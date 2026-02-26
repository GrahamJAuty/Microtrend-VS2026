/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "DefEditRoomTypeDlg.h"
#include "PMSOptions.h"
#include "PMSRoomCSVArray.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "DefEditRoomDlg.h"
/**********************************************************************/

CDefEditRoomDlg::CDefEditRoomDlg( int nIndex, bool bAdd, CWnd* pParent)
	: CSSDialog(CDefEditRoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditRoomDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_bAdd = bAdd;
	m_pPropPage = NULL;
	m_nSize = DataManagerNonDb.PMSRoom.GetSize();
	m_nSleeps = 1;

	for ( int n = 0; n < 3; n++ )
	{
		m_dStandardRate[n] = 0.0;
		m_dSingleRate[n] = 0.0;
	}
	m_dExtraBed = 0.0;
	m_dExtraCot = 0.0;

	m_IDStaticRate[0] = IDC_STATIC_RATE1;
	m_IDStaticRate[1] = IDC_STATIC_RATE2;
	m_IDStaticRate[2] = IDC_STATIC_RATE3;

	m_IDStandardRate[0] = IDC_EDIT_STANDARD_RATE1;
	m_IDStandardRate[1] = IDC_EDIT_STANDARD_RATE2;
	m_IDStandardRate[2] = IDC_EDIT_STANDARD_RATE3;

	m_IDSingleRate[0] = IDC_EDIT_SINGLE_RATE1;
	m_IDSingleRate[1] = IDC_EDIT_SINGLE_RATE2;
	m_IDSingleRate[2] = IDC_EDIT_SINGLE_RATE3;

	if ( PMSOptions.GetMiscEnableRateRoom() == TRUE ) m_arrayRateTypes.Add( PMS_BOOKING_RATE_ROOM );
	if ( PMSOptions.GetMiscEnableRateBB() == TRUE ) m_arrayRateTypes.Add( PMS_BOOKING_RATE_BB );
	if ( PMSOptions.GetMiscEnableRateDBB() == TRUE ) m_arrayRateTypes.Add( PMS_BOOKING_RATE_DBB );
}

/**********************************************************************/

void CDefEditRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditRoomDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_SPIN, m_buttonSpin);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, PMSRoom::RoomName.Max);
	DDX_Text(pDX, IDC_EDIT_SLEEPS, m_nSleeps);
	DDV_MinMaxInt(pDX, m_nSleeps, PMSRoom::Sleeps.Min, PMSRoom::Sleeps.Max);
	DDX_Control(pDX, IDC_CHECK_SINGLE, m_checkSingle);
	DDX_Control(pDX, IDC_CHECK_EXTRABED, m_checkExtraBed);
	DDX_Control(pDX, IDC_CHECK_EXTRACOT, m_checkExtraCot);

	for ( int n = 0; n < 3; n++ )
	{
		DDX_Control( pDX, m_IDStaticRate[n], m_staticRate[n] );

		DDX_Text(pDX, m_IDStandardRate[n], m_dStandardRate[n]);
		DDV_MinMaxDouble(pDX, m_dStandardRate[n], PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max);

		DDX_Text(pDX, m_IDSingleRate[n], m_dSingleRate[n]);
		DDV_MinMaxDouble(pDX, m_dSingleRate[n], PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max);
	}

	DDX_Text(pDX, IDC_EDIT_EXTRABED, m_dExtraBed);
	DDV_MinMaxDouble(pDX, m_dExtraBed, PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max);

	DDX_Text(pDX, IDC_EDIT_EXTRACOT, m_dExtraCot);
	DDV_MinMaxDouble(pDX, m_dExtraCot, PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditRoomDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON_GROUP, OnButtonGroup)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_CHECK_SINGLE, OnToggleRates)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectType)
	ON_EN_KILLFOCUS(IDC_EDIT_SLEEPS, OnKillFocusSleeps)
	ON_BN_CLICKED(IDC_CHECK_EXTRABED, OnToggleExtraBed)
	ON_BN_CLICKED(IDC_CHECK_EXTRACOT, OnToggleExtraCot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditRoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit( IDC_EDIT_SLEEPS, SS_NUM, 1, "%d" );

	for ( int n = 0; n < 3; n++ )
	{
		SubclassEdit( m_IDStandardRate[n], SS_NUM_DP, 6, "%.2f" );
		SubclassEdit( m_IDSingleRate[n], SS_NUM_DP, 6, "%.2f" );
	}

	SubclassEdit( IDC_EDIT_EXTRABED, SS_NUM_DP, 6, "%.2f" );
	SubclassEdit( IDC_EDIT_EXTRACOT, SS_NUM_DP, 6, "%.2f" );

	if ( ( TRUE == m_bAdd ) || ( m_nSize <= 1 ) )
	{
		m_buttonSpin.EnableWindow( FALSE );
		m_buttonSpin.ShowWindow( SW_HIDE );
	}

	for ( int n = 0; n < 3; n++ )
	{
		if ( m_arrayRateTypes.GetSize() > n )
		{
			switch( m_arrayRateTypes.GetAt(n) )
			{
			case PMS_BOOKING_RATE_ROOM:	m_staticRate[n].SetWindowText( "Room Only" );	break;
			case PMS_BOOKING_RATE_BB:	m_staticRate[n].SetWindowText( "B && B" );		break;
			case PMS_BOOKING_RATE_DBB:	m_staticRate[n].SetWindowText( "DB && B" );		break;
			}
		}
		else
		{
			m_staticRate[n].ShowWindow( SW_HIDE );
			GetEditStandardRate(n) -> ShowWindow( SW_HIDE );
			GetEditStandardRate(n) -> EnableWindow( FALSE );
			GetEditSingleRate(n) -> ShowWindow( SW_HIDE );
			GetEditSingleRate(n) -> EnableWindow( FALSE );
		}
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditRoomDlg::GetEditSleeps(){ return GetEdit( IDC_EDIT_SLEEPS ); }
CEdit* CDefEditRoomDlg::GetEditStandardRate( int n ){ return GetEdit( m_IDStandardRate[n] ); }
CEdit* CDefEditRoomDlg::GetEditSingleRate( int n ){ return GetEdit( m_IDSingleRate[n] ); }
CEdit* CDefEditRoomDlg::GetEditExtraBed(){ return GetEdit( IDC_EDIT_EXTRABED ); }
CEdit* CDefEditRoomDlg::GetEditExtraCot(){ return GetEdit( IDC_EDIT_EXTRACOT ); }
/**********************************************************************/

void CDefEditRoomDlg::FillRoomTypeCombo( int nRoomSleeps, int nRoomType )
{
	m_comboType.ResetContent();

	for ( int nSleeps = 1; nSleeps <= PMSRoom::Sleeps.Max; nSleeps++ )
	{
		CCSV csv( DataManagerNonDb.PMSRoomType.GetRoomTypes( nSleeps ) );

		int nPos = m_comboType.AddString( csv.GetString(0) );
		m_comboType.SetItemData( nPos, nSleeps * 100 );

		for ( int nType = 1; nType < 6; nType++ )
		{
			if ( csv.GetBool( ( nType * 2 ) + 1 ) == TRUE )
			{
				int nPos = m_comboType.AddString( csv.GetString( nType * 2 ) );
				m_comboType.SetItemData( nPos, ( nSleeps * 100 ) + nType );
			}
		}
	}

	int nSel = -1;
	int nTarget = ( nRoomSleeps * 100 ) + nRoomType;

	for ( int n = 0; n < m_comboType.GetCount(); n++ )
	{
		int nItemData = m_comboType.GetItemData(n);

		if ( ( nSel == -1 ) && ( nItemData / 100 == nRoomSleeps ) )
			nSel = n;
		
		if ( nItemData == nTarget )
		{
			nSel = n;
			break;
		}
	}

	if ( nSel == -1 )
		nSel = 0;

	m_comboType.SetCurSel( nSel );
}

/**********************************************************************/

void CDefEditRoomDlg::FillRoomGroupCombo( int nSel )
{
	m_comboGroup.ResetContent();
	for ( int n = 1; n <= 5; n++ )
		m_comboGroup.AddString( DataManagerNonDb.PMSRoomGroup.GetRoomGroupName(n) );

	if ( ( nSel >= 0 ) && ( nSel < m_comboGroup.GetCount() ) )
		m_comboGroup.SetCurSel(nSel);
	else
		m_comboGroup.SetCurSel(0);
}

/**********************************************************************/

void CDefEditRoomDlg::OnSelectType()
{
	int nSel = m_comboType.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < m_comboType.GetCount() ) )
	{
		int nType = m_comboType.GetItemData( nSel );
		CString str;
		str.Format( "%d", nType / 100 );
		GetEditSleeps() -> SetWindowText( str );
	}
}

/**********************************************************************/

void CDefEditRoomDlg::OnKillFocusSleeps()
{
	CString str;
	GetEditSleeps() -> GetWindowText( str );
	int nSleeps = atoi( str );

	int nSel = m_comboType.GetCurSel();
	int nItemData = m_comboType.GetItemData( nSel );

	if ( ( nItemData / 100 ) != nSleeps )
	{
		FillRoomTypeCombo( nSleeps, 0 );
		OnSelectType();
	}
}

/**********************************************************************/

void CDefEditRoomDlg::OnButtonSet() 
{
	int nSel = m_comboType.GetCurSel();
	int nItemData = m_comboType.GetItemData( nSel );

	int nSleeps = ( nItemData / 100 );
	int nType = ( nItemData % 100 );

	if ( ( nSleeps >= 1 ) && ( nSleeps <= 6 ) )
	{
		CString strLineOld = DataManagerNonDb.PMSRoomType.GetRoomTypes( nSleeps );
		CString strLineNew = strLineOld;

		CDefEditRoomTypeDlg dlg ( nSleeps, strLineNew, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			DataManagerNonDb.PMSRoomType.SetRoomTypes( nSleeps, strLineNew );

			CFilenameUpdater FnUp( SysFiles::PMSRoomType );
			if ( DataManagerNonDb.PMSRoomType.Write( FnUp.GetFilenameToUse() ) == FALSE )
			{
				Prompter.Error( "Unable to write Guest Account room type file" );
				DataManagerNonDb.PMSRoomType.SetRoomTypes( nSleeps, strLineOld );
			}
			else
				FillRoomTypeCombo( nSleeps, nType );
		}
	}
}

/**********************************************************************/

void CDefEditRoomDlg::OnButtonGroup() 
{
	int nGroup = m_comboGroup.GetCurSel() + 1;
	if ( ( nGroup >= 1 ) && ( nGroup <= 5 ) )
	{
		CChangeRoomGroupNameDlg dlgName( nGroup, this );
		if ( dlgName.DoModal() == IDOK )
		{
			CString strOldName = DataManagerNonDb.PMSRoomGroup.GetRoomGroupName( nGroup );

			CString strNewName = dlgName.m_strName;
			::TrimSpaces( strNewName, FALSE );

			if ( strNewName == "" )
			{
				if ( 1 == nGroup )
					strNewName = "Standard";
				else
					strNewName.Format( "Group %d", nGroup );
			}

			DataManagerNonDb.PMSRoomGroup.SetRoomGroupName( nGroup, strNewName );

			CFilenameUpdater FnUp( SysFiles::PMSRoomGroup );
			if ( DataManagerNonDb.PMSRoomGroup.Write( FnUp.GetFilenameToUse() ) == FALSE )
			{
				Prompter.Error( "Unable to write Guest Account room group file" );
				DataManagerNonDb.PMSRoomGroup.SetRoomGroupName( nGroup, strOldName );
			}
			else
			{
				FillRoomGroupCombo( nGroup - 1 );
			}
		}
	}
}

/**********************************************************************/

void CDefEditRoomDlg::OnToggleRates()
{
	bool bSingle = ( m_checkSingle.GetCheck() != 0 );

	for ( int n = 0; n < m_arrayRateTypes.GetSize(); n++ )
		GetEditSingleRate(n) -> EnableWindow( bSingle );
}

/**********************************************************************/

void CDefEditRoomDlg::OnToggleExtraBed()
{
	GetEditExtraBed() -> EnableWindow( m_checkExtraBed.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomDlg::OnToggleExtraCot()
{
	GetEditExtraCot() -> EnableWindow( m_checkExtraCot.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditRoomDlg::OnDefaultButton() 
{
	if ( ( TRUE == m_bAdd ) || ( m_nSize <= 1 ) )
		OnOK();
	else
		SpinNext();
}

/**********************************************************************/

void CDefEditRoomDlg::SpinPrevious()
{
	if ( m_nSize > 1 )
	{
		if ( ValidateRoomInfo() == TRUE )
		{
			SaveData();

			if ( m_nIndex > 0 )
				m_nIndex--;
			else
				m_nIndex = m_nSize - 1;

			GetData();

			if ( m_pPropPage != NULL )
				m_pPropPage -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditRoomDlg::SpinNext()
{
	if ( m_nSize > 1 )
	{
		if ( ValidateRoomInfo() == TRUE )
		{
			SaveData();

			if ( m_nIndex < m_nSize - 1 )
				m_nIndex++;
			else
				m_nIndex = 0;
			
			GetData();

			if ( m_pPropPage != NULL )
				m_pPropPage -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditRoomDlg::GetData()
{
	CString strTitle;

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt ( m_nIndex, Room );

	m_strName = Room.GetRoomName();
	m_nSleeps = Room.GetSleeps();
	m_nSleepsOld = Room.GetSleeps();

	for ( int n = 0; n < m_arrayRateTypes.GetSize(); n++ )
	{
		switch( m_arrayRateTypes.GetAt(n) )
		{
		case PMS_BOOKING_RATE_ROOM:
			m_dStandardRate[n] = Room.GetRoomOnlyRate();
			m_dSingleRate[n] = Room.GetRoomOnlySingleRate();
			break;

		case PMS_BOOKING_RATE_BB:
			m_dStandardRate[n] = Room.GetRoomBBRate();
			m_dSingleRate[n] = Room.GetRoomBBSingleRate();
			break;

		case PMS_BOOKING_RATE_DBB:
			m_dStandardRate[n] = Room.GetRoomDBBRate();
			m_dSingleRate[n] = Room.GetRoomDBBSingleRate();
			break;
		}
	}

	m_dExtraBed = Room.GetRoomExtraBedRate();
	m_dExtraCot = Room.GetRoomExtraCotRate();

	UpdateData( FALSE );

	strTitle.Format ( "Room %d", Room.GetRoomNo() );
	SetWindowText ( strTitle );

	FillRoomGroupCombo( Room.GetRoomGroup() - 1 );

	m_editName.SetSel ( 0, -1 );
	m_editName.SetFocus();

	FillRoomTypeCombo( Room.GetSleeps(), Room.GetRoomType() );

	m_checkSingle.SetCheck( Room.GetRoomSingleRateFlag() );
	OnToggleRates();

	m_checkExtraBed.SetCheck( Room.GetRoomAllowExtraBedFlag() );
	OnToggleExtraBed();

	m_checkExtraCot.SetCheck( Room.GetRoomAllowExtraCotFlag() );
	OnToggleExtraCot();
}

/**********************************************************************/

void CDefEditRoomDlg::SaveData()
{
	int nSel = m_comboType.GetCurSel();
	int nItemData = m_comboType.GetItemData( nSel );

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt ( m_nIndex, Room );
	Room.SetRoomName( m_strName );
	Room.SetRoomGroup( m_comboGroup.GetCurSel() + 1 );
	Room.SetSleeps( nItemData / 100 );
	Room.SetRoomType( nItemData % 100 );
	Room.SetRoomSingleRateFlag( m_checkSingle.GetCheck() != 0 );
	
	for ( int n = 0; n < m_arrayRateTypes.GetSize(); n++ )
	{
		switch( m_arrayRateTypes.GetAt(n) )
		{
		case PMS_BOOKING_RATE_ROOM:
			Room.SetRoomOnlyRate( m_dStandardRate[n] );
			Room.SetRoomOnlySingleRate( m_dSingleRate[n] );
			break;

		case PMS_BOOKING_RATE_BB:
			Room.SetRoomBBRate( m_dStandardRate[n] );
			Room.SetRoomBBSingleRate( m_dSingleRate[n] );
			break;

		case PMS_BOOKING_RATE_DBB:
			Room.SetRoomDBBRate( m_dStandardRate[n] );
			Room.SetRoomDBBSingleRate( m_dSingleRate[n] );
			break;
		}
	}

	Room.SetRoomAllowExtraBedFlag( m_checkExtraBed.GetCheck() != 0 );
	Room.SetRoomExtraBedRate( m_dExtraBed );

	Room.SetRoomAllowExtraCotFlag( m_checkExtraCot.GetCheck() != 0 );
	Room.SetRoomExtraCotRate( m_dExtraCot );

	DataManagerNonDb.PMSRoom.SetAt ( m_nIndex, Room );
}

/**********************************************************************/

bool CDefEditRoomDlg::ValidateRoomInfo()
{
	OnKillFocusSleeps();

	if ( UpdateData( TRUE ) == FALSE )
		return FALSE;

	if ( FALSE == m_bAdd )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( m_nIndex, infoRoom );

		if ( infoRoom.GetBookingId() != 0 )
		{
			if ( m_nSleeps < m_nSleepsOld )
			{
				Prompter.Error( "You cannot reduce the capacity of a room\nthat is currently occupied." );
				GetEditSleeps() -> SetSel( 0, -1 );
				GetEditSleeps() -> SetFocus();
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

void CDefEditRoomDlg::OnOK() 
{
	if ( ValidateRoomInfo() == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
