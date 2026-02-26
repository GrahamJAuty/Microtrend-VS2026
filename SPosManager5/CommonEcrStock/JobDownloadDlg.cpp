/**********************************************************************/
#include "KeyboardJobInfo.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "JobDownloadDlg.h"
/**********************************************************************/

CJobDownloadDlg::CJobDownloadDlg( CEposTaskDatabase& TaskDatabase, CWnd* pParent)
	: CSSDialog(CJobDownloadDlg::IDD, pParent), m_TaskDatabase( TaskDatabase )
{
	//{{AFX_DATA_INIT(CJobDownloadDlg)
	//}}AFX_DATA_INIT
	m_nPriceSet = 0;
	m_nKbNo = 1;
	m_nPluDownloadType = 0;
	m_nInfoDownloadType = 0;
	m_bAllowPriceSets = EcrmanOptions.GetPluPriceSetsFlag(); 
	m_bAllowKeyboards = EcrmanOptions.GetFeaturesKeyboardFlag() && PasswordArray.GetEnable( PasswordTicks::EditKeyboards );
	m_bChangesMode = FALSE;
}

/**********************************************************************/

void CJobDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobDownloadDlg)
	DDX_Control(pDX, IDC_CHECK_SERVER_NAME, m_checkServerName);
	DDX_Control(pDX, IDC_CHECK_SERVER_PRESET, m_checkServerPreset);
	DDX_Control(pDX, IDC_CHECK_CATEGORY, m_checkCategory);
	DDX_Control(pDX, IDC_CHECK_BARCODE, m_checkBarcode);
	DDX_Control(pDX, IDC_CHECK_MIXMATCH, m_checkMixMatch);
	DDX_Control(pDX, IDC_CHECK_PROMOTION, m_checkPromotion);
	DDX_Control(pDX, IDC_CHECK_WALLETSTAMPRULE, m_checkWalletStampRule);
	DDX_Control(pDX, IDC_CHECK_ALLOFFER, m_checkAllOffer);
	DDX_Control(pDX, IDC_CHECK_TIMEPLAN, m_checkTimePlan);
	DDX_Control(pDX, IDC_CHECK_LOYALTY_SCHEME, m_checkLoyaltyScheme);
	DDX_Control(pDX, IDC_CHECK_TABLE_AREA, m_checkTableArea);
	DDX_Control(pDX, IDC_CHECK_TABLE_NAME, m_checkTableName);
	DDX_Control(pDX, IDC_CHECK_RECEIPT, m_checkReceipt);
	DDX_Control(pDX, IDC_CHECK_FUNCTION, m_checkFunction);
	DDX_Control(pDX, IDC_CHECK_PAYMENT, m_checkPayment);
	DDX_Control(pDX, IDC_CHECK_MODIFIER, m_checkModifier);
	DDX_Control(pDX, IDC_CHECK_GROUP, m_checkGroup);
	DDX_Control(pDX, IDC_CHECK_ALLERGEN, m_checkAllergen);
	DDX_Control(pDX, IDC_CHECK_DEPARTMENT, m_checkDepartment);
	DDX_Control(pDX, IDC_CHECK_KEYBOARD, m_checkKeyboard);
	DDX_Control(pDX, IDC_STATIC_RESETMACRO2, m_staticResetMacro2);
	DDX_Control(pDX, IDC_CHECK_RESETMACRO1, m_checkResetMacro1);
	DDX_Control(pDX, IDC_CHECK_RESETMACRO2, m_checkResetMacro2);
	DDX_Control(pDX, IDC_CHECK_IMAGES, m_checkImages);
	DDX_Control(pDX, IDC_CHECK_MACRO, m_checkMacro);
	DDX_Control(pDX, IDC_CHECK_TAX, m_checkTax);
	DDX_Control(pDX, IDC_CHECK_REASON, m_checkReason);
	DDX_Control(pDX, IDC_RADIO_PLU1, m_radioPlu1);
	DDX_Control(pDX, IDC_RADIO_PLU2, m_radioPlu2);
	DDX_Control(pDX, IDC_RADIO_PLU3, m_radioPlu3);
	DDX_Control(pDX, IDC_RADIO_PLU4, m_radioPlu4);
	DDX_Control(pDX, IDC_RADIO_PRICESET0, m_radioPriceSet0);
	DDX_Control(pDX, IDC_RADIO_PRICESET1, m_radioPriceSet1);
	DDX_Control(pDX, IDC_RADIO_PRICESET2, m_radioPriceSet2);
	DDX_Control(pDX, IDC_RADIO_PRICESET3, m_radioPriceSet3);
	DDX_Control(pDX, IDC_CHECK_WITHKEYBOARD, m_checkWithKeyboard);
	DDX_Radio(pDX, IDC_RADIO_PRICESET0, m_nPriceSet);
	DDX_Radio(pDX, IDC_RADIO_PLUINFO1, m_nInfoDownloadType);
	DDX_Text(pDX, IDC_EDIT_KEYBOARD, m_nKbNo);
	DDV_MinMaxInt(pDX, m_nKbNo, 1, EcrmanOptions.GetFeaturesKeyboardsPerLocation());
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CJobDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CJobDownloadDlg)
	ON_BN_CLICKED(IDC_CHECK_KEYBOARD, OnToggleKeyboard)
	ON_BN_CLICKED(IDC_CHECK_WITHKEYBOARD, OnToggleWithKeyboard)
	ON_BN_CLICKED(IDC_RADIO_PLU1, OnRadioPlu1)
	ON_BN_CLICKED(IDC_RADIO_PLU2, OnRadioPlu2)
	ON_BN_CLICKED(IDC_RADIO_PLU3, OnRadioPlu3)
	ON_BN_CLICKED(IDC_RADIO_PLU4, OnRadioPlu4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CJobDownloadDlg::GetEditKeyboard() { return GetEdit( IDC_EDIT_KEYBOARD ); }
/**********************************************************************/

BOOL CJobDownloadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bChangesMode = m_TaskDatabase.GetPluChangesMode(); 

	m_checkTax.SetWindowText( EcrmanOptions.GetTaxString( "%T rates" ) );

	{
		ShowAndEnableWindow( &m_radioPriceSet0, m_bAllowPriceSets && (!m_bChangesMode), FALSE );
		ShowAndEnableWindow( &m_radioPriceSet1, m_bAllowPriceSets && (!m_bChangesMode), FALSE );
		ShowAndEnableWindow( &m_radioPriceSet2, m_bAllowPriceSets && (!m_bChangesMode), FALSE );
		ShowAndEnableWindow( &m_radioPriceSet3, m_bAllowPriceSets && (!m_bChangesMode), FALSE );
		ShowAndEnableWindow( &m_checkWithKeyboard, m_bAllowKeyboards && m_bAllowPriceSets, FALSE );
		ShowAndEnableWindow( &m_checkResetMacro2, m_bAllowKeyboards && m_bAllowPriceSets, FALSE );
		ShowAndEnableWindow( &m_staticResetMacro2, m_bAllowKeyboards && m_bAllowPriceSets );

		if ( ( FALSE == m_bAllowPriceSets ) || ( TRUE == m_bChangesMode ) )
		{
			MoveControl( &m_checkWithKeyboard, 104, 159 );
			MoveControl( &m_checkResetMacro2, 117, 175 );
			MoveControl( &m_staticResetMacro2, 127, 175 );
		}
	}

	SubclassEdit ( IDC_EDIT_KEYBOARD, SS_NUM, 2, "%d" );
	
	if ( PasswordArray.GetEnable( PasswordTicks::EditPluRecords ) == FALSE )
	{
		m_radioPlu1.EnableWindow( FALSE );
		m_radioPlu2.EnableWindow( FALSE );
		m_radioPlu3.EnableWindow( FALSE );
		m_radioPlu4.EnableWindow( FALSE );
	}

	//COLUMN 1
	int nXPos = 22;
	int nNextYPos = 23;
	{
		bool bOptionModifier = ( SysInfo.GetMaxBasePluLen() != 0 );
		bool bPasswdModifier = PasswordArray.GetEnable( PasswordTicks::EditModifier );
	
		ProcessTick( m_checkBarcode, PasswordArray.GetEnable( PasswordTicks::EditPluRecords ), nXPos, nNextYPos ); 
		ProcessTick( m_checkCategory, PasswordArray.GetEnable( PasswordTicks::EditAnalysisCategory ), nXPos, nNextYPos );
		ProcessTick( m_checkDepartment, PasswordArray.GetEnable( PasswordTicks::EditDepartment ), nXPos, nNextYPos );
		ProcessTick( m_checkGroup, PasswordArray.GetEnable( PasswordTicks::EditGroup ), nXPos, nNextYPos );
		ProcessTick( m_checkAllergen, PasswordArray.GetEnable( PasswordTicks::EditAllergen ), nXPos, nNextYPos );
		ProcessTick( m_checkTax, PasswordArray.GetEnable( PasswordTicks::EditTax ), nXPos, nNextYPos );
		ProcessTick( m_checkModifier, bOptionModifier && bPasswdModifier, nXPos, nNextYPos );
	}

	if ( nNextYPos != 23 )
	{
		nXPos = 104;
	}

	nNextYPos = 23;
	{
		bool bOptionServer = ( EcrmanOptions.GetFeaturesServerSettingsType() != 0 );
		bool bPasswdServer = PasswordArray.GetEnable( PasswordTicks::EditServerNames );
		bool bOptionPreset = EcrmanOptions.GetFeaturesServerPresetFlag();
		bool bPasswdPreset = PasswordArray.GetEnable( PasswordTicks::EditServerPresets );
		
		ProcessTick( m_checkPayment, PasswordArray.GetEnable( PasswordTicks::EditPayment ), nXPos, nNextYPos );
		ProcessTick( m_checkServerName, bOptionServer && bPasswdServer, nXPos, nNextYPos );
		ProcessTick( m_checkServerPreset, bOptionServer && bOptionPreset && bPasswdPreset , nXPos, nNextYPos );
		ProcessTick( m_checkReceipt, PasswordArray.GetEnable( PasswordTicks::SetupMenuBillTexts ), nXPos, nNextYPos );
		ProcessTick( m_checkFunction, PasswordArray.GetEnable( PasswordTicks::SetupMenuFunction ), nXPos, nNextYPos );
		ProcessTick( m_checkMacro, PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros ), nXPos, nNextYPos );
		ProcessTick( m_checkReason, PasswordArray.GetEnable(PasswordTicks::SetupMenuReasons), nXPos, nNextYPos);
	}

	if (nNextYPos != 23)
	{
		if (22 == nXPos)
		{
			nXPos = 104;
		}
		else
		{
			nXPos = 191;
		}
	}

	{
		bool bOptionMixMatch = EcrmanOptions.GetFeaturesMixMatchFlag();
		bool bPasswdMixMatch = PasswordArray.GetEnable( PasswordTicks::EditMixMatch );
		bool bOptionPromo = EcrmanOptions.GetFeaturesPromotionFlag();
		bool bOptionWalletStampRule = EcrmanOptions.GetFeaturesWalletStampRuleFlag();
		bool bPasswdWalletStampRule = PasswordArray.GetEnable(PasswordTicks::EditWalletStampRule);
		bool bOptionTimePlan = EcrmanOptions.GetFeaturesTimePlanFlag();
		bool bPasswdTimePlan = PasswordArray.GetEnable( PasswordTicks::EditTimePlan );
		bool bOptionLoyalty = ((SysInfo.GotLoyaltyLink() == TRUE) || ((SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_UNKNOWN)));
		bool bPasswdLoyalty = PasswordArray.GetEnable( PasswordTicks::Loyalty );
		bool bPasswdTableName = PasswordArray.GetEnable( PasswordTicks::SetupMenuTableNames );

		bool bShowMixMatch = bOptionMixMatch && bPasswdMixMatch;
		bool bShowPromotion = bOptionMixMatch && bPasswdMixMatch;
		bool bShowWalletStampRule = bOptionWalletStampRule && bPasswdWalletStampRule;

		nNextYPos = 23;
		ProcessTick( m_checkMixMatch, bShowMixMatch, nXPos, nNextYPos );
		
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_LOY_V4:
			m_checkWalletStampRule.SetWindowText("Stamp Offers");
			ProcessTick(m_checkWalletStampRule, bShowWalletStampRule, nXPos, nNextYPos);
			break;

		case LOYALTY_TYPE_SMP_V4:
			m_checkWalletStampRule.SetWindowText("Purchase Control");
			ProcessTick(m_checkWalletStampRule, bShowWalletStampRule, nXPos, nNextYPos);
			break;

		default:
			ProcessTick(m_checkWalletStampRule, FALSE, nXPos, nNextYPos);
			break;
		}

		ProcessTick( m_checkPromotion, bShowPromotion, nXPos, nNextYPos );
		ProcessTick( m_checkTimePlan, bOptionMixMatch && bOptionTimePlan && bPasswdTimePlan, nXPos, nNextYPos );
		ProcessTick( m_checkAllOffer, bShowMixMatch && bShowWalletStampRule, nXPos, nNextYPos);
		ProcessTick( m_checkLoyaltyScheme, bOptionLoyalty && bPasswdLoyalty, nXPos, nNextYPos );
		ProcessTick( m_checkTableArea, bPasswdTableName, nXPos, nNextYPos );
		ProcessTick( m_checkTableName, bPasswdTableName, nXPos, nNextYPos );
	}

	if ( nNextYPos != 23 )
	{
		switch( nXPos )
		{
		case 22:	nXPos = 104;	break;
		case 104:	nXPos = 191;	break;
		default:	nXPos = 273;	break;
		}
	}

	nNextYPos = 23;
	MoveControl( &m_checkKeyboard, nXPos, 23 );
	MoveControl( &m_checkResetMacro1, nXPos + 11, 39 );
	MoveControl( &m_checkImages, nXPos, 55 );
	MoveControl( GetEditKeyboard(), nXPos + 48, 22 );

	if ( FALSE == m_bAllowKeyboards )	
	{
		ShowAndEnableWindow( &m_checkKeyboard, FALSE );
		ShowAndEnableWindow( &m_checkResetMacro1, FALSE );
		ShowAndEnableWindow( &m_checkImages, FALSE );
		ShowAndEnableWindow( GetEditKeyboard(), FALSE );
	}
	else
	{
		GetEditKeyboard() -> EnableWindow( FALSE );
		m_checkResetMacro1.EnableWindow( FALSE );
	}

	OnRadioPlu1();

	return TRUE;  
}

/**********************************************************************/

void CJobDownloadDlg::ProcessTick( CButton& tick, bool bShow, int nXPos, int& nNextYPos )
{
	if ( FALSE == bShow )
	{
		tick.SetCheck( FALSE );
		tick.EnableWindow( FALSE );
		tick.ShowWindow( SW_HIDE );
	}
	else
	{
		tick.EnableWindow( TRUE );
		tick.ShowWindow( SW_SHOW );
		MoveControl( &tick, nXPos, nNextYPos );
		nNextYPos += 16;
	}
}

/**********************************************************************/

void CJobDownloadDlg::OnToggleKeyboard() 
{
	if ( EcrmanOptions.GetFeaturesKeyboardsPerLocation() > 1 )
	{
		GetEditKeyboard() -> EnableWindow ( IsTicked( m_checkKeyboard ) );
	}

	m_checkResetMacro1.EnableWindow( IsTicked( m_checkKeyboard ) );
}

/**********************************************************************/

void CJobDownloadDlg::OnToggleWithKeyboard() 
{
	m_checkResetMacro2.EnableWindow( IsTicked( m_checkWithKeyboard ) );
}

/**********************************************************************/

void CJobDownloadDlg::OnRadioPlu1()
{
	m_nPluDownloadType = 0;
	TogglePluDownloadType();
}

/**********************************************************************/

void CJobDownloadDlg::OnRadioPlu2()
{
	m_nPluDownloadType = 1;
	TogglePluDownloadType();
}

/**********************************************************************/

void CJobDownloadDlg::OnRadioPlu3()
{
	m_nPluDownloadType = 2;
	TogglePluDownloadType();
}

/**********************************************************************/

void CJobDownloadDlg::OnRadioPlu4()
{
	m_nPluDownloadType = 3;
	TogglePluDownloadType();
}

/**********************************************************************/

void CJobDownloadDlg::TogglePluDownloadType() 
{
	m_radioPlu1.SetCheck( 0 == m_nPluDownloadType );
	m_radioPlu2.SetCheck( 1 == m_nPluDownloadType );
	m_radioPlu3.SetCheck( 2 == m_nPluDownloadType );
	m_radioPlu4.SetCheck( 3 == m_nPluDownloadType );

	bool bEnable = ( m_nPluDownloadType != 0 );
		
	m_radioPriceSet0.EnableWindow ( bEnable & m_bAllowPriceSets );
	m_radioPriceSet1.EnableWindow ( bEnable & m_bAllowPriceSets );
	m_radioPriceSet2.EnableWindow ( bEnable & m_bAllowPriceSets );
	m_radioPriceSet3.EnableWindow ( bEnable & m_bAllowPriceSets );
	m_checkWithKeyboard.EnableWindow( bEnable & m_bAllowPriceSets & m_bAllowKeyboards );
}

/**********************************************************************/

void CJobDownloadDlg::AddJobsToTask()
{
	if ( m_nPluDownloadType != 0 )			
	{	
		int nJobType = DN_PLU;
				
		if ( TRUE == m_bChangesMode )
		{
			if ( IsTicked( m_checkWithKeyboard ) && m_bAllowPriceSets ) 
			{
				nJobType = DN_PLU_KBD;		
			}
		}
		else 
		{
			if ( IsTicked( m_checkWithKeyboard ) && m_bAllowPriceSets )
			{
				switch ( m_nPriceSet )
				{
				case 0:	nJobType = DN_PLU_KBD;			break;
				case 1:	nJobType = DN_PLU_SET1_KBD;		break;
				case 2:	nJobType = DN_PLU_SET2_KBD;		break;
				case 3:	nJobType = DN_PLU_SET3_KBD;		break;
				}
			}
			else
			{
				switch ( m_nPriceSet )
				{
				case 0:	nJobType = DN_PLU;			break;
				case 1:	nJobType = DN_PLU_SET1;		break;
				case 2:	nJobType = DN_PLU_SET2;		break;
				case 3:	nJobType = DN_PLU_SET3;		break;
				}
			}
		}

		int nParam1 = 0;
		switch( m_nPluDownloadType )
		{
		case 1:		nParam1 = 2;	break;	//CHANGES ONLY
		case 2:		nParam1 = 1;	break;	//ALL
		case 3:		nParam1 = 0;	break;	//ALL WITH CLEAR
		default:	nParam1 = 0;	break;
		}

		int nExtraJobParam1 = 0;
		switch ( nJobType )
		{
		case DN_PLU_KBD:
		case DN_PLU_SET1_KBD:
		case DN_PLU_SET2_KBD:
		case DN_PLU_SET3_KBD:
			nExtraJobParam1 = ( IsTicked( m_checkResetMacro2 ) ? 1 : 0 );
			break;
		}

		m_TaskDatabase.AddEditableTaskJob( nJobType, nParam1, FALSE, nExtraJobParam1 );
	}

	switch( m_nInfoDownloadType )
	{
	case 1:	m_TaskDatabase.AddEditableTaskJob( DN_PLU_INFO, 2 );	break;
	case 2:	m_TaskDatabase.AddEditableTaskJob( DN_PLU_INFO, 1 );	break;
	}
	
	if ( IsTicked(m_checkBarcode) )			m_TaskDatabase.AddEditableTaskJob ( DN_BAR );
	if ( IsTicked(m_checkCategory) )		m_TaskDatabase.AddEditableTaskJob ( DN_ACAT );
	
	{
		bool bGotPromotion = IsTicked(m_checkPromotion);
		bool bGotMixMatch = IsTicked(m_checkMixMatch);
		bool bGotWalletStampRule = IsTicked(m_checkWalletStampRule);
		bool bGotAllOffer = IsTicked(m_checkAllOffer);

		if ( (TRUE == bGotPromotion) && ( FALSE == bGotMixMatch) && ( FALSE == bGotAllOffer ))
		{
			m_TaskDatabase.AddEditableTaskJob(DN_PROMO);
		}

		if ( (TRUE == bGotMixMatch) && ( FALSE == bGotAllOffer ) && ( FALSE == bGotWalletStampRule ) )
		{
			m_TaskDatabase.AddEditableTaskJob(DN_MIX);
		}

		if ( (TRUE == bGotWalletStampRule ) && ( FALSE == bGotAllOffer ) && ( FALSE == bGotMixMatch ) )
		{
			m_TaskDatabase.AddEditableTaskJob(DN_WALLETSTAMPRULE);
		}

		if ( (TRUE == bGotAllOffer) || ( ( TRUE == bGotWalletStampRule ) && ( TRUE == bGotMixMatch ) ) )
 		{
			m_TaskDatabase.AddEditableTaskJob(DN_ALLOFFER);
		}
	}
	
	if ( IsTicked(m_checkTimePlan) )		m_TaskDatabase.AddEditableTaskJob ( DN_TPLAN );
	if ( IsTicked(m_checkLoyaltyScheme) )	m_TaskDatabase.AddEditableTaskJob ( DN_LOYALTY_SCHEME );
	if ( IsTicked(m_checkTableArea) )		m_TaskDatabase.AddEditableTaskJob ( DN_TABLE_AREA );
	if ( IsTicked(m_checkTableName) )		m_TaskDatabase.AddEditableTaskJob ( DN_TABLE_NAME );
	if ( IsTicked(m_checkReceipt) )			m_TaskDatabase.AddEditableTaskJob ( DN_RECEIPT );
	if ( IsTicked(m_checkFunction) )		m_TaskDatabase.AddEditableTaskJob ( DN_FUNCTION );
	if ( IsTicked(m_checkDepartment) )		m_TaskDatabase.AddEditableTaskJob ( DN_DEPT );
	if ( IsTicked(m_checkGroup) )			m_TaskDatabase.AddEditableTaskJob ( DN_GROUP );
	if ( IsTicked(m_checkAllergen) )		m_TaskDatabase.AddEditableTaskJob ( DN_ALLERGEN );

	if (SysInfo.GetMaxBasePluLen() != 0)
	{
		if (IsTicked(m_checkModifier))
		{
			m_TaskDatabase.AddEditableTaskJob(DN_MOD);
		}
	}

	if ( IsTicked(m_checkPayment) )			m_TaskDatabase.AddEditableTaskJob ( DN_PAY );
	if ( IsTicked(m_checkServerName) )		m_TaskDatabase.AddEditableTaskJob ( DN_SERVER_NAME );
	if ( IsTicked(m_checkServerPreset) )	m_TaskDatabase.AddEditableTaskJob ( DN_SERVER_PRESET );
	if ( IsTicked(m_checkTax) )				m_TaskDatabase.AddEditableTaskJob ( DN_TAX );
	if ( IsTicked(m_checkReason))			m_TaskDatabase.AddEditableTaskJob ( DN_REASON, 0xFFFF );
			
	if ( IsTicked(m_checkKeyboard) )
	{
		CKeyboardJobInfo infoKb;
		infoKb.m_nKbNo = m_nKbNo;
		
		int nJobType = infoKb.GetJobNo();

		if ( nJobType != 0 )
		{
			m_TaskDatabase.AddEditableTaskJob( nJobType, IsTicked( m_checkResetMacro1 ) ? 1 : 0 );
		}
	}
	
	if ( IsTicked(m_checkImages) )			m_TaskDatabase.AddEditableTaskJob( DN_IMAGES );
	if ( IsTicked(m_checkMacro) )			m_TaskDatabase.AddEditableTaskJob( DN_MACRO );
}

/**********************************************************************/

void CJobDownloadDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		AddJobsToTask();
		m_TaskDatabase.TidyEditableTaskJobs();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
