//********************************************************************
#include "DeleteLeaverDlg.h"
//********************************************************************

CDeleteLeaverDlg::CDeleteLeaverDlg(const char* szCaption, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CDeleteLeaverDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
{
	m_strCaption = szCaption;
	m_strButton2 = "";
	m_strButton3 = "";
	m_strComment = "";
	m_strMessage = "";
	m_nDeleteLeaver = 0;						// 0=delete, 1=move CurrentYear/NextYear, 2= PreviousYear/CurrentYear
	m_strLeaversYear = "";						// year to assign leaver to
	m_strLeaverYear1 = "";						// LastYear-ThisYear
	m_strLeaverYear2 = "";						// ThisYear-NextYear
}

//********************************************************************

void CDeleteLeaverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_DELETE2, m_radioDelete2);
	DDX_Control(pDX, IDC_RADIO_DELETE3, m_radioDelete3);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_strMessage);
	DDX_Radio(pDX, IDC_RADIO_DELETE1, m_nDeleteLeaver);
}

//********************************************************************

BEGIN_MESSAGE_MAP(CDeleteLeaverDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//********************************************************************

void CDeleteLeaverDlg::CSSAutoShutdownPreInitDialog()
{
	CSSDate dateToday;		dateToday.SetCurrentDate();

	CString strThisYear;	strThisYear.Format("%d", dateToday.GetYear());
	CString strNextYear;	strNextYear.Format("%d", dateToday.GetYear() + 1);
	CString strLastYear;	strLastYear.Format("%d", dateToday.GetYear() - 1);

	CString strButton2;		m_strButton2.Format("Move record to %s / %s Leavers", strLastYear, strThisYear);
	CString strButton3;		m_strButton3.Format("Move record to %s / %s Leavers", strThisYear, strNextYear);

	m_strLeaverYear1.Format("%s-%s", strLastYear, strThisYear);
	m_strLeaverYear2.Format("%s-%s", strThisYear, strNextYear);

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
		m_nDeleteLeaver = 1;
		break;

	case 10:
	case 11:
	case 12:
		m_nDeleteLeaver = 2;
		break;
	}
}

//********************************************************************

BOOL CDeleteLeaverDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strCaption);
	m_buttonOK.SetWindowText("Confirm");
	m_buttonCancel.SetWindowText("No");

	m_radioDelete2.SetWindowText(m_strButton2);
	m_radioDelete3.SetWindowText(m_strButton3);

	AddComments();

	return TRUE;
}

//********************************************************************

void CDeleteLeaverDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//********************************************************************

void CDeleteLeaverDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		switch ( m_nDeleteLeaver )
		{
		case 0:	
			m_strLeaversYear = "";
			break;

		case 1:		
			m_strLeaversYear = m_strLeaverYear1;
			break;

		case 2:		
			m_strLeaversYear = m_strLeaverYear2;
			break;
		}

		m_Comments.Save ( m_strComment );				// save comment if required
		EndDialog(IDOK);
	}
}

//*******************************************************************

void CDeleteLeaverDlg::OnClickStaticComment()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strComment != "" )
		{
			m_Comments.Delete ( m_strComment );			// delete comment
			m_comboComments.ResetContent();				// new list
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//*******************************************************************
