/**********************************************************************/
#include "FileRemover.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluLengthDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPluLengthDlg, CDialog)
/**********************************************************************/

CPluLengthDlg::CPluLengthDlg(CWnd* pParent)
	: CSSDialog(CPluLengthDlg::IDD, pParent)
{
	m_IDLength[0] = IDC_RADIO_LENGTH0;
	m_IDLength[1] = IDC_RADIO_LENGTH1;
	m_IDLength[2] = IDC_RADIO_LENGTH2;
	m_IDLength[3] = IDC_RADIO_LENGTH3;
	m_IDLength[4] = IDC_RADIO_LENGTH4;
	m_IDLength[5] = IDC_RADIO_LENGTH5;
	m_IDLength[6] = IDC_RADIO_LENGTH6;
	m_IDLength[7] = IDC_RADIO_LENGTH7;
	m_IDLength[8] = IDC_RADIO_LENGTH8;
	m_IDLength[9] = IDC_RADIO_LENGTH9;
	m_IDLength[10] = IDC_RADIO_LENGTH10;
	m_IDLength[11] = IDC_RADIO_LENGTH11;
	m_IDLength[12] = IDC_RADIO_LENGTH12;
	m_IDLength[13] = IDC_RADIO_LENGTH13;
}

/**********************************************************************/

CPluLengthDlg::~CPluLengthDlg()
{
}

/**********************************************************************/

void CPluLengthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for (int n = 0; n <= 13; n++)
	{
		DDX_Control(pDX, m_IDLength[n], m_radioLength[n]);
	}

	DDX_Control(pDX, IDC_STATIC_TEXT1, m_staticText1);
	DDX_Control(pDX, IDC_STATIC_TEXT2, m_staticText2);
	DDX_Control(pDX, IDC_STATIC_TEXT3, m_staticText3);
	DDX_Control(pDX, IDC_CHECK_ALTFILES, m_checkAltFiles);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPluLengthDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_LENGTH0, OnSelectLength0)
	ON_BN_CLICKED(IDC_RADIO_LENGTH1, OnSelectLength1)
	ON_BN_CLICKED(IDC_RADIO_LENGTH2, OnSelectLength2)
	ON_BN_CLICKED(IDC_RADIO_LENGTH3, OnSelectLength3)
	ON_BN_CLICKED(IDC_RADIO_LENGTH4, OnSelectLength4)
	ON_BN_CLICKED(IDC_RADIO_LENGTH5, OnSelectLength5)
	ON_BN_CLICKED(IDC_RADIO_LENGTH6, OnSelectLength6)
	ON_BN_CLICKED(IDC_RADIO_LENGTH7, OnSelectLength7)
	ON_BN_CLICKED(IDC_RADIO_LENGTH8, OnSelectLength8)
	ON_BN_CLICKED(IDC_RADIO_LENGTH9, OnSelectLength9)
	ON_BN_CLICKED(IDC_RADIO_LENGTH10, OnSelectLength10)
	ON_BN_CLICKED(IDC_RADIO_LENGTH11, OnSelectLength11)
	ON_BN_CLICKED(IDC_RADIO_LENGTH12, OnSelectLength12)
	ON_BN_CLICKED(IDC_RADIO_LENGTH13, OnSelectLength13)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPluLengthDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle = "";
	strTitle.Format( "Plu Number Settings for %s", AfxGetAppName() );
	SetWindowText( strTitle );

	SelectLength(SysInfo.GetMaxBasePluLen());
	m_checkAltFiles.SetCheck(SysInfo.GetAltFilenamesIfNoModifiersFlag());
	
	return TRUE;
}

/**********************************************************************/
void CPluLengthDlg::OnSelectLength0(){ SelectLength(0); }
void CPluLengthDlg::OnSelectLength1(){ SelectLength(1); }
void CPluLengthDlg::OnSelectLength2(){ SelectLength(2); }
void CPluLengthDlg::OnSelectLength3(){ SelectLength(3); }
void CPluLengthDlg::OnSelectLength4(){ SelectLength(4); }
void CPluLengthDlg::OnSelectLength5(){ SelectLength(5); }
void CPluLengthDlg::OnSelectLength6(){ SelectLength(6); }
void CPluLengthDlg::OnSelectLength7(){ SelectLength(7); }
void CPluLengthDlg::OnSelectLength8(){ SelectLength(8); }
void CPluLengthDlg::OnSelectLength9(){ SelectLength(9); }
void CPluLengthDlg::OnSelectLength10(){ SelectLength(10); }
void CPluLengthDlg::OnSelectLength11(){ SelectLength(11); }
void CPluLengthDlg::OnSelectLength12(){ SelectLength(12); }
void CPluLengthDlg::OnSelectLength13(){ SelectLength(13); }
/**********************************************************************/

void CPluLengthDlg::SelectLength(int nLength)
{
	if ((nLength >= 0) && (nLength <= 13))
	{
		for (int n = 0; n <= 13; n++)
		{
			m_radioLength[n].SetCheck(nLength == n);
		}

		CString strText1 = "";
		CString strText2 = "";
		CString strText3 = "";

		GetTexts(nLength, strText1, strText2, strText3);

		m_staticText1.SetWindowText(strText1);
		m_staticText2.SetWindowText(strText2);
		m_staticText3.SetWindowText(strText3);
	}

	m_checkAltFiles.EnableWindow(0 == nLength);
}

/**********************************************************************/

void CPluLengthDlg::GetTexts( int nLength, CString& strText1, CString& strText2, CString& strText3 )
{
	strText1 = "";
	strText2 = "";
	strText3 = "";

	switch( nLength )
	{
	case 0:
		strText1 = "Modifiers cannot be used with any Plu number.";
		break;

	case 1:
		strText1 = "1 digit Plu numbers can be downloaded with modifiers.";
		break;
			
	case 2:
		strText1 = "1 or 2 digit Plu numbers can be downloaded with modifiers.";
		break;

	default:
		strText1.Format( "Plu numbers of %d or fewer digits can be downloaded with modifiers.", nLength );
		break;
	}

	switch( nLength )
	{
	case 0:
		strText2 = "Plu numbers of up to 14 digits can be downloaded directly.";
		break;

	case 12:
		strText2 = "14 digit Plu numbers can be downloaded directly.";
		break;

	case 13:
		strText2.Format ( "14 digit Plu numbers cannot be used in %s.", AfxGetAppName() );
		break;

	default:
		strText2.Format( "Plu numbers of %d or more digits can be downloaded directly.", nLength + 2 );
		break;
	}

	switch( nLength )
	{
	case 0:
	case 13:
		break;

	default:
		strText3.Format( "%d digit Plu numbers cannot be used in %s.", nLength + 1, AfxGetAppName() );
		break;
	}
}

/**********************************************************************/

void CPluLengthDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

void CPluLengthDlg::OnOK()
{
	int nNewLength = 0;

	for ( int n = 0; n <= 13; n++ )
	{
		if ( m_radioLength[n].GetCheck() != 0 )
		{
			nNewLength = n;
			break;
		}
	}

	{
		bool bChanged = (nNewLength != SysInfo.GetMaxBasePluLen());

		if ((FALSE == bChanged) && (0 == nNewLength))
		{
			bChanged = (IsTicked(m_checkAltFiles) != SysInfo.GetAltFilenamesIfNoModifiersFlag());
		}

		if (FALSE == bChanged)
		{
			EndDialog(IDCANCEL);
			return;
		}
	}

	CString strText1 = "";
	CString strText2 = "";
	CString strText3 = "";

	GetTexts( nNewLength, strText1, strText2, strText3 );

	CString strMsg = "Are you sure you wish to change Plu Number Settings so that:\n\n\n";
	strMsg += strText1;
	strMsg += "\n\n";
	strMsg += strText2;

	if ( strText3 != "" )
	{
		strMsg += "\n\n";
		strMsg += strText3;
	}

	strMsg += "\n\n\n";
	strMsg += "Please restart ";
	strMsg += AfxGetAppName();
	strMsg += " after confirming this change.";

	if ( Prompter.YesNo( strMsg ) != IDYES )
	{
		EndDialog( IDCANCEL );
		return;
	}

	CString strPath = "SysFiles\\";
	GetDataProgramPath( strPath );

	if ((0 == nNewLength) && (IsTicked(m_checkAltFiles) == FALSE))
	{
		nNewLength = 14;
	}

	for ( int n = 0; n <= 14; n++ )
	{
		CString strFilename = "";

		if (n != 14)
		{
			strFilename.Format("%sFILE%1.1X0078.DAT",
				(const char*)strPath,
				n);
		}
		else
		{
			strFilename += strPath;
			strFilename += "FILEX0078.DAT";
		}

		if ( n == nNewLength )
		{
			CSSFile fileToken;
			fileToken.Open( strFilename, "wb" );
			fileToken.WriteLine( "Microtrend" );
			fileToken.Close();
		}
		else
		{
			CFileRemover FileRemover(strFilename);
		}
	}

	EndDialog( IDOK );
}

/**********************************************************************/
