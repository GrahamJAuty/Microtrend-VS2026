//******************************************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//******************************************************************************************
#include "SPosLoyalty4Server.h"
#include "SQLTranMemberMojoImport.h"
//******************************************************************************************
#include "MemberMojoImportForServer.h"
//******************************************************************************************

CMemberMojoImportForServer::CMemberMojoImportForServer( CString strFilename )
{
	m_strFilename = strFilename;
	ResetFieldIdx();
}

//******************************************************************************************

void CMemberMojoImportForServer::ResetFieldIdx()
{
	m_nFieldIdxUserID = -1;
	m_nFieldIdxQRInfo = -1;
	m_arrayHeader.RemoveAll();
}

//******************************************************************************************

void CMemberMojoImportForServer::Execute()
{
	CSSFile fileImport;
	if (fileImport.Open(m_strFilename, "rb") == FALSE)
	{
		LogError("Unable to open import file.");
		return;
	}

	CString strHeader = "";
	if (ReadFileLine(fileImport, strHeader) == FALSE)
	{
		LogError("Unable to read import file header.");
		return;
	}

	if (ProcessHeaderLine(strHeader) == FALSE)
	{
		LogError("Duplicate fields found in header.");
		return;
	}

	if ((-1 == m_nFieldIdxUserID) && (-1 == m_nFieldIdxQRInfo))
	{
		LogError("No UserID or QRInfo found in import file.");
		return;
	}

	if (GotDuplicateID(fileImport) == TRUE)
	{
		LogError("Duplicate ID found in import file.");
		return;
	}

	CString strCheckHeader = "";
	fileImport.Seek(0, SEEK_SET);
	ReadFileLine(fileImport, strCheckHeader);
	if (strCheckHeader != strHeader)
	{
		LogError("Header changed in import file.");
		return;
	}

	ProcessImportLines(fileImport);
}

//******************************************************************************************

bool CMemberMojoImportForServer::ProcessHeaderLine(CString strHeader)
{
	ResetFieldIdx();

	strHeader.MakeUpper();
	CCSV csvHeader(strHeader);
	LoyaltyHeaderHelpers.CreateNumericHeaderMemberMojo(csvHeader, m_arrayHeader);

	CReportConsolidationArray<CSortedIntItem> arrayFields;

	for (int n = 0; n < m_arrayHeader.GetSize(); n++)
	{
		int nField = m_arrayHeader.GetAt(n);

		if ( (nField != LOYALTYFIELD_ADDRESS5) && ( nField != -1 ) )
		{
			CSortedIntItem item;
			item.m_nItem = nField;
			int nPos = 0;
			if (arrayFields.Find(item, nPos) == TRUE)
			{
				return FALSE;
			}
			else
			{
				arrayFields.InsertAt(nPos, item);
			}
		}

		if (LOYALTYFIELD_USERID == nField)
		{
			m_nFieldIdxUserID = n;
		}
		else if (LOYALTYFIELD_QRINFO == nField)
		{
			m_nFieldIdxQRInfo = n;
		}
	}

	return TRUE;
}

//******************************************************************************************

void CMemberMojoImportForServer::ProcessImportLines(CSSFile& file)
{
	__int64 nStartFromUserID = 1;

	CSQLTranMemberMojoImport ImportTran;
	ImportTran.BeginTrans();
	if (ImportTran.GetState() != SQLTRAN_STATE_ACTIVE)
	{
		LogError("Unable to create SQL transaction.");
		return;
	}

	if (ImportTran.SetAllAccountsInactive() == FALSE )
	{
		ImportTran.EndTrans();
		LogError("Unable to set all accounts to inactive.");
		return;
	}

	CString strLine = "";
	int nAdded = 0;
	int nModified = 0;
	int nIgnored = 0;

	while (ReadFileLine(file, strLine) == TRUE)
	{
		CCSV csvImport(strLine);

		__int64 nUserID = 0;
		CString strQRInfo = "";
		if (m_nFieldIdxQRInfo != -1)
		{
			strQRInfo = csvImport.GetString(m_nFieldIdxQRInfo);
		}

		if (m_nFieldIdxUserID != -1)
		{
			nUserID = csvImport.GetInt64(m_nFieldIdxUserID);
		}
		else
		{
			nUserID = ImportTran.GetUserIDForQRInfo(strQRInfo, nStartFromUserID);
		}

		if (0 == nUserID)
		{
			nIgnored++;
			continue;
		}

		CSQLRowAccountFull RowAccount;
		if (ImportTran.GetAccountRecord(nUserID, RowAccount, nAdded, nModified) == FALSE)
		{
			LogError("Unable to find or create account record.");
			ImportTran.EndTrans();
			return;
		}

		bool bGotName = FALSE;
		for (int n = 0; n < m_arrayHeader.GetSize(); n++)
		{
			switch (m_arrayHeader.GetAt(n))
			{
			case LOYALTYFIELD_QRINFO:
				RowAccount.SetQRInfo(strQRInfo);
				break;

			case LOYALTYFIELD_FORENAME:
				RowAccount.SetForename(csvImport.GetString(n));
				bGotName = TRUE;
				break;

			case LOYALTYFIELD_SURNAME:
				RowAccount.SetSurname(csvImport.GetString(n));
				bGotName = TRUE;
				break;

			case LOYALTYFIELD_GROUPNO:
				RowAccount.SetGroupNo(csvImport.GetInt(n));
				break;

			case LOYALTYFIELD_ADDRESS1:
				RowAccount.SetAddress1(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_ADDRESS2:
				RowAccount.SetAddress2(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_ADDRESS3:
				RowAccount.SetAddress3(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_ADDRESS4:
				RowAccount.SetAddress4(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_ADDRESS5:
				RowAccount.SetAddress5(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_PHONE1:
				RowAccount.SetPhone1(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_PHONE2:
				RowAccount.SetPhone2(csvImport.GetString(n));
				break;

			case LOYALTYFIELD_EXPIRYDATE:
				{
					CString strDate = csvImport.GetString(n);
					if (strDate == "")
					{
						RowAccount.SetExpiryDate("");
					}
					else if (strDate.GetLength() == 10)
					{
						RowAccount.SetExpiryDate(strDate);
					}
				}
				break;
			}
		}
			
		if (TRUE == bGotName)
		{
			CString strFullName = "";
			strFullName += RowAccount.GetForename();
			strFullName.TrimRight(' ');
			
			if (strFullName != "")
			{
				strFullName += " ";
			}
			
			strFullName += RowAccount.GetSurname();
			RowAccount.SetFullname(strFullName);
		}

		RowAccount.SetInactive(FALSE);

		if (ImportTran.UpdateAccountRecord(RowAccount) == FALSE)
		{
			LogError("Unable to update account record.");
			ImportTran.EndTrans();
			return;
		}
	}

	ImportTran.SetCanCommitFlag();
	
	switch (ImportTran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		ImportTran.AfterTransaction();

		{
			CString strMsg = "";
			strMsg.Format("OK (%d:%d:%d)", nAdded, nModified, nIgnored);
			LogError(strMsg, TRUE);
		}
		
		return;

	default:
		LogError("Unable to commit SQL transaction.");
		return;
	}
}

//******************************************************************************************

bool CMemberMojoImportForServer::GotDuplicateID(CSSFile& file)
{
	CReportConsolidationArray<CSortedInt64Item> arrayUserID;
	CReportConsolidationArray<CSortedStringItem> arrayQRInfo;

	CString strLine = "";
	
	while (ReadFileLine(file, strLine) == TRUE)
	{
		CCSV csv(strLine);

		if (m_nFieldIdxUserID != -1)
		{
			CSortedInt64Item item;
			item.m_nItem = csv.GetInt64(m_nFieldIdxUserID);

			if (item.m_nItem != 0)
			{
				int nPos = 0;
				if (arrayUserID.Find(item, nPos) == TRUE)
				{
					return TRUE;
				}

				arrayUserID.InsertAt(nPos, item);
			}
		}

		if (m_nFieldIdxQRInfo != -1)
		{
			CSortedStringItem item;
			item.m_strItem = csv.GetString(m_nFieldIdxQRInfo);

			if (item.m_strItem != "")
			{
				int nPos = 0;
				if (arrayQRInfo.Find(item, nPos) == TRUE)
				{
					return TRUE;
				}

				arrayQRInfo.InsertAt(nPos, item);
			}
		}
	}

	return FALSE;
}

//******************************************************************************************

bool CMemberMojoImportForServer::ReadFileLine(CSSFile& file,  CString& strResult)
{
	if ( file.ReadLine(strResult) == FALSE )
	{
		return FALSE;
	}

	strResult.TrimRight("\n");
	return TRUE;
}

//******************************************************************************************

void CMemberMojoImportForServer::LogError(CString strError, bool bForce)
{
	CString strMessage = "";
	strMessage += "Member Mojo Import: ";
	strMessage += strError;
	MessageLogger.LogMemberMojoImportMessage(strMessage, bForce);
}

//******************************************************************************************
