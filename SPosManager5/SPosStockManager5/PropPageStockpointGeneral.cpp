/**********************************************************************/
#include "MaxLengths.h"
#include "PropPageStockpointGeneral.h"
/**********************************************************************/

CPropPageStockpointGeneral::CPropPageStockpointGeneral() : CSSPropertyPage(CPropPageStockpointGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockpointGeneral)
	//}}AFX_DATA_INIT
	m_pAddressInfo = NULL;
	m_pStockpointBuffer = NULL;
}

/**********************************************************************/

CPropPageStockpointGeneral::~CPropPageStockpointGeneral()
{
}

/**********************************************************************/

void CPropPageStockpointGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockpointGeneral)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_IMPORTSITE, m_staticImportSite);
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
	DDX_Text(pDX, IDC_EDIT_IMPORTSITE, m_nImportSiteNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockpointGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockpointGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageStockpointGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit( IDC_EDIT_IMPORTSITE, SS_NUM, 3, "%d" );

	if ( DealerFlags.GetDorbiereImportFlag() == FALSE )
	{
		ShowAndEnableWindow( &m_staticImportSite, FALSE );
		ShowAndEnableWindow( GetEditImportSite(), FALSE );
	}

	return TRUE;    
}

/**********************************************************************/
CEdit* CPropPageStockpointGeneral::GetEditImportSite() { return GetEdit( IDC_EDIT_IMPORTSITE ); }
/**********************************************************************/

BOOL CPropPageStockpointGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageStockpointGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageStockpointGeneral::GetRecordData()
{
	m_strName = m_pStockpointBuffer -> GetName(0);
	m_strAddress1 = m_pAddressInfo -> m_strAddress1;
	m_strAddress2 = m_pAddressInfo -> m_strAddress2;
	m_strAddress3 = m_pAddressInfo -> m_strAddress3;
	m_strPostcode = m_pAddressInfo -> m_strPostcode;
	m_strPhone = m_pAddressInfo -> m_strPhone;
	m_strFax = m_pAddressInfo -> m_strFax;
	m_strEmail = m_pAddressInfo -> m_strEmail;
	m_nImportSiteNo = m_pStockpointBuffer -> GetImportSiteNo(0);
}

/**********************************************************************/

void CPropPageStockpointGeneral::SaveRecord()
{
	m_pStockpointBuffer -> SetName( 0, m_strName );
	m_pAddressInfo -> m_strAddress1 = m_strAddress1;
	m_pAddressInfo -> m_strAddress2 = m_strAddress2;
	m_pAddressInfo -> m_strAddress3 = m_strAddress3;
	m_pAddressInfo -> m_strPostcode = m_strPostcode;
	m_pAddressInfo -> m_strPhone = m_strPhone;
	m_pAddressInfo -> m_strFax = m_strFax;
	m_pAddressInfo -> m_strEmail = m_strEmail;
	m_pStockpointBuffer -> SetImportSiteNo( 0, m_nImportSiteNo );
}

/**********************************************************************/



