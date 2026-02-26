//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "GroupPropertyPage4.h"
//$$******************************************************************

CGroupPropertyPage4::CGroupPropertyPage4(CSQLRowGroup& RowGroup, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopupBonus)
	: CSSPropertyPage(CGroupPropertyPage4::IDD), m_RowGroup(RowGroup), m_arrayGroupTopupBonus(arrayGroupTopupBonus)
{
	m_bShowPurse2 = System.GetEnablePurse2Flag();

	for (int p = 1; p <= 2; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			m_nLastSetting[p][t] = -1;
		}
	}
}

//$$******************************************************************

CGroupPropertyPage4::~CGroupPropertyPage4()
{
}

//$$******************************************************************

void CGroupPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_SETTING1, m_comboSetting[1][1]);
	DDX_Control(pDX, IDC_COMBO_SETTING2, m_comboSetting[1][2]);
	DDX_Control(pDX, IDC_COMBO_SETTING3, m_comboSetting[1][3]);
	DDX_Control(pDX, IDC_COMBO_SETTING4, m_comboSetting[1][4]);
	DDX_Control(pDX, IDC_COMBO_SETTING5, m_comboSetting[1][5]);
	DDX_Control(pDX, IDC_COMBO_SETTING6, m_comboSetting[2][1]);
	DDX_Control(pDX, IDC_COMBO_SETTING7, m_comboSetting[2][2]);
	DDX_Control(pDX, IDC_COMBO_SETTING8, m_comboSetting[2][3]);
	DDX_Control(pDX, IDC_COMBO_SETTING9, m_comboSetting[2][4]);
	DDX_Control(pDX, IDC_COMBO_SETTING10, m_comboSetting[2][5]);
	DDX_Control(pDX, IDC_COMBO_TYPE1, m_comboType[1][1]);
	DDX_Control(pDX, IDC_COMBO_TYPE2, m_comboType[1][2]);
	DDX_Control(pDX, IDC_COMBO_TYPE3, m_comboType[1][3]);
	DDX_Control(pDX, IDC_COMBO_TYPE4, m_comboType[1][4]);
	DDX_Control(pDX, IDC_COMBO_TYPE5, m_comboType[1][5]);
	DDX_Control(pDX, IDC_COMBO_TYPE6, m_comboType[2][1]);
	DDX_Control(pDX, IDC_COMBO_TYPE7, m_comboType[2][2]);
	DDX_Control(pDX, IDC_COMBO_TYPE8, m_comboType[2][3]);
	DDX_Control(pDX, IDC_COMBO_TYPE9, m_comboType[2][4]);
	DDX_Control(pDX, IDC_COMBO_TYPE10, m_comboType[2][5]);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP1, m_staticP2Topup1);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP2, m_staticP2Topup2);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP3, m_staticP2Topup3);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP4, m_staticP2Topup4);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP5, m_staticP2Topup5);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage4, CPropertyPage)

	ON_CBN_SELCHANGE(IDC_COMBO_SETTING1, OnSelectSettingPurse1Topup1)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING2, OnSelectSettingPurse1Topup2)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING3, OnSelectSettingPurse1Topup3)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING4, OnSelectSettingPurse1Topup4)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING5, OnSelectSettingPurse1Topup5)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING6, OnSelectSettingPurse2Topup1)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING7, OnSelectSettingPurse2Topup2)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING8, OnSelectSettingPurse2Topup3)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING9, OnSelectSettingPurse2Topup4)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING10, OnSelectSettingPurse2Topup5)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE1, OnSelectPurse1BonusType1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE2, OnSelectPurse1BonusType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE3, OnSelectPurse1BonusType3)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE4, OnSelectPurse1BonusType4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE5, OnSelectPurse1BonusType5)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE6, OnSelectPurse2BonusType1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE7, OnSelectPurse2BonusType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE8, OnSelectPurse2BonusType3)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE9, OnSelectPurse2BonusType4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE10, OnSelectPurse2BonusType5)

END_MESSAGE_MAP()


//$$******************************************************************

BOOL CGroupPropertyPage4::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	Server.Read(Filenames.GetServerDataFilename());

	SubclassEdit(IDC_EDIT_REVAL_PLU1, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU2, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU3, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU4, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU5, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU6, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU7, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU8, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU9, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_PLU10, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_REVAL_TRIP1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP3, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP4, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP5, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP6, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP7, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP8, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP9, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_TRIP10, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_POINTS1, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS2, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS3, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS4, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS5, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS6, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS7, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS8, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS9, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_POINTS10, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REVAL_VALUE1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE3, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE4, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE5, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE6, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE7, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE8, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE9, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_VALUE10, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT1, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT2, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT3, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT4, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT5, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT6, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT7, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT8, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT9, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_REVAL_PERCENT10, SS_NUM, 3, "%d");

	int nMaxPurse = m_bShowPurse2 ? 2 : 1;

	for (int p = 1; p <= nMaxPurse; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			CSQLRowGroupTopupBonus RowGroupBonus;
			RowGroupBonus.SetGroupNo(m_RowGroup.GetGroupNo());
			RowGroupBonus.SetTopupNo(t);
			RowGroupBonus.SetPurseNo(p);

			int nPos = 0;
			if (m_arrayGroupTopupBonus.Find(RowGroupBonus, nPos) == TRUE)
			{
				m_arrayGroupTopupBonus.GetAt(nPos, RowGroupBonus);
			}

			bool bCanEdit = FALSE;
			bCanEdit |= ((1 == p) && (1 == t));
			
			if (1 == p)
			{
				bCanEdit |= (Server.GetTopupPCBonusTypePurse1() == t);
			}
			else
			{
				bCanEdit |= (Server.GetTopupPCBonusTypePurse2() == t);
			}

			bCanEdit |= Server.HaveRevaluationPluCode(p, t);

			int nType = 0;
			if ((RowGroupBonus.GetBonusFlag() == FALSE) || (RowGroupBonus.GetBonusDisable() == TRUE) || (FALSE == bCanEdit))
			{
				nType = Server.GetTopupBonusType(p,t);
				SetEditBoxDouble(*GetEditRevalTrip(p, t), Server.GetTopupBonusTrip(p, t), 2);
				SetEditBoxInt(*GetEditRevalPoints(p, t), Server.GetTopupBonusPoints(p, t));
				SetEditBoxDouble(*GetEditRevalValue(p, t), Server.GetTopupBonusValue(p, t), 2);
				SetEditBoxInt(*GetEditRevalPercent(p, t), Server.GetTopupBonusPercent(p, t));
			}
			else
			{
				nType = RowGroupBonus.GetBonusType();
				SetEditBoxDouble(*GetEditRevalTrip(p, t), RowGroupBonus.GetBonusTrip(), 2);
				SetEditBoxInt(*GetEditRevalPoints(p, t), RowGroupBonus.GetBonusPoints());
				SetEditBoxDouble(*GetEditRevalValue(p, t), RowGroupBonus.GetBonusValue(), 2);
				SetEditBoxInt(*GetEditRevalPercent(p, t), RowGroupBonus.GetBonusPercent());
			}

			if ((nType < 0) || (nType > 2))
			{
				nType = 0;
			}

			m_comboType[p][t].ResetContent();
			m_comboType[p][t].AddString("Points");
			m_comboType[p][t].AddString("Value");
			m_comboType[p][t].AddString("Percent");
			m_comboType[p][t].SetCurSel(nType);

			m_comboSetting[p][t].ResetContent();
			if (TRUE == bCanEdit)
			{
				m_comboSetting[p][t].AddString("Global");
				m_comboSetting[p][t].AddString("Group");
				m_comboSetting[p][t].AddString("Disable");
				m_comboSetting[p][t].SetCurSel(0);

				if (RowGroupBonus.GetBonusFlag() == FALSE)
				{
					m_comboSetting[p][t].SetCurSel(0);
				}
				else if (RowGroupBonus.GetBonusDisable() == TRUE)
				{
					m_comboSetting[p][t].SetCurSel(2);
				}
				else
				{
					m_comboSetting[p][t].SetCurSel(1);
				}
			}
			else
			{
				m_comboSetting[p][t].AddString("Not used");
				m_comboSetting[p][t].SetCurSel(0);
			}

			m_nLastSetting[p][t] = -1;
		}
	}

	OnSelectSettingPurse1Topup1();
	OnSelectSettingPurse1Topup2();
	OnSelectSettingPurse1Topup3();
	OnSelectSettingPurse1Topup4();
	OnSelectSettingPurse1Topup5();

	if (TRUE == m_bShowPurse2)
	{
		OnSelectSettingPurse2Topup1();
		OnSelectSettingPurse2Topup2();
		OnSelectSettingPurse2Topup3();
		OnSelectSettingPurse2Topup4();
		OnSelectSettingPurse2Topup5();
	}
	else
	{
		ShowAndEnableWindow(&m_staticP2Topup1, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup2, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup3, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup4, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup5, FALSE);

		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			ShowAndEnableWindow(&m_comboSetting[2][t], FALSE);
			ShowAndEnableWindow(&m_comboType[2][t], FALSE);
			ShowAndEnableWindow(GetEditRevalTrip(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalPoints(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalValue(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalPercent(2, t), FALSE);
		}
	}

	return TRUE;
}

//$$******************************************************************

CEdit* CGroupPropertyPage4::GetEditRevalTrip(int p, int t)
{
	switch (p)
	{
	case 1:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_TRIP1);
		case 2:	return GetEdit(IDC_EDIT_REVAL_TRIP2);
		case 3:	return GetEdit(IDC_EDIT_REVAL_TRIP3);
		case 4:	return GetEdit(IDC_EDIT_REVAL_TRIP4);
		case 5:	return GetEdit(IDC_EDIT_REVAL_TRIP5);
		default: return NULL;
		}
		break;

	case 2:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_TRIP6);
		case 2:	return GetEdit(IDC_EDIT_REVAL_TRIP7);
		case 3:	return GetEdit(IDC_EDIT_REVAL_TRIP8);
		case 4:	return GetEdit(IDC_EDIT_REVAL_TRIP9);
		case 5:	return GetEdit(IDC_EDIT_REVAL_TRIP10);
		default: return NULL;
		}
		break;

	default:
		return NULL;
	}
}

//$$******************************************************************

CEdit* CGroupPropertyPage4::GetEditRevalPoints(int p, int t)
{
	switch (p)
	{
	case 1:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_POINTS1);
		case 2:	return GetEdit(IDC_EDIT_REVAL_POINTS2);
		case 3:	return GetEdit(IDC_EDIT_REVAL_POINTS3);
		case 4:	return GetEdit(IDC_EDIT_REVAL_POINTS4);
		case 5:	return GetEdit(IDC_EDIT_REVAL_POINTS5);
		default: return NULL;
		}
		break;

	case 2:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_POINTS6);
		case 2:	return GetEdit(IDC_EDIT_REVAL_POINTS7);
		case 3:	return GetEdit(IDC_EDIT_REVAL_POINTS8);
		case 4:	return GetEdit(IDC_EDIT_REVAL_POINTS9);
		case 5:	return GetEdit(IDC_EDIT_REVAL_POINTS10);
		default: return NULL;
		}
		break;

	default:
		return NULL;
	}
}

//$$******************************************************************

CEdit* CGroupPropertyPage4::GetEditRevalValue(int p, int t)
{
	switch(p)
	{
	case 1:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_VALUE1);
		case 2:	return GetEdit(IDC_EDIT_REVAL_VALUE2);
		case 3:	return GetEdit(IDC_EDIT_REVAL_VALUE3);
		case 4:	return GetEdit(IDC_EDIT_REVAL_VALUE4);
		case 5:	return GetEdit(IDC_EDIT_REVAL_VALUE5);
		default: return NULL;
		}
		break;

	case 2:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_VALUE6);
		case 2:	return GetEdit(IDC_EDIT_REVAL_VALUE7);
		case 3:	return GetEdit(IDC_EDIT_REVAL_VALUE8);
		case 4:	return GetEdit(IDC_EDIT_REVAL_VALUE9);
		case 5:	return GetEdit(IDC_EDIT_REVAL_VALUE10);
		default: return NULL;
		}
		break;

	default:
		return NULL;
	}
}

//$$******************************************************************

CEdit* CGroupPropertyPage4::GetEditRevalPercent(int p, int t)
{
	switch (p)
	{
	case 1:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_PERCENT1);
		case 2:	return GetEdit(IDC_EDIT_REVAL_PERCENT2);
		case 3:	return GetEdit(IDC_EDIT_REVAL_PERCENT3);
		case 4:	return GetEdit(IDC_EDIT_REVAL_PERCENT4);
		case 5:	return GetEdit(IDC_EDIT_REVAL_PERCENT5);
		default: return NULL;
		}
		break;

	case 2:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_PERCENT6);
		case 2:	return GetEdit(IDC_EDIT_REVAL_PERCENT7);
		case 3:	return GetEdit(IDC_EDIT_REVAL_PERCENT8);
		case 4:	return GetEdit(IDC_EDIT_REVAL_PERCENT9);
		case 5:	return GetEdit(IDC_EDIT_REVAL_PERCENT10);
		default: return NULL;
		}
		break;

	default:
		return NULL;
	}
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse1BonusType1()
{
	SelectType(1,1);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse1BonusType2()
{
	SelectType(1, 2);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse1BonusType3()
{
	SelectType(1, 3);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse1BonusType4()
{
	SelectType(1, 4);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse1BonusType5()
{
	SelectType(1, 5);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse2BonusType1()
{
	SelectType(2, 1);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse2BonusType2()
{
	SelectType(2, 2);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse2BonusType3()
{
	SelectType(2, 3);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse2BonusType4()
{
	SelectType(2, 4);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectPurse2BonusType5()
{
	SelectType(2, 5);
}

//$$******************************************************************

void CGroupPropertyPage4::SelectType(int p, int t)
{
	bool bEnableEdit = (m_comboSetting[p][t].GetCurSel() == 1);

	int nType = m_comboType[p][t].GetCurSel();
	ShowAndEnableWindow(GetEditRevalPoints(p, t), (0 == nType), bEnableEdit);
	ShowAndEnableWindow(GetEditRevalValue(p, t), (1 == nType), bEnableEdit);
	ShowAndEnableWindow(GetEditRevalPercent(p, t), (2 == nType), bEnableEdit);

	CEdit* pEdit = NULL;
	switch (nType)
	{
	case 1:
		pEdit = GetEditRevalValue(p, t);
		break;

	case 2:
		pEdit = GetEditRevalPercent(p, t);
		break;

	case 0:
	default:
		pEdit = GetEditRevalPoints(p, t);
		break;
	}

	pEdit->SetFocus();
	pEdit->SetSel(0, -1);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse1Topup1()
{
	SelectSetting(1,1);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse1Topup2()
{
	SelectSetting(1, 2);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse1Topup3()
{
	SelectSetting(1, 3);
}


//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse1Topup4()
{
	SelectSetting(1, 4);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse1Topup5()
{
	SelectSetting(1, 5);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse2Topup1()
{
	SelectSetting(2, 1);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse2Topup2()
{
	SelectSetting(2, 2);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse2Topup3()
{
	SelectSetting(2, 3);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse2Topup4()
{
	SelectSetting(2, 4);
}

//$$******************************************************************

void CGroupPropertyPage4::OnSelectSettingPurse2Topup5()
{
	SelectSetting(2, 5);
}

//$$******************************************************************

void CGroupPropertyPage4::SelectSetting(int p, int t)
{
	int nSel = m_comboSetting[p][t].GetCurSel();
	if (nSel != m_nLastSetting[p][t])
	{
		int nType = 0;
		if ((m_nLastSetting[p][t] == -1) && (1 == nSel))
		{
			CSQLRowGroupTopupBonus RowGroupBonus;
			RowGroupBonus.SetGroupNo(m_RowGroup.GetGroupNo());
			RowGroupBonus.SetTopupNo(t);
			RowGroupBonus.SetPurseNo(p);

			int nPos = 0;
			if (m_arrayGroupTopupBonus.Find(RowGroupBonus, nPos) == TRUE)
			{
				m_arrayGroupTopupBonus.GetAt(nPos, RowGroupBonus);
			}

			SetEditBoxDouble(*GetEditRevalTrip(p,t), RowGroupBonus.GetBonusTrip(), 2);
			SetEditBoxInt(*GetEditRevalPoints(p,t), RowGroupBonus.GetBonusPoints());
			SetEditBoxDouble(*GetEditRevalValue(p,t), RowGroupBonus.GetBonusValue(), 2);
			SetEditBoxInt(*GetEditRevalPercent(p,t), RowGroupBonus.GetBonusPercent());
			nType = RowGroupBonus.GetBonusType();
		}
		else
		{
			SetEditBoxDouble(*GetEditRevalTrip(p, t), Server.GetTopupBonusTrip(p, t), 2);
			SetEditBoxInt(*GetEditRevalPoints(p, t), Server.GetTopupBonusPoints(p, t));
			SetEditBoxDouble(*GetEditRevalValue(p, t), Server.GetTopupBonusValue(p, t), 2);
			SetEditBoxInt(*GetEditRevalPercent(p, t), Server.GetTopupBonusPercent(p, t));
			nType = Server.GetTopupBonusType(p, t);
		}
		
		m_nLastSetting[p][t] = nSel;

		if ((nType < 0) || (nType > 2))
		{
			nType = 0;
		}

		m_comboType[p][t].SetCurSel(nType);

		m_comboType[p][t].EnableWindow(1 == nSel);
		GetEditRevalTrip(p,t)->EnableWindow(1 == nSel);
		GetEditRevalPoints(p, t)->EnableWindow(1 == nSel);
		GetEditRevalValue(p, t)->EnableWindow(1 == nSel);
		GetEditRevalPercent(p, t)->EnableWindow(1 == nSel);

		SelectType(p, t);
	}
}

//$$******************************************************************

BOOL CGroupPropertyPage4::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CGroupPropertyPage4::OnKillActive()
{
	m_arrayGroupTopupBonus.RemoveAll();

	int nMaxPurse = m_bShowPurse2 ? 2 : 1;

	for (int p = 1; p <= nMaxPurse; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			int nRevalType = m_comboType[p][t].GetCurSel();
			int nPercent = GetEditBoxInt(*GetEditRevalPercent(p,t));

			CSQLRowGroupTopupBonus RowGroupBonus;
			RowGroupBonus.SetGroupNo(m_RowGroup.GetGroupNo());
			RowGroupBonus.SetTopupNo(t);
			RowGroupBonus.SetPurseNo(p);

			switch (m_comboSetting[p][t].GetCurSel())
			{
			case 1:
				RowGroupBonus.SetBonusFlag(TRUE);
				RowGroupBonus.SetBonusDisable(FALSE);
				RowGroupBonus.SetBonusTrip(GetEditBoxDouble(*GetEditRevalTrip(p,t)));
				RowGroupBonus.SetBonusType(nRevalType);
				RowGroupBonus.SetBonusPoints(GetEditBoxInt(*GetEditRevalPoints(p, t)));
				RowGroupBonus.SetBonusValue(GetEditBoxDouble(*GetEditRevalValue(p, t)));
				RowGroupBonus.SetBonusPercent(nPercent);
				break;

			case 2:
				RowGroupBonus.SetBonusFlag(TRUE);
				RowGroupBonus.SetBonusDisable(TRUE);
				break;

			case 0:
			default:
				RowGroupBonus.SetBonusFlag(FALSE);
				break;
			}

			m_arrayGroupTopupBonus.Consolidate(RowGroupBonus);

			if ((m_comboSetting[p][t].GetCurSel() == 1) && (2 == nRevalType))
			{
				if ((nPercent < 0) || (nPercent > 100))
				{
					Prompter.Info("Please enter a percentage between 0 and 100");
					GetEditRevalPercent(p,t)->SetFocus();
					return FALSE;
				}
			}
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

