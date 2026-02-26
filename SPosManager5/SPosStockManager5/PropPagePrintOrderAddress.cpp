/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
 
#include "SiteCSVArray.h"
/**********************************************************************/
#include "PropPagePrintOrderAddress.h"
/**********************************************************************/

CPropPagePrintOrderAddress::CPropPagePrintOrderAddress() : CPropertyPage(CPropPagePrintOrderAddress::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePrintOrderAddress)
	m_strAddress1 = _T("");
	m_strAddress2 = _T("");
	m_strAddress3 = _T("");
	m_strEmail = _T("");
	m_strFax = _T("");
	m_strPhone = _T("");
	m_strPostcode = _T("");
	//}}AFX_DATA_INIT
	m_nDefSpIdx = -1;
}

/**********************************************************************/

void CPropPagePrintOrderAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePrintOrderAddress)
	DDX_Control(pDX, IDC_EDIT_SITE_POSTCODE, m_editPostcode);
	DDX_Control(pDX, IDC_EDIT_SITE_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_EDIT_SITE_FAX, m_editFax);
	DDX_Control(pDX, IDC_EDIT_SITE_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_EDIT_SITE_ADDRESS3, m_editAddress3);
	DDX_Control(pDX, IDC_EDIT_SITE_ADDRESS2, m_editAddress2);
	DDX_Control(pDX, IDC_EDIT_SITE_ADDRESS1, m_editAddress1);
	DDX_Control(pDX, IDC_COMBO_SITE, m_comboSite);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS1, m_strAddress1);
	DDV_MaxChars(pDX, m_strAddress1, MAX_LENGTH_ADDRESS_STREET);
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS2, m_strAddress2);
	DDV_MaxChars(pDX, m_strAddress2, MAX_LENGTH_ADDRESS_STREET);
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS3, m_strAddress3);
	DDV_MaxChars(pDX, m_strAddress3, MAX_LENGTH_ADDRESS_STREET);
	DDX_Text(pDX, IDC_EDIT_SITE_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, MAX_LENGTH_ADDRESS_EMAIL);
	DDX_Text(pDX, IDC_EDIT_SITE_FAX, m_strFax);
	DDV_MaxChars(pDX, m_strFax, MAX_LENGTH_ADDRESS_FAX);
	DDX_Text(pDX, IDC_EDIT_SITE_POSTCODE, m_strPostcode);
	DDV_MaxChars(pDX, m_strPostcode, MAX_LENGTH_ADDRESS_POSTCODE);
	DDX_Text(pDX, IDC_EDIT_SITE_PHONE, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, MAX_LENGTH_ADDRESS_PHONE);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePrintOrderAddress, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePrintOrderAddress)
	ON_CBN_SELCHANGE(IDC_COMBO_SITE, OnSelectSite)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePrintOrderAddress::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );
	arrayTypes.Add ( NODE_SITE );
	arrayTypes.Add ( NODE_STOCKPOINT );

	m_LocationSelector.SetSingleDbIdx ( DataManager.GetActiveDbIdx() );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboSite );

	if ( m_nDefSpIdx >= 0 )
		m_LocationSelector.ForceSelection( NODE_STOCKPOINT, m_nDefSpIdx );
		
	OnSelectSite();
	return TRUE;  
}

/**********************************************************************/

void CPropPagePrintOrderAddress::OnSelectSite() 
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	DisplayAddress( LocSelEntity );
}

/**********************************************************************/

void CPropPagePrintOrderAddress::DisplayAddress ( CLocationSelectorEntity& LocSelEntity )
{
	CAddressInfo infoAddress;
	
	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		{
			int nDbIdx = DataManager.GetActiveDbIdx();
			dbDatabase.GetAddressInfo( nDbIdx, infoAddress );
		}
		break;

	case NODE_SITE:
		dbSite.GetAddressInfo( LocSelEntity.GetEntityIdx(), infoAddress );
		break;

	case NODE_STOCKPOINT:
		dbStockpoint.GetAddressInfo( LocSelEntity.GetEntityIdx(), infoAddress );
		break;
	}

	m_editAddress1.SetWindowText ( infoAddress.m_strAddress1 );
	m_editAddress2.SetWindowText ( infoAddress.m_strAddress2 );
	m_editAddress3.SetWindowText ( infoAddress.m_strAddress3 );
	m_editEmail.SetWindowText ( infoAddress.m_strEmail );
	m_editFax.SetWindowText ( infoAddress.m_strFax );
	m_editPhone.SetWindowText ( infoAddress.m_strPhone );
	m_editPostcode.SetWindowText ( infoAddress.m_strPostcode );
}

/**********************************************************************/

BOOL CPropPagePrintOrderAddress::OnKillActive() 
{
	BOOL b = CPropertyPage::OnKillActive();
	int nDbIdx = DataManager.GetActiveDbIdx();
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	//BUSINESS NAME FOR TOP OF ORDER SHEET
	m_pOrderDetails -> m_strTopName = dbDatabase.GetName( nDbIdx );
		
	//BUSINESS ADDRESS FOR TOP OF ORDER SHEET
	switch( LocSelEntity.GetConType() )
	{
	case NODE_STOCKPOINT:
	case NODE_SITE:
		{
			CAddressInfo infoAddress;
			dbDatabase.GetAddressInfo( nDbIdx, infoAddress );

			m_pOrderDetails -> m_strTopName = dbDatabase.GetName( nDbIdx );
			m_pOrderDetails -> m_strTopAddress1 = infoAddress.m_strAddress1;
			m_pOrderDetails -> m_strTopAddress2 = infoAddress.m_strAddress2;
			m_pOrderDetails -> m_strTopAddress3 = infoAddress.m_strAddress3;
			m_pOrderDetails -> m_strTopPhone = infoAddress.m_strPhone; 
			m_pOrderDetails -> m_strTopPostcode = infoAddress.m_strPostcode;
			m_pOrderDetails -> m_strTopFax = infoAddress.m_strFax;
			m_pOrderDetails -> m_strTopEmail = infoAddress.m_strEmail;
			m_pOrderDetails -> m_bDeliverAsOrdered = FALSE;
		}
		break;

	case NODE_DATABASE:
	default:
		{
			m_pOrderDetails -> m_strTopAddress1 = m_strAddress1;
			m_pOrderDetails -> m_strTopAddress2 = m_strAddress2;
			m_pOrderDetails -> m_strTopAddress3 = m_strAddress3;
			m_pOrderDetails -> m_strTopPhone = m_strPhone;
			m_pOrderDetails -> m_strTopPostcode = m_strPostcode;
			m_pOrderDetails -> m_strTopFax = m_strFax;
			m_pOrderDetails -> m_strTopEmail = m_strEmail;
			m_pOrderDetails -> m_bDeliverAsOrdered = TRUE;	
		}
		break;
	}

	//GET BUSINESS NAME FOR DELIVERY ADDRESS
	switch( LocSelEntity.GetConType() )
	{
	case NODE_STOCKPOINT:
		m_pOrderDetails -> m_strName = dbStockpoint.GetName ( LocSelEntity.GetEntityIdx() );
		break;

	case NODE_SITE:
		m_pOrderDetails -> m_strName = dbSite.GetName ( LocSelEntity.GetEntityIdx() );
		break;

	case NODE_DATABASE:
	default:
		break;
	}

	//DELIVERY ADDRESS IF REQUIRED
	m_pOrderDetails -> m_strAddress1 = m_strAddress1;
	m_pOrderDetails -> m_strAddress2 = m_strAddress2;
	m_pOrderDetails -> m_strAddress3 = m_strAddress3;
	m_pOrderDetails -> m_strPhone = m_strPhone;
	m_pOrderDetails -> m_strPostcode = m_strPostcode;
	m_pOrderDetails -> m_strFax = m_strFax;
	m_pOrderDetails -> m_strEmail = m_strEmail;

	return b;
}

/**********************************************************************/

void CPropPagePrintOrderAddress::OnButtonSave() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to save these address details.", TRUE ) == IDNO )
			return;

		CAddressInfo infoAddress;
		infoAddress.m_strAddress1 = m_strAddress1;
		infoAddress.m_strAddress2 = m_strAddress2;
		infoAddress.m_strAddress3 = m_strAddress3;
		infoAddress.m_strPostcode = m_strPostcode;
		infoAddress.m_strPhone = m_strPhone;
		infoAddress.m_strFax = m_strFax;
		infoAddress.m_strEmail = m_strEmail;
			
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		switch ( LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
			{
				int nDbIdx = DataManager.GetActiveDbIdx();
				dbDatabase.SetAddressInfo( nDbIdx, infoAddress );
			}
			break;

		case NODE_SITE:
			dbSite.SetAddressInfo( LocSelEntity.GetEntityIdx(), infoAddress );
			break;

		case NODE_STOCKPOINT:
			dbStockpoint.SetAddressInfo( LocSelEntity.GetEntityIdx(), infoAddress );
			break;
		}
	}
}

/**********************************************************************/
