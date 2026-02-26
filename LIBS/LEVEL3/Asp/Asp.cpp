/**********************************************************************/

CAspArray::~CAspArray()
{
	RemoveAll();
}

CAsp* CAspArray::FindID ( const char* szID )
{
	for ( int i = 0 ; i < m_arPtr.GetSize() ; i++ )
	{
		if ( ((CAsp*)m_arPtr.GetAt(i))->GetId() == szID )
			return (CAsp*)m_arPtr.GetAt(i);
	}
	return NULL;
}

void CAspArray::AddObject ( CAsp* pObject )
{
	if ( pObject != NULL )
		m_arPtr.Add ( pObject );
}

void CAspArray::RemoveAll()
{
	for ( int i = 0 ; i < m_arPtr.GetSize() ; i++ )
		delete (CAsp*)m_arPtr.GetAt(i);			// delete all objects

	m_arPtr.RemoveAll();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CAsp::CAsp()
{
	m_strId = "";
	m_strCssClass = "";
	m_strBackColor = "";
	m_strTextColor = "";
	m_strBorderColor = "";
	m_strBorderWidth = "";
	m_strBorderStyle = "";
	m_strCellPadding = "";
	m_strCellSpacing = "";
	m_strWidth = "";
	m_strStyle = "";
}

/**********************************************************************/

void CAsp::ParseAttributes ( CCSV& csvAttributes, const char* szAttributes )	// eg. Id="check1" Text="Label 1"
{
	GetTokens ( csvAttributes, szAttributes, " \t\r\n", "\"" );
}

/**********************************************************************/

void CAsp::SetAttributes ( CCSV& csvAttributes )
{
	// common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_ID, strValue ) == TRUE )				m_strId = strValue;
	if ( GetParamValue ( csvAttributes, ASP_CSS_CLASS, strValue ) == TRUE )		m_strCssClass = strValue;
	if ( GetParamValue ( csvAttributes, ASP_BACK_COLOR, strValue ) == TRUE )		m_strBackColor = strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT_COLOR, strValue ) == TRUE )		m_strTextColor = strValue;
	if ( GetParamValue ( csvAttributes, ASP_BORDER_COLOR, strValue ) == TRUE )		m_strBorderColor = strValue;
	if ( GetParamValue ( csvAttributes, ASP_BORDER_WIDTH, strValue ) == TRUE )		m_strBorderWidth = strValue;
	if ( GetParamValue ( csvAttributes, ASP_BORDER_STYLE, strValue ) == TRUE )		m_strBorderStyle = strValue;
	if ( GetParamValue ( csvAttributes, ASP_CELL_PADDING, strValue ) == TRUE )		m_strCellPadding = strValue;
	if ( GetParamValue ( csvAttributes, ASP_CELL_SPACING, strValue ) == TRUE )		m_strCellSpacing = strValue;
	if ( GetParamValue ( csvAttributes, ASP_WIDTH, strValue ) == TRUE )			m_strWidth = strValue;
	if ( GetParamValue ( csvAttributes, ASP_STYLE, strValue ) == TRUE )			m_strStyle = strValue;
}

/**********************************************************************/

CString CAsp::GetId ( CCSV& csvAttributes )
{
	CString strValue;
	GetParamValue ( csvAttributes, ASP_ID, strValue );
	return strValue;
}

/**********************************************************************/

void CAsp::AddStyleAttribute ( const char* szStyle )
{
	m_strStyle += szStyle;		// eg. width:40px;height:40px; etc
}

CString CAsp::GetStyleAttribute()
{
	return m_strStyle;
}

/**********************************************************************/

CString CAsp::GetCss ( const char* szLabel, const char* szValue )
{
	CString strCss;
	if ( strlen(szValue) > 0 )
		strCss.Format ( "%s:%s;", szLabel, szValue );

	return strCss;
}

/**********************************************************************/

void CAsp::Execute ( void (*AspFunction)(bool,CCSV&,CAspArray&,CCSV&), const char* szSourceFilename, CString& strDest, CCSV& csvUserParams, CCSV& csvSessionCookies )
{
	CString strBuf;

	CSSFile fileSource;
	if ( fileSource.Open ( szSourceFilename, "rb" ) == TRUE )
	{
		CString str;
		while ( fileSource.ReadLine ( str ) == TRUE )	// keep crlfs
			strBuf += str;

		fileSource.Close();
	}

	CAspArray arObjects;	// array of asp objects
	CString strHead;
	CString strStyle;
	CString strScript;

	for ( int nPass = 1 ; nPass <= 3 ; nPass++ )		// pass 1 creates all the asp objects, pass 3 generates the html
	{
		CString strPadding;

		int nPtr = 0;
		while ( nPtr < strBuf.GetLength() )
		{
			int n1 = strBuf.Find ( "<asp:", nPtr );
			if ( n1 >= 0 )
			{
				if ( nPass == 3 )		// generating html
				{
					strPadding = "";

					for ( int i = n1-1 ; i >= 0 ; i-- )		// find prev crlf
					{
						if ( strBuf.GetAt(i) == '\n' || i == 0 )
						{
							strPadding = strBuf.Mid(i+1,n1-(i+1));
							break;
						}
					}

					strDest += strBuf.Mid ( nPtr, n1-nPtr-strPadding.GetLength() );	// copy html upto asp object (minus padding)
				}

				nPtr = n1;
				int n2 = strBuf.Find ( "/>", nPtr );
				if ( n2 >= 0 )		// got an asp object
				{
					CString strObject = strBuf.Mid ( nPtr, n2-nPtr+2 );		// <asp:Type attributes .../>
					if (( n1 = strObject.Find ( ' ' )) >= 0 )
					{
						CString strObjectType = strObject.Mid ( 5, n1-5 );
						CString strAttributes = strObject.Mid ( n1, strObject.GetLength()-n1-2 );	// eg. Id="check1" Text="Label 1" etc

						CCSV csvAttributes;
						ParseAttributes ( csvAttributes, strAttributes );

						if ( strObjectType == "AutoComplete" )
						{
							if ( nPass == 1 )
							{
								CAspAutoComplete* p = new CAspAutoComplete;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspAutoComplete* p = (CAspAutoComplete*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strScript, strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "Button" )
						{
							if ( nPass == 1 )
							{
								CAspButton* p = new CAspButton;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspButton* p = (CAspButton*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "CheckBox" )
						{
							if ( nPass == 1 )
							{
								CAspCheckBox* p = new CAspCheckBox;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspCheckBox* p = (CAspCheckBox*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "CheckBoxList" )
						{
							if ( nPass == 1 )
							{
								CAspCheckBoxList* p = new CAspCheckBoxList;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspCheckBoxList* p = (CAspCheckBoxList*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "DataGrid" )
						{
							if ( nPass == 1 )
							{
								CAspDataGrid* p = new CAspDataGrid;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspDataGrid* p = (CAspDataGrid*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "Html" )
						{
							if ( nPass == 1 )
							{
								CAspHtml* p = new CAspHtml;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspHtml* p = (CAspHtml*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strHead, strScript, strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "Label" )
						{
							if ( nPass == 1 )
							{
								CAspLabel* p = new CAspLabel;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspLabel* p = (CAspLabel*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else if ( strObjectType == "TextBox" )
						{
							if ( nPass == 1 )
							{
								CAspTextBox* p = new CAspTextBox;
								arObjects.AddObject ( p );
								p->SetAttributes ( csvAttributes );
							}
							else
							{
								CAspTextBox* p = (CAspTextBox*) arObjects.FindID ( GetId(csvAttributes) );
								if ( p != NULL )
								{
									if ( nPass == 2 )
										p->ParseUserParams ( csvUserParams );
									else if ( nPass == 3 )
										p->Write ( strStyle, strDest, strPadding );
								}
							}
						}
						else
						{
						}
					}

					nPtr = n2 + 2;		// move past asp object
				}
				else		// invalid asp object - cannot find />
				{
				}
			}
			else		// all objects have been created
			{
				if ( nPass == 1 )		// user function can create or modify any of the objects in the array before html is generated
				{
					if ( AspFunction != NULL )
						AspFunction ( FALSE, csvUserParams, arObjects, csvSessionCookies );		// postback = false
				}
				else if ( nPass == 2 )	// user params have been parsed, objects can still be modified before html is generated
				{
					if ( AspFunction != NULL )
						AspFunction ( TRUE, csvUserParams, arObjects, csvSessionCookies );		// postback = true
				}
				else if ( nPass == 3 )	// generating html
				{
					strDest += strBuf.Mid(nPtr);
				}
				break;
			}
		}
	}

	if ( strHead != "" )		// got some generated head
	{
		int n = strDest.Find ( "</head>" );	// insert before end of head section
		if ( n >= 0 )
		{
			strDest.Insert ( n, strHead );
		}
	}

	if ( strStyle != "" )		// got some generated style
	{
		int n = strDest.Find ( "</style>" );	// end of style section
		if ( n >= 0 )
		{
			strDest.Insert ( n, strStyle );
		}
		else		// no style section found - need to create one
		{
			int n = strDest.Find ( "</head>" );	// insert before end of head section
			if ( n >= 0 )
			{
				CString str = "<style>\r\n";
				str += strStyle;
				str += "</style>\r\n";
				strDest.Insert ( n, str );
			}
		}
	}

	if ( strScript != "" )		// got some generated script
	{
		int n = strDest.Find ( "</head>" );	// insert before end of head section
		if ( n >= 0 )
		{
			strDest.Insert ( n, strScript );
		}
	}
}

/**********************************************************************/
