/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "ColourEditDlg.h"
#include "PMSOptions.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PropPagePMSOptionsColour.h"
/**********************************************************************/

CPropPagePMSOptionsColour::CPropPagePMSOptionsColour() : CSSPropertyPage(CPropPagePMSOptionsColour::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsColour)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePMSOptionsColour::~CPropPagePMSOptionsColour()
{
}

/**********************************************************************/

void CPropPagePMSOptionsColour::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsColour)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COLOURBUTTON_VACANT, m_buttonColourVacant);
	DDX_Control( pDX, IDC_COLOURBUTTON_PENDING, m_buttonColourPending);
	DDX_Control( pDX, IDC_COLOURBUTTON_ACTIVE, m_buttonColourActive);
	DDX_Control( pDX, IDC_COLOURBUTTON_COMPLETE, m_buttonColourComplete);
	DDX_Control( pDX, IDC_COLOURBUTTON_CLOSED, m_buttonColourClosed);
	DDX_Control( pDX, IDC_COLOURBUTTON_SELECTED, m_buttonColourSelected);
	DDX_Control( pDX, IDC_COLOURBUTTON_HIGHLIGHTED, m_buttonColourHighlighted);
	DDX_Control( pDX, IDC_COLOURBUTTON_UNHIGHLIGHTED, m_buttonColourUnhighlighted);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsColour, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsColour)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_VACANT_BACK, OnButtonVacantBack)
	ON_BN_CLICKED(IDC_BUTTON_VACANT_TEXT, OnButtonVacantText)
	ON_BN_CLICKED(IDC_BUTTON_PENDING_BACK, OnButtonPendingBack)
	ON_BN_CLICKED(IDC_BUTTON_PENDING_TEXT, OnButtonPendingText)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE_BACK, OnButtonActiveBack)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE_TEXT, OnButtonActiveText)
	ON_BN_CLICKED(IDC_BUTTON_COMPLETE_BACK, OnButtonCompleteBack)
	ON_BN_CLICKED(IDC_BUTTON_COMPLETE_TEXT, OnButtonCompleteText)
	ON_BN_CLICKED(IDC_BUTTON_CLOSED_BACK, OnButtonClosedBack)
	ON_BN_CLICKED(IDC_BUTTON_CLOSED_TEXT, OnButtonClosedText)
	ON_BN_CLICKED(IDC_BUTTON_SELECTED_BACK, OnButtonSelectedBack)
	ON_BN_CLICKED(IDC_BUTTON_SELECTED_TEXT, OnButtonSelectedText)
	ON_BN_CLICKED(IDC_BUTTON_HIGHLIGHTED_BACK, OnButtonHighlightedBack)
	ON_BN_CLICKED(IDC_BUTTON_HIGHLIGHTED_TEXT, OnButtonHighlightedText)
	ON_BN_CLICKED(IDC_BUTTON_UNHIGHLIGHTED_BACK, OnButtonUnhighlightedBack)
	ON_BN_CLICKED(IDC_BUTTON_UNHIGHLIGHTED_TEXT, OnButtonUnhighlightedText)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsColour::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UpdateButtonColours();
	
	InitialiseColourButton( m_buttonColourVacant, 0, "Vacant" );
	InitialiseColourButton( m_buttonColourPending, 1, "Pending" );
	InitialiseColourButton( m_buttonColourActive, 2, "Active" );
	InitialiseColourButton( m_buttonColourComplete, 3, "Complete" );
	InitialiseColourButton( m_buttonColourClosed, 4, "Closed" );
	InitialiseColourButton( m_buttonColourSelected, 5, "Selected" );
	InitialiseColourButton( m_buttonColourHighlighted, 6, "Highlighted" );
	InitialiseColourButton( m_buttonColourUnhighlighted, 7, "Unhighlighted" );

	UpdateButtonColours();
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSOptionsColour::InitialiseColourButton( CColourButtonText& button, int nNo, const char* szText )
{
	button.m_nButtonNo = nNo;
	button.m_strText = szText;
}

/**********************************************************************/

void CPropPagePMSOptionsColour::UpdateButtonColours()
{
	m_buttonColourVacant.m_ColourRefBack = COLORREF( PMSOptions.GetColourVacantBack() );
	m_buttonColourVacant.m_ColourRefText = COLORREF( PMSOptions.GetColourVacantText() );
	m_buttonColourPending.m_ColourRefBack = COLORREF( PMSOptions.GetColourPendingBack() );
	m_buttonColourPending.m_ColourRefText = COLORREF( PMSOptions.GetColourPendingText() );
	m_buttonColourActive.m_ColourRefBack = COLORREF( PMSOptions.GetColourActiveBack() );
	m_buttonColourActive.m_ColourRefText = COLORREF( PMSOptions.GetColourActiveText() );
	m_buttonColourComplete.m_ColourRefBack = COLORREF( PMSOptions.GetColourCompleteBack() );
	m_buttonColourComplete.m_ColourRefText = COLORREF( PMSOptions.GetColourCompleteText() );
	m_buttonColourClosed.m_ColourRefBack = COLORREF( PMSOptions.GetColourClosedBack() );
	m_buttonColourClosed.m_ColourRefText = COLORREF( PMSOptions.GetColourClosedText() );
	m_buttonColourSelected.m_ColourRefBack = COLORREF( PMSOptions.GetColourSelectedBack() );
	m_buttonColourSelected.m_ColourRefText = COLORREF( PMSOptions.GetColourSelectedText() );
	m_buttonColourHighlighted.m_ColourRefBack = COLORREF( PMSOptions.GetColourHighlightedBack() );
	m_buttonColourHighlighted.m_ColourRefText = COLORREF( PMSOptions.GetColourHighlightedText() );
	m_buttonColourUnhighlighted.m_ColourRefBack = COLORREF( PMSOptions.GetColourUnhighlightedBack() );
	m_buttonColourUnhighlighted.m_ColourRefText = COLORREF( PMSOptions.GetColourUnhighlightedText() );

	m_buttonColourVacant.Invalidate();
	m_buttonColourPending.Invalidate();
	m_buttonColourActive.Invalidate();
	m_buttonColourComplete.Invalidate();
	m_buttonColourClosed.Invalidate();
	m_buttonColourSelected.Invalidate();
	m_buttonColourHighlighted.Invalidate();
	m_buttonColourUnhighlighted.Invalidate();
}

/**********************************************************************/

long CPropPagePMSOptionsColour::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	CColourButtonText* pButton = NULL;

	switch( ( m_nColourMode - 1 ) / 2 )
	{
	case 0:	pButton = &m_buttonColourVacant;		break;
	case 1:	pButton = &m_buttonColourPending;		break;
	case 2:	pButton = &m_buttonColourActive;		break;
	case 3: pButton = &m_buttonColourComplete;		break;
	case 4:	pButton = &m_buttonColourClosed;		break;
	case 5:	pButton = &m_buttonColourSelected;		break;
	case 6:	pButton = &m_buttonColourHighlighted;	break;
	case 7:	pButton = &m_buttonColourUnhighlighted;	break;
	}

	if ( pButton != NULL )
	{
		if ( ( m_nColourMode % 2 ) == 0 )
			pButton -> m_ColourRefText = l;
		else
			pButton -> m_ColourRefBack = l;

		pButton -> Invalidate();
	}

	return 0l;
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonVacantBack()
{
	m_nColourMode = 1;
	int nColour = PMSOptions.GetColourVacantBack();
	ChangeButtonColour( nColour, "Vacant Room Background Colour" );
	PMSOptions.SetColourVacantBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonVacantText()
{
	m_nColourMode = 2;
	int nColour = PMSOptions.GetColourVacantText();
	ChangeButtonColour( nColour, "Vacant Room Text Colour" );
	PMSOptions.SetColourVacantText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonPendingBack()
{
	m_nColourMode = 3;	
	int nColour = PMSOptions.GetColourPendingBack();
	ChangeButtonColour( nColour, "Pending Booking Background Colour" );
	PMSOptions.SetColourPendingBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonPendingText()
{
	m_nColourMode = 4;
	int nColour = PMSOptions.GetColourPendingText();
	ChangeButtonColour( nColour, "Pending Booking Text Colour" );
	PMSOptions.SetColourPendingText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonActiveBack()
{
	m_nColourMode = 5;
	int nColour = PMSOptions.GetColourActiveBack();
	ChangeButtonColour( nColour, "Active Booking Background Colour" );
	PMSOptions.SetColourActiveBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonActiveText()
{
	m_nColourMode = 6;
	int nColour = PMSOptions.GetColourActiveText();
	ChangeButtonColour( nColour, "Active Booking Text Colour" );
	PMSOptions.SetColourActiveText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonCompleteBack()
{
	m_nColourMode = 7;
	int nColour = PMSOptions.GetColourCompleteBack();
	ChangeButtonColour( nColour, "Completed Booking Background Colour" );
	PMSOptions.SetColourCompleteBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonCompleteText()
{
	m_nColourMode = 8;
	int nColour = PMSOptions.GetColourCompleteText();
	ChangeButtonColour( nColour, "Completed Booking Text Colour" );
	PMSOptions.SetColourCompleteText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonClosedBack()
{
	m_nColourMode = 9;
	int nColour = PMSOptions.GetColourClosedBack();
	ChangeButtonColour( nColour, "Closed Room Background Colour" );
	PMSOptions.SetColourClosedBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonClosedText()
{
	m_nColourMode = 10;
	int nColour = PMSOptions.GetColourClosedText();
	ChangeButtonColour( nColour, "Closed Room Text Colour" );
	PMSOptions.SetColourClosedText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonSelectedBack()
{
	m_nColourMode = 11;
	int nColour = PMSOptions.GetColourSelectedBack();
	ChangeButtonColour( nColour, "Selected Booking Background Colour" );
	PMSOptions.SetColourSelectedBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonSelectedText()
{
	m_nColourMode = 12;
	int nColour = PMSOptions.GetColourSelectedText();
	ChangeButtonColour( nColour, "Selected Booking Text Colour" );
	PMSOptions.SetColourSelectedText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonHighlightedBack()
{
	m_nColourMode = 13;
	int nColour = PMSOptions.GetColourHighlightedBack();
	ChangeButtonColour( nColour, "Highlighted Booking Background Colour" );
	PMSOptions.SetColourHighlightedBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonHighlightedText()
{
	m_nColourMode = 14;
	int nColour = PMSOptions.GetColourHighlightedText();
	ChangeButtonColour( nColour, "Highlighted Booking Text Colour" );
	PMSOptions.SetColourHighlightedText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonUnhighlightedBack()
{
	m_nColourMode = 15;
	int nColour = PMSOptions.GetColourUnhighlightedBack();
	ChangeButtonColour( nColour, "Unhighlighted Booking Background Colour" );
	PMSOptions.SetColourUnhighlightedBack( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonUnhighlightedText()
{
	m_nColourMode = 16;
	int nColour = PMSOptions.GetColourUnhighlightedText();
	ChangeButtonColour( nColour, "Unhighlighted Booking Text Colour" );
	PMSOptions.SetColourUnhighlightedText( nColour );
	UpdateButtonColours();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::ChangeButtonColour( int& nColour, const char* szText )
{
	CColourEditDlg dlg( nColour, szText, this, FALSE, this );
			
	if ( dlg.DoModal() == IDOK )
		nColour = dlg.GetColourRef();
}

/**********************************************************************/

void CPropPagePMSOptionsColour::OnButtonDefault()
{
	if ( Prompter.YesNo( "Restore Default Colours" ) == IDYES )
	{
		PMSOptions.SetDefaultColours();
		UpdateButtonColours();
	}
}

/**********************************************************************/

BOOL CPropPagePMSOptionsColour::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsColour::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsColour::SaveRecord()
{
}

/**********************************************************************/
#endif
/**********************************************************************/
