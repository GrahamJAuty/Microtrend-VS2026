/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsReports.h"
/**********************************************************************/

CPropPagePMSOptionsReports::CPropPagePMSOptionsReports() : CSSPropertyPage(CPropPagePMSOptionsReports::IDD), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsReports)
	//}}AFX_DATA_INIT
	m_bCanChangeDatabase = FALSE;
	m_bHasChangedDatabase = FALSE;
}

/**********************************************************************/

CPropPagePMSOptionsReports::~CPropPagePMSOptionsReports()
{
}

/**********************************************************************/

void CPropPagePMSOptionsReports::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsReports)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_TIMEPICKER1, m_TimePicker1);
	DDX_Control( pDX, IDC_TIMEPICKER2, m_TimePicker2);
	DDX_Control( pDX, IDC_TIMEPICKER3, m_TimePicker3);
	DDX_Control( pDX, IDC_CHECK_FIXED_TIME, m_checkFixedTime);
	DDX_Control( pDX, IDC_COMBO_ACCOM, m_comboAccom);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION1, m_editDescription1);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION2, m_editDescription2);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION3, m_editDescription3);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION4, m_editDescription4);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION5, m_editDescription5);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION6, m_editDescription6);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION7, m_editDescription7);
	DDX_Control( pDX, IDC_EDIT_DESCRIPTION8, m_editDescription8);
	DDX_Control( pDX, IDC_BUTTON_DEFAULT, m_buttonDefault);
	DDX_Text( pDX, IDC_EDIT_BREAKFAST, m_dBreakfast );
	DDV_MinMaxDouble( pDX, m_dBreakfast, 0.0, 25.0 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsReports, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsReports)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO1, OnKillFocusPluNo1)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO2, OnKillFocusPluNo2)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO3, OnKillFocusPluNo3)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO4, OnKillFocusPluNo4)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO5, OnKillFocusPluNo5)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO6, OnKillFocusPluNo6)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO7, OnKillFocusPluNo7)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU1, OnButtonBrowse1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU3, OnButtonBrowse3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU4, OnButtonBrowse4)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU5, OnButtonBrowse5)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU6, OnButtonBrowse6)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU7, OnButtonBrowse7)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_CHECK_FIXED_TIME, OnToggleFixedTime)
END_MESSAGE_MAP()

/**********************************************************************/

void CPropPagePMSOptionsReports::GetEditBoxes( int n, CEdit** pEditPlu, CEdit** pEditDescription1, CEdit** pEditDescription2 )
{
	switch(n)
	{
	case 2:
		*pEditPlu = GetEditPluNo2();
		*pEditDescription1 = &m_editDescription2;
		*pEditDescription2 = NULL;
		break;

	case 3:
		*pEditPlu = GetEditPluNo3();
		*pEditDescription1 = &m_editDescription3;
		*pEditDescription2 = NULL;
		break;

	case 4:
		*pEditPlu = GetEditPluNo4();
		*pEditDescription1 = &m_editDescription4;
		*pEditDescription2 = NULL;
		break;

	case 5:
		*pEditPlu = GetEditPluNo5();
		*pEditDescription1 = &m_editDescription5;
		*pEditDescription2 = NULL;
		break;

	case 6:
		*pEditPlu = GetEditPluNo6();
		*pEditDescription1 = &m_editDescription6;
		*pEditDescription2 = NULL;
		break;

	case 7:
		*pEditPlu = GetEditPluNo7();
		*pEditDescription1 = &m_editDescription7;
		*pEditDescription2 = &m_editDescription8;
		break;

	case 1:
	default:
		*pEditPlu = GetEditPluNo1();
		*pEditDescription1 = &m_editDescription1;
		*pEditDescription2 = NULL;
		break;
	}
}

/**********************************************************************/

void CPropPagePMSOptionsReports::UpdateDescription( __int64 nPluNo, int n )
{
	CEdit* pEditPlu = NULL;
	CEdit* pEditDescription1 = NULL;
	CEdit* pEditDescription2 = NULL;
	GetEditBoxes( n, &pEditPlu, &pEditDescription1, &pEditDescription2 );
	
	if ( 0 == nPluNo )
	{
		pEditDescription1 -> SetWindowText( "" );

		if ( NULL != pEditDescription2 )
			pEditDescription2 -> SetWindowText( "" );
	}
	else
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord Plu;
			DataManager.Plu.GetAt( nPluIdx, Plu );

			if ( NULL == pEditDescription2 )
				pEditDescription1 -> SetWindowText( Plu.GetReportText() );
			else
			{
				pEditDescription1 -> SetWindowText( Plu.GetEposText() );
				pEditDescription2 -> SetWindowText( Plu.GetReportText() );
			}
		}
		else
		{
			pEditDescription1 -> SetWindowText( "Unknown" );

			if ( NULL != pEditDescription2 )
				pEditDescription2 -> SetWindowText( "Unknown" );
		}
	}
}

/**********************************************************************/

BOOL CPropPagePMSOptionsReports::OnInitDialog() 
{
	m_dBreakfast = PMSOptions.GetEposReportBreakfastValue();

	CPropertyPage::OnInitDialog();

	SetInitialPluNo( PMSOptions.GetEposReportRoomOnlyPlu(), 1 );
	SetInitialPluNo( PMSOptions.GetEposReportRoomBBPlu(), 2 );
	SetInitialPluNo( PMSOptions.GetEposReportRoomDBBPlu(), 3 );
	SetInitialPluNo( PMSOptions.GetEposReportExtraBedPlu(), 4 );
	SetInitialPluNo( PMSOptions.GetEposReportExtraCotPlu(), 5 );
	SetInitialPluNo( PMSOptions.GetEposReportBreakfastPlu(), 6 );
	SetInitialPluNo( PMSOptions.GetEposReportDinnerPlu(), 7 );

	SubclassEdit( IDC_EDIT_PLUNO1, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO2, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO3, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO4, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO5, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO6, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_PLUNO7, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit( IDC_EDIT_BREAKFAST, SS_NUM_DP, 5, "%.2f" );
	
	m_TimePicker1.SetFormat ( "HH:mm" );
	m_TimePicker2.SetFormat ( "HH:mm" );
	m_TimePicker3.SetFormat ( "HH:mm" );

	{
		COleDateTime date = COleDateTime ( 2010, 1, 1, PMSOptions.GetEposReportHour(), PMSOptions.GetEposReportMinute(), 0 );
		
		if ( date.m_status != COleDateTime::valid )
			date = COleDateTime ( 2010, 1, 1, 0, 0, 0 );
			
		m_TimePicker1.SetTime( date );
	}

	{
		COleDateTime date = COleDateTime ( 2010, 1, 1, PMSOptions.GetAccomReportHour(), PMSOptions.GetAccomReportMinute(), 0 );
		
		if ( date.m_status != COleDateTime::valid )
			date = COleDateTime ( 2010, 1, 1, 0, 0, 0 );
			
		m_TimePicker2.SetTime( date );
	}

	{
		COleDateTime date = COleDateTime ( 2010, 1, 1, PMSOptions.GetAccomChargeHour(), PMSOptions.GetAccomChargeMinute(), 0 );
		
		if ( date.m_status != COleDateTime::valid )
			date = COleDateTime ( 2010, 1, 1, 0, 0, 0 );
			
		m_TimePicker3.SetTime( date );
	}

	m_checkFixedTime.SetCheck( PMSOptions.GetFixedTimeReportEnableFlag() );
	OnToggleFixedTime();

	m_comboAccom.AddString( "From day of check in" );
	m_comboAccom.AddString( "From day after check in" );
	m_comboAccom.SetCurSel( PMSOptions.GetAccomReportNextDayFlag() ? 1 : 0 );

	if ( FALSE == m_bCanChangeDatabase )
	{
		m_buttonDefault.EnableWindow( FALSE );
		m_buttonDefault.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSOptionsReports::OnToggleFixedTime()
{
	m_TimePicker1.EnableWindow( m_checkFixedTime.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPagePMSOptionsReports::SetInitialPluNo( __int64 nPluNo, int n )
{
	CEdit* pEditPlu = NULL;
	CEdit* pEditDescription1 = NULL;
	CEdit* pEditDescription2 = NULL;
	GetEditBoxes( n, &pEditPlu, &pEditDescription1, &pEditDescription2 );

	if ( 0 == nPluNo )
	{
		pEditPlu -> SetWindowText( "" );
		pEditDescription1 -> SetWindowText( "" );

		if ( NULL != pEditDescription2 )
			pEditDescription2 -> SetWindowText( "" );
	}
	else
	{
		CString strPluNo;
		::FormatPluNo( nPluNo, strPluNo, FALSE );
		pEditPlu -> SetWindowText( strPluNo);
		UpdateDescription( nPluNo, n );
	}
}

/**********************************************************************/

__int64 CPropPagePMSOptionsReports::GetFinalPluNo( int n )
{
	CEdit* pEditPlu = NULL;
	CEdit* pEditDescription1 = NULL;
	CEdit* pEditDescription2 = NULL;
	GetEditBoxes( n, &pEditPlu, &pEditDescription1, &pEditDescription2 );

	CString strPluNo;
	pEditPlu -> GetWindowText( strPluNo );
	return _atoi64( strPluNo );
}

/**********************************************************************/
CEdit* CPropPagePMSOptionsReports::GetEditPluNo1(){ return GetEdit( IDC_EDIT_PLUNO1 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo2(){ return GetEdit( IDC_EDIT_PLUNO2 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo3(){ return GetEdit( IDC_EDIT_PLUNO3 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo4(){ return GetEdit( IDC_EDIT_PLUNO4 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo5(){ return GetEdit( IDC_EDIT_PLUNO5 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo6(){ return GetEdit( IDC_EDIT_PLUNO6 ); }
CEdit* CPropPagePMSOptionsReports::GetEditPluNo7(){ return GetEdit( IDC_EDIT_PLUNO7 ); }
CEdit* CPropPagePMSOptionsReports::GetEditBreakfast(){ return GetEdit( IDC_EDIT_BREAKFAST ); }
/**********************************************************************/

BOOL CPropPagePMSOptionsReports::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsReports::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/
void CPropPagePMSOptionsReports::OnKillFocusPluNo1(){ OnKillFocusPluNo(1); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo2(){ OnKillFocusPluNo(2); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo3(){ OnKillFocusPluNo(3); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo4(){ OnKillFocusPluNo(4); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo5(){ OnKillFocusPluNo(5); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo6(){ OnKillFocusPluNo(6); }
void CPropPagePMSOptionsReports::OnKillFocusPluNo7(){ OnKillFocusPluNo(7); }
/**********************************************************************/

void CPropPagePMSOptionsReports::OnKillFocusPluNo( int n )
{
	CEdit* pEditPlu = NULL;
	CEdit* pEditDescription1 = NULL;
	CEdit* pEditDescription2 = NULL;
	GetEditBoxes( n, &pEditPlu, &pEditDescription1, &pEditDescription2 );

	UpdateData ( TRUE, FALSE );

	CString strPluNo;
	pEditPlu -> GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	pEditPlu -> SetWindowText( strPluNo );

	__int64 nPluNo = _atoi64( strPluNo );
	UpdateDescription( nPluNo, n );
}

/**********************************************************************/
void CPropPagePMSOptionsReports::OnButtonBrowse1(){ OnButtonBrowse(1); }
void CPropPagePMSOptionsReports::OnButtonBrowse2(){ OnButtonBrowse(2); }
void CPropPagePMSOptionsReports::OnButtonBrowse3(){ OnButtonBrowse(3); }
void CPropPagePMSOptionsReports::OnButtonBrowse4(){ OnButtonBrowse(4); }
void CPropPagePMSOptionsReports::OnButtonBrowse5(){ OnButtonBrowse(5); }
void CPropPagePMSOptionsReports::OnButtonBrowse6(){ OnButtonBrowse(6); }
void CPropPagePMSOptionsReports::OnButtonBrowse7(){ OnButtonBrowse(7); }
/**********************************************************************/

void CPropPagePMSOptionsReports::OnButtonBrowse( int n )
{
	CEdit* pEditPlu = NULL;
	CEdit* pEditDescription1 = NULL;
	CEdit* pEditDescription2 = NULL;
	GetEditBoxes( n, &pEditPlu, &pEditDescription1, &pEditDescription2 );

	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;
		
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	__int64 nPluNo = PluRecord.GetPluNoInt();
	UpdateDescription( nPluNo, n );

	CString strPluNo = PluRecord.GetPluNoString();
	pEditPlu -> SetWindowText( strPluNo );
	pEditPlu -> SetFocus();
	pEditPlu -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPropPagePMSOptionsReports::OnButtonDefault()
{
	CString strMsg = "";
	strMsg += "Do you wish to restore the default Plu items, Departments\n";
	strMsg += "and Groups for EPOS reporting of PMS sales";

	if ( Prompter.YesNo( strMsg ) == IDYES )
	{
		::AddPMSPluItem ( 1, "Room Charge", "", 999 );
		::AddPMSPluItem ( 2, "B & B Room Charge", "", 998 );
		::AddPMSPluItem ( 3, "DB & B Room Charge", "", 997 );
		::AddPMSPluItem ( 4, "Breakfast", "", 996 );
		::AddPMSPluItem ( 5, "Dinner Supplement", "Dinner Credit", 995 );
		::AddPMSPluItem ( 6, "Extra Bed", "", 994 );
		::AddPMSPluItem ( 7, "Extra Cot", "", 993 );

		DataManager.Department.AddPMSDepartment( 999, "Room Charge" );
		DataManager.Department.AddPMSDepartment( 998, "B & B Room Charge" );
		DataManager.Department.AddPMSDepartment( 997, "DB & B Room Charge" );
		DataManager.Department.AddPMSDepartment( 996, "Breakfast" );
		DataManager.Department.AddPMSDepartment( 995, "Dinner Credit" );
		DataManager.Department.AddPMSDepartment( 994, "Extra Bed" );
		DataManager.Department.AddPMSDepartment( 993, "Extra Cot" );

		DataManager.EposGroup.AddPMSGroup( 99, "Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 98, "B & B Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 97, "DB & B Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 96, "Breakfast" );
		DataManager.EposGroup.AddPMSGroup( 95, "Dinner Credit" );
		DataManager.EposGroup.AddPMSGroup( 94, "Extra Bed" );
		DataManager.EposGroup.AddPMSGroup( 93, "Extra Cot" );
				
		SetInitialPluNo( 99999999999901, 1 );
		SetInitialPluNo( 99999999999902, 2 );
		SetInitialPluNo( 99999999999903, 3 );
		SetInitialPluNo( 99999999999906, 4 );
		SetInitialPluNo( 99999999999907, 5 );
		SetInitialPluNo( 99999999999904, 6 );
		SetInitialPluNo( 99999999999905, 7 );

		m_bHasChangedDatabase = TRUE;
	}
}

/**********************************************************************/

void CPropPagePMSOptionsReports::SaveRecord()
{
	COleDateTime time1, time2, time3;
	m_TimePicker1.GetTime ( time1 );
	m_TimePicker2.GetTime ( time2 );
	m_TimePicker3.GetTime ( time3 );
	PMSOptions.SetEposReportBreakfastValue( m_dBreakfast );
	PMSOptions.SetEposReportHour ( time1.GetHour() );
	PMSOptions.SetFixedTimeReportEnableFlag( m_checkFixedTime.GetCheck() != 0 );
	PMSOptions.SetEposReportMinute ( time1.GetMinute() );
	PMSOptions.SetAccomReportHour( time2.GetHour() );
	PMSOptions.SetAccomReportMinute( time2.GetMinute() );
	PMSOptions.SetAccomChargeHour( time3.GetHour() );
	PMSOptions.SetAccomChargeMinute( time3.GetMinute() );
	PMSOptions.SetAccomReportNextDayFlag( m_comboAccom.GetCurSel() == 1 );
	PMSOptions.SetEposReportRoomOnlyPlu( GetFinalPluNo(1) );
	PMSOptions.SetEposReportRoomBBPlu( GetFinalPluNo(2) );
	PMSOptions.SetEposReportRoomDBBPlu( GetFinalPluNo(3) );
	PMSOptions.SetEposReportExtraBedPlu( GetFinalPluNo(4) );
	PMSOptions.SetEposReportExtraCotPlu( GetFinalPluNo(5) );
	PMSOptions.SetEposReportBreakfastPlu( GetFinalPluNo(6) );
	PMSOptions.SetEposReportDinnerPlu( GetFinalPluNo(7) );
}

/**********************************************************************/
#endif
/**********************************************************************/
