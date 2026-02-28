//*******************************************************************
#include "DatabaseImportForServer.h"
#include "MemberMojoImportForServer.h"
//*******************************************************************
#include "ServerWrapper.h"
#include "SPosLoyalty4Server.h"
//*******************************************************************
#define nIMPORT_MAXCOUNT 10	// 5 seconds - number of cycles to check for databse imports
//*******************************************************************

void CServerWrapper::CheckImports()
{
	// only check ever so many cycles
	if (++m_nImportCounter >= nIMPORT_MAXCOUNT)
	{
		CheckNormalImport();
		CheckQRInfoImport();
		m_nImportCounter = 0;
	}
}

//*******************************************************************

void CServerWrapper::CheckNormalImport()
{
	CString strFilename = Filenames.GetImportsPath("srvimport.csv");
	if (SolutionGlobalFunctions::FileExists(strFilename) == TRUE)
	{
		{
			CString strRenamedFile = Filenames.GetImportsPath("srvimport.$$1");
			if (SolutionGlobalFunctions::FileExists(strRenamedFile) == TRUE)						// see if already have a $$1 file waiting
			{
				if (::AppendDataFile(strFilename, strRenamedFile, TRUE) == TRUE)	// add to end of $$1 file
				{
					::SendToRecycleBin(strFilename);							// delte import file	 
				}
			}
			else
			{
				int nRenameResult = ::rename(strFilename, strRenamedFile);						// rename import file
			}

			CDatabaseImportForServer import(strRenamedFile);

			if (System.GetSrvImportUnknownFlag() == FALSE)
			{
				import.SetImportMethod(IMPORT_METHOD_EXISTONLY);			// only chnage if record exists
			}

			if (import.Execute() == TRUE)
			{
				::SendToRecycleBin(strRenamedFile);
			}

			m_nImportCounter = 0;
		}
	}
}

//*******************************************************************

void CServerWrapper::CheckQRInfoImport()
{
	CString strBaseFilename = "c:\\sposloyaltyimport\\importaccount_raw_mm";
	CString strRomanFilename = strBaseFilename + ".csv";

	CString strDateTime = "";
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	strDateTime.Format("_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.csv",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CString strRenamedFilename = strBaseFilename + strDateTime;

	if (SolutionGlobalFunctions::FileExists(strRomanFilename) == TRUE)
	{
		::SendToRecycleBin(strRenamedFilename);
		int nRenameResult = ::rename(strRomanFilename, strRenamedFilename);					
		
		CMemberMojoImportForServer import(strRenamedFilename);

		import.Execute();
		::SendToRecycleBin(strRenamedFilename);
	}
}

//*******************************************************************
