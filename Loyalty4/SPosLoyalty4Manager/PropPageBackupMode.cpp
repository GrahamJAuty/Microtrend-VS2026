/**********************************************************************/
#include "AutoBackupOptions.h"
/**********************************************************************/
#include "PropPageBackupMode.h"
#include "PropertySheetBackup.h"
/**********************************************************************/

CPropPageBackupMode::CPropPageBackupMode() : CPropertyPage(CPropPageBackupMode::IDD)
{
	//{{AFX_DATA_INIT(CPropPageBackupMode)
	//}}AFX_DATA_INIT
	m_nBackupMode = BACKUP_MODE_MON_FRI;
	m_nMinutes = 0;
}

/**********************************************************************/

CPropPageBackupMode::~CPropPageBackupMode()
{
}

/**********************************************************************/

void CPropPageBackupMode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageBackupMode)
	DDX_Radio(pDX, IDC_RADIO_MODE1, m_nBackupMode);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageBackupMode, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageBackupMode)
	ON_BN_CLICKED(IDC_RADIO_MODE1, OnRadioMode1)
	ON_BN_CLICKED(IDC_RADIO_MODE2, OnRadioMode2)
	ON_BN_CLICKED(IDC_RADIO_MODE3, OnRadioMode3)
	ON_BN_CLICKED(IDC_RADIO_MODE4, OnRadioMode4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageBackupMode::OnInitDialog() 
{
	m_nBackupMode = AutoBackupOptions.GetCustomMode();
	if ( m_nBackupMode < 0 || m_nBackupMode >= 4 )
	{
		m_nBackupMode = 0;
		AutoBackupOptions.SetCustomMode(0);
	}

	CPropertyPage::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/
void CPropPageBackupMode::OnRadioMode1() { SetBackupMode(0); }
void CPropPageBackupMode::OnRadioMode2() { SetBackupMode(1); }
void CPropPageBackupMode::OnRadioMode3() { SetBackupMode(2); }
void CPropPageBackupMode::OnRadioMode4() { SetBackupMode(3); }
/**********************************************************************/

void CPropPageBackupMode::SetBackupMode( int n ) 
{	
	if ( AutoBackupOptions.GetCustomMode() != n )
	{
		AutoBackupOptions.SetCustomMode(n);
		( (CPropertySheetBackup*) m_pPropertySheet ) -> AddDayPages(m_nMinutes);
	}
}

/**********************************************************************/

