/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "MaxLengths.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsAddress.h"
/**********************************************************************/

CPropPagePMSOptionsAddress::CPropPagePMSOptionsAddress() : CPropertyPage(CPropPagePMSOptionsAddress::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsAddress)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePMSOptionsAddress::~CPropPagePMSOptionsAddress()
{
}

/**********************************************************************/

void CPropPagePMSOptionsAddress::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsAddress)
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
	DDX_Control(pDX, IDC_STATIC_TAXNO, m_staticTaxNo);
	DDX_Text(pDX, IDC_EDIT_TAXNO, m_strTaxNo);
	DDV_MaxChars(pDX, m_strTaxNo, MAX_LENGTH_TAXNO); 
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsAddress, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsAddress::OnInitDialog() 
{
	m_strName = PMSOptions.GetHotelName();
	m_strAddress1 = PMSOptions.GetHotelAddress1();
	m_strAddress2 = PMSOptions.GetHotelAddress2();
	m_strAddress3 = PMSOptions.GetHotelAddress3();
	m_strPostcode = PMSOptions.GetHotelPostcode();
	m_strPhone = PMSOptions.GetHotelPhone();
	m_strFax = PMSOptions.GetHotelFax();
	m_strEmail = PMSOptions.GetHotelEmail();
	m_strTaxNo = PMSOptions.GetHotelTaxNo();

	CPropertyPage::OnInitDialog();	
	m_staticTaxNo.SetWindowText( EcrmanOptions.GetTaxString( "%T No" ) );
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePMSOptionsAddress::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsAddress::SaveRecord()
{
	PMSOptions.SetHotelName( m_strName );
	PMSOptions.SetHotelAddress1( m_strAddress1 );
	PMSOptions.SetHotelAddress2( m_strAddress2 );
	PMSOptions.SetHotelAddress3( m_strAddress3 );
	PMSOptions.SetHotelPostcode( m_strPostcode );
	PMSOptions.SetHotelPhone( m_strPhone );
	PMSOptions.SetHotelFax( m_strFax );
	PMSOptions.SetHotelEmail( m_strEmail );
	PMSOptions.SetHotelTaxNo( m_strTaxNo );
}

/**********************************************************************/
#endif
/**********************************************************************/
