/**********************************************************************/
#include "OfferGroupHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DefEditWalletStampRuleDlg.h"
/**********************************************************************/

CDefEditWalletStampRuleDlg::CDefEditWalletStampRuleDlg(int nIndex, CReportConsolidationArray<CSortedIntByInt>& arrayEditions, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditWalletStampRuleDlg::IDD, pParent),
	m_WarnCancelAfterChange(WarnCancelAfterChange),
	m_arrayOriginalEditions(arrayEditions)
{
	//{{AFX_DATA_INIT(CDefEditWalletStampRuleDlg)
	m_strDescription = "";
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_bAllowSpin = bAllowSpin;
	m_nOriginalEdition = 1;
	m_bIsOriginalEdition = TRUE;
	m_nInitialSelection = 0;
	m_nInitialType = 0;
	m_pListDataDlg = NULL;
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditWalletStampRuleDlg)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable);
	DDX_Control(pDX, IDC_STATIC_ACTION, m_staticAction);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	DDX_Control(pDX, IDC_STATIC_OFFERGROUP, m_staticOfferGroup);
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP, m_comboOfferGroup);
	DDX_Control(pDX, IDC_BUTTON_EDIT_GROUP, m_buttonEditGroup);
	DDX_Control(pDX, IDC_STATIC_EDITION, m_staticEdition);
	DDX_Control(pDX, IDC_EDIT_EDITION, m_editEdition);
	DDX_Control(pDX, IDC_BUTTON_EDITION, m_buttonEdition);
	DDX_Control(pDX, IDC_STATIC_THRESHOLD, m_staticThreshold);
	DDX_Control(pDX, IDC_STATIC_EXPIRY, m_staticExpiry);
	DDX_Control(pDX, IDC_STATIC_NEVEREXPIRE, m_staticNeverExpire);
	DDX_Control(pDX, IDC_SPIN, m_SpinCtrl);
	DDX_Control(pDX, IDC_DEFAULT_BUTTON, m_buttonDefault);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, WalletStampRule::Description.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditWalletStampRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditWalletStampRuleDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP, OnButtonEditGroup)
	ON_BN_CLICKED(IDC_BUTTON_EDITION, OnButtonEdition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditWalletStampRuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_THRESHOLD, SS_NUM, 6, "%d" );
	SubclassEdit( IDC_EDIT_EXPIRY, SS_NUM, 3, "%s" );

	if ( m_bAllowSpin == FALSE )
	{
		m_SpinCtrl.ShowWindow( SW_HIDE );
		m_SpinCtrl.EnableWindow( FALSE );
	}

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		m_checkEnable.SetWindowText( "Enable Purchase Control" );
		m_staticOfferGroup.SetWindowText("Item Group");
		ShowAndEnableWindow(&m_staticAction, FALSE);
		ShowAndEnableWindow(&m_comboAction, FALSE);
		ShowAndEnableWindow(&m_staticEdition, FALSE);
		ShowAndEnableWindow(&m_editEdition, FALSE);
		ShowAndEnableWindow(&m_buttonEdition, FALSE);
		MoveControl(&m_staticOfferGroup, 18, 79);
		MoveControl(&m_comboOfferGroup, 76, 78);
		MoveControl(&m_buttonEditGroup, 218, 78);
		MoveControl(&m_staticThreshold, 18, 109);
		MoveControl(GetEditThreshold(), 75, 108);
		MoveControl(&m_staticExpiry, 18, 129);
		MoveControl(GetEditExpiry(), 75, 128);
		MoveControl(&m_staticNeverExpire, 75, 151);
		MoveControl(&m_SpinCtrl, 7, 206);
		MoveControl(&m_buttonDefault, 125, 206);
		MoveControl(&m_buttonOK, 289, 206);
		MoveControl(&m_buttonCancel, 344, 206);
		ResizeDialog( 401, 227 );
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		m_checkEnable.SetWindowText("Enable Offer");
		m_staticOfferGroup.SetWindowText("Offer Group");
		
		//ADD THIS SECTION WHEN ACTION FIELD IS ENABLED
		/*
		{
			m_comboAction.AddString("Next item free");
			m_comboAction.AddString("Cheapest item free");
		}
		*/

		//REMOVE THIS SECTION WHEN ACTION FIELD IS ENABLED
		{
			ShowAndEnableWindow(&m_staticAction, FALSE);
			ShowAndEnableWindow(&m_comboAction, FALSE);
			MoveControl(&m_staticOfferGroup, 18, 79);
			MoveControl(&m_comboOfferGroup, 76, 78);
			MoveControl(&m_buttonEditGroup, 218, 78);
			MoveControl(&m_staticThreshold, 18, 109);
			MoveControl(GetEditThreshold(), 75, 108);
			MoveControl(&m_staticExpiry, 18, 129);
			MoveControl(GetEditExpiry(), 75, 128);
			MoveControl(&m_staticNeverExpire, 75, 151);
			MoveControl(&m_staticEdition, 18, 177);
			MoveControl(&m_editEdition, 75, 176);
			MoveControl(&m_buttonEdition, 118, 176);
			MoveControl(&m_SpinCtrl, 7, 226);
			MoveControl(&m_buttonDefault, 125, 226);
			MoveControl(&m_buttonOK, 289, 226);
			MoveControl(&m_buttonCancel, 344, 226);
			ResizeDialog(401, 247);
		}

		break;
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditWalletStampRuleDlg::GetEditThreshold(){ return GetEdit( IDC_EDIT_THRESHOLD ); }
CEdit* CDefEditWalletStampRuleDlg::GetEditExpiry(){ return GetEdit( IDC_EDIT_EXPIRY ); }
/**********************************************************************/

void CDefEditWalletStampRuleDlg::GetData()
{
	CWalletStampRuleCSVRecord Rule;
	DataManager.WalletStampRule.GetAt( m_nIndex, Rule );

	m_strInitialName = Rule.GetDescription();

	CString strType = "";
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		strType = "Purchase Control";
		break;
	
	case LOYALTY_TYPE_LOY_V4:
	default:
		strType = "Stamp Offer";
		//m_comboAction.SetCurSel( (Rule.GetAction() == 1) ? 0 : 1);
		break;
	}

	CString strTitle = "";
	strTitle.Format( "Edit %s (%d, %s)",
		(const char*) strType,
		Rule.GetRuleID(),
		Rule.GetDisplayName() );
	SetWindowText( strTitle );

	m_editDescription.SetWindowText( Rule.GetDescription() );
	m_checkEnable.SetCheck( Rule.GetEnableFlag() );

	COfferGroupHelpers::FillOfferGroupCombo(m_comboOfferGroup, Rule.GetOfferGroupNo());

	{
		CString str = "";
		str.Format("%d", Rule.GetThreshold());
		GetEditThreshold()->SetWindowText(str);
	}

	{
		CString str = "";
		str.Format("%d", Rule.GetExpiry());
		GetEditExpiry()->SetWindowText(str);
	}

	m_nOriginalEdition = 1;
	{
		CSortedIntByInt item;
		item.m_nKey = Rule.GetRuleID();

		int nPos = 0;
		if (m_arrayOriginalEditions.Find(item, nPos) == TRUE)
		{
			m_arrayOriginalEditions.GetAt(nPos, item);
		}

		m_nOriginalEdition = item.m_nVal;
	}
	
	m_bIsOriginalEdition = (Rule.GetEdition() == m_nOriginalEdition);
	{
		CString str = "";
		str.Format("%d", Rule.GetEdition());
		m_editEdition.SetWindowText(str);

		if (Rule.GetEdition() == m_nOriginalEdition)
		{
			m_buttonEdition.SetWindowText("Raise");
			m_bIsOriginalEdition = TRUE;
		}
		else
		{
			m_buttonEdition.SetWindowText("Revert");
			m_bIsOriginalEdition = FALSE;
		}
	}
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if (pNMUpDown->iDelta > 0)
	{
		SpinPrevious();
	}
	else
	{
		SpinNext();
	}

	*pResult = 0;
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::OnDefaultButton()
{
	SpinNext();
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::OnButtonEditGroup()
{
	if (COfferGroupHelpers::EditOfferGroupInternal( TRUE, m_comboOfferGroup, this) == TRUE)
	{
		COfferGroupHelpers::RefillOfferGroupCombo(m_comboOfferGroup);
	}
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::OnButtonEdition()
{
	if (TRUE == m_bIsOriginalEdition)
	{
		CString strWarn = "";
		strWarn += "Raising the edition of this offer will invalidate all of its existing stamps.\n\n";
		strWarn += "You should only proceed if you intend to replace this offer with a new\n";
		strWarn += "offer for which existing stamps may not be carried forward.\n\n";
		strWarn += "Are you sure you wish to proceed ?";

		if (Prompter.YesNo(strWarn) == IDYES)
		{
			m_bIsOriginalEdition = FALSE;
		}
	}
	else
	{
		m_bIsOriginalEdition = TRUE;
	}

	if (TRUE == m_bIsOriginalEdition)
	{
		CString str = "";
		str.Format("%d", m_nOriginalEdition);
		m_editEdition.SetWindowText(str);
		m_buttonEdition.SetWindowText("Raise");
	}
	else
	{
		CString str = "";
		str.Format("%d", m_nOriginalEdition + 1);
		m_editEdition.SetWindowText(str);
		m_buttonEdition.SetWindowText("Revert");
	}
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if (m_pListDataDlg != NULL)
			{
				m_pListDataDlg->SelectLine(m_nIndex);
			}
		}
	}
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManager.WalletStampRule.GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if (m_pListDataDlg != NULL)
			{
				m_pListDataDlg->SelectLine(m_nIndex);
			}
		}
	}
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::SaveData()
{
	CWalletStampRuleCSVRecord Rule;
	DataManager.WalletStampRule.GetAt( m_nIndex, Rule );

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		Rule.SetAction(3);
		Rule.SetEdition(1);
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		Rule.SetAction(1);
		//Rule.SetAction(m_comboAction.GetCurSel() + 1);

		if (TRUE == m_bIsOriginalEdition)
		{
			Rule.SetEdition(m_nOriginalEdition);
		}
		else
		{
			Rule.SetEdition(m_nOriginalEdition + 1);
		}
		break;
	}
	
	Rule.SetDescription( m_strDescription );
	Rule.SetEnableFlag( m_checkEnable.GetCheck() != 0 );
	Rule.SetOfferGroupNo(COfferGroupHelpers::GetOfferGroup(m_comboOfferGroup));
	Rule.SetThreshold(GetEditBoxInt(*GetEditThreshold()));
	Rule.SetExpiry(GetEditBoxInt(*GetEditExpiry()));
			
	DataManager.WalletStampRule.SetAt( m_nIndex, Rule );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditWalletStampRuleDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/