/**********************************************************************/

#include "Register.h"

/**********************************************************************/
// 4471757534625673 7639
void Register ( bool bPostBack, CCSV& csvUserParams, CAspArray& arObjects, CCSV& csvSessionCookies )
{
	if ( bPostBack == TRUE )
		return;

	bool bReply = FALSE;

	CString strUserCode;
	CString strUserInput;

	if ( GetParamValue ( csvUserParams, "AccessCode", strUserInput ) == TRUE )
	{
		CString strAccessCode;
		for ( int i = 0 ; i < strUserInput.GetLength() ; i++ )
		{
			char c = strUserInput.GetAt(i);
			if ( c >= '0' && c <= '9' )		// only allow digits
				strAccessCode += c;
		}

		if ( strAccessCode.GetLength() == 16 )
		{
			CString strBuf;

			CSSRegister reg;
			reg.RegisterSoftware ( "Website", strAccessCode, strBuf, TRUE, TRUE );

			if ( strBuf.SpanIncluding ( " 0123456789" ) == strBuf )	// if response consists of only digits and spaces, must be usercode (not error message)
			{
				strUserInput.Format ( "%s %s %s %s", strAccessCode.Mid(0,4), strAccessCode.Mid(4,4), strAccessCode.Mid(8,4), strAccessCode.Mid(12,4) );

				strUserCode.Format ( "Your User Code is: %s", strBuf );
				bReply = TRUE;
			}
			else
				strUserCode = strBuf;		// error message
		}

		if ( strUserCode == "" )
			strUserCode = "Invalid Access Code";
	}

	CString strCssClass = ( bReply == TRUE ) ? "ok" : "error";

	CAspLabel* p;
	if (( p = (CAspLabel*) arObjects.FindID ( "AccessCode" )) !=NULL )
		p->SetText ( strUserInput );

	if (( p = (CAspLabel*) arObjects.FindID ( "UserCode" )) !=NULL )
	{
		p->SetCssClass ( strCssClass );
		p->SetText ( strUserCode );
	}
}

/**********************************************************************/
