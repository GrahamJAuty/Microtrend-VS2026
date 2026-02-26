/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "PropPageOptionsGlobals.h"
/**********************************************************************/

CPropPageOptionsGlobals::CPropPageOptionsGlobals() : CPropertyPage(CPropPageOptionsGlobals::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsGlobals)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsGlobals::~CPropPageOptionsGlobals()
{
}

/**********************************************************************/

void CPropPageOptionsGlobals::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsGlobals)
	DDX_Control(pDX, IDC_CHECK_GLOBAL_TAX, m_checkTax);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_MODIFIER, m_checkModifier);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_ACAT, m_checkAnalysis);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_PAIDIO, m_checkPaidIO);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_PAYMENT, m_checkPayment);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_PAYMENT_GROUP, m_checkPaymentGroup);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_DEPT, m_checkDepartment);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_DEPT_GROUP_REPORT, m_checkDeptGroupReport);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_DEPT_GROUP_CONSOL, m_checkDeptGroupConsol);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_DEPT_GROUP_EPOS, m_checkDeptGroupEpos);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_CATEGORY, m_checkCategory);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_CUSTOM_FIELD, m_checkCustomField);
	DDX_Control(pDX, IDC_CHECK_GLOBAL_ALLOWANCE, m_checkAllowance);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsGlobals, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsGlobals)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_DEPT, OnToggleGlobalDept)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_DEPT_GROUP_REPORT, OnToggleGlobalDept)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_PAYMENT, OnToggleGlobalPayment)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsGlobals::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetRecordControls();
	
#ifndef STOCKMAN_SYSTEM
	m_checkAllowance.ShowWindow ( SW_HIDE );
	m_checkCustomField.ShowWindow( SW_HIDE );
	m_checkCategory.ShowWindow ( SW_HIDE );
#endif

	OnToggleGlobalDept();
	
	return TRUE;  
}

/**********************************************************************/

void CPropPageOptionsGlobals::OnToggleGlobalDept()
{
	if ( m_checkDepartment.GetCheck() != 0 )
	{
		m_checkDeptGroupReport.SetCheck( TRUE );
		m_checkDeptGroupConsol.SetCheck( TRUE );
		m_checkDeptGroupReport.EnableWindow( FALSE );
		m_checkDeptGroupConsol.EnableWindow( FALSE );
	}
	else
	{
		m_checkDeptGroupReport.EnableWindow( TRUE );

		if ( m_checkDeptGroupReport.GetCheck() != 0 )
		{
			m_checkDeptGroupConsol.SetCheck( TRUE );
			m_checkDeptGroupConsol.EnableWindow( FALSE );
		}
		else
			m_checkDeptGroupConsol.EnableWindow( TRUE );
	}
}

/**********************************************************************/

void CPropPageOptionsGlobals::OnToggleGlobalPayment()
{
	if ( m_checkPayment.GetCheck() != 0 )
	{
		m_checkPaymentGroup.SetCheck( TRUE );
		m_checkPaymentGroup.EnableWindow( FALSE );
	}
	else
		m_checkPaymentGroup.EnableWindow( TRUE );
}

/**********************************************************************/

BOOL CPropPageOptionsGlobals::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsGlobals::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsGlobals::SetRecordControls()
{
	m_checkTax.SetCheck( EcrmanOptions.GetGlobalTaxFlag() );
	m_checkModifier.SetCheck( EcrmanOptions.GetGlobalModifierFlag() );
	m_checkAnalysis.SetCheck( EcrmanOptions.GetGlobalAnalysisCategoryFlag() );
	m_checkPaidIO.SetCheck( EcrmanOptions.GetGlobalPaidInOutFlag() );
	m_checkDepartment.SetCheck( EcrmanOptions.GetGlobalDepartmentFlag() );
	m_checkDeptGroupReport.SetCheck( EcrmanOptions.GetGlobalDeptGroupReportFlag() );
	m_checkDeptGroupConsol.SetCheck( EcrmanOptions.GetGlobalDeptGroupConsolFlag() );
	m_checkDeptGroupEpos.SetCheck( EcrmanOptions.GetGlobalDeptGroupEposFlag() );

#ifdef STOCKMAN_SYSTEM
	m_checkAllowance.SetCheck( StockOptions.GetGlobalAllowanceFlag() );
	m_checkCategory.SetCheck( StockOptions.GetGlobalCategoryFlag() );
	m_checkCustomField.SetCheck( StockOptions.GetGlobalCustomFieldFlag() );
#endif

	switch( EcrmanOptions.GetGlobalPaymentType() )
	{
	case 1:
		m_checkPayment.SetCheck( TRUE );
		m_checkPaymentGroup.SetCheck( TRUE );
		m_checkPaymentGroup.EnableWindow( FALSE );
		break;
		
	case 2:
		m_checkPayment.SetCheck( FALSE );
		m_checkPaymentGroup.SetCheck( TRUE );
		m_checkPaymentGroup.EnableWindow( TRUE );
		break;

	case 0:
	default:
		m_checkPayment.SetCheck( FALSE );
		m_checkPaymentGroup.SetCheck( FALSE );
		m_checkPaymentGroup.EnableWindow( TRUE );
		break;
	}
}

/**********************************************************************/

void CPropPageOptionsGlobals::SaveRecord()
{
	EcrmanOptions.SetGlobalTaxFlag ( m_checkTax.GetCheck() != 0 );
	EcrmanOptions.SetGlobalModifierFlag( m_checkModifier.GetCheck() != 0 );
	EcrmanOptions.SetGlobalAnalysisCategoryFlag( m_checkAnalysis.GetCheck() != 0 );
	EcrmanOptions.SetGlobalPaidInOutFlag( m_checkPaidIO.GetCheck() != 0 );

	int nGlobalDeptType = 0;
	if ( m_checkDepartment.GetCheck() != 0 ) nGlobalDeptType = 1;
	else if ( m_checkDeptGroupReport.GetCheck() != 0 ) nGlobalDeptType = 2;
	else if ( m_checkDeptGroupConsol.GetCheck() != 0 ) nGlobalDeptType = 3;
	EcrmanOptions.SetGlobalDepartmentType ( nGlobalDeptType );

	int nGlobalPaymentType = 0;
	if ( m_checkPayment.GetCheck() != 0 ) nGlobalPaymentType = 1;
	else if ( m_checkPaymentGroup.GetCheck() != 0 ) nGlobalPaymentType = 2;
	EcrmanOptions.SetGlobalPaymentType ( nGlobalPaymentType );
	
	EcrmanOptions.SetGlobalDeptGroupEposFlag ( m_checkDeptGroupEpos.GetCheck() != 0 );

#ifdef STOCKMAN_SYSTEM
	StockOptions.SetGlobalAllowanceFlag ( m_checkAllowance.GetCheck() != 0 );
	StockOptions.SetGlobalCategoryFlag ( m_checkCategory.GetCheck() != 0 );
	StockOptions.SetGlobalCustomFieldFlag( m_checkCustomField.GetCheck() != 0 );
#endif
}

/**********************************************************************/
