//$$******************************************************************
#include "..\SmartPay4Shared\GroupSetText.h"
#include "..\SmartPay4Shared\UserTypeText.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "GroupPropertyPage1.h"
//$$******************************************************************

CGroupPropertyPage1::CGroupPropertyPage1(CSQLRowGroupFull& RowGroup)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CGroupPropertyPage1::IDD)
	, m_RowGroup(RowGroup)
{
	m_nUserTypeIndex = 0;
	m_bEnablePriceLevel = TRUE;
	m_nSetNo = 0;
	m_nPriceLevel = 0;
	m_nPointsAchievement = 0;
	m_bVatShift = FALSE;
}

//$$******************************************************************

CGroupPropertyPage1::~CGroupPropertyPage1()
{
}

//$$******************************************************************

void CGroupPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_USERTYPE, m_comboUserType);
	DDX_Control(pDX, IDC_CHECK_PURCHASECONTROL, m_checkPurchaseControl);
	DDX_Control(pDX, IDC_CHECK_KIOSKREVAL, m_checkKioskReval);
	DDX_Control(pDX, IDC_STATIC_SETNO, m_staticSetNo );
	DDX_Control(pDX, IDC_SPIN_GROUPSET, m_spinSetNo );
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL, m_checkPriceLevel);
	DDX_Control(pDX, IDC_SPIN_PRICELEVEL, m_spinPriceLevel );
	DDX_Control(pDX, IDC_CHECK_VATSHIFT, m_checkVATShift );
	DDX_Control(pDX, IDC_STATIC2, m_static2 );
	DDX_Control(pDX, IDC_STATIC3, m_static3 );
	DDX_Control(pDX, IDC_STATIC4, m_static4 );
	DDX_Control(pDX, IDC_STATIC5, m_static5 );
	DDX_Control(pDX, IDC_EDIT_DEPTSINHIBITED, m_editDeptsInhibited);
	DDX_Control(pDX, IDC_STATIC_SHIFTGROUP, m_staticShiftGroup);
	DDX_Control(pDX, IDC_EDIT_SHIFTGROUP, m_editShiftGroup);
	DDX_CBIndex(pDX, IDC_COMBO_USERTYPE, m_nUserTypeIndex);
	DDX_Text(pDX, IDC_EDIT_SETNO, m_nSetNo);
	DDV_MinMaxInt(pDX, m_nSetNo, 1, 9);
	DDX_Text(pDX, IDC_EDIT_PRICELEVEL, m_nPriceLevel);
	DDV_MinMaxInt(pDX, m_nPriceLevel, 1, 9);
	DDX_Check(pDX, IDC_CHECK_VATSHIFT, m_bVatShift);
	DDX_Text(pDX, IDC_EDIT_POINTSACHIEVEMENT, m_nPointsAchievement);
	DDX_Text(pDX, IDC_EDIT_DEPTSINHIBITED, m_strDeptInhibitList);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRICELEVEL, OnSpinPriceLevel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GROUPSET, OnSpinGroupSet)
	ON_BN_CLICKED(IDC_CHECK_PRICELEVEL,OnTogglePriceLevel)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage1::OnInitDialog()
{
	m_nPriceLevel = m_RowGroup.GetPriceLevel();

	if (0 == m_nPriceLevel)
	{
		m_bEnablePriceLevel = FALSE;
		m_nPriceLevel = 1;
	}
	else
	{
		m_bEnablePriceLevel = TRUE;
	}

	m_nSetNo = m_RowGroup.GetSetNo();
	int nUserType = m_RowGroup.GetUserType();
	m_bVatShift = ( m_RowGroup.GetVatShiftFlag() != 0 );
	m_strDeptInhibitList = m_RowGroup.GetDeptInhibitCash();
	m_nPointsAchievement = m_RowGroup.GetPointsAchievement();

	CSSPropertyPage::OnInitDialog();

	m_editName.SetWindowText(m_RowGroup.GetGroupName());

	SubclassEdit(IDC_EDIT_SETNO, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_PRICELEVEL, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_POINTSACHIEVEMENT, SS_NUM, 6, "%d");

	// user types

	int nUserCount = 0;
	int nUserTypeCursel = 0;
	CUserTypeText usertext;
	usertext.Open(Filenames.GetUserTypeFilename());			// readonly
	for (int n = 1; n <= USERTYPE_MAX; n++)
	{
		CString strText = usertext.GetText(n);
		if (strText.GetLength() > 0)
		{
			++nUserCount;
			CString strBuf;
			strBuf.Format("%d : %s", 
				n, 
				(const char*) strText);

			m_comboUserType.AddString(strBuf);

			if (n == nUserType)
			{
				nUserTypeCursel = m_comboUserType.GetCount() - 1;
			}
		}
	}
	m_comboUserType.SetCurSel(nUserTypeCursel);
	if (nUserCount < 2)
	{
		m_comboUserType.EnableWindow(FALSE);
	}

	m_editShiftGroup.LimitText(29);
	m_editShiftGroup.SetWindowText(m_RowGroup.GetShiftGroups());

	if (System.GetAllowGroupSetsFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_staticSetNo, FALSE);
		ShowAndEnableWindow(GetEditSetNo(), FALSE);
		ShowAndEnableWindow(&m_spinSetNo, FALSE);
		MoveControl(&m_checkPriceLevel, 17, 68);
		MoveControl(GetEditPriceLevel(), 94, 66);
		MoveControl(&m_spinPriceLevel, 120, 66);
		MoveControl(&m_checkVATShift, 17, 88);
		MoveControl(&m_static2, 17, 120);
		MoveControl(&m_static3, 17, 130);
		MoveControl(GetEditPointsAchievement(), 94, 118);
		MoveControl(&m_static4, 17, 152);
		MoveControl(&m_static5, 17, 162);
		MoveControl(&m_editDeptsInhibited, 94, 150);
		MoveControl(&m_staticShiftGroup, 17, 184);
		MoveControl(&m_editShiftGroup, 94, 182);
	}

	if (System.GetAllowGroupShiftDateRangeFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_staticShiftGroup, FALSE);
		ShowAndEnableWindow(&m_editShiftGroup, FALSE);
	}

	m_checkPriceLevel.SetCheck(m_bEnablePriceLevel);
	OnTogglePriceLevel();

	m_checkPurchaseControl.SetCheck(m_RowGroup.GetPurchaseControlFlag());
	m_checkKioskReval.SetCheck(m_RowGroup.GetAllowRevalKioskFlag());

	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage1::GetEditSetNo(){ return GetEdit( IDC_EDIT_SETNO ); }
CEdit* CGroupPropertyPage1::GetEditPriceLevel(){ return GetEdit( IDC_EDIT_PRICELEVEL ); }
CEdit* CGroupPropertyPage1::GetEditPointsAchievement(){ return GetEdit( IDC_EDIT_POINTSACHIEVEMENT ); }
//$$******************************************************************

BOOL CGroupPropertyPage1::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		if (m_RowGroup.ValidateDeptInhibitList(m_strDeptInhibitList) == FALSE)
		{
			m_RowGroup.DisplayDeptListError();
			m_editDeptsInhibited.SetFocus();
			m_editDeptsInhibited.SetSel(0, -1);
			return FALSE;
		}

		CString strName = GetEditBoxText(m_editName);

		if (strName == "")
		{
			strName.Format("Group %d", m_RowGroup.GetGroupNo());
		}

		m_RowGroup.SetGroupName(strName);
		m_RowGroup.SetSetNo(m_nSetNo);

		if (TRUE == m_bEnablePriceLevel)
		{
			m_RowGroup.SetPriceLevel(m_nPriceLevel);
		}
		else
		{
			m_RowGroup.SetPriceLevel(0);
		}

		int nUserType = Group::UserType.Min;
		if (m_nUserTypeIndex != -1)
		{
			CString strText;
			m_comboUserType.GetLBText(m_nUserTypeIndex, strText);
			nUserType = atoi(strText.Left(1));
		}
		m_RowGroup.SetUserType(nUserType);
		m_RowGroup.SetPurchaseControlFlag(IsTicked(m_checkPurchaseControl));
		m_RowGroup.SetAllowRevalKioskFlag(IsTicked(m_checkKioskReval));
		m_RowGroup.SetVatShiftFlag(m_bVatShift ? 1 : 0);
		m_RowGroup.SetDeptInhibitCash(m_strDeptInhibitList);
		m_RowGroup.SetPointsAchievement(m_nPointsAchievement);

		{
			CString strIn = GetEditBoxText(m_editShiftGroup);
			CString strOut = "";
			CString strError = "";

			m_RowGroup.ValidateShiftGroups(strIn, strOut, strError);

			if (strError != "")
			{
				CString strMsg = "";
				strMsg += "Invalid entry for shift groups.\n\n";
				strMsg += strError;
				Prompter.Error(strMsg);
				m_editShiftGroup.SetFocus();
				m_editShiftGroup.SetSel(0, -1);
				return FALSE;
			}

			m_RowGroup.SetShiftGroups(strOut);
		}
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CGroupPropertyPage1::OnTogglePriceLevel()
{
	m_bEnablePriceLevel = IsTicked(m_checkPriceLevel);
	GetEditPriceLevel()->EnableWindow(m_bEnablePriceLevel);
	m_spinPriceLevel.EnableWindow(m_bEnablePriceLevel);
}

//$$******************************************************************

void CGroupPropertyPage1::OnSpinPriceLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if ( UpdateData() == TRUE )
	{
		if (pNMUpDown->iDelta > 0)			// previous / Down
		{
			if (m_nPriceLevel > 1)
			{
				--m_nPriceLevel;
			}
		}
		else									// next / Up
		{
			if (m_nPriceLevel < 9)
			{
				++m_nPriceLevel;
			}
		}	

		UpdateData(FALSE);	
	}
	
	*pResult = 0;
}

//*******************************************************

void CGroupPropertyPage1::OnSpinGroupSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if ( UpdateData() == TRUE )
	{
		if ( pNMUpDown->iDelta > 0 )			// previous / Down
		{	
			if (m_nSetNo > 1)
			{
				--m_nSetNo;
			}
		}
		else									// next / Up
		{
			if (m_nSetNo < GROUPSET_MAX)
			{
				++m_nSetNo;
			}
		}	

		UpdateData(FALSE);	
	}
	*pResult = 0;
}

//$$******************************************************************
