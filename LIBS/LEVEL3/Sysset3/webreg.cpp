/**********************************************************************/

#include "local.h"
#include "fields.h"
#include "SyssetDlg3.h"

/**********************************************************************/

static bool GetStaticIPAddress ( CString& strResponse )
{
	bool bReply = FALSE;
	strResponse = "Static IP error";

	CInternetSession session;
	CHttpConnection* pConnection = NULL;
	CHttpFile* pFile1 = NULL;
	try
	{
		pConnection = session.GetHttpConnection ( "www.microtrend.co.uk", (INTERNET_PORT)80);	

		CString strRequest = "";
		pFile1 = pConnection->OpenRequest ( 1, "/setup/info.htm" );
		pFile1->SendRequest();

		CString strBuf;
		while ( pFile1->ReadString ( strBuf ) == TRUE )
		{
			if ( strBuf.Left(16) == "StaticIPAddress=")
			{
				strResponse = strBuf.Mid(16);
				bReply = TRUE;
				break;
			}
		}
	}
	catch (CInternetException* e)
	{
		strResponse.Format("Static IP error: %d", e->m_dwError);
		e->Delete();
	}

	if ( pFile1 != NULL )
	{
		pFile1->Close();
		delete pFile1;
	}
	if ( pConnection != NULL )
	{
		pConnection->Close();
		delete pConnection;
	}

	return bReply;
}

/**********************************************************************/

static CString WebReg ( const char* szIPAddress, const char* szFunction, const char* szInput, CString& strOutput )
{
	CString strIPAddress = szIPAddress;

	if (strIPAddress == "")	// use static ip address
	{
		if (GetStaticIPAddress(strIPAddress) == FALSE)
		{
			return strIPAddress;	// error message
		}
	}

	CString strError = "WebReg error";

	CInternetSession session;
	CHttpConnection* pConnection = NULL;
	CHttpFile* pFile1 = NULL;
	try
	{
		pConnection = session.GetHttpConnection ( strIPAddress, (INTERNET_PORT)80 );

		CString strRequest;
		strRequest.Format ( "/webreg.asp?inputFunction=%s&inputParams=%s", szFunction, szInput);	// "/webreg.asp?inputFunction=%s&inputParams=%s"
		pFile1 = pConnection->OpenRequest ( 1, strRequest );
		pFile1->SendRequest();

		CString strBuf = "";
		while ( pFile1->ReadString ( strBuf ) == TRUE )
		{
			if ((strBuf.Left(12) == "<WebRegData>") && (strBuf.Right(13) == "</WebRegData>")) 	// "<WebRegData>" "</WebRegData>"
			{
				strOutput = strBuf.Mid(12, strBuf.GetLength() - 25);

				CCSV csv(strOutput);
				strError = csv.GetString(csv.GetSize() - 1);	// last param is error message
				csv.RemoveAt(csv.GetSize() - 1);			// discard error message
				strOutput = csv.GetLine();
				break;
			}
		}
	}
	catch ( CInternetException* e )
	{
		strError.Format ( "WebReg error: %d", e->m_dwError );
		e->Delete();
	}

	if ( pFile1 != NULL )
	{
		pFile1->Close();
		delete pFile1;
	}
	if ( pConnection != NULL )
	{
		pConnection->Close();
		delete pConnection;
	}
	return strError;
}

/**********************************************************************/

bool CSysset::RegisterOnline(const char* szAccessCode, CString& strUserCode)
{
	CString strError = "";
	CString strFunction = "GetUserCode";

	CSyssetDlg3 dlg(CMessageTexts::InternetWait());

	CString strIPAddress = m_UserData.GetString(szServerIP, "");

	if ((strError = WebReg(strIPAddress, strFunction, szAccessCode, strUserCode)) == "")	// no error
	{
		return TRUE;
	}

	strUserCode = strError;
	return FALSE;
}

/**********************************************************************/

CString CSysset::UpdateOnline ( const char* szSecurityCode, const char* szFilename )
{
	CCSV csv;
	csv.Add ( szSerialNo );
	csv.Add ( szUserName );
	csv.Add ( szAdd1 );
	csv.Add ( szAdd2 );
	csv.Add ( szAdd3 );
	csv.Add ( szAdd4 );
	csv.Add ( szAdd5 );
	csv.Add ( szDealer );
	csv.Add ( szExpiry );
	csv.Add ( szMaxSites );
	csv.Add ( szCustNo );
	csv.Add ( szScheduler );
	csv.Add ( szSmartcard );
	csv.Add ( szCardFormat );
	csv.Add ( szMealDeal );
	csv.Add ( szGcrReport );
	csv.Add ( szBarcode );
	csv.Add ( szPhotoID );
	csv.Add ( szExportStock );
	csv.Add ( szMaxDatabases );
	csv.Add ( szMaxStockPoints );
	csv.Add ( szMaxRemoteSites );
	csv.Add ( szSystemType );
	csv.Add ( szMaxEcrModels );
	csv.Add ( szMaxUsers );
	csv.Add ( szMaxSessions );
	csv.Add ( szProgramNo );
	csv.Add ( szTLogConvert );
	csv.Add ( szPMS );
	csv.Add ( szExternalLink );

	CString strIPAddress = m_UserData.GetString ( szServerIP, "" );
	CString strError = GetUserData(strIPAddress, szSecurityCode, "Update", csv.GetLine(), szFilename);
	return strError;
}

/**********************************************************************/
/*														*/
/*	szSecurityCode: code generated from serial number				*/
/*	szSource: Inhouse or Update								*/
/*	szFields: the field names to get data for					*/
/*														*/
/**********************************************************************/

CString CSysset::GetUserData(const char* szIPAddress, const char* szSecurityCode, const char* szSource, const char* szFields, const char* szIniFilename)
{
	CString strError = "";

	CString strInput = "";
	strInput.Format("%s,%s,%s", szSecurityCode, szSource, szFields);

	CString strOutput = "";
	CString strFunction = "GetUserData";

	CSyssetDlg3 dlg(CMessageTexts::InternetWait());

	if ((strError = WebReg(szIPAddress, strFunction, strInput, strOutput)) == "")	// no error
	{
		CCSV csvData(strOutput);
		CCSV csvFields(szFields);

		CSSIniFile file(SS_INI_HEX);

		for (int i = 0; i < csvFields.GetSize(); i++)
		{
			file.Set(csvFields.GetString(i), csvData.GetString(i));
		}

		file.Write(szIniFilename);
	}

	return strError;
}

/**********************************************************************/
