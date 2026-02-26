/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "PropPagePMSOptionsMisc.h"
/**********************************************************************/

CPropPagePMSOptionsMisc::CPropPagePMSOptionsMisc() : CSSPropertyPage(CPropPagePMSOptionsMisc::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsMisc)
	//}}AFX_DATA_INIT
	m_nCurrentGroup = 0;
	m_pEmailSignatures = NULL;
	m_ImageList.Create ( IDB_ICONS_COMBO, 31, 0, COLORREF(0xFFFFFF) );
}

/**********************************************************************/

CPropPagePMSOptionsMisc::~CPropPagePMSOptionsMisc()
{
}

/**********************************************************************/

void CPropPagePMSOptionsMisc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsMisc)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CHECK_ACCOUNTS, m_checkAccounts);
	DDX_Control( pDX, IDC_CHECK_EMAIL, m_checkEmail);
	DDX_Control( pDX, IDC_CHECK_EMAIL_NOTE, m_checkEmailNote);
	DDX_Control( pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Control( pDX, IDC_CHECK_RATE_ROOM, m_checkRateRoom);
	DDX_Control( pDX, IDC_CHECK_RATE_BB, m_checkRateBB);
	DDX_Control( pDX, IDC_CHECK_RATE_DBB, m_checkRateDBB);
	DDX_Control( pDX, IDC_CHECK_RATE_FREE, m_checkRateFree);
	DDX_Control( pDX, IDC_STATIC_STOCKPOINT, m_staticStockpoint);
	DDX_Control( pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Control( pDX, IDC_COMBO_RATE, m_comboRate);
	DDX_Control( pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control( pDX, IDC_EDIT_SIGNATURE, m_editSignature);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsMisc, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsMisc)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP,OnSelectGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsMisc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_comboStockpoint.SetImageList( &m_ImageList );
	
	m_checkAccounts.SetCheck( PMSOptions.GetMiscEnableMultipleAccounts() );
	m_checkEmail.SetCheck( PMSOptions.GetMiscEnableEmailConfirmation() );
	m_checkEmailNote.SetCheck( PMSOptions.GetMiscEmailShowNote() );
	m_checkTraining.SetCheck( PMSOptions.GetMiscEnableTrainingMode() );
	m_checkRateBB.SetCheck( PMSOptions.GetMiscEnableRateBB() );
	m_checkRateDBB.SetCheck( PMSOptions.GetMiscEnableRateDBB() );
	m_checkRateFree.SetCheck( PMSOptions.GetMiscEnableFreeRoom() );

	m_comboRate.AddString( "Room Only" );
	m_comboRate.AddString( "B & B" );
	m_comboRate.AddString( "DB & B" );

#ifndef STOCKMAN_SYSTEM
	
	m_staticStockpoint.ShowWindow( SW_HIDE );
	m_comboStockpoint.ShowWindow( SW_HIDE );

#else
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( 0, nStartIdx, nEndIdx );

	int nSel = 0;

	AddStockpointToCombo( "None", NODE_COMBOICON_NONE );
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		int nType;
		if ( dbStockpoint.GetWarehouseFlag( nSpIdx ) == TRUE )
			nType = NODE_COMBOICON_WAREHOUSE;
		else
			nType = NODE_COMBOICON_STOCKPOINT;

		AddStockpointToCombo( dbStockpoint.GetName( nSpIdx ), nType );

		if ( dbStockpoint.GetSpNo( nSpIdx ) == PMSOptions.GetMiscManualSalesStockpoint() )
			nSel = ( nSpIdx - nStartIdx ) + 1;
	}

	m_comboStockpoint.SetCurSel( nSel );

#endif

	FillRoomGroupCombo(0);
	OnSelectGroup();

	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSOptionsMisc::AddStockpointToCombo ( const char* szText, int nImage )
{
	char* p = (char*) szText;

	COMBOBOXEXITEM item{};
	item.mask = CBEIF_DI_SETITEM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = m_comboStockpoint.GetCount();
	item.pszText = p;
	item.iImage = nImage;
	item.iSelectedImage = nImage;

	m_comboStockpoint.InsertItem ( &item );
}

/**********************************************************************/

void CPropPagePMSOptionsMisc::FillRoomGroupCombo( int nSel )
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

BOOL CPropPagePMSOptionsMisc::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	
	if ( TRUE == bResult )
	{
		bResult = FALSE;
		bResult |= ( m_checkRateRoom.GetCheck() != 0 );
		bResult |= ( m_checkRateBB.GetCheck() != 0 );
		bResult |= ( m_checkRateDBB.GetCheck() != 0 );
		
		if ( FALSE == bResult )
			Prompter.Error( "You must enable at least one room rate type\n\n(Room Only, B & B or DB & B)" );
	}

	if ( TRUE == bResult )
	{
		int nSel = m_comboRate.GetCurSel();
		
		bResult = FALSE;
		bResult |= ( ( nSel == 0 ) && ( m_checkRateRoom.GetCheck() != 0 ) );
		bResult |= ( ( nSel == 1 ) && ( m_checkRateBB.GetCheck() != 0 ) );
		bResult |= ( ( nSel == 2 ) && ( m_checkRateDBB.GetCheck() != 0 ) );

		if ( FALSE == bResult )
			Prompter.Error( "You must enable the room rate that you have\nselected as the default for new bookings." );
	}

	if ( TRUE == bResult )
		SaveRecord();
	
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsMisc::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	
	m_checkRateRoom.SetCheck( PMSOptions.GetMiscEnableRateRoom() );

	int nSel = PMSOptions.GetMiscDefaultRoomRateType();
	
	if ( ( nSel < 0 ) || ( nSel > 2 ) )
		nSel = 0;

	m_comboRate.SetCurSel( nSel );

	FillRoomGroupCombo( m_comboGroup.GetCurSel() );
	
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsMisc::OnSelectGroup()
{
	int nNewGroup = m_comboGroup.GetCurSel() + 1;
	
	if ( ( nNewGroup != m_nCurrentGroup ) && ( nNewGroup >= 1 ) && ( nNewGroup <= 5 ) )
	{
		if ( m_pEmailSignatures != NULL )
		{
			if ( m_nCurrentGroup != 0 )
				m_pEmailSignatures -> CopyEditBoxToBuffer ( m_nCurrentGroup, m_editSignature );
			
			m_pEmailSignatures -> CopyBufferToEditBox ( nNewGroup, m_editSignature );
			m_nCurrentGroup = nNewGroup;
		}
	}
}

/**********************************************************************/

void CPropPagePMSOptionsMisc::SaveRecord()
{
	PMSOptions.SetMiscEnableMultipleAccounts( m_checkAccounts.GetCheck() != 0 );
	PMSOptions.SetMiscEnableEmailConfirmation( m_checkEmail.GetCheck() != 0 );
	PMSOptions.SetMiscEmailShowNote( m_checkEmailNote.GetCheck() != 0 );
	PMSOptions.SetMiscEnableTrainingMode( m_checkTraining.GetCheck() != 0 );
	PMSOptions.SetMiscDefaultRoomRateType( m_comboRate.GetCurSel() );
	PMSOptions.SetMiscEnableRateRoom( m_checkRateRoom.GetCheck() != 0 );
	PMSOptions.SetMiscEnableRateBB( m_checkRateBB.GetCheck() != 0 );
	PMSOptions.SetMiscEnableRateDBB( m_checkRateDBB.GetCheck() != 0 );
	PMSOptions.SetMiscEnableFreeRoom( m_checkRateFree.GetCheck() != 0 );

	//AUTOMATIC CORRECTION FOR IMPOSSIBLE OPTION SETTINGS
	PMSOptions.SetMiscEnableRateRoom( PMSOptions.GetMiscEnableRateRoom() );
	PMSOptions.SetMiscDefaultRoomRateType( PMSOptions.GetMiscDefaultRoomRateType() );
	
#ifdef STOCKMAN_SYSTEM

	int nSel = m_comboStockpoint.GetCurSel();

	if ( 0 == nSel )
		PMSOptions.SetMiscManualSalesStockpoint(0);
	else
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( 0, nStartIdx, nEndIdx );

		nSel--;
		if ( ( nSel >= 0 ) || ( nSel <= nEndIdx - nStartIdx  ) )
			PMSOptions.SetMiscManualSalesStockpoint( dbStockpoint.GetSpNo( nStartIdx + nSel ) );
	}

#endif

	if ( ( m_pEmailSignatures != NULL ) && ( m_nCurrentGroup != 0 ) )
		m_pEmailSignatures -> CopyEditBoxToBuffer( m_nCurrentGroup, m_editSignature );
}

/**********************************************************************/
#endif
/**********************************************************************/
