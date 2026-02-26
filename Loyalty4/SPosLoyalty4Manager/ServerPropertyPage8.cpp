//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "ServerPropertyPage8.h"
//$$******************************************************************

CServerPropertyPage8::CServerPropertyPage8()
	: CSSPropertyPage(CServerPropertyPage8::IDD)
{
	m_bShowPurse2 = System.GetEnablePurse2Flag();
}

//$$******************************************************************

void CServerPropertyPage8::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_COMBO_REVAL_BONUS1, m_comboRevalBonus1);
	DDX_Control(pDX, IDC_COMBO_REVAL_BONUS2, m_comboRevalBonus2);
	DDX_Control(pDX, IDC_EDIT_REVAL_COMMENT, m_editRevalComment1);
	DDX_Control(pDX, IDC_EDIT_REVAL_COMMENT2, m_editRevalComment2);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP1, m_staticP2Topup1);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP2, m_staticP2Topup2);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP3, m_staticP2Topup3);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP4, m_staticP2Topup4);
	DDX_Control(pDX, IDC_STATIC_P2TOPUP5, m_staticP2Topup5);
	DDX_Control(pDX, IDC_STATIC_P2BONUS, m_staticP2Bonus);
	DDX_Control(pDX, IDC_STATIC_P2COMMENT, m_staticP2Comment);
	DDX_Control(pDX, IDC_STATIC_P1BONUS, m_staticP1Bonus);
	DDX_Control(pDX, IDC_STATIC_P1COMMENT, m_staticP1Comment);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage8, CPropertyPage)
	
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE1, OnSelectPurse1BonusType1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE2, OnSelectPurse1BonusType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE3, OnSelectPurse1BonusType3)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE4, OnSelectPurse1BonusType4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE5, OnSelectPurse1BonusType5)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE6, OnSelectPurse2BonusType1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE7, OnSelectPurse2BonusType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE8, OnSelectPurse1BonusType3)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE9, OnSelectPurse2BonusType4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE10, OnSelectPurse2BonusType5)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage8::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

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
	/*****/
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
	/*****/	
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
	/*****/
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
	/*****/
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
			GetEditRevalPlu(p,t)->SetWindowText(Server.GetTopupPluNo(p,t));
			SetEditBoxDouble(*GetEditRevalTrip(p,t), Server.GetTopupBonusTrip(p,t), 2);
			SetEditBoxInt(*GetEditRevalPoints(p,t), Server.GetTopupBonusPoints(p,t));
			SetEditBoxDouble(*GetEditRevalValue(p,t), Server.GetTopupBonusValue(p,t), 2);
			SetEditBoxInt(*GetEditRevalPercent(p,t), Server.GetTopupBonusPercent(p,t));

			int nType = Server.GetTopupBonusType(p,t);

			if ((nType < 0) || (nType > 2))
			{
				nType = 0;
			}

			m_comboType[p][t].ResetContent();
			m_comboType[p][t].AddString("Points");
			m_comboType[p][t].AddString("Value");
			m_comboType[p][t].AddString("Percent");
			m_comboType[p][t].SetCurSel(nType);
		}
	}

	CStandardCombos::FillPCBonusCombo(m_comboRevalBonus1, Server.GetTopupPCBonusTypePurse1());
	m_editRevalComment1.SetWindowText(Server.GetTopupCommentPurse1());

	OnSelectPurse1BonusType1();
	OnSelectPurse1BonusType2();
	OnSelectPurse1BonusType3();
	OnSelectPurse1BonusType4();
	OnSelectPurse1BonusType5();

	if (TRUE == m_bShowPurse2)
	{
		CStandardCombos::FillPCBonusCombo(m_comboRevalBonus2, Server.GetTopupPCBonusTypePurse2());
		m_editRevalComment2.SetWindowText(Server.GetTopupCommentPurse2());	

		OnSelectPurse2BonusType1();
		OnSelectPurse2BonusType2();
		OnSelectPurse1BonusType3();
		OnSelectPurse2BonusType4();
		OnSelectPurse2BonusType5();
	}
	else
	{
		ShowAndEnableWindow(&m_staticP2Topup1, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup2, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup3, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup4, FALSE);
		ShowAndEnableWindow(&m_staticP2Topup5, FALSE);
		ShowAndEnableWindow(&m_staticP2Bonus, FALSE);
		ShowAndEnableWindow(&m_staticP2Comment, FALSE);

		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			ShowAndEnableWindow(GetEditRevalPlu(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalTrip(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalPoints(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalValue(2, t), FALSE);
			ShowAndEnableWindow(GetEditRevalPercent(2, t), FALSE);
			ShowAndEnableWindow(&m_comboType[2][t], FALSE);
		}

		ShowAndEnableWindow(&m_comboRevalBonus2, FALSE);
		ShowAndEnableWindow(&m_editRevalComment2, FALSE);

		MoveControl(&m_staticP1Bonus, 25, 163);
		MoveControl(&m_comboRevalBonus1, 101, 161);
		MoveControl(&m_staticP1Comment, 25, 181);
		MoveControl(&m_editRevalComment1, 101, 179);
	}

	return TRUE;
}

//$$******************************************************************

CEdit* CServerPropertyPage8::GetEditRevalPlu(int p, int t)
{
	switch (p)
	{
	case 1:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_PLU1);
		case 2:	return GetEdit(IDC_EDIT_REVAL_PLU2);
		case 3:	return GetEdit(IDC_EDIT_REVAL_PLU3);
		case 4:	return GetEdit(IDC_EDIT_REVAL_PLU4);
		case 5:	return GetEdit(IDC_EDIT_REVAL_PLU5);
		default: return NULL;
		}
		break;

	case 2:
		switch (t)
		{
		case 1:	return GetEdit(IDC_EDIT_REVAL_PLU6);
		case 2:	return GetEdit(IDC_EDIT_REVAL_PLU7);
		case 3:	return GetEdit(IDC_EDIT_REVAL_PLU8);
		case 4:	return GetEdit(IDC_EDIT_REVAL_PLU9);
		case 5:	return GetEdit(IDC_EDIT_REVAL_PLU10);
		default: return NULL;
		}
		break;

	default:
		return NULL;
	}
}

//$$******************************************************************

CEdit* CServerPropertyPage8::GetEditRevalTrip(int p, int t)
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

CEdit* CServerPropertyPage8::GetEditRevalPoints(int p, int t)
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

CEdit* CServerPropertyPage8::GetEditRevalValue(int p, int t)
{
	switch (p)
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

CEdit* CServerPropertyPage8::GetEditRevalPercent(int p, int t)
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

BOOL CServerPropertyPage8::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse1BonusType1()
{
	SelectType(1,1);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse1BonusType2()
{
	SelectType(1, 2);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse1BonusType3()
{
	SelectType(1, 3);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse1BonusType4()
{
	SelectType(1, 4);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse1BonusType5()
{
	SelectType(1, 5);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse2BonusType1()
{
	SelectType(2, 1);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse2BonusType2()
{
	SelectType(2, 2);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse2BonusType3()
{
	SelectType(2, 3);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse2BonusType4()
{
	SelectType(2, 4);
}

//$$******************************************************************

void CServerPropertyPage8::OnSelectPurse2BonusType5()
{
	SelectType(2, 5);
}

//$$******************************************************************

void CServerPropertyPage8::SelectType(int p, int t)
{
	int nType = m_comboType[p][t].GetCurSel();
	ShowAndEnableWindow(GetEditRevalPoints(p, t), (0 == nType));
	ShowAndEnableWindow(GetEditRevalValue(p, t), (1 == nType));
	ShowAndEnableWindow(GetEditRevalPercent(p, t), (2 == nType));

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

BOOL CServerPropertyPage8::OnKillActive()
{
	int nMaxPurse = m_bShowPurse2 ? 2 : 1;
	
	for (int p = 1; p <= nMaxPurse; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			int nType = m_comboType[p][t].GetCurSel();
			int nPercent = GetEditBoxInt(*GetEditRevalPercent(p, t));

			Server.SetTopupPluNo(p, t, GetEditBoxText(*GetEditRevalPlu(p, t)));
			Server.SetTopupBonusTrip(p, t, GetEditBoxDouble(*GetEditRevalTrip(p, t)));
			Server.SetTopupBonusType(p, t, nType);
			Server.SetTopupBonusPoints(p, t, GetEditBoxInt(*GetEditRevalPoints(p, t)));
			Server.SetTopupBonusValue(p, t, GetEditBoxDouble(*GetEditRevalValue(p, t)));
			Server.SetTopupBonusPercent(p, t, nPercent);

			if (2 == nType)
			{
				if ((nPercent < 0) || (nPercent > 100))
				{
					Prompter.Info("Please enter a percentage between 0 and 100");
					GetEditRevalPercent(p, t)->SetFocus();
					return FALSE;
				}
			}
		}
	}

	Server.SetTopupPCBonusTypePurse1(m_comboRevalBonus1.GetCurSel());
	Server.SetTopupCommentPurse1(GetEditBoxText(m_editRevalComment1));

	if (TRUE == m_bShowPurse2)
	{
		Server.SetTopupPCBonusTypePurse2(m_comboRevalBonus2.GetCurSel());
		Server.SetTopupCommentPurse2(GetEditBoxText(m_editRevalComment2));	
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************


