/**********************************************************************/
#include "..\CommonEcrStock\KeyboardJobInfo.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "JobDownloadDlgEcrLite.h"
/**********************************************************************/

CJobDownloadDlgEcrLite::CJobDownloadDlgEcrLite( CEposTaskDatabase& TaskDatabase, CWnd* pParent)
	: CSSDialog(CJobDownloadDlgEcrLite::IDD, pParent), m_TaskDatabase( TaskDatabase )
{
	//{{AFX_DATA_INIT(CJobDownloadDlgEcrLite)
	//}}AFX_DATA_INIT
	m_nKbNo = 1;
}

/**********************************************************************/

void CJobDownloadDlgEcrLite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobDownloadDlgEcrLite)
	DDX_Control(pDX, IDC_CHECK_PLU, m_checkPlu);
	DDX_Control(pDX, IDC_CHECK_BARCODE, m_checkBarcode);
	DDX_Control(pDX, IDC_CHECK_MODIFIER, m_checkModifier);
	DDX_Control(pDX, IDC_CHECK_CATEGORY, m_checkCategory);
	DDX_Control(pDX, IDC_CHECK_ALLERGEN, m_checkAllergen);
	DDX_Control(pDX, IDC_CHECK_LOYALTY_SCHEME, m_checkLoyaltyScheme);
	DDX_Control(pDX, IDC_CHECK_DEPARTMENT, m_checkDepartment);
	DDX_Control(pDX, IDC_CHECK_GROUP, m_checkGroup);
	DDX_Control(pDX, IDC_CHECK_TAX, m_checkTax);
	DDX_Control(pDX, IDC_CHECK_PAYMENT, m_checkPayment);
	DDX_Control(pDX, IDC_CHECK_KEYBOARD, m_checkKeyboard);
	DDX_Control(pDX, IDC_CHECK_IMAGES, m_checkImages);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_KEYBOARD, m_nKbNo);
	DDV_MinMaxInt(pDX, m_nKbNo, 1, EcrmanOptions.GetFeaturesKeyboardsPerLocation());
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CJobDownloadDlgEcrLite, CDialog)
	//{{AFX_MSG_MAP(CJobDownloadDlgEcrLite)
	ON_BN_CLICKED(IDC_CHECK_KEYBOARD, OnToggleKeyboard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CJobDownloadDlgEcrLite::GetEditKeyboard() { return GetEdit( IDC_EDIT_KEYBOARD ); }
/**********************************************************************/

BOOL CJobDownloadDlgEcrLite::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_checkTax.SetWindowText( EcrmanOptions.GetTaxString( "%T rates" ) );

	if ( PasswordArray.GetEnable( PasswordTicks::EditPluRecords ) == FALSE )
		m_checkPlu.EnableWindow( FALSE );

	SubclassEdit ( IDC_EDIT_KEYBOARD, SS_NUM, 2, "%d" );

	if ( EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE )
	{
		m_checkKeyboard.ShowWindow ( SW_HIDE );
		GetEditKeyboard() -> ShowWindow ( SW_HIDE );
		m_checkImages.ShowWindow( SW_HIDE );
	}
	else
		GetEditKeyboard() -> EnableWindow( FALSE );

	int nNextYPos = 48;
	ProcessTick( m_checkModifier, PasswordArray.GetEnable( PasswordTicks::EditModifier ), nNextYPos );
	ProcessTick( m_checkCategory, PasswordArray.GetEnable( PasswordTicks::EditAnalysisCategory ), nNextYPos );
	ProcessTick( m_checkAllergen, PasswordArray.GetEnable( PasswordTicks::EditAllergen ), nNextYPos );
	ProcessTick( m_checkDepartment, PasswordArray.GetEnable( PasswordTicks::EditDepartment ), nNextYPos );
	ProcessTick( m_checkGroup, PasswordArray.GetEnable( PasswordTicks::EditGroup ), nNextYPos );
	ProcessTick( m_checkTax, PasswordArray.GetEnable( PasswordTicks::EditTax ), nNextYPos );
	ProcessTick( m_checkPayment, PasswordArray.GetEnable( PasswordTicks::EditPayment ), nNextYPos );
	ProcessTick( m_checkLoyaltyScheme, PasswordArray.GetEnable( PasswordTicks::Loyalty ) && ( SysInfo.GotLoyaltyLink() ), nNextYPos );

	if ( PasswordArray.GetEnable( PasswordTicks::EditKeyboards ) == FALSE )	
	{
		m_checkKeyboard.EnableWindow( FALSE );
		GetEditKeyboard() -> EnableWindow( FALSE );
		m_checkImages.EnableWindow( FALSE );
		m_checkKeyboard.ShowWindow( SW_HIDE );
		GetEditKeyboard() -> ShowWindow( SW_HIDE );
		m_checkImages.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CJobDownloadDlgEcrLite::OnToggleKeyboard() 
{
	if ( EcrmanOptions.GetFeaturesKeyboardsPerLocation() > 1 )
		GetEditKeyboard() -> EnableWindow ( m_checkKeyboard.GetCheck() );
}

/**********************************************************************/

void CJobDownloadDlgEcrLite::ProcessTick( CButton& tick, bool bShow, int& nNextYPos )
{
	if ( FALSE == bShow )
	{
		tick.SetCheck( FALSE );
		tick.EnableWindow( FALSE );
		tick.ShowWindow( SW_HIDE );
	}
	else
	{
		tick.EnableWindow( TRUE );
		tick.ShowWindow( SW_SHOW );
		MoveControl( &tick, 16, nNextYPos );
		nNextYPos += 16;
	}
}

/**********************************************************************/

void CJobDownloadDlgEcrLite::AddJobsToTask()
{
	if (IsTicked(m_checkPlu))				m_TaskDatabase.AddEditableTaskJob( DN_PLU );
	if (IsTicked(m_checkBarcode))			m_TaskDatabase.AddEditableTaskJob( DN_BAR );
	if (IsTicked(m_checkModifier))			m_TaskDatabase.AddEditableTaskJob( DN_MOD );
	if (IsTicked(m_checkCategory))			m_TaskDatabase.AddEditableTaskJob( DN_ACAT );
	if (IsTicked(m_checkAllergen))			m_TaskDatabase.AddEditableTaskJob( DN_ALLERGEN );
	if (IsTicked(m_checkLoyaltyScheme))		m_TaskDatabase.AddEditableTaskJob( DN_LOYALTY_SCHEME );
	if (IsTicked(m_checkDepartment))		m_TaskDatabase.AddEditableTaskJob( DN_DEPT );
	if (IsTicked(m_checkGroup))				m_TaskDatabase.AddEditableTaskJob( DN_GROUP );
	if (IsTicked(m_checkTax))				m_TaskDatabase.AddEditableTaskJob( DN_TAX );
	if (IsTicked(m_checkPayment))			m_TaskDatabase.AddEditableTaskJob( DN_PAY );

	if (IsTicked(m_checkKeyboard))
	{
		CKeyboardJobInfo infoKb;
		infoKb.m_nKbNo = m_nKbNo;
		
		int nJobType = infoKb.GetJobNo();

		if ( nJobType != 0 )
			m_TaskDatabase.AddEditableTaskJob( nJobType );
	}
	
	if (IsTicked(m_checkImages))			m_TaskDatabase.AddEditableTaskJob( DN_IMAGES );
}

/**********************************************************************/

void CJobDownloadDlgEcrLite::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		AddJobsToTask();
		m_TaskDatabase.TidyEditableTaskJobs();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
