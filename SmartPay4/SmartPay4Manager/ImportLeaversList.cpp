//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DeleteRecord.h"
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "ImportRecordsDlg.h"
#include "MoveLeaversDlg.h"
//$$******************************************************************
#include "ImportLeaversList.h"
//$$******************************************************************

CImportLeaversList::CImportLeaversList ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_strTitle = "";
}

//$$******************************************************************

CImportLeaversList::~CImportLeaversList(void)
{
}

//$$******************************************************************

void CImportLeaversList::Import()
{
	CMoveLeaversDlg dlgMove(m_pParent);
	if (dlgMove.DoModal() == IDOK)
	{
		CString strLeaversYear = dlgMove.m_strLeaversYear;
		m_strTitle.Format("Process %s Leavers List", 
			(const char*) strLeaversYear);

		CString strImportFilename = Filenames.GetImportsPath("leavers.csv");

		CImportRecordsDlg dlgImport(m_strTitle, strImportFilename, m_pParent);
		dlgImport.m_bEnableSelective = FALSE;
		dlgImport.m_bDeleteFile = TRUE;

		if (dlgImport.DoModal() == IDOK)
		{
			int nCount = 0;
			CString strMsg = "";

			if ((nCount = ProcessFile(dlgImport.m_strImportFilename, strLeaversYear, dlgMove.m_strComment)) == -1)
			{
				strMsg = m_strError;								// may have some error message
			}
			else
			{
				strMsg.Format("Move finished - %d records have been moved to %s Leavers.", 
					nCount, 
					(const char*) strLeaversYear);

				if (dlgImport.m_bDeleteFile == TRUE)
				{
					CFileRecycler::SendToRecycleBin(dlgImport.m_strImportFilename);
				}
			}

			if (strMsg != "")
			{
				Prompter.Info(strMsg, m_strTitle);
			}
		}
	}
}

//$$******************************************************************

int CImportLeaversList::ProcessFile(const char* szImportFile, const char* szLeaversYear, const char* szAuditComment)
{
	int nCount = -1;

	// number of data lines - ignore header line in count
	int nLineCount = ::GetFileLineCount(szImportFile);
	if (nLineCount < 2)
	{
		m_strError = "Insufficent data lines found in import file!";
		return nCount;
	}
	--nLineCount;

	// read header line from import file **********************************

	CSSFile fileIn{};
	if (fileIn.Open(szImportFile, "rb") == FALSE)
	{
		m_strError.Format("Unable to open import file %s!", szImportFile);
		return nCount;
	}

	// read header line from import file **********************************

	CCSV csvImportHeader{};
	if (fileIn.Read(csvImportHeader) == FALSE)					// read header line 
	{
		fileIn.Close();
		m_strError = "Unable to read the header line in the csv import file!";
		return nCount;
	}

	// scan header line

	int nIndexCardNo = -1;

	for (int n = 0; n < csvImportHeader.GetSize(); n++)
	{
		CString strField = csvImportHeader.GetString(n);

		if (strField == Account::UserID.Label)
		{
			nIndexCardNo = n;
		}

		else if (strField == "CardNo" && nIndexCardNo == -1)	// allow a field header of CardNo
		{
			nIndexCardNo = n;
		}
	}

	// ** see if have CardNo field in header.

	if (nIndexCardNo == -1)										// must have a CardNo field
	{
		fileIn.Close();
		m_strError.Format("Invalid Format\n\nThe import file contains %d lines of data.\n\n", nLineCount);	// ignore header line in count
		m_strError += "However there is no UserID field in the file header line!";
		return nCount;
	}

	CString strMsg = "";
	strMsg.Format("The number of lines in the leavers list = %d.\n\nDo you wish to continue?", nLineCount);

	MessageBeep(MB_ICONQUESTION);
	if (Prompter.YesNo(strMsg, m_strTitle) == IDNO)
	{
		fileIn.Close();
		return nCount;
	}

	CWorkingDlg dlgWorking("Processing Leavers List", TRUE);
	dlgWorking.Create();

	CSQLAuditRecord atc;
	CLeaversHandler dataLeavers(szLeaversYear);						// create leavers folder\files if required

	nCount = 0;

	CCSV csvLine;
	while (fileIn.Read(csvLine) == TRUE)
	{
		if (dlgWorking.SetCaption2PercentDone(fileIn.GetPercentPosition(), TRUE) == TRUE)
		{
			break;
		}

		CString strCardNo = csvLine.GetString(nIndexCardNo);

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strCardNo);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE) //FIDDLE ACCOUNTINUSE?
		{
			atc.Set(&RowAccount);										// set audit is for a pc line
			atc.SetApplicationNo(APPNO_IMPORTLEAVER);
			atc.SetOperatorID(Passwords.GetPasswordInUse());
			atc.SetLineComment(szAuditComment);
			atc.SaveNewLine();												// save new audit line
			++nCount;

			atc.SaveSessionLine(dataLeavers.m_strAuditFilename);			// save same audit line to leaver folder ( doesn't increment CCNO's etc )

			CDeleteRecord deleteRecord(RowAccount, szAuditComment, szLeaversYear, m_pParent);
		}
	}

	fileIn.Close();

	return nCount;
}

//$$******************************************************************
