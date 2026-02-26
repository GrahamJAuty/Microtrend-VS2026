//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "ServerPropertyPage4.h"
//$$******************************************************************

CServerPropertyPage4::CServerPropertyPage4()
	: CSSPropertyPage(CServerPropertyPage4::IDD)
{
	m_staticRenewPluID[1] = IDC_STATIC_RENEW_PLU1;
	m_staticRenewPluID[2] = IDC_STATIC_RENEW_PLU2;
	m_staticRenewPluID[3] = IDC_STATIC_RENEW_PLU3;
	m_staticRenewPluID[4] = IDC_STATIC_RENEW_PLU4;
	m_staticRenewPluID[5] = IDC_STATIC_RENEW_PLU5;
	m_staticRenewPluID[6] = IDC_STATIC_RENEW_PLU6;
	m_staticRenewPluID[7] = IDC_STATIC_RENEW_PLU7;
	m_staticRenewPluID[8] = IDC_STATIC_RENEW_PLU8;
	m_staticRenewPluID[9] = IDC_STATIC_RENEW_PLU9;
	m_staticRenewPluID[10] = IDC_STATIC_RENEW_PLU10;
	m_staticRenewPluID[11] = IDC_STATIC_RENEW_PLU11;
	m_staticRenewPluID[12] = IDC_STATIC_RENEW_PLU12;
	m_staticRenewPluID[13] = IDC_STATIC_RENEW_PLU13;
	m_staticRenewPluID[14] = IDC_STATIC_RENEW_PLU14;
	m_staticRenewPluID[15] = IDC_STATIC_RENEW_PLU15;
	m_staticRenewPluID[16] = IDC_STATIC_RENEW_PLU16;
	/*****/
	m_staticRenewXID[1] = IDC_STATIC_RENEW_X1;
	m_staticRenewXID[2] = IDC_STATIC_RENEW_X2;
	m_staticRenewXID[3] = IDC_STATIC_RENEW_X3;
	m_staticRenewXID[4] = IDC_STATIC_RENEW_X4;
	m_staticRenewXID[5] = IDC_STATIC_RENEW_X5;
	m_staticRenewXID[6] = IDC_STATIC_RENEW_X6;
	m_staticRenewXID[7] = IDC_STATIC_RENEW_X7;
	m_staticRenewXID[8] = IDC_STATIC_RENEW_X8;
	m_staticRenewXID[9] = IDC_STATIC_RENEW_X9;
	m_staticRenewXID[10] = IDC_STATIC_RENEW_X10;
	m_staticRenewXID[11] = IDC_STATIC_RENEW_X11;
	m_staticRenewXID[12] = IDC_STATIC_RENEW_X12;
	m_staticRenewXID[13] = IDC_STATIC_RENEW_X13;
	m_staticRenewXID[14] = IDC_STATIC_RENEW_X14;
	m_staticRenewXID[15] = IDC_STATIC_RENEW_X15;
	m_staticRenewXID[16] = IDC_STATIC_RENEW_X16;
	/*****/
	m_comboRenewTypeID[1] = IDC_COMBO_RENEW_TYPE1;
	m_comboRenewTypeID[2] = IDC_COMBO_RENEW_TYPE2;
	m_comboRenewTypeID[3] = IDC_COMBO_RENEW_TYPE3;
	m_comboRenewTypeID[4] = IDC_COMBO_RENEW_TYPE4;
	m_comboRenewTypeID[5] = IDC_COMBO_RENEW_TYPE5;
	m_comboRenewTypeID[6] = IDC_COMBO_RENEW_TYPE6;
	m_comboRenewTypeID[7] = IDC_COMBO_RENEW_TYPE7;
	m_comboRenewTypeID[8] = IDC_COMBO_RENEW_TYPE8;
	m_comboRenewTypeID[9] = IDC_COMBO_RENEW_TYPE9;
	m_comboRenewTypeID[10] = IDC_COMBO_RENEW_TYPE10;
	m_comboRenewTypeID[11] = IDC_COMBO_RENEW_TYPE11;
	m_comboRenewTypeID[12] = IDC_COMBO_RENEW_TYPE12;
	m_comboRenewTypeID[13] = IDC_COMBO_RENEW_TYPE13;
	m_comboRenewTypeID[14] = IDC_COMBO_RENEW_TYPE14;
	m_comboRenewTypeID[15] = IDC_COMBO_RENEW_TYPE15;
	m_comboRenewTypeID[16] = IDC_COMBO_RENEW_TYPE16;
	/*****/
	m_dtpickRenewID[1] = IDC_DTPICK_RENEW1;
	m_dtpickRenewID[2] = IDC_DTPICK_RENEW2;
	m_dtpickRenewID[3] = IDC_DTPICK_RENEW3;
	m_dtpickRenewID[4] = IDC_DTPICK_RENEW4;
	m_dtpickRenewID[5] = IDC_DTPICK_RENEW5;
	m_dtpickRenewID[6] = IDC_DTPICK_RENEW6;
	m_dtpickRenewID[7] = IDC_DTPICK_RENEW7;
	m_dtpickRenewID[8] = IDC_DTPICK_RENEW8;
	m_dtpickRenewID[9] = IDC_DTPICK_RENEW9;
	m_dtpickRenewID[10] = IDC_DTPICK_RENEW10;
	m_dtpickRenewID[11] = IDC_DTPICK_RENEW11;
	m_dtpickRenewID[12] = IDC_DTPICK_RENEW12;
	m_dtpickRenewID[13] = IDC_DTPICK_RENEW13;
	m_dtpickRenewID[14] = IDC_DTPICK_RENEW14;
	m_dtpickRenewID[15] = IDC_DTPICK_RENEW15;
	m_dtpickRenewID[16] = IDC_DTPICK_RENEW16;
	/*****/
	m_comboRenewFixedYearID[1] = IDC_COMBO_RENEW_YEAR1;
	m_comboRenewFixedYearID[2] = IDC_COMBO_RENEW_YEAR2;
	m_comboRenewFixedYearID[3] = IDC_COMBO_RENEW_YEAR3;
	m_comboRenewFixedYearID[4] = IDC_COMBO_RENEW_YEAR4;
	m_comboRenewFixedYearID[5] = IDC_COMBO_RENEW_YEAR5;
	m_comboRenewFixedYearID[6] = IDC_COMBO_RENEW_YEAR6;
	m_comboRenewFixedYearID[7] = IDC_COMBO_RENEW_YEAR7;
	m_comboRenewFixedYearID[8] = IDC_COMBO_RENEW_YEAR8;
	m_comboRenewFixedYearID[9] = IDC_COMBO_RENEW_YEAR9;
	m_comboRenewFixedYearID[10] = IDC_COMBO_RENEW_YEAR10;
	m_comboRenewFixedYearID[11] = IDC_COMBO_RENEW_YEAR11;
	m_comboRenewFixedYearID[12] = IDC_COMBO_RENEW_YEAR12;
	m_comboRenewFixedYearID[13] = IDC_COMBO_RENEW_YEAR13;
	m_comboRenewFixedYearID[14] = IDC_COMBO_RENEW_YEAR14;
	m_comboRenewFixedYearID[15] = IDC_COMBO_RENEW_YEAR15;
	m_comboRenewFixedYearID[16] = IDC_COMBO_RENEW_YEAR16;
	/*****/
	m_editRenewPluID[1] = IDC_EDIT_RENEW_PLU1;
	m_editRenewPluID[2] = IDC_EDIT_RENEW_PLU2;
	m_editRenewPluID[3] = IDC_EDIT_RENEW_PLU3;
	m_editRenewPluID[4] = IDC_EDIT_RENEW_PLU4;
	m_editRenewPluID[5] = IDC_EDIT_RENEW_PLU5;
	m_editRenewPluID[6] = IDC_EDIT_RENEW_PLU6;
	m_editRenewPluID[7] = IDC_EDIT_RENEW_PLU7;
	m_editRenewPluID[8] = IDC_EDIT_RENEW_PLU8;
	m_editRenewPluID[9] = IDC_EDIT_RENEW_PLU9;
	m_editRenewPluID[10] = IDC_EDIT_RENEW_PLU10;
	m_editRenewPluID[11] = IDC_EDIT_RENEW_PLU11;
	m_editRenewPluID[12] = IDC_EDIT_RENEW_PLU12;
	m_editRenewPluID[13] = IDC_EDIT_RENEW_PLU13;
	m_editRenewPluID[14] = IDC_EDIT_RENEW_PLU14;
	m_editRenewPluID[15] = IDC_EDIT_RENEW_PLU15;
	m_editRenewPluID[16] = IDC_EDIT_RENEW_PLU16;
	/*****/
	m_editRenewMultiplierID[1] = IDC_EDIT_RENEW_MULTIPLIER1;
	m_editRenewMultiplierID[2] = IDC_EDIT_RENEW_MULTIPLIER2;
	m_editRenewMultiplierID[3] = IDC_EDIT_RENEW_MULTIPLIER3;
	m_editRenewMultiplierID[4] = IDC_EDIT_RENEW_MULTIPLIER4;
	m_editRenewMultiplierID[5] = IDC_EDIT_RENEW_MULTIPLIER5;
	m_editRenewMultiplierID[6] = IDC_EDIT_RENEW_MULTIPLIER6;
	m_editRenewMultiplierID[7] = IDC_EDIT_RENEW_MULTIPLIER7;
	m_editRenewMultiplierID[8] = IDC_EDIT_RENEW_MULTIPLIER8;
	m_editRenewMultiplierID[9] = IDC_EDIT_RENEW_MULTIPLIER9;
	m_editRenewMultiplierID[10] = IDC_EDIT_RENEW_MULTIPLIER10;
	m_editRenewMultiplierID[11] = IDC_EDIT_RENEW_MULTIPLIER11;
	m_editRenewMultiplierID[12] = IDC_EDIT_RENEW_MULTIPLIER12;
	m_editRenewMultiplierID[13] = IDC_EDIT_RENEW_MULTIPLIER13;
	m_editRenewMultiplierID[14] = IDC_EDIT_RENEW_MULTIPLIER14;
	m_editRenewMultiplierID[15] = IDC_EDIT_RENEW_MULTIPLIER15;
	m_editRenewMultiplierID[16] = IDC_EDIT_RENEW_MULTIPLIER16;
}

//$$******************************************************************

void CServerPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	for ( int n = 1; n <= 16; n++ )
	{
		DDX_Control( pDX, m_staticRenewPluID[n], m_staticRenewPlu[n] );
		DDX_Control( pDX, m_staticRenewXID[n], m_staticRenewX[n] );
		DDX_Control( pDX, m_comboRenewTypeID[n], m_comboRenewType[n] );
		DDX_Control( pDX, m_dtpickRenewID[n], m_dtpickRenew[n]);
		DDX_Control(pDX, m_comboRenewFixedYearID[n], m_comboRenewFixedYear[n]);
	}
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage4, CPropertyPage)
	
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE1, OnSelectRenewType1)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE2, OnSelectRenewType2)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE3, OnSelectRenewType3)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE4, OnSelectRenewType4)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE5, OnSelectRenewType5)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE6, OnSelectRenewType6)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE7, OnSelectRenewType7)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE8, OnSelectRenewType8)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE9, OnSelectRenewType9)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE10, OnSelectRenewType10)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE11, OnSelectRenewType11)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE12, OnSelectRenewType12)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE13, OnSelectRenewType13)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE14, OnSelectRenewType14)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE15, OnSelectRenewType15)
	ON_CBN_SELCHANGE( IDC_COMBO_RENEW_TYPE16, OnSelectRenewType16)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage4::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	for ( int n = 1; n <= 16; n++ )
	{
		m_strRenewPluNo[n]			= Server.GetRenewPluNo(n);
		m_nRenewMultiplier[n]		= Server.GetRenewMultiplier(n);
		m_nRenewYearDayNumber[n]	= Server.GetRenewYearDayNumber(n);
		m_nRenewType[n]				= Server.GetRenewType(n);
		m_nRenewFixedYear[n]		= Server.GetRenewFixedYear(n);
	}

	{
		COleDateTime rangeStart = COleDateTime( 2020,1,1,0,0,0 );
		COleDateTime rangeEnd = COleDateTime( 2020,12,31,0,0,0 );
		for ( int n = 1; n <= 16; n++ )
		{
			m_dtpickRenew[n].SetRange( &rangeStart, &rangeEnd  );
			m_dtpickRenew[n].SetFormat("dd MMM");
			m_dtpickRenew[n].SetTime( rangeStart );
		}
	}

	for ( int n = 1; n <= 16; n++ )
	{
		SubclassEdit ( m_editRenewPluID[n],			SS_NUM, MAX_PLU_LEN, "%s" );
		SubclassEdit ( m_editRenewMultiplierID[n],	SS_NUM, 2, "%d" );
	}

	LoadRenewalSettings();
	
	return TRUE;
}

//$$******************************************************************

CEdit* CServerPropertyPage4::GetEditRenewPlu(int n)
{
	if ((n >= 1) && (n <= 16))
	{
		return GetEdit(m_editRenewPluID[n]);
	}
	else
	{
		return NULL;
	}
}

//$$******************************************************************

CEdit* CServerPropertyPage4::GetEditRenewMultiplier(int n)
{
	if ((n >= 1) && (n <= 16))
	{
		return GetEdit(m_editRenewMultiplierID[n]);
	}
	else
	{
		return NULL;
	}
}

//$$******************************************************************

void CServerPropertyPage4::LoadRenewalSettings()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nMinYear = timeNow.GetYear();
	int nMaxYear = nMinYear + 2;

	for ( int n = 1; n <= 16; n++ )
	{
		GetEditRenewPlu(n) -> SetWindowText( m_strRenewPluNo[ n ] );

		CString str;
		str.Format( "%d", m_nRenewMultiplier[n] );
		GetEditRenewMultiplier(n) -> SetWindowText( str );

		CStandardCombos::FillRenewTypeCombo( m_comboRenewType[n], m_nRenewType[n] );
		SelectRenewType(n);

		COleDateTime dateBase = COleDateTime(2019,12,31,0,0,0 );
		COleDateTimeSpan span = COleDateTimeSpan( m_nRenewYearDayNumber[n], 0, 0, 0 );
		COleDateTime dateRenew = dateBase + span;
		m_dtpickRenew[n].SetTime( dateRenew );

		int nYear = m_nRenewFixedYear[n];

		m_comboRenewFixedYear[n].AddString("Extend Current Expiry");
		m_comboRenewFixedYear[n].SetItemData(0, 0);

		if (nYear != 0)
		{
			if ((nYear < nMinYear) || (nYear > nMaxYear))
			{
				CString str = "";
				str.Format("Fixed Year (%d)", nYear);
				m_comboRenewFixedYear[n].AddString(str);
				m_comboRenewFixedYear[n].SetItemData(1, nYear);
			}
		}

		for (int nYearToAdd = nMinYear; nYearToAdd <= nMaxYear; nYearToAdd++)
		{
			CString str = "";
			str.Format("Fixed Year (%d)", nYearToAdd);
			int nPos = m_comboRenewFixedYear[n].AddString(str);
			m_comboRenewFixedYear[n].SetItemData(nPos, nYearToAdd);
		}

		m_comboRenewFixedYear[n].SelectUsingItemData(nYear);
	}
}

//$$******************************************************************

void CServerPropertyPage4::SaveRenewalSettings()
{
	for ( int n = 1; n <= 16; n++ )
	{
		CString str;
		GetEditRenewPlu(n) -> GetWindowText(str);
		m_strRenewPluNo[n] = str;

		GetEditRenewMultiplier(n) -> GetWindowText(str);
		m_nRenewMultiplier[n] = atoi(str);

		m_nRenewType[n] = m_comboRenewType[n].GetCurSel();

		COleDateTime renewDate;
		m_dtpickRenew[n].GetTime( renewDate );
		if ( renewDate.m_status == COleDateTime::valid )
		{
			COleDateTimeSpan renewSpan = COleDateTime( 2020, renewDate.GetMonth(), renewDate.GetDay(), 0, 0, 0 ) - COleDateTime( 2019, 12, 31, 0, 0, 0 );
			m_nRenewYearDayNumber[n] = renewSpan.GetDays();
		}

		m_nRenewFixedYear[n] = m_comboRenewFixedYear[n].GetItemDataOfSelection();
	}
}

//$$******************************************************************

BOOL CServerPropertyPage4::OnSetActive()
{
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

BOOL CServerPropertyPage4::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		SaveRenewalSettings();

		for (int n = 1; n <= 16; n++)
		{
			Server.SetRenewPluNo(n, m_strRenewPluNo[n]);
			Server.SetRenewMultiplier(n, m_nRenewMultiplier[n]);
			Server.SetRenewType(n, m_nRenewType[n]);
			Server.SetRenewYearDayNumber(n, m_nRenewYearDayNumber[n]);
			Server.SetRenewFixedYear(n, m_nRenewFixedYear[n]);
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
void CServerPropertyPage4::OnSelectRenewType1()		{ SelectRenewType(1); }
void CServerPropertyPage4::OnSelectRenewType2()		{ SelectRenewType(2); }
void CServerPropertyPage4::OnSelectRenewType3()		{ SelectRenewType(3); }
void CServerPropertyPage4::OnSelectRenewType4()		{ SelectRenewType(4); }
void CServerPropertyPage4::OnSelectRenewType5()		{ SelectRenewType(5); }
void CServerPropertyPage4::OnSelectRenewType6()		{ SelectRenewType(6); }
void CServerPropertyPage4::OnSelectRenewType7()		{ SelectRenewType(7); }
void CServerPropertyPage4::OnSelectRenewType8()		{ SelectRenewType(8); }
void CServerPropertyPage4::OnSelectRenewType9()		{ SelectRenewType(9); }
void CServerPropertyPage4::OnSelectRenewType10()	{ SelectRenewType(10); }
void CServerPropertyPage4::OnSelectRenewType11()	{ SelectRenewType(11); }
void CServerPropertyPage4::OnSelectRenewType12()	{ SelectRenewType(12); }
void CServerPropertyPage4::OnSelectRenewType13()	{ SelectRenewType(13); }
void CServerPropertyPage4::OnSelectRenewType14()	{ SelectRenewType(14); }
void CServerPropertyPage4::OnSelectRenewType15()	{ SelectRenewType(15); }
void CServerPropertyPage4::OnSelectRenewType16()	{ SelectRenewType(16); }
//$$******************************************************************

void CServerPropertyPage4::SelectRenewType(int n)
{
	int nRenewType = m_comboRenewType[n].GetCurSel();

	m_nRenewType[n] = nRenewType;
	ShowAndEnableWindow(GetEditRenewMultiplier(n), TRUE, (3 != nRenewType));
	ShowAndEnableWindow(&m_dtpickRenew[n], 3 == nRenewType);
	ShowAndEnableWindow(&m_comboRenewFixedYear[n], 3 == nRenewType);
}

//$$******************************************************************

