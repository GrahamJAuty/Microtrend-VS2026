/**********************************************************************/
#include "SPOSVersions.h"
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "PropertySheetLocation.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageLocationGeneral.h"
/**********************************************************************/

CPropPageLocationGeneral::CPropPageLocationGeneral() : CSSPropertyPage(CPropPageLocationGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationGeneral)
	//}}AFX_DATA_INIT
	m_strName = "";
	m_strPathCommsIn = "";
	m_strPathCommsInNow = "";
	m_strPathExportControl = "";
	m_strAgresso = "";
	m_pLocationBuffer = NULL;
	m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	m_nSPOSVersion = SPOS_V4;
	m_nExportControlDelay = 0;
}

/**********************************************************************/

CPropPageLocationGeneral::~CPropPageLocationGeneral()
{
}

/**********************************************************************/

void CPropPageLocationGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationGeneral)
	DDX_Control(pDX, IDC_STATIC_PATHS, m_staticPaths);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_STATIC_AGRESSO, m_staticAgresso);
	DDX_Control(pDX, IDC_EDIT_AGRESSO, m_editAgresso);
	DDX_Control(pDX, IDC_EDIT_PATH_SALES, m_editPathSales);
	DDX_Control(pDX, IDC_EDIT_PATH_COMMSIN, m_editPathCommsIn);
	DDX_Control(pDX, IDC_EDIT_PATH_COMMSIN_NOW, m_editPathCommsInNow);
	DDX_Control(pDX, IDC_STATIC_HANDHELD1, m_staticHandheld1);
	DDX_Control(pDX, IDC_STATIC_HANDHELD2, m_staticHandheld2);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticExport);
	DDX_Control(pDX, IDC_CHECK_EXPORT, m_checkExportControl);
	DDX_Control(pDX, IDC_EDIT_PATH_EXPORT, m_editPathExportControl);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_EXPORT, m_buttonBrowseExportControl);
	DDX_Control(pDX, IDC_STATIC_LOYALTY, m_staticLoyalty);
	DDX_Control(pDX, IDC_COMBO_LOYALTY, m_comboLoyalty);
	DDX_Text(pDX, IDC_EDIT_AGRESSO, m_strAgresso);
	DDX_Text(pDX, IDC_EDIT_PATH_COMMSIN, m_strPathCommsIn);
	DDX_Text(pDX, IDC_EDIT_PATH_COMMSIN_NOW, m_strPathCommsInNow);
	DDX_Text(pDX, IDC_EDIT_PATH_EXPORT, m_strPathExportControl);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	DDX_Text(pDX, IDC_EDIT_EXPORT, m_nExportControlDelay);
	DDV_MinMaxInt(pDX, m_nExportControlDelay, 1, 30);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationGeneral)
	ON_BN_CLICKED(IDC_CHECK_EXPORT, OnToggleExportControl)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_COMMSIN, OnButtonBrowseCommsIn)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_COMMSIN_NOW, OnButtonBrowseCommsInNow)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_EXPORT, OnButtonBrowseExportControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit( IDC_EDIT_EXPORT, SS_NUM, 2, "%d" );
	
	m_editPathSales.SetWindowText ( m_strNetworkPath );

	m_checkExportControl.SetCheck( m_pLocationBuffer -> GetExportControlFlag(0) );
	OnToggleExportControl();

	if ( ( CONNECTION_TYPE_STANDARD_REALTIME != m_nConnectionType ) || ( CSPOSVersions::CheckSPOSVersion( m_nSPOSVersion, SPOS_V4_861 ) == FALSE ) )
	{
		ShowAndEnableWindow( &m_checkExportControl, FALSE );
		ShowAndEnableWindow( &m_staticExport, FALSE );
		ShowAndEnableWindow( GetEditExportControlDelay(), FALSE );
		ShowAndEnableWindow( &m_editPathExportControl, FALSE );
		ShowAndEnableWindow( &m_buttonBrowseExportControl, FALSE );
		ShowAndEnableWindow( &m_staticHandheld1, FALSE );
		ShowAndEnableWindow( &m_staticHandheld2, FALSE );
		ResizeControl( &m_staticPaths, 418, 236 );
	}

	m_editAgresso.LimitText( MAX_LENGTH_AGRESSO_LOCGROUP );
	if ( DealerFlags.Get_LIPA_LJMU_ExportType() == 0 )
	{
		ShowAndEnableWindow( &m_staticAgresso, FALSE );
		ShowAndEnableWindow( &m_editAgresso, FALSE );
	}
	else
	{
		ResizeControl( &m_editName, 182, 14 );
		MoveControl( &m_staticAgresso, 294, 25 );
		MoveControl( &m_editAgresso, 347, 24 );
	}

	m_comboLoyalty.AddString("None");
	for (int n = 1; n <= 10; n++)
	{
		CString str = "";
		str.Format("%d", n);
		m_comboLoyalty.AddString(str);
	}

	{
		int nSet = m_pLocationBuffer->GetLoyaltyFolderSet(0);
		if ((nSet >= 0) && (nSet < m_comboLoyalty.GetCount()))
		{
			m_comboLoyalty.SetCurSel(nSet);
		}
		else
		{
			m_comboLoyalty.SetCurSel(0);
		}
	}

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V3:
	case LOYALTY_TYPE_LOY_V4:
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		break;

	default:
		ShowAndEnableWindow(&m_staticLoyalty, FALSE);
		ShowAndEnableWindow(&m_comboLoyalty, FALSE);
		break;
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPageLocationGeneral::GetEditExportControlDelay(){ return GetEdit( IDC_EDIT_EXPORT ); }
/**********************************************************************/

BOOL CPropPageLocationGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationGeneral::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationGeneral::OnButtonBrowseCommsIn() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Deferred CommsIn Path", NULL, this ) == TRUE )
		m_editPathCommsIn.SetWindowText ( strPathname );
}

/**********************************************************************/

void CPropPageLocationGeneral::OnButtonBrowseCommsInNow() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Immediate CommsIn Path", NULL, this ) == TRUE )
		m_editPathCommsInNow.SetWindowText ( strPathname );
}

/**********************************************************************/

void CPropPageLocationGeneral::OnButtonBrowseExportControl() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Export Control Path", NULL, this ) == TRUE )
		m_editPathExportControl.SetWindowText ( strPathname );
}

/**********************************************************************/

void CPropPageLocationGeneral::OnToggleExportControl()
{
	bool bEnable = ( m_checkExportControl.GetCheck() != 0 );
	m_editPathExportControl.EnableWindow( bEnable );
	m_buttonBrowseExportControl.EnableWindow( bEnable );
	GetEditExportControlDelay() -> EnableWindow( bEnable );
}

/**********************************************************************/

void CPropPageLocationGeneral::GetRecordData()
{
	m_strName = m_pLocationBuffer -> GetName(0);
	m_strPathCommsIn = m_pLocationBuffer -> GetDownloadPath(0);
	m_strPathCommsInNow = m_pLocationBuffer -> GetDownloadNowPath(0);
	m_strPathExportControl = m_pLocationBuffer -> GetExportControlPath(0);
	m_strAgresso = m_pLocationBuffer -> GetAgressoLocationGroup(0);
	m_nExportControlDelay = m_pLocationBuffer -> GetExportControlDelay(0);
	
	m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	m_nSPOSVersion = SPOS_V4;
	m_strNetworkPath = "";

	int nNetworkIdx;
	int nNetworkNo = m_pLocationBuffer -> GetNetworkNo(0);
	if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) )
	{
		m_nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );
		m_nSPOSVersion = dbNetwork.GetSPOSVersion( nNetworkIdx );

		switch( m_nConnectionType )
		{
		case CONNECTION_TYPE_STANDARD_BATCH:
			dbNetwork.GetSafeFileNetworkPath( nNetworkIdx, m_strNetworkPath );		
			m_strNetworkPath += "remote";
			break;

		case CONNECTION_TYPE_STANDARD_REALTIME:
		case CONNECTION_TYPE_STANDARD_NONE:
			m_strNetworkPath = dbNetwork.GetFileNetworkPath( nNetworkIdx );
			break;

		default:
			m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
			m_strNetworkPath = "";
			break;
		}
	}
}

/**********************************************************************/

void CPropPageLocationGeneral::SaveRecord()
{
	m_pLocationBuffer->SetName(0, m_strName);
	m_pLocationBuffer->SetDownloadPath(0, m_strPathCommsIn);
	m_pLocationBuffer->SetDownloadNowPath(0, m_strPathCommsInNow);
	m_pLocationBuffer->SetAgressoLocationGroup(0, m_strAgresso);
	m_pLocationBuffer->SetLoyaltyFolderSet(0, m_comboLoyalty.GetCurSel());
	m_pLocationBuffer->SetExportControlFlag(0, m_checkExportControl.GetCheck() != 0);
	m_pLocationBuffer->SetExportControlPath(0, m_strPathExportControl);
	m_pLocationBuffer->SetExportControlDelay(0, m_nExportControlDelay);
}

/**********************************************************************/
