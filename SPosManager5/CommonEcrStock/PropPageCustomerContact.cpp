/**********************************************************************/
#include "PropPageCustomerContact.h"
/**********************************************************************/

CPropPageCustomerContact::CPropPageCustomerContact( int& nIndex, bool bOneOff ) : CSSPropertyPage(CPropPageCustomerContact::IDD), m_nIndex( nIndex )
{
	//{{AFX_DATA_INIT(CPropPageCustomerContact)
	//}}AFX_DATA_INIT
	m_bOneOff = bOneOff;
}

/**********************************************************************/

CPropPageCustomerContact::~CPropPageCustomerContact()
{
}

/**********************************************************************/

void CPropPageCustomerContact::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCustomerContact)
	DDX_Control(pDX, IDC_EDIT_NAME1, m_editName1);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_editName2);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_CHECK_REDIRECT, m_checkRedirect);
	DDX_Control(pDX, IDC_EDIT_PHONE1, m_editPhone1);
	DDX_Control(pDX, IDC_EDIT_PHONE2, m_editPhone2);
	DDX_Control(pDX, IDC_EDIT_PHONE3, m_editPhone3);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageCustomerContact, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCustomerContact)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageCustomerContact::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editName1.LimitText( Customer::CustomerName1.Max );
	m_editName2.LimitText( Customer::CustomerName2.Max );
	m_editEmail.LimitText( Customer::Email.Max );
	m_editPhone1.LimitText( Customer::Phone.Max );
	m_editPhone2.LimitText( Customer::Phone.Max );
	m_editPhone3.LimitText( Customer::Phone.Max );

	if ( TRUE == m_bOneOff )
	{
		ShowAndEnableWindow( &m_checkRedirect, FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageCustomerContact::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageCustomerContact::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageCustomerContact::GetRecordData()
{	
	CCustomerCSVRecord CustomerRecord;

	if ( FALSE == m_bOneOff )
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
	}

	m_editName1.SetWindowText( CustomerRecord.GetCustomerName1() );
	m_editName2.SetWindowText( CustomerRecord.GetCustomerName2() );
	m_editEmail.SetWindowText(  CustomerRecord.GetEmail() );
	m_editPhone1.SetWindowText(  CustomerRecord.GetPhone1() );
	m_editPhone2.SetWindowText(  CustomerRecord.GetPhone2() );
	m_editPhone3.SetWindowText(  CustomerRecord.GetPhone3() );

	m_editName1.SetFocus();
	m_editName1.SetSel ( 0, -1 );

	m_checkRedirect.SetCheck( CustomerRecord.GetEmailRedirectFlag() );
}

/**********************************************************************/

void CPropPageCustomerContact::SetRecordControls()
{
}

/**********************************************************************/

bool CPropPageCustomerContact::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageCustomerContact::SaveRecord()
{
	CCustomerCSVRecord CustomerRecord;

	int nCustomerIdx;
	if ( FALSE == m_bOneOff )
	{
		nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
	}

	CustomerRecord.SetCustomerName1( GetEditBoxText( m_editName1  ) );
	CustomerRecord.SetCustomerName2( GetEditBoxText( m_editName2  ) );
	CustomerRecord.SetEmail( GetEditBoxText( m_editEmail  ) );
	CustomerRecord.SetPhone1( GetEditBoxText( m_editPhone1 ) );
	CustomerRecord.SetPhone2( GetEditBoxText( m_editPhone2 ) );
	CustomerRecord.SetPhone3( GetEditBoxText( m_editPhone3 ) );
	
	if ( FALSE == m_bOneOff )
	{
		CustomerRecord.SetEmailRedirectFlag( m_checkRedirect.GetCheck() != 0 );
		DataManagerNonDb.CustomerAccount.SetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		DataManagerNonDb.CustomerOneOff.SetAt ( nCustomerIdx, CustomerRecord );
	}
}

/**********************************************************************/

void CPropPageCustomerContact::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

