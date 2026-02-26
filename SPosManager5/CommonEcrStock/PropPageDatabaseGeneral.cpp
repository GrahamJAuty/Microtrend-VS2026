/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "PropPageDatabaseGeneral.h"
/**********************************************************************/

CPropPageDatabaseGeneral::CPropPageDatabaseGeneral() : CPropertyPage(CPropPageDatabaseGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageDatabaseGeneral)
	//}}AFX_DATA_INIT
	m_pDatabaseBuffer = NULL;
}

/**********************************************************************/

CPropPageDatabaseGeneral::~CPropPageDatabaseGeneral()
{
}

/**********************************************************************/

void CPropPageDatabaseGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDatabaseGeneral)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SITE_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_ADDRESS_NAME); 
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS1, m_strAddress1);
	DDV_MaxChars(pDX, m_strAddress1, MAX_LENGTH_ADDRESS_STREET); 
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS2, m_strAddress2);
	DDV_MaxChars(pDX, m_strAddress2, MAX_LENGTH_ADDRESS_STREET);
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS3, m_strAddress3);
	DDV_MaxChars(pDX, m_strAddress3, MAX_LENGTH_ADDRESS_STREET); 
	DDX_Text(pDX, IDC_EDIT_SITE_PHONE, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, MAX_LENGTH_ADDRESS_PHONE); 
	DDX_Text(pDX, IDC_EDIT_SITE_POSTCODE, m_strPostcode);
	DDV_MaxChars(pDX, m_strPostcode, MAX_LENGTH_ADDRESS_POSTCODE); 
	DDX_Text(pDX, IDC_EDIT_SITE_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, MAX_LENGTH_ADDRESS_EMAIL); 
	DDX_Text(pDX, IDC_EDIT_SITE_FAX, m_strFax);
	DDV_MaxChars(pDX, m_strFax, MAX_LENGTH_ADDRESS_FAX); 
	DDX_Control(pDX, IDC_STATIC_TAXNO, m_staticTaxNo );
	DDX_Text(pDX, IDC_EDIT_TAXNO, m_strTaxNo);
	DDV_MaxChars(pDX, m_strTaxNo, MAX_LENGTH_TAXNO); 
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDatabaseGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDatabaseGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDatabaseGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_staticTaxNo.SetWindowText( EcrmanOptions.GetTaxString( "%T No" ) );
	return TRUE;    
}

/**********************************************************************/

BOOL CPropPageDatabaseGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageDatabaseGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageDatabaseGeneral::GetRecordData()
{
	m_strName = m_pDatabaseBuffer -> GetName(0);
	m_strAddress1 = m_pDatabaseBuffer -> GetLegacyAddress1(0);
	m_strAddress2 = m_pDatabaseBuffer -> GetLegacyAddress2(0);
	m_strAddress3 = m_pDatabaseBuffer -> GetLegacyAddress3(0);
	m_strPostcode = m_pDatabaseBuffer -> GetLegacyPostcode(0);
	m_strPhone = m_pDatabaseBuffer -> GetLegacyPhone(0);
	m_strFax = m_pDatabaseBuffer -> GetLegacyFax(0);
	m_strEmail = m_pDatabaseBuffer -> GetLegacyEmail(0);
	m_strTaxNo = m_pDatabaseBuffer -> GetTaxNo(0);
}

/**********************************************************************/

void CPropPageDatabaseGeneral::SaveRecord()
{
	m_pDatabaseBuffer -> SetName( 0, m_strName );
	m_pDatabaseBuffer -> SetLegacyAddress1( 0, m_strAddress1 );
	m_pDatabaseBuffer -> SetLegacyAddress2( 0, m_strAddress2 );
	m_pDatabaseBuffer -> SetLegacyAddress3( 0, m_strAddress3 );
	m_pDatabaseBuffer -> SetLegacyPostcode( 0, m_strPostcode );
	m_pDatabaseBuffer -> SetLegacyPhone( 0, m_strPhone );
	m_pDatabaseBuffer -> SetLegacyFax( 0, m_strFax );
	m_pDatabaseBuffer -> SetLegacyEmail( 0, m_strEmail );
	m_pDatabaseBuffer -> SetTaxNo( 0, m_strTaxNo );
}

/**********************************************************************/



