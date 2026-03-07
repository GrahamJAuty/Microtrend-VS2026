/**********************************************************************/
/*
	csvParams:

	eg. inputFunction=GetUserCode&inputParams=4471+7575+3462+5673

	eg. inputFunction=GetUserData&inputParams=2516+7960+0735+3925,Inhouse,UserName,Add1,Add2
*/
/**********************************************************************/

void WebReg ( CCSV& csvUserParams, CString& strDest )
{
	CString strData;

	if ( csvUserParams.GetSize() >= 2 )	// need at least 2 parameters
	{
		CString strFunction;
		GetParamValue ( csvUserParams, "inputFunction", strFunction );

		CString strParams;
		GetParamValue ( csvUserParams, "inputParams", strParams );

		CSSRegister reg;
 
		if ( strFunction == "GetUserCode" )
		{
			CCSV csv ( strParams );

			CString strAccessCode = csv.GetString(0);
			CString strProgramName = ( csv.GetInt(1) == 0 ) ? "Online" : "Inhouse";

			reg.RegisterSoftware ( strProgramName, strAccessCode, strData, TRUE, TRUE );

			if ( strData.SpanIncluding ( " 0123456789" ) == strData )	// if response consists of only digits and spaces, must be usercode (not error message)
				strData += ",";		// add no error message
		}
		else if ( strFunction == "GetUserData" )
		{
			strData = reg.GetUserData ( strParams );	// GetUserData assumes security code contains spaces
		}
		else
			strData = "Invalid function";
	}

	CString str;
	str.Format ( "<WebRegData>%s</WebRegData>", strData );
	strDest = str;
}

/**********************************************************************/
