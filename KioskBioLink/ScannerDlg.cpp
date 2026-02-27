//*******************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*******************************************************************
#include "ScannerDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CScannerDlg, CDialog)
//*******************************************************************

CScannerDlg::CScannerDlg(CWnd* pParent)
	: CSSColourDialog(CScannerDlg::IDD, pParent)
{
}

//*******************************************************************

CScannerDlg::~CScannerDlg()
{
}

//*******************************************************************

void CScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KBINPUT, m_editKbInput);
	DDX_Text(pDX, IDC_EDIT_KBINPUT, m_strKbInput);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CScannerDlg, CSSColourDialog)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CScannerDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image ( System.m_strSysFilesImagePath, "ScannerBackground" );
	
	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	bool bRounded = ( strFilename == "" );

	CSSColourDialog::OnInitDialog( MYCOLOUR_DIALOG, bRounded );
	CSSColourDialog::SetImage ( strFilename, TRANSPARENT_COLOUR );

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "" );
	m_buttonCancel.Init(IDCANCEL, this, BLACK, MYCOLOUR_BUTTON, "", 14 );

	m_staticPrompt.SetWindowText ( System.m_strEnrolmentPrompt );
	m_editKbInput.SetFocus();

	return TRUE;
}

//*******************************************************************