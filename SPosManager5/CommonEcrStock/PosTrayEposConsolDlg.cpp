/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolParams.h"
#include "PosTrayTask.h"
#include "ReportEposConsolDlg.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "PosTrayEposConsolDlg.h"
/**********************************************************************/

CPosTrayEposConsolDlg::CPosTrayEposConsolDlg( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams, int nTaskType, CWnd* pParent)
	: CSSDialog(CPosTrayEposConsolDlg::IDD, pParent), m_infoReport( infoReport ), m_infoParams( infoParams )
{
	//{{AFX_DATA_INIT(CPosTrayEposConsolDlg)
	//}}AFX_DATA_INIT
	m_nConsolidationOptionType = CReportEposConsolDlg::GetConsolidationOptionType( infoReport );
	m_nTaskType = nTaskType;
}

/**********************************************************************/

bool CPosTrayEposConsolDlg::CanDoAdhocReport( CEposReportSelectInfo& infoReport )
{
	return ( CReportEposConsolDlg::GetConsolidationOptionType( infoReport ) != 0 );
}

/**********************************************************************/

void CPosTrayEposConsolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayEposConsolDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_RADIO_CONSOL_POSTRAY, m_radioPosTray );
	DDX_Control( pDX, IDC_RADIO_CONSOL_DEFAULT, m_radioDefault );
	DDX_Control( pDX, IDC_RADIO_CONSOL_SAVED, m_radioSaved );
	DDX_Control( pDX, IDC_RADIO_CONSOL_SUMMARY, m_radioSummary );
	DDX_Control( pDX, IDC_RADIO_CONSOL_CUSTOM, m_radioCustom );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE1, m_checkConsolType1 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE2, m_checkConsolType2 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE3, m_checkConsolType3 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE4, m_checkConsolType4 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayEposConsolDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayEposConsolDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_CONSOL_POSTRAY, OnSelectType)
	ON_BN_CLICKED(IDC_RADIO_CONSOL_DEFAULT, OnSelectType)
	ON_BN_CLICKED(IDC_RADIO_CONSOL_SAVED, OnSelectType)
	ON_BN_CLICKED(IDC_RADIO_CONSOL_SUMMARY, OnSelectType)
	ON_BN_CLICKED(IDC_RADIO_CONSOL_CUSTOM, OnSelectType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayEposConsolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strReport = "Location Consolidation for ";

	int nReportType = m_infoReport.GetReportType();

	if (nReportType < EPOS_CUSTOM_FIRST_REPORT)
	{
		strReport += CEposReport::GetReportNameExternal(nReportType);
	}
	else
	{
		strReport += m_infoReport.GetCustomReportName();
	}

	SetWindowText(strReport + " Report");

	int nType = 0;
	if (TRUE == m_infoParams.m_bOverride)
	{
		if (TRUE == m_infoParams.m_bCustomPrefs)
		{
			nType = 4;
		}
		else switch (m_infoParams.m_nConsolPrefsType)
		{
		case POSTRAY_EPOS_CONSOL_DEFAULT:	nType = 1;		break;
		case POSTRAY_EPOS_CONSOL_SAVED:		nType = 2;		break;
		case POSTRAY_EPOS_CONSOL_SYSTEM:	nType = 3;		break;
		}
	}

	m_radioPosTray.SetCheck(0 == nType);
	m_radioDefault.SetCheck(1 == nType);
	m_radioSaved.SetCheck(2 == nType);
	m_radioSummary.SetCheck(3 == nType);
	m_radioCustom.SetCheck(4 == nType);
	OnSelectType();

	if (4 == nType)
	{
		UpdateCustomTicks(m_infoParams.m_strCustomPrefs);
	}

	switch (m_nConsolidationOptionType)
	{
	case CONSOLIDATION_OPTION_SALES_NOSYS:
		m_checkConsolType1.SetWindowText("Show sales by terminal");
		m_checkConsolType2.SetWindowText("Show sales by location");
		m_checkConsolType3.SetWindowText("Show sales by database");
		m_checkConsolType4.ShowWindow(SW_HIDE);
		break;

	case CONSOLIDATION_OPTION_PROMO_NOSYS:
		m_checkConsolType1.SetWindowText("Show promotions by terminal");
		m_checkConsolType2.SetWindowText("Show promotions by location");
		m_checkConsolType3.SetWindowText("Show promotions by database");
		m_checkConsolType4.ShowWindow(SW_HIDE);
		break;

	case CONSOLIDATION_OPTION_SALES_ALL:
		m_checkConsolType1.SetWindowText("Show sales by terminal");
		m_checkConsolType2.SetWindowText("Show sales by location");
		m_checkConsolType3.SetWindowText("Show sales by database");
		m_checkConsolType4.SetWindowText("Show sales by system");
		break;

	case CONSOLIDATION_OPTION_PAYMENT_ALL:
		m_checkConsolType1.SetWindowText("Show payments by terminal");
		m_checkConsolType2.SetWindowText("Show payments by location");
		m_checkConsolType3.SetWindowText("Show payments by database");
		m_checkConsolType4.SetWindowText("Show payments by system");
		break;

	case CONSOLIDATION_OPTION_ACTIVITY_ALL:
		m_checkConsolType1.SetWindowText("Show activity by terminal");
		m_checkConsolType2.SetWindowText("Show activity by location");
		m_checkConsolType3.SetWindowText("Show activity by database");
		m_checkConsolType4.SetWindowText("Show activity by system");
		break;
	}

	return TRUE;
}

/**********************************************************************/

int CPosTrayEposConsolDlg::GetSelectedType()
{
	int nType = 0;

	if (m_radioDefault.GetCheck() != 0)
	{
		nType = 1;
	}
	else if (m_radioSaved.GetCheck() != 0)
	{
		nType = 2;
	}
	else if (m_radioSummary.GetCheck() != 0)
	{
		nType = 3;
	}
	else if (m_radioCustom.GetCheck() != 0)
	{
		nType = 4;
	}

	return nType;
}

/**********************************************************************/

void CPosTrayEposConsolDlg::OnSelectType()
{
	int nType = GetSelectedType();

	m_checkConsolType1.EnableWindow( 4 == nType );
	m_checkConsolType2.EnableWindow( 4 == nType );
	m_checkConsolType3.EnableWindow( 4 == nType );
	m_checkConsolType4.EnableWindow( 4 == nType );

	int nTickType = -1;

	switch( nType )
	{
	case 0:	
		nTickType = m_nTaskType;
		break;

	case 1:	
		nTickType = POSTRAY_EPOS_CONSOL_DEFAULT;
		break;

	case 2:	
		nTickType = POSTRAY_EPOS_CONSOL_SAVED;	
		break;

	case 3:
		nTickType = POSTRAY_EPOS_CONSOL_SYSTEM;	
		break;
	}

	switch( nTickType )
	{
	case POSTRAY_EPOS_CONSOL_DEFAULT:
		UpdateCustomTicks( m_infoReport.GetConsolPrefsDefault() );
		break;

	case POSTRAY_EPOS_CONSOL_SAVED:
		UpdateCustomTicks( m_infoReport.GetConsolPrefsSaved() );
		break;

	case POSTRAY_EPOS_CONSOL_SYSTEM:
		UpdateCustomTicks( m_infoReport.GetConsolPrefsSummary() );
		break;
	}
}

/**********************************************************************/

void CPosTrayEposConsolDlg::UpdateCustomTicks( const char* szParams )
{
	CString strParams = szParams;

	switch( m_nConsolidationOptionType )
	{
	case CONSOLIDATION_OPTION_SALES_NOSYS:
	case CONSOLIDATION_OPTION_PROMO_NOSYS:
		{
			CEposReportConsolParamsPluSales Params;
			Params.LoadSettings( strParams );
			
			m_checkConsolType1.SetCheck( Params.m_bConsolTerm );
			m_checkConsolType2.SetCheck( Params.m_bConsolLoc );
			m_checkConsolType3.SetCheck( Params.m_bConsolDb );
		}
		break;

	case CONSOLIDATION_OPTION_SALES_ALL:
	case CONSOLIDATION_OPTION_PAYMENT_ALL:
	case CONSOLIDATION_OPTION_ACTIVITY_ALL:
		{
			CEposReportConsolParamsStandard Params;
			Params.LoadSettings( strParams );
			
			m_checkConsolType1.SetCheck( Params.m_bConsolTerm );
			m_checkConsolType2.SetCheck( Params.m_bConsolLoc );
			m_checkConsolType3.SetCheck( Params.m_bConsolDb );
			m_checkConsolType4.SetCheck( Params.m_bConsolSys );
		}
		break;
	}
}

/**********************************************************************/

const char* CPosTrayEposConsolDlg::GetCSVLine()
{
	switch( m_nConsolidationOptionType )
	{
	case CONSOLIDATION_OPTION_SALES_NOSYS:
	case CONSOLIDATION_OPTION_PROMO_NOSYS:
		{
			CEposReportConsolParamsPluSales Params;
			Params.m_bConsolTerm = ( m_checkConsolType1.GetCheck() != 0 );
			Params.m_bConsolLoc = ( m_checkConsolType2.GetCheck() != 0 );
			Params.m_bConsolDb = ( m_checkConsolType3.GetCheck() != 0 );
			Params.SaveSettings( m_strCSVLine );
		}
		break;

	case CONSOLIDATION_OPTION_SALES_ALL:
	case CONSOLIDATION_OPTION_PAYMENT_ALL:
	case CONSOLIDATION_OPTION_ACTIVITY_ALL:
		{
			CEposReportConsolParamsStandard Params;
			Params.m_bConsolTerm = ( m_checkConsolType1.GetCheck() != 0 );
			Params.m_bConsolLoc = ( m_checkConsolType2.GetCheck() != 0 );
			Params.m_bConsolDb = ( m_checkConsolType3.GetCheck() != 0 );
			Params.m_bConsolSys = ( m_checkConsolType4.GetCheck() != 0 );
			Params.SaveSettings( m_strCSVLine );
		}
		break;
	}
	
	return m_strCSVLine;
}

/**********************************************************************/

void CPosTrayEposConsolDlg::OnOK() 
{
	int nType = GetSelectedType();

	switch ( nType )
	{
	case 0:
		m_infoParams.m_bOverride = FALSE;
		m_infoParams.m_nConsolPrefsType = 0;
		m_infoParams.m_bCustomPrefs = FALSE;
		m_infoParams.m_strCustomPrefs = "";
		break;

	case 1:
		m_infoParams.m_bOverride = TRUE;
		m_infoParams.m_nConsolPrefsType = POSTRAY_EPOS_CONSOL_DEFAULT;
		m_infoParams.m_bCustomPrefs = FALSE;
		m_infoParams.m_strCustomPrefs = "";
		break;

	case 2:
		m_infoParams.m_bOverride = TRUE;
		m_infoParams.m_nConsolPrefsType = POSTRAY_EPOS_CONSOL_SAVED;
		m_infoParams.m_bCustomPrefs = FALSE;
		m_infoParams.m_strCustomPrefs = "";
		break;

	case 3:
		m_infoParams.m_bOverride = TRUE;
		m_infoParams.m_nConsolPrefsType = POSTRAY_EPOS_CONSOL_SYSTEM;
		m_infoParams.m_bCustomPrefs = FALSE;
		m_infoParams.m_strCustomPrefs = "";
		break;

	case 4:
		m_infoParams.m_bOverride = TRUE;
		m_infoParams.m_nConsolPrefsType = 0;
		m_infoParams.m_bCustomPrefs = TRUE;
		m_infoParams.m_strCustomPrefs = GetCSVLine();
		break;
	}
	
	EndDialog( IDOK );
}

/**********************************************************************/

