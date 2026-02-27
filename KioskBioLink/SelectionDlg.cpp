//*******************************************************************
#include "ImageHandler.h"
#include "SystemData.h"
//*******************************************************************
#include "SelectionDlg.h"
//*******************************************************************

CSelectionDlg::CSelectionDlg(CStringArray* parrayUserLines, CUserIDRecord* pRecord, CWnd* pParent)
	: CSSColourDialog(CSelectionDlg::IDD, pParent)
	, m_parrayUserLines(parrayUserLines)
	, m_pRecord(pRecord)
{
}

//*******************************************************************

void CSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSelectionDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS,OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE1, OnButtonImage1)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE2, OnButtonImage2)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSelectionDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "SelectBackground");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}

	bool bRounded = (strFilename == "");

	CSSColourDialog::OnInitDialog(BLACK, bRounded);
	CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonImage1.Init(IDC_BUTTON_IMAGE1, this, WHITE, MYCOLOUR_BUTTON, "", 20);
	m_buttonImage2.Init(IDC_BUTTON_IMAGE2, this, WHITE, MYCOLOUR_BUTTON, "", 20);

	m_buttonPrevious.Init(IDC_BUTTON_PREVIOUS, this, WHITE, RED, "", 20);
	m_buttonNext.Init(IDC_BUTTON_NEXT, this, WHITE, RED, "", 20);
	m_buttonFinish.Init(IDCANCEL, this, BLACK, WHITE, "", 20);

	m_staticTitle.Init(IDC_STATIC_TITLE, this, WHITE, BLACK, "");

	m_staticCount1.Init(IDC_STATIC_COUNT1, this, WHITE, BLACK, "");
	m_staticSurname1.Init(IDC_STATIC_SURNAME1, this, WHITE, BLACK, "");
	m_staticForename1.Init(IDC_STATIC_FORENAME1, this, WHITE, BLACK, "");
	m_staticCardNo1.Init(IDC_STATIC_CARDNO1, this, WHITE, BLACK, "");
	m_staticTextGroupNo1.Init(IDC_STATIC_TEXTGROUPNO1, this, WHITE, BLACK, "");
	m_staticGroupNo1.Init(IDC_STATIC_GROUPNO1, this, WHITE, BLACK, "");

	m_staticCount2.Init(IDC_STATIC_COUNT2, this, WHITE, BLACK, "");
	m_staticSurname2.Init(IDC_STATIC_SURNAME2, this, WHITE, BLACK, "");
	m_staticForename2.Init(IDC_STATIC_FORENAME2, this, WHITE, BLACK, "");
	m_staticCardNo2.Init(IDC_STATIC_CARDNO2, this, WHITE, BLACK, "");
	m_staticTextGroupNo2.Init(IDC_STATIC_TEXTGROUPNO2, this, WHITE, BLACK, "");
	m_staticGroupNo2.Init(IDC_STATIC_GROUPNO2, this, WHITE, BLACK, "");

	DisplayScreen(0);

	return TRUE;
}

//*******************************************************************

void CSelectionDlg::DisplayScreen(int nScreenNo)
{
	int nMaxCount = m_parrayUserLines->GetCount();

	m_nIndexPtrButton1 = nScreenNo * 2;
	m_nIndexPtrButton2 = m_nIndexPtrButton1 + 1;

	bool bPreviousAllowed = (nScreenNo != 0);

	bool bNextAllowed = (1 + m_nIndexPtrButton2 < m_parrayUserLines->GetCount());
	int nShowImage2 = (1 + m_nIndexPtrButton1 >= m_parrayUserLines->GetCount()) ? SW_HIDE : SW_SHOW;

	m_pRecord->ExtractUserData(m_parrayUserLines->GetAt(m_nIndexPtrButton1));

	m_buttonImage1.SetImage(m_pRecord->m_strImage, TRANSPARENT_COLOUR);
	SetImageButtonText(m_buttonImage1);

	m_staticCardNo1.SetWindowText(m_pRecord->GetUserID());
	m_staticGroupNo1.SetWindowText(m_pRecord->GetGroupNoString());
	m_staticForename1.SetWindowText(m_pRecord->m_strForename);
	m_staticSurname1.SetWindowText(m_pRecord->m_strSurname);

	CString strMsg = "";
	strMsg.Format("%d of %d", 1 + m_nIndexPtrButton1, nMaxCount);
	m_staticCount1.SetWindowText(strMsg);

	if (SW_SHOW == nShowImage2)
	{
		m_pRecord->ExtractUserData(m_parrayUserLines->GetAt(m_nIndexPtrButton2));

		m_buttonImage2.SetImage(m_pRecord->m_strImage, TRANSPARENT_COLOUR);
		SetImageButtonText(m_buttonImage2);

		m_staticCardNo2.SetWindowText(m_pRecord->GetUserID());
		m_staticGroupNo2.SetWindowText(m_pRecord->GetGroupNoString());
		m_staticForename2.SetWindowText(m_pRecord->m_strForename);
		m_staticSurname2.SetWindowText(m_pRecord->m_strSurname);

		strMsg.Format("%d of %d", 1 + m_nIndexPtrButton2, nMaxCount);
		m_staticCount2.SetWindowText(strMsg);
	}

	m_buttonImage2.ShowWindow(nShowImage2);
	m_staticCardNo2.ShowWindow(nShowImage2);
	m_staticTextGroupNo2.ShowWindow(nShowImage2);
	m_staticGroupNo2.ShowWindow(nShowImage2);
	m_staticForename2.ShowWindow(nShowImage2);
	m_staticSurname2.ShowWindow(nShowImage2);
	m_staticCount2.ShowWindow(nShowImage2);

	m_buttonPrevious.EnableWindow(bPreviousAllowed);
	m_buttonNext.EnableWindow(bNextAllowed);
}

//*******************************************************************

void CSelectionDlg::SetImageButtonText(CSSColourButton& button)
{
	CString strText = m_pRecord->GetError();

	if (strText == "")
	{
		if (m_pRecord->m_strImage == "")
		{
			strText = "No image";
		}
	}

	button.SetWindowText(strText);
}

//*******************************************************************
	
void CSelectionDlg::OnButtonPrevious()
{
	if (m_nScreenNo > 0)
	{
		DisplayScreen(--m_nScreenNo);
		Invalidate();
	}
}

//*******************************************************************

void CSelectionDlg::OnButtonNext()
{
	DisplayScreen(++m_nScreenNo);
	Invalidate();
}

//*******************************************************************

void CSelectionDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CSelectionDlg::OnButtonImage1()
{
	if (m_pRecord->ExtractUserData(m_parrayUserLines->GetAt(m_nIndexPtrButton1)) == TRUE)
	{
		EndDialog(IDOK);
	}
}

//*******************************************************************

void CSelectionDlg::OnButtonImage2()
{
	if (m_pRecord->ExtractUserData(m_parrayUserLines->GetAt(m_nIndexPtrButton2)) == TRUE)
	{
		EndDialog(IDOK);
	}
}

//*******************************************************************