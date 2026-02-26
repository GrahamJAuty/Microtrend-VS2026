/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePaymentText.h"
/**********************************************************************/

CPropPagePaymentText::CPropPagePaymentText() : CSSPropertyPage(CPropPagePaymentText::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePaymentText)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePaymentText::~CPropPagePaymentText()
{
}

/**********************************************************************/

void CPropPagePaymentText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePaymentText)
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_EDIT_EPOSNAME, m_editEPOSName);
	DDX_Control(pDX, IDC_EDIT_REPORTNAME, m_editReportName);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePaymentText, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePaymentText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePaymentText::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SetRecordControls();
	m_editEPOSName.LimitText( Payment::PaymentEPOSName.Max );
	m_editReportName.LimitText( Payment::PaymentReportName.Max );
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePaymentText::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePaymentText::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePaymentText::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePaymentText::SetRecordControls()
{
	m_editEPOSName.SetWindowText( m_pPaymentRecord -> GetEPOSName() );
	m_editReportName.SetWindowText( m_pPaymentRecord -> GetReportName() );
	m_editText.SetWindowText( m_pPaymentRecord -> GetSQLText() );
}

/**********************************************************************/

bool CPropPagePaymentText::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePaymentText::SaveRecord()
{
	CString strText;

	CString strEPOSName;
	m_editEPOSName.GetWindowText( strEPOSName );
	m_pPaymentRecord -> SetEPOSName( strEPOSName );
	
	CString strReportName;
	m_editReportName.GetWindowText( strReportName );
	m_pPaymentRecord -> SetReportName( strReportName );

	m_editText.GetWindowText( strText );
	m_pPaymentRecord -> ImportSQLText( strText );
}

/**********************************************************************/

void CPropPagePaymentText::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
