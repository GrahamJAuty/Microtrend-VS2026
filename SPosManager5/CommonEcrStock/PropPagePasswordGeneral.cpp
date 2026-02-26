/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "PropPagePasswordGeneral.h"
/**********************************************************************/

CPropPagePasswordGeneral::CPropPagePasswordGeneral() : CPropertyPage(CPropPagePasswordGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePasswordGeneral)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePasswordGeneral::~CPropPagePasswordGeneral()
{
}

/**********************************************************************/

void CPropPagePasswordGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePasswordGeneral)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, Super.MaxPasswordLen() ); 
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, Super.MaxUsernameLen() ); 
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_CHECK_LOCATION, m_checkLocation);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePasswordGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePasswordGeneral)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_LOCATION,OnToggleLocation)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePasswordGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_SYSTEM );
	arrayTypes.Add ( NODE_DATABASE );
	arrayTypes.Add ( NODE_LOCATION );
	m_LocationSelector.SetForceSystemFlag( TRUE );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );

	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePasswordGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePasswordGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePasswordGeneral::GetRecordData()
{
	m_strPassword = m_pPasswordRecord -> GetPassword();
	m_strUserName = m_pPasswordRecord -> GetUserName();
}

/**********************************************************************/

void CPropPagePasswordGeneral::SetRecordControls()
{
	int nType, nDbIdx, nLocIdx, nSpIdx;
	m_pPasswordRecord -> GetLocationAccess( nType, nDbIdx, nLocIdx, nSpIdx );

	bool bLocation = FALSE;
	switch( nType )
	{
	case LOCATION_ACCESS_NONE:
		m_LocationSelector.ForceSelection( NODE_SYSTEM, 0 );
		break;

	case LOCATION_ACCESS_SYSTEM:
		m_LocationSelector.ForceSelection( NODE_SYSTEM, 0 );
		bLocation = TRUE;
		break;

	case LOCATION_ACCESS_DB:
		m_LocationSelector.ForceSelection( NODE_DATABASE, nDbIdx );
		bLocation = TRUE;
		break;

	case LOCATION_ACCESS_LOC:
		m_LocationSelector.ForceSelection( NODE_LOCATION, nLocIdx );
		bLocation = TRUE;
		break;
	}

	m_checkLocation.SetCheck( bLocation );
	m_comboLocation.EnableWindow( bLocation );
}

/**********************************************************************/

bool CPropPagePasswordGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePasswordGeneral::SaveRecord()
{
	m_pPasswordRecord -> SetPassword ( m_strPassword );
	m_pPasswordRecord -> SetUserName ( m_strUserName );
	
	if ( m_checkLocation.GetCheck() != 0 )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		
		switch( LocSelEntity.GetConType() )
		{
		case NODE_SYSTEM:
			m_pPasswordRecord -> SetLocationAccess( LOCATION_ACCESS_SYSTEM, 0 );
			break;

		case NODE_DATABASE:
			m_pPasswordRecord -> SetLocationAccess( LOCATION_ACCESS_DB, LocSelEntity.GetEntityIdx() );
			break;

		case NODE_LOCATION:
			m_pPasswordRecord -> SetLocationAccess( LOCATION_ACCESS_LOC, LocSelEntity.GetEntityIdx() );
			break;

		default:
			m_pPasswordRecord -> SetLocationAccess( LOCATION_ACCESS_NONE, 0 );
			break;
		}
	}
	else
		m_pPasswordRecord -> SetLocationAccess( LOCATION_ACCESS_NONE, 0 );
}

/**********************************************************************/

void CPropPagePasswordGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePasswordGeneral::OnToggleLocation()
{
	m_comboLocation.EnableWindow( m_checkLocation.GetCheck() != 0 );
}

/**********************************************************************/
