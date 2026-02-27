//*****************************************************************************
#include "..\SmartPay4Shared\SNoDatabase.h"
//*****************************************************************************
#include "CommentHandler.h"
#include "TerminalSetsDlg.h"
//*****************************************************************************

CTerminalSetsDlg::CTerminalSetsDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CTerminalSetsDlg::IDD, pParent)
{
	m_strText1 = "";
	m_strList1 = "";
	m_strText2 = "";
	m_strList2 = "";
	m_strText3 = "";
	m_strList3 = "";
	m_strText4 = "";
	m_strList4 = "";

	CSNoDatabase db;
	db.OpenForSharing();																	// file0044 in ReadOnly mode 
	
	db.FindRecord ( 1 );
	m_strText1 = db.GetName();
	m_strList1 = db.GetTList();

	db.FindRecord ( 2 );
	m_strText2 = db.GetName();
	m_strList2 = db.GetTList();

	db.FindRecord ( 3 );
	m_strText3 = db.GetName();
	m_strList3 = db.GetTList();

	db.FindRecord ( 4 );
	m_strText4 = db.GetName();
	m_strList4 = db.GetTList();

	db.Close();
}

//*****************************************************************************

void CTerminalSetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LIST2, m_editList2);
	DDX_Control(pDX, IDC_EDIT_LIST3, m_editList3);
	DDX_Control(pDX, IDC_EDIT_LIST4, m_editList4);
	DDX_Text(pDX, IDC_EDIT_TEXT1, m_strText1);
	DDX_Text(pDX, IDC_EDIT_LIST1, m_strList1);
	DDX_Text(pDX, IDC_EDIT_TEXT2, m_strText2);
	DDX_Text(pDX, IDC_EDIT_LIST2, m_strList2);
	DDX_Text(pDX, IDC_EDIT_TEXT3, m_strText3);
	DDX_Text(pDX, IDC_EDIT_LIST3, m_strList3);
	DDX_Text(pDX, IDC_EDIT_TEXT4, m_strText4);
	DDX_Text(pDX, IDC_EDIT_LIST4, m_strList4);
}

//*****************************************************************************

BEGIN_MESSAGE_MAP(CTerminalSetsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//*****************************************************************************

CString CTerminalSetsDlg::ValidateLists()
{
	CString strError = "";

	CTNoDatabase db;
	int nRecordCount = 0;

	CString strTmpFilename = Filenames.GetTerminalGroupSetTempFilename();
	CFileRecycler::SendToRecycleBin(strTmpFilename);

	if (db.OpenForSharing(strTmpFilename, DB_READWRITE) == DB_ERR_NONE)
	{
		ValidateTerminals(&db, 1, m_strList1);

		if ((strError = ValidateTerminals(&db, 2, m_strList2)) == "")
		{
			if ((strError = ValidateTerminals(&db, 3, m_strList3)) == "")
			{
				if ((strError = ValidateTerminals(&db, 4, m_strList4)) != "")
				{
					m_editList4.SetFocus();
				}
			}
			else
			{
				m_editList3.SetFocus();
			}
		}
		else
		{
			m_editList2.SetFocus();
		}

		nRecordCount = db.GetRecordCount();							// see how many entries

		db.Close();
	}

	if (strError == "")
	{
		CString strFilename = Filenames.GetTerminalGroupSetFilename();
		CFileRecycler::SendToRecycleBin(strFilename);

		if (nRecordCount > 0)
		{
			::rename(strTmpFilename, strFilename);
		}
	}

	return strError;
}

//*****************************************************************************

CString CTerminalSetsDlg::ValidateTerminals ( CTNoDatabase* db, int nSetNo, const char* szList )
{
	CString strError = "";

	CUIntArray arrayTNo;

	CCommentHandler TNoList;
	TNoList.ExtractTNos ( szList, &arrayTNo );	// extract terminal numbers

	for ( int i = 0 ;  i < arrayTNo.GetSize() ; i ++ )
	{
		int nTNo = arrayTNo.GetAt(i);
		if ( db->FindRecord ( nTNo ) == TRUE )
		{
			strError.Format ( "Terminal %d has been assigned to set %d!", nTNo, db->GetSetNo() );
			break; 
		}
		else
		{
			db->AddBlankRecord ( nTNo );
			db->SetSetNo ( nSetNo );
		}
	}

	return strError;
}

//*****************************************************************************

void CTerminalSetsDlg::OnButtonClear()
{
	m_strText1 = "";
	m_strList1 = "";
	m_strText2 = "";
	m_strList2 = "";
	m_strText3 = "";
	m_strList3 = "";
	m_strText4 = "";
	m_strList4 = "";
	UpdateData(FALSE);
}

//*****************************************************************************

void CTerminalSetsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strError = ValidateLists();
		if (strError == "")
		{
			CSNoDatabase db;
			if (db.OpenForSharing("", DB_READWRITE) == DB_ERR_NONE)
			{
				if (db.FindRecord(1) == FALSE)
				{
					db.AddBlankRecord(1);
				}

				db.SetName(m_strText1);
				db.SetTList(m_strList1);

				if (db.FindRecord(2) == FALSE)
				{
					db.AddBlankRecord(2);
				}

				db.SetName(m_strText2);
				db.SetTList(m_strList2);

				if (db.FindRecord(3) == FALSE)
				{
					db.AddBlankRecord(3);
				}

				db.SetName(m_strText3);
				db.SetTList(m_strList3);

				if (db.FindRecord(4) == FALSE)
				{
					db.AddBlankRecord(4);
				}

				db.SetName(m_strText4);
				db.SetTList(m_strList4);

				db.Close();
			}
			else
			{
				strError = db.GetError();
			}
		}

		if (strError == "")
		{
			EndDialog(IDOK);
		}
		else
		{
			Prompter.Error(strError, "Entry Error");
		}
	}
}

//**********************************************************************

