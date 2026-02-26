/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolParams.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "ReportEposConsolDlg.h"
/**********************************************************************/

CReportEposConsolDlg::CReportEposConsolDlg( CEposReportSelectInfo& infoReport, CWnd* pParent)
	: CSSDialog(CReportEposConsolDlg::IDD, pParent), m_infoReport( infoReport )
{
	//{{AFX_DATA_INIT(CReportEposConsolDlg)
	//}}AFX_DATA_INIT
	m_nConsolidationOptionType = GetConsolidationOptionType( infoReport );
}

/**********************************************************************/

bool CReportEposConsolDlg::CanDoAdhocReport( CEposReportSelectInfo& infoReport )
{
	return ( GetConsolidationOptionType( infoReport ) != 0 );
}

/**********************************************************************/

int CReportEposConsolDlg::GetConsolidationOptionType( CEposReportSelectInfo& infoReport )
{
	int nType = 0;

	if ( infoReport.GetReportType() >= EPOS_CUSTOM_FIRST_REPORT )
	{
		switch( infoReport.GetCustomReportFamily() )
		{
		case EPOS_CUSTOM_FAMILY_GENERAL:
			nType = CONSOLIDATION_OPTION_ACTIVITY_ALL;
			break;

		case EPOS_CUSTOM_FAMILY_HOURLY:
		case EPOS_CUSTOM_FAMILY_HOURLYGRP:
		case EPOS_CUSTOM_FAMILY_HOURLYDPT:
		case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
		case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
		case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
		case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
			nType = CONSOLIDATION_OPTION_SALES_ALL;
			break;

		case EPOS_CUSTOM_FAMILY_MIXMATCH:
		case EPOS_CUSTOM_FAMILY_DISCOUNT:
			nType = CONSOLIDATION_OPTION_PROMO_NOSYS;
			break;

		case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
			nType = CONSOLIDATION_OPTION_SALES_NOSYS;
			break;
		}
	}
	else
	{
		int nReportType = infoReport.GetReportType();
		switch( nReportType )
		{
		case REPORT_TYPE_PLUSALES:
		case REPORT_TYPE_PLUSALES_NOZEROSKIP:
		case REPORT_TYPE_PLUSALES_DEPT_ITEM:
		case REPORT_TYPE_PLUSALES_TAX_ITEM:
		case REPORT_TYPE_PLUSALES_TRANSERVER:
		case REPORT_TYPE_COVERS_PLU_TRANSERVER:
		case REPORT_TYPE_PLUSALES_MODIFIER:
		case REPORT_TYPE_VIPSALES:
		case REPORT_TYPE_VIPSALES_DEPT_ITEM:
		case REPORT_TYPE_VIPSALES_TRANSERVER:
		case REPORT_TYPE_VIPSALES_MODIFIER:
		case REPORT_TYPE_PLUWASTE:
		case REPORT_TYPE_PLUWASTE_DEPT_ITEM:
		case REPORT_TYPE_PLUWASTE_TRANSERVER:
		case REPORT_TYPE_PLUWASTE_MODIFIER:
			nType = CONSOLIDATION_OPTION_SALES_NOSYS;
			break;

		case REPORT_TYPE_PROMOTION_SUMMARY:
		case REPORT_TYPE_CHEAPESTONEFREE_SUMMARY:
		case REPORT_TYPE_MIXMATCH_GROUP:
		case REPORT_TYPE_MIXMATCH_DEPT:
		case REPORT_TYPE_MIXMATCH_ITEM:
		case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
		case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
		case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
		case REPORT_TYPE_TRANSERVER_DISCOUNT:
			nType = CONSOLIDATION_OPTION_PROMO_NOSYS;
			break;

		case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:
		case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:
		case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:
		case REPORT_TYPE_HOURLY:
		case REPORT_TYPE_HOURLY_GROUP:
		case REPORT_TYPE_HOURLY_DEPT:
		case REPORT_TYPE_DAILY:
		case REPORT_TYPE_DAILY_GROUP:
		case REPORT_TYPE_DAILY_DEPT:
		case REPORT_TYPE_COVERS_GROUP:
		case REPORT_TYPE_COVERS_DEPT:
		case REPORT_TYPE_COVERS_HOURLY:
		case REPORT_TYPE_COVERS_HOURLY_GROUP:
		case REPORT_TYPE_COVERS_HOURLY_DEPT:
		case REPORT_TYPE_COVERS_DAILY:
		case REPORT_TYPE_COVERS_DAILY_GROUP:
		case REPORT_TYPE_COVERS_DAILY_DEPT:
		case REPORT_TYPE_COVERS_MONTHLY:
		case REPORT_TYPE_COVERS_PRICELEVEL:
		case REPORT_TYPE_COVERS_PAYMENT:
		case REPORT_TYPE_TAX_SUMMARY_TAX:
		case REPORT_TYPE_TAX_SUMMARY_PRICE:
		case REPORT_TYPE_SPTBOOK_CASCADE:
		case REPORT_TYPE_TRANSERVER_SUMMARY:
			nType = CONSOLIDATION_OPTION_SALES_ALL;
			break;

		case REPORT_TYPE_GENERAL_DEPT:
		case REPORT_TYPE_GENERAL_GROUP:
		case REPORT_TYPE_GENERAL_DEPT_QTY:
		case REPORT_TYPE_GENERAL_GROUP_QTY:
		case REPORT_TYPE_WEEKLY_DEPT:
		case REPORT_TYPE_WEEKLY_GROUP:
		case REPORT_TYPE_WEEKLY_DEPT_QTY:
		case REPORT_TYPE_WEEKLY_GROUP_QTY:
			nType = CONSOLIDATION_OPTION_ACTIVITY_ALL;
			break;

		case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
		case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		case REPORT_TYPE_PAYMENT_ESTIMATE_TAX:
		case REPORT_TYPE_PAYMENT_SUMMARY:
		case REPORT_TYPE_PAYMENT_SERVER:
		case REPORT_TYPE_PAYMENT_DETAIL:
		case REPORT_TYPE_PAYMENT_GROUP_SUMMARY:
			nType = CONSOLIDATION_OPTION_PAYMENT_ALL;
			break;
		}
	}

	return nType;
}

/**********************************************************************/

void CReportEposConsolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportEposConsolDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE1, m_checkConsolType1 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE2, m_checkConsolType2 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE3, m_checkConsolType3 );
	DDX_Control( pDX, IDC_CHECK_CONSOL_TYPE4, m_checkConsolType4 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportEposConsolDlg, CDialog)
	//{{AFX_MSG_MAP(CReportEposConsolDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_SAVE, OnButtonSave )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReportEposConsolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strReport = "";

	int nReportType = m_infoReport.GetReportType();

	if ( nReportType < EPOS_CUSTOM_FIRST_REPORT )
		strReport += CEposReport::GetReportNameExternal( nReportType );
	else
		strReport += m_infoReport.GetCustomReportName();

	SetWindowText( strReport + " Report" );

	CCSV csv( m_infoReport.GetConsolPrefsSaved() );

	{
		CString strShow = "Show ";
		CString strSale = "sales ";
		CString strPromo = "promotions ";
		CString strPay = "payments ";
		CString strAct = "activity ";
		CString strBy = "by ";
		CString strTerm = "terminal";
		CString strLoc = ( EcrmanOptions.GetReportsGroupReportpointFlag() ) ? "report point" : "location";
		CString strDb = "database";
		CString strSys = "system";

		switch( m_nConsolidationOptionType )
		{
		case CONSOLIDATION_OPTION_SALES_NOSYS:
			m_checkConsolType1.SetWindowText( strShow + strSale + strBy + strTerm );
			m_checkConsolType2.SetWindowText( strShow + strSale + strBy + strLoc );
			m_checkConsolType3.SetWindowText( strShow + strSale + strBy + strDb );
			m_checkConsolType4.ShowWindow( SW_HIDE );
			break;

		case CONSOLIDATION_OPTION_PROMO_NOSYS:
			m_checkConsolType1.SetWindowText( strShow + strPromo + strBy + strTerm );
			m_checkConsolType2.SetWindowText( strShow + strPromo + strBy + strLoc );
			m_checkConsolType3.SetWindowText( strShow + strPromo + strBy + strDb );
			m_checkConsolType4.ShowWindow( SW_HIDE );
			break;

		case CONSOLIDATION_OPTION_SALES_ALL:
			m_checkConsolType1.SetWindowText( strShow + strSale + strBy + strTerm );
			m_checkConsolType2.SetWindowText( strShow + strSale + strBy + strLoc );
			m_checkConsolType3.SetWindowText( strShow + strSale + strBy + strDb );
			m_checkConsolType4.SetWindowText( strShow + strSale + strBy + strSys );
			break;

		case CONSOLIDATION_OPTION_PAYMENT_ALL:
			m_checkConsolType1.SetWindowText( strShow + strPay + strBy + strTerm );
			m_checkConsolType2.SetWindowText( strShow + strPay + strBy + strLoc );
			m_checkConsolType3.SetWindowText( strShow + strPay + strBy + strDb );
			m_checkConsolType4.SetWindowText( strShow + strPay + strBy + strSys );
			break;

		case CONSOLIDATION_OPTION_ACTIVITY_ALL:
			m_checkConsolType1.SetWindowText( strShow + strAct + strBy + strTerm );
			m_checkConsolType2.SetWindowText( strShow + strAct + strBy + strLoc );
			m_checkConsolType3.SetWindowText( strShow + strAct + strBy + strDb );
			m_checkConsolType4.SetWindowText( strShow + strAct + strBy + strSys );
			break;
		}
	}

	switch( m_nConsolidationOptionType )
	{
	case CONSOLIDATION_OPTION_SALES_NOSYS:
	case CONSOLIDATION_OPTION_PROMO_NOSYS:
		{
			CEposReportConsolParamsPluSales Params;
			CString strParams = m_infoReport.GetConsolPrefsSaved();
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
			CString strParams = m_infoReport.GetConsolPrefsSaved();
			Params.LoadSettings( strParams );
				
			m_checkConsolType1.SetCheck( Params.m_bConsolTerm );
			m_checkConsolType2.SetCheck( Params.m_bConsolLoc );
			m_checkConsolType3.SetCheck( Params.m_bConsolDb );
			m_checkConsolType4.SetCheck( Params.m_bConsolSys );
		}
		break;
	}
	
	return TRUE;  
}

/**********************************************************************/

const char* CReportEposConsolDlg::GetCSVLine()
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

void CReportEposConsolDlg::OnButtonSave()
{
	m_infoReport.SetConsolPrefsSaved( GetCSVLine() );
	m_infoReport.SaveConsolPrefs();
}

/**********************************************************************/

void CReportEposConsolDlg::OnOK() 
{
	m_infoReport.SetConsolPrefsAdhoc( GetCSVLine() );
	EndDialog( IDOK );
}

/**********************************************************************/

