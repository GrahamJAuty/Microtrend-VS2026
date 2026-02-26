/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsInvoice.h"
/**********************************************************************/

CPropPagePMSOptionsInvoice::CPropPagePMSOptionsInvoice() : CSSPropertyPage(CPropPagePMSOptionsInvoice::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsInvoice)
	//}}AFX_DATA_INIT

	m_nLines = PMSOptions.GetInvoiceHeaderLines();

	if ( ( m_nLines < 0 ) || ( m_nLines > 20 ) )
		m_nLines = 0;
}

/**********************************************************************/

CPropPagePMSOptionsInvoice::~CPropPagePMSOptionsInvoice()
{
}

/**********************************************************************/

void CPropPagePMSOptionsInvoice::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsInvoice)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO_INVOICE, m_comboInvoice );
	DDX_Control( pDX, IDC_CHECK_ADDRESS, m_checkAddress );
	DDX_Control( pDX, IDC_CHECK_LINES, m_checkLines);
	DDX_Control( pDX, IDC_CHECK_REFERENCE, m_checkReference);
	DDX_Control( pDX, IDC_CHECK_NUMBER, m_checkNumber);
	DDX_Control( pDX, IDC_CHECK_INTERIM, m_checkInterim);
	DDX_Control( pDX, IDC_CHECK_NOTE, m_checkNote);
	DDX_Control( pDX, IDC_CHECK_STAY, m_checkStay);
	DDX_Text(pDX, IDC_EDIT_LINES, m_nLines);
	DDV_MinMaxInt( pDX, m_nLines, 0, 20);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsInvoice, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsInvoice)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_CHECK_LINES, OnToggleLines )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsInvoice::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SubclassEdit( IDC_EDIT_LINES, SS_NUM, 2, "%d" );
	
	m_comboInvoice.AddString( "Itemised Sales" );
	m_comboInvoice.AddString( "Group Summary" );
	m_comboInvoice.AddString( "Transaction Summary" );

	int nType = PMSOptions.GetInvoiceDefaultType();
	if ( ( nType >= 0 ) && ( nType <= 2 ) )
		m_comboInvoice.SetCurSel( nType );
	else
		m_comboInvoice.SetCurSel( 0 );

	m_checkAddress.SetCheck( PMSOptions.GetInvoiceShowAddressFlag() );

	m_checkLines.SetCheck( m_nLines != 0 );
	GetEditLines() -> EnableWindow( m_nLines != 0 );

	m_checkReference.SetCheck( PMSOptions.GetInvoiceHideReferenceFlag() == 0 );
	m_checkNumber.SetCheck( PMSOptions.GetInvoiceHideNumberFlag() == 0 );
	m_checkInterim.SetCheck( PMSOptions.GetInvoiceHideInterimFlag() == 0 );
	m_checkNote.SetCheck( PMSOptions.GetInvoiceShowNoteFlag() != 0 );
	m_checkStay.SetCheck( PMSOptions.GetInvoiceHideDatesOfStayFlag() == 0 );

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPagePMSOptionsInvoice::GetEditLines(){ return GetEdit( IDC_EDIT_LINES ); }
/**********************************************************************/

void CPropPagePMSOptionsInvoice::OnToggleLines()
{
	GetEditLines() -> EnableWindow( m_checkLines.GetCheck() != 0 );
}

/**********************************************************************/

BOOL CPropPagePMSOptionsInvoice::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsInvoice::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsInvoice::SaveRecord()
{
	PMSOptions.SetInvoiceDefaultType( m_comboInvoice.GetCurSel() );
	PMSOptions.SetInvoiceShowAddressFlag( m_checkAddress.GetCheck() != 0 );

	if ( m_checkLines.GetCheck() != 0 )
		PMSOptions.SetInvoiceHeaderLines( m_nLines );
	else
		PMSOptions.SetInvoiceHeaderLines( 0 );

	PMSOptions.SetInvoiceHideReferenceFlag( m_checkReference.GetCheck() == 0 );
	PMSOptions.SetInvoiceHideNumberFlag( m_checkNumber.GetCheck() == 0 );
	PMSOptions.SetInvoiceHideInterimFlag( m_checkInterim.GetCheck() == 0 );
	PMSOptions.SetInvoiceShowNoteFlag( m_checkNote.GetCheck() != 0 );
	PMSOptions.SetInvoiceHideDatesOfStayFlag( m_checkStay.GetCheck() == 0 );
}

/**********************************************************************/
#endif
/**********************************************************************/
