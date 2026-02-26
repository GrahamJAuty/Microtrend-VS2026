/**********************************************************************/
#include "ListDataGroupDlg.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePaymentGeneral.h"
/**********************************************************************/

CPropPagePaymentGeneral::CPropPagePaymentGeneral() : CSSPropertyPage(CPropPagePaymentGeneral::IDD),
	m_GroupSelector( m_editGroup, m_comboGroup, NODE_PAYMENT_GROUP )
{
	//{{AFX_DATA_INIT(CPropPagePaymentGeneral)
	m_dLimit = 0.0;
	m_nForexNo = 0;
	m_nDepositChangeVoucherMin = 0;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePaymentGeneral::~CPropPagePaymentGeneral()
{
}

/**********************************************************************/

void CPropPagePaymentGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePaymentGeneral)
	
	DDX_Control(pDX, IDC_CHECK_TENDER, m_checkTender);
	DDX_Control(pDX, IDC_CHECK_OVERTENDER, m_checkOverTender);
	DDX_Control(pDX, IDC_CHECK_CHANGE, m_checkChange);
	DDX_Control(pDX, IDC_CHECK_GRATUITY, m_checkGratuity);
	DDX_Control(pDX, IDC_CHECK_DRAWER, m_checkDrawer);
	DDX_Control(pDX, IDC_CHECK_NONBANK, m_checkNonBank);
	DDX_Control(pDX, IDC_CHECK_RECEIPT, m_checkReceipt);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT, m_checkAccount);
	DDX_Control(pDX, IDC_CHECK_CASHDECL, m_checkCashDecl);
	DDX_Control(pDX, IDC_CHECK_CHGASCBACK, m_checkChangeAsCashback);
	
	DDX_Control(pDX, IDC_EDIT_EPOSNAME, m_editEPOSName);
	DDX_Control(pDX, IDC_EDIT_REPORTNAME, m_editReportName);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_EDIT_GROUP, m_editGroup);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_comboFunction);
	DDX_Control(pDX, IDC_STATIC_OPTION1, m_staticOption1);
	DDX_Control(pDX, IDC_EDIT_PARAMS, m_editParams);
	DDX_Control(pDX, IDC_COMBO_DEPOSIT_CHANGE_TYPE, m_comboDepositChangeType);
	DDX_Control(pDX, IDC_STATIC_OPTION2, m_staticOption2);
	DDX_Control(pDX, IDC_CHECK_CASHBACK, m_checkCashback);
	DDX_Control(pDX, IDC_CHECK_HANDHELD, m_checkHandheld);
	DDX_Control(pDX, IDC_CHECK_CASHLIMIT, m_checkCashLimit);

	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_FOREX, m_nForexNo);
	DDV_MinMaxInt(pDX, m_nForexNo, Payment::ForexNo.Min, Payment::ForexNo.Max);

	DDX_Text(pDX, IDC_EDIT_DEPOSIT_VOUCHER_MIN, m_nDepositChangeVoucherMin);
	DDV_MinMaxInt(pDX, m_nDepositChangeVoucherMin, Payment::DepositChangeVoucherMin.Min, Payment::DepositChangeVoucherMin.Max);

	DDX_Text(pDX, IDC_EDIT_LIMIT, m_dLimit);
	DDV_MinMaxDouble(pDX, m_dLimit, Payment::Limit.Min, Payment::Limit.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePaymentGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePaymentGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectPaymentType)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUP, OnKillFocusGroupNo)
	ON_BN_CLICKED(IDC_BUTTON_SET_GROUP, OnButtonSetGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePaymentGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit( IDC_EDIT_LIMIT, SS_NUM_DP, 11, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit( IDC_EDIT_DEPOSIT_VOUCHER_MIN, SS_NUM, 3, "%d" );
	SubclassEdit( IDC_EDIT_FOREX, SS_NUM, 3, "%d" );

	m_editParams.LimitText( Payment::ExternalParams.Max );
	m_editEPOSName.LimitText( Payment::PaymentEPOSName.Max );
	m_editReportName.LimitText( Payment::PaymentReportName.Max );
	
	FillTypeCombo();
	FillFunctionCombo();

	m_comboDepositChangeType.AddString( "Change Only" );
	m_comboDepositChangeType.AddString( "Voucher Only" );
	m_comboDepositChangeType.AddString( "Change or Voucher" );

	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPagePaymentGeneral::FillTypeCombo()
{
	m_comboType.ResetContent();

	CWordArray arrayTypes;
	arrayTypes.Add(PAYMENT_TYPE_CASH);
	arrayTypes.Add(PAYMENT_TYPE_CHEQUE);
	arrayTypes.Add(PAYMENT_TYPE_EFT);
	arrayTypes.Add(PAYMENT_TYPE_EFT_CNP);
	arrayTypes.Add(PAYMENT_TYPE_EFT_SHARE);
	arrayTypes.Add(PAYMENT_TYPE_ROOM_AUTO);
	arrayTypes.Add(PAYMENT_TYPE_ROOM_MANUAL);
	arrayTypes.Add(PAYMENT_TYPE_LOYALTY);
	arrayTypes.Add(PAYMENT_TYPE_CURRENCY);
	arrayTypes.Add(PAYMENT_TYPE_DEPOSIT);
	arrayTypes.Add(PAYMENT_TYPE_ACCOUNT);
	arrayTypes.Add(PAYMENT_TYPE_CREDITNOTE);
	arrayTypes.Add(PAYMENT_TYPE_GIFTCARD);
	arrayTypes.Add(PAYMENT_TYPE_EXTERNAL_APP);
	arrayTypes.Add(PAYMENT_TYPE_EXTERNAL_PAY);
	arrayTypes.Add(PAYMENT_TYPE_VOUCHERCART);
	arrayTypes.Add(PAYMENT_TYPE_OTHER);

	for (int n = 0; n < arrayTypes.GetSize(); n++)
	{
		CPaymentCSVRecord PayMeth;

		int nType = arrayTypes.GetAt(n);
		PayMeth.SetPaymentType(nType);
		int nPos = m_comboType.AddString(PayMeth.GetDisplayType());
		m_comboType.SetItemData(nPos, nType);
	}
}

/**********************************************************************/

void CPropPagePaymentGeneral::FillFunctionCombo()
{
	m_comboFunction.ResetContent();

	for ( int n = Payment::FunctionNo.Min; n <= Payment::FunctionNo.Max; n++ )
	{
		CString strFuncName;
		strFuncName.Format ( "%d Payment %d", n, n + 1 - Payment::FunctionNo.Min );
		
		int nPos = m_comboFunction.AddString( strFuncName );
		m_comboFunction.SetItemData( nPos, n );
	}
}

/**********************************************************************/

BOOL CPropPagePaymentGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePaymentGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePaymentGeneral::GetRecordData()
{
	m_dLimit = m_pPaymentRecord -> GetLimit();
	m_nForexNo = m_pPaymentRecord -> GetForexNo();
	m_nDepositChangeVoucherMin = m_pPaymentRecord -> GetDepositChangeVoucherMin();
}

/**********************************************************************/

void CPropPagePaymentGeneral::SetRecordControls()
{
	bool bForceCashLimit = FALSE;
	bool bForceCashDecl = FALSE;
	bool bAllowCashback = TRUE;
	m_checkCashLimit.EnableWindow( TRUE );
	m_checkCashDecl.EnableWindow( TRUE );
	m_checkCashback.EnableWindow( TRUE );

	m_GroupSelector.FillGroupCombo ( m_pPaymentRecord -> GetReportGroup() );

	m_editEPOSName.SetWindowText( m_pPaymentRecord -> GetEPOSName() );
	m_editReportName.SetWindowText( m_pPaymentRecord -> GetReportName() );
	m_checkChange.SetCheck( m_pPaymentRecord -> GetChange() );
	m_checkDrawer.SetCheck( m_pPaymentRecord -> GetDrawer() );
	m_checkGratuity.SetCheck( m_pPaymentRecord -> GetGratuity() );
	m_checkNonBank.SetCheck( m_pPaymentRecord -> GetNonBnk() );
	m_checkOverTender.SetCheck( m_pPaymentRecord -> GetOverTender() );
	m_checkReceipt.SetCheck( m_pPaymentRecord -> GetReceipt() );
	m_checkAccount.SetCheck( m_pPaymentRecord -> GetAccountAllow() );
	m_checkTender.SetCheck( m_pPaymentRecord -> GetTender() );
	m_checkHandheld.SetCheck( m_pPaymentRecord -> GetHandheld() );
	m_checkCashLimit.SetCheck( m_pPaymentRecord -> GetCashLimitFlag() );
	m_checkCashDecl.SetCheck( m_pPaymentRecord -> GetCashDeclarationFlag() );
	m_checkChangeAsCashback.SetCheck( m_pPaymentRecord -> GetChangeAsCashbackFlag() );
	m_checkCashback.SetCheck( m_pPaymentRecord -> GetEFTCashback() );

	int nType = m_pPaymentRecord -> GetPaymentType();

	if ( m_pPaymentRecord -> GetPaymentNo() == 1 )
	{
		nType = 0;
		m_comboType.SetCurSel(0);
		m_comboType.EnableWindow(FALSE);
		bForceCashLimit = TRUE;
		bForceCashDecl = TRUE;
	}
	else
	{
		m_comboType.SetCurSel(0);
		m_comboType.EnableWindow( TRUE );

		for ( int n = 0; n < m_comboType.GetCount(); n++ )
		{
			if ( nType == (int) m_comboType.GetItemData(n) )
			{
				m_comboType.SetCurSel(n);
				break;
			}
		}
	}

	m_comboFunction.SetCurSel(0);
	int nFuncNo = m_pPaymentRecord -> GetFunctionNo();
	for ( int n = 0; n < m_comboFunction.GetCount(); n++ )
	{
		if ( nFuncNo == (int) m_comboFunction.GetItemData(n) )
		{
			m_comboFunction.SetCurSel(n);
			break;
		}
	}

	m_comboDepositChangeType.SetCurSel( m_pPaymentRecord ->GetDepositChangeType() % 3 );
	m_editParams.SetWindowText( m_pPaymentRecord -> GetExternalParams() );
		
	switch( nType )
	{
	case PAYMENT_TYPE_EFT:
	case PAYMENT_TYPE_EFT_CNP:
	case PAYMENT_TYPE_EXTERNAL_PAY:
		bAllowCashback = TRUE;
		break;

	default:
		bAllowCashback = FALSE;
		break;
	}

	OnSelectPaymentType();

	if ( TRUE == bForceCashLimit )
	{
		m_checkCashLimit.SetCheck( TRUE );
		m_checkCashLimit.EnableWindow( FALSE );
	}

	if ( TRUE == bForceCashDecl )
	{
		m_checkCashDecl.SetCheck( TRUE );
		m_checkCashDecl.EnableWindow( FALSE );
	}

	if ( FALSE == bAllowCashback )
	{
		m_checkCashback.SetCheck( FALSE );
		m_checkCashback.EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPropPagePaymentGeneral::OnSelectPaymentType() 
{
	int nType = PAYMENT_TYPE_CASH;
	int nSel = m_comboType.GetCurSel();
	if (nSel >= 0 && nSel < m_comboType.GetCount())
	{
		nType = m_comboType.GetItemData(nSel);
	}

	bool bShowCashback = FALSE;
	bool bShowChangeAsCashback = FALSE;
	bool bShowForex = FALSE;
	bool bShowDepositChangeType = FALSE;
	bool bShowDepositVoucherMin = FALSE;
	bool bShowEditParams = FALSE;
	bool bShowHandheld = TRUE;
	bool bShowCashLimit = TRUE;
	bool bEnableCashDecl = TRUE;
	CString strOption1 = "";
	CString strOption2 = "";

	switch( nType )
	{
	case PAYMENT_TYPE_EFT:
	case PAYMENT_TYPE_EFT_CNP:
		bShowCashback = TRUE;
		break;

	case PAYMENT_TYPE_CURRENCY:
		bShowForex = TRUE;
		bEnableCashDecl = FALSE;
		strOption1 = "Forex No" ;
		break;

	case PAYMENT_TYPE_DEPOSIT:
		bShowDepositChangeType = TRUE;
		bShowDepositVoucherMin = TRUE;
		strOption1 = "Change type";
		strOption2 = "Min Voucher £";
		break;

	case PAYMENT_TYPE_EXTERNAL_APP:
		bShowEditParams = TRUE;
		strOption1 = "Parameter";
		break;

	case PAYMENT_TYPE_EXTERNAL_PAY:
		bShowCashback = TRUE;
		bShowHandheld = FALSE;
		bShowCashLimit = FALSE;
		break;

	case PAYMENT_TYPE_OTHER:
		bShowChangeAsCashback = TRUE;
		break;
	}

	ShowAndEnableWindow ( &m_checkCashback, bShowCashback );
	ShowAndEnableWindow ( &m_checkChangeAsCashback, bShowChangeAsCashback );
	ShowAndEnableWindow ( GetEditForex(), bShowForex );
	ShowAndEnableWindow ( &m_comboDepositChangeType, bShowDepositChangeType );
	ShowAndEnableWindow ( GetEditDepositVoucherMin(), bShowDepositVoucherMin  );
	ShowAndEnableWindow ( &m_editParams, bShowEditParams );
	ShowAndEnableWindow ( &m_staticOption1, strOption1 != "" );
	ShowAndEnableWindow ( &m_staticOption2, strOption2 != "" );
	ShowAndEnableWindow ( &m_checkHandheld, bShowHandheld );
	ShowAndEnableWindow ( &m_checkCashLimit, bShowCashLimit );
	m_staticOption1.SetWindowText( strOption1 );
	m_staticOption2.SetWindowText( strOption2 );

	m_checkCashDecl.EnableWindow( bEnableCashDecl );
	if (FALSE == bEnableCashDecl)
	{
		m_checkCashDecl.SetCheck(TRUE);
	}
}

/**********************************************************************/

bool CPropPagePaymentGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePaymentGeneral::SaveRecord()
{
	CString strEPOSName;
	m_editEPOSName.GetWindowText( strEPOSName );

	CString strReportName;
	m_editReportName.GetWindowText( strReportName );

	CString strParams;
	m_editParams.GetWindowText( strParams );

	m_pPaymentRecord -> SetEPOSName( strEPOSName );
	m_pPaymentRecord -> SetReportName( strReportName );
	m_pPaymentRecord -> SetChange( m_checkChange.GetCheck() != 0 );
	m_pPaymentRecord -> SetDrawer( m_checkDrawer.GetCheck() != 0 );
	m_pPaymentRecord -> SetGratuity( m_checkGratuity.GetCheck() != 0 );
	m_pPaymentRecord -> SetNonBnk( m_checkNonBank.GetCheck() != 0 );
	m_pPaymentRecord -> SetOverTender( m_checkOverTender.GetCheck() != 0 );
	m_pPaymentRecord -> SetReceipt( m_checkReceipt.GetCheck() != 0 );
	m_pPaymentRecord -> SetAccountAllow( m_checkAccount.GetCheck() != 0 );
	m_pPaymentRecord -> SetTender( m_checkTender.GetCheck() != 0 );
	m_pPaymentRecord -> SetHandheld( m_checkHandheld.GetCheck() != 0 );
	m_pPaymentRecord -> SetLimit( m_dLimit );
	m_pPaymentRecord -> SetForexNo( m_nForexNo );
	m_pPaymentRecord -> SetExternalParams( strParams );

	//COMPULSORY SETTINGS FOR STANDARD CASH
	if ( m_pPaymentRecord -> GetPaymentNo() == 1 )
	{
		m_pPaymentRecord -> SetPaymentType( PAYMENT_TYPE_CASH );
		m_pPaymentRecord -> SetCashLimitFlag( TRUE );
		m_pPaymentRecord -> SetCashDeclarationFlag( TRUE );
	}
	else
	{
		m_pPaymentRecord -> SetCashLimitFlag( m_checkCashLimit.GetCheck() != 0 );
		m_pPaymentRecord -> SetCashDeclarationFlag( m_checkCashDecl.GetCheck() != 0 );

		int nType = PAYMENT_TYPE_CASH;
		int nSel = m_comboType.GetCurSel();
		if ((nSel >= 0) && (nSel < m_comboType.GetCount()))
		{
			nType = m_comboType.GetItemData(nSel);
		}

		m_pPaymentRecord -> SetPaymentType( nType );
	}

	switch( m_pPaymentRecord -> GetPaymentType() )
	{
	case PAYMENT_TYPE_OTHER:
		m_pPaymentRecord -> SetChangeAsCashbackFlag( m_checkChangeAsCashback.GetCheck() != 0 );
		m_pPaymentRecord -> SetEFTCashback ( FALSE );
		break;

	case PAYMENT_TYPE_EFT:
	case PAYMENT_TYPE_EFT_CNP:
	case PAYMENT_TYPE_EXTERNAL_PAY:
		m_pPaymentRecord -> SetChangeAsCashbackFlag( FALSE );
		m_pPaymentRecord -> SetEFTCashback ( m_checkCashback.GetCheck() != 0 );
		break;

	default:
		m_pPaymentRecord -> SetChangeAsCashbackFlag( FALSE );
		m_pPaymentRecord -> SetEFTCashback ( FALSE );
		break;
	}

	m_GroupSelector.SelectGroupFromEditBox();
	m_pPaymentRecord -> SetReportGroup( m_GroupSelector.GetGroupFilter() );

	int nFuncNo = 18;
	int nSel = m_comboFunction.GetCurSel();
	if ( (nSel >= 0) && (nSel < m_comboFunction.GetCount()))
	{
		nFuncNo = m_comboFunction.GetItemData(nSel);
	}

	m_pPaymentRecord -> SetFunctionNo( nFuncNo );
	m_pPaymentRecord -> SetDepositChangeType( m_comboDepositChangeType.GetCurSel() );
	m_pPaymentRecord -> SetDepositChangeVoucherMin( m_nDepositChangeVoucherMin );
}

/**********************************************************************/

void CPropPagePaymentGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePaymentGeneral::OnSelectGroup() 
{
	m_GroupSelector.SelectGroupFromCombo();
}

/**********************************************************************/

void CPropPagePaymentGeneral::OnKillFocusGroupNo() 
{
	m_GroupSelector.SelectGroupFromEditBox();
}

/**********************************************************************/

void CPropPagePaymentGeneral::OnButtonSetGroup()
{
	m_GroupSelector.SelectGroupFromEditBox();

	DataManager.PaymentGroup.RememberSettings();
	CListDataGroupDlg dlg( NODE_PAYMENT_GROUP, m_GroupSelector.GetGroupFilter(), this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CDataManagerInfo info;
		if ( DataManager.WritePaymentGroup( info ) == TRUE )
			DataManager.Payment.WriteSMPaymentGroupFile();
		else
			Prompter.WriteError( info );

		m_GroupSelector.FillGroupCombo ( dlg.GetGroupNoSel() );
	}
	else
		DataManager.PaymentGroup.RestoreSettings();
}

/**********************************************************************/

