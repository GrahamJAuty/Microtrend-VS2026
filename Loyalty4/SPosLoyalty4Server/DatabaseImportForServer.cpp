//******************************************************************************************
// Import database records 
//******************************************************************************************
// CAUTION: Any fields change in here MUST also be added to DatabaseImport in LoyaltyManager 
//******************************************************************************************
#include "resource.h"
//******************************************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTranImportDatabase.h"
//******************************************************************************************
#include "SPosLoyalty4Server.h"
//******************************************************************************************
#include "DatabaseImportForServer.h"
//**********************************************************************

CDatabaseImportForServer::CDatabaseImportForServer( const char* szFilename )
{
	m_strFilename = szFilename;
	m_nImportMethod = IMPORT_METHOD_ALL;
	m_nIndexCardNo = -1;
	m_nRead = 0;
	m_nAdded = 0;
	m_nInvalid = 0;
}

/**********************************************************************/

bool CDatabaseImportForServer::IsValidFile()
{
	if (::GetFileLineCount(m_strFilename) < 2)
	{
		return FALSE;
	}

	CSSFile file1;
	if (file1.Open(m_strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	bool bReply = file1.Read ( csvHeader );					// read header line 
	file1.Close();
	
	if (bReply == FALSE)
	{
		return FALSE;
	}

// scan header line

	for ( int n = 0 ; n < csvHeader.GetSize() ; n++ )
	{
		CString strField = csvHeader.GetString(n);
		
		if (LoyaltyHeaderHelpers.GetNumericField(strField) == LOYALTYFIELD_USERID)
		{
			m_nIndexCardNo = n;
			break;
		}
	}

	return ( m_nIndexCardNo == -1 ) ? FALSE : TRUE;				// ** see if have CardNo field in header.
}

//**********************************************************************
// Database open before being called \ then closed afterwards

bool CDatabaseImportForServer::Execute()
{
	bool bReply = FALSE;

	if (IsValidFile() == TRUE)
	{
		CSSFile file;
		if (file.Open(m_strFilename, "rb") == TRUE)
		{
			CCSV csvHeader;
			if ((bReply = file.Read(csvHeader)) == TRUE)					// read header line 
			{
				CArray<int, int> arrayHeader;
				LoyaltyHeaderHelpers.CreateNumericHeader(csvHeader, arrayHeader);

				CSQLTranImportDatabase Tran;
				Tran.BeginTrans();
				Tran.DoWorkServerImport(m_nImportMethod, file, arrayHeader, m_nIndexCardNo);

				switch (Tran.EndTrans())
				{
				case SQLTRAN_STATE_COMMITOK:
					Tran.AfterTransaction();
					break;

				default:
					break;
				}
			}
			file.Close();
		}
	}
	return bReply;
}

//*******************************************************************