//****************************************************************
#include "MoveLeaversDlg.h"
//****************************************************************

CMoveLeaversDlg::CMoveLeaversDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CMoveLeaversDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())					
{
	m_nMoveLeaver = 0;				// 0=move CurrentYear/NextYear, 1= PreviousYear/CurrentYear
	m_strLeaversYear = "";			// year to assign leaver to
	m_strLeaverYear1 = "";			// LastYear-ThisYear
	m_strLeaverYear2 = "";			// ThisYear-NextYear

	m_strButton2 = "";
	m_strButton3 = "";
}

//****************************************************************

CMoveLeaversDlg::~CMoveLeaversDlg()
{
}

//****************************************************************

void CMoveLeaversDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_Control(pDX, IDC_RADIO_DELETE2, m_radioDelete2);
	DDX_Control(pDX, IDC_RADIO_DELETE3, m_radioDelete3);
	DDX_Radio(pDX, IDC_RADIO_DELETE2, m_nMoveLeaver);
}

//****************************************************************

BEGIN_MESSAGE_MAP(CMoveLeaversDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//****************************************************************

void CMoveLeaversDlg::CSSAutoShutdownPreInitDialog()
{
	CSSDate dateToday{};
	dateToday.SetCurrentDate();

	CString strThisYear = "";
	strThisYear.Format("%d", dateToday.GetYear());
	
	CString strNextYear = "";
	strNextYear.Format("%d", dateToday.GetYear() + 1);
	
	CString strLastYear = "";
	strLastYear.Format("%d", dateToday.GetYear() - 1);

	m_strButton2.Format("Move records to %s / %s Leavers", strLastYear, strThisYear);
	m_strButton3.Format("Move records to %s / %s Leavers", strThisYear, strNextYear);

	m_strLeaverYear1.Format("%s_%s", strLastYear, strThisYear);
	m_strLeaverYear2.Format("%s_%s", strThisYear, strNextYear);

	switch (dateToday.GetMonth())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:		
		m_nMoveLeaver = 0;
		break;
	
	case 10:
	case 11:
	case 12:	
		m_nMoveLeaver = 1;
		break;
	}
}

//****************************************************************

BOOL CMoveLeaversDlg::CSSAutoShutdownPostInitDialog()
{
	m_radioDelete2.SetWindowText ( m_strButton2 );
	m_radioDelete3.SetWindowText ( m_strButton3 );
	AddComments();
	return TRUE;
}

//****************************************************************

void CMoveLeaversDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//****************************************************************

void CMoveLeaversDlg::OnClickStaticComment()
{
	CString strComment = GetComboBoxText(m_comboComments);

	if (strComment != "")
	{
		m_Comments.Delete(strComment);			// delete comment
		m_comboComments.ResetContent();			// new list
		AddComments();

		m_comboComments.SetWindowText("");
	}
}

//*******************************************************************

void CMoveLeaversDlg::OnOK()
{
	switch (m_nMoveLeaver)
	{
	case 0:		m_strLeaversYear = m_strLeaverYear1;	break;
	case 1:		m_strLeaversYear = m_strLeaverYear2;	break;
	}

	m_strComment = GetComboBoxText(m_comboComments);
	m_Comments.Save(m_strComment);
	EndDialog(IDOK);
}

//*******************************************************************
