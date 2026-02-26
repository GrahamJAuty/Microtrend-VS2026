/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "DefEditStartupMsgDlg.h"
/**********************************************************************/

CDefEditStartupMsgDlg::CDefEditStartupMsgDlg( CWnd* pParent )
	: CSSDialog(CDefEditStartupMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditStartupMsgDlg)
	//}}AFX_DATA_INIT

	m_strStartupMsg = "";

	CSSFile fileMessage;
	CFilenameUpdater FnUp(SysFiles::StartupMessage);
	if (fileMessage.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
	{
		int nLineCount = 0;
		CString strText = "";
		while ((fileMessage.ReadString(strText) == TRUE) && ( nLineCount < MAX_STARTUPMSG_LINES))
		{
			if (m_strStartupMsg != "")
			{
				m_strStartupMsg += "\r\n";
			}

			strText = strText.Left(MAX_STARTUPMSG_LINELENGTH);
			m_strStartupMsg += strText;
			nLineCount++;
		}
	}
}

/**********************************************************************/

void CDefEditStartupMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditStartupMsgDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_STARTUPMSG, m_editStartupMsg);
	DDX_Text(pDX, IDC_EDIT_STARTUPMSG, m_strStartupMsg);
	DDV_MaxChars(pDX, m_strStartupMsg, (MAX_STARTUPMSG_LINES * ( MAX_STARTUPMSG_LINELENGTH + 3)));
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditStartupMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditStartupMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditStartupMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTitle = "";
	strTitle += "Edit ";
	strTitle += AfxGetAppName();
	strTitle += " Startup Message";
	SetWindowText(strTitle);

	return TRUE;  
}

/**********************************************************************/

void CDefEditStartupMsgDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		CString strLine = "";

		CSSFile fileMessage;
		CFilenameUpdater FnUp(SysFiles::StartupMessage);
		if (fileMessage.Open(FnUp.GetFilenameToUse(), "wb") == TRUE)
		{
			int nLength = m_strStartupMsg.GetLength();
			int nLineCount = 0;

			for (int nPos = 0; (nPos < nLength) && (nLineCount < MAX_STARTUPMSG_LINES); nPos++)
			{
				if (nPos == nLength - 1)
				{
					if (strLine.GetLength() < MAX_STARTUPMSG_LINELENGTH)
					{
						strLine += m_strStartupMsg.GetAt(nPos);
					}
				}
				else
				{
					if (m_strStartupMsg.Mid(nPos, 2) == "\r\n")
					{
						fileMessage.WriteLine(strLine);
						nLineCount++;

						strLine = "";
						nPos++;
					}
					else if (strLine.GetLength() < MAX_STARTUPMSG_LINELENGTH)
					{
						strLine += m_strStartupMsg.GetAt(nPos);
					}
				}
			}
		}

		if (strLine != "")
		{
			fileMessage.WriteLine(strLine);
		}
	}

	EndDialog( IDOK );
}

/**********************************************************************/
