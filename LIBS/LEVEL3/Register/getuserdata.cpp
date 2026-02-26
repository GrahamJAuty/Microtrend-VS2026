/**********************************************************************/
/*
	source:	"Update"		// called from sysset3 in ecrman, stockman etc
	source:	"Inhouse"		// called from ecrset inhouse
	source:	"EndUser"		// called from ecrset enduser - not implemented yet
*/
/**********************************************************************/

CString CSSRegister::GetUserData ( const char* szParams )
{
	CCSV csvFields ( szParams );		// securitycode,source,fields
	CString strSecurityCode = csvFields.GetString(0);
	CString strProgName = "GetUserData (Unknown)";
	CString strError;
	CString strSerialNo;
	CString strLineNo;
	CString strSource;
	CCSV csvData;

	if ( csvFields.GetSize() >= 3 )
	{
		strSource = csvFields.GetString(1);

		if ( strSource == "Update" )			strProgName = "GetUserData (Online)";
		else if ( strSource == "Inhouse" )		strProgName = "GetUserData (Ecrset)";
		else if ( strSource == "EndUser" )		strProgName = "GetUserData (EndUser)";		// not implemented yet

		csvFields.RemoveAt(0,2);		// delete securitycode and source

		int nSerialNo = CSecurityCode::GetSerialNo ( strSecurityCode );	// nnnn nnnn nnnn nnnn
		if ( nSerialNo != -1 )
		{
			strSerialNo.Format ( "%6.6d", nSerialNo );

			CSSIntDatabase dbReg;
			if ( dbReg.OpenReadOnly ( m_strRegFilename, "SerialNo,ascending", 10 ) == DB_ERR_NONE )
			{
				if ( dbReg.FindRecord ( atoi(strSerialNo) ) == TRUE )
				{
					if ( dbReg.GetBool ( "RegDisable" ) == FALSE )
					{
						if ( strSource == "Inhouse" )
							strLineNo = dbReg.GetString ( "LineNo" );
						else
						{
							if ( dbReg.GetBool ( "Updated" ) == FALSE )
								strLineNo = dbReg.GetString ( "LineNo" );
							else
								strError = "Program has already been updated";
						}
					}
					else
						strError = "This record has been disabled";
				}
				else
					strError = "Cannot find record";

				dbReg.Close();
			}
			else
			{
				strError = "Cannot open database ";
				strError += m_strRegFilename;
			}
		}
		else
			strError = "Invalid security code";
	}
	else
		strError = "Invalid input";

	if ( strLineNo != "" )
	{
		CSSIntDatabase dbEcr;
		if ( dbEcr.OpenReadOnly ( m_strEcrsetFilename, "LineNo,ascending", 10 ) == DB_ERR_NONE )
		{
			if ( dbEcr.FindRecord ( atoi(strLineNo) ) == TRUE )
			{
				for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
					csvData.Add ( dbEcr.GetString(csvFields.GetString(i)) );

				csvData.Add ( "" );		// add no error message
			}
			else
				strError = "Cannot find record";

			dbEcr.Close();
		}
		else
		{
			strError = "Cannot open database ";
			strError += m_strEcrsetFilename;
		}
	}

	if ( strLineNo != "" && strError == "" && strSource != "Inhouse" )
	{
		CSSIntDatabase dbReg;
		if ( dbReg.OpenReadWrite ( m_strRegFilename, "SerialNo,ascending", 10 ) == DB_ERR_NONE )
		{
			if ( dbReg.FindRecord ( atoi(strSerialNo) ) == TRUE )
				dbReg.Set ( "Updated", TRUE );

			dbReg.Close();
		}
	}

	LogRegistration ( strSerialNo, strSecurityCode, strError, strProgName );

	CString strData = ( strError == "" ) ? csvData.GetLine() : strError;

	return strData;
}

/**********************************************************************/
