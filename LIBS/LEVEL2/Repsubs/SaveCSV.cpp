/**********************************************************************/

void CReportFileDoc::SaveAsCSV ( const char* szFilename )
{
	SaveAsCSV ( szFilename, m_Ruler );
}

/**********************************************************************/

static void ProcessTextAlign ( CString& strBuf )
{
	int n = strBuf.Find ( "<AL>" );	// align left
	if ( n >= 0 )
	{
		strBuf.Delete ( n, 4 );		// remove <AL> from strbuf
	}

	n = strBuf.Find ( "<AC>" );		// align centre
	if ( n >= 0 )
	{
		strBuf.Delete ( n, 4 );		// remove <AC> from strbuf
	}

	n = strBuf.Find ( "<AR>" );		// align right
	if ( n >= 0 )
	{
		strBuf.Delete ( n, 4 );		// remove <AR> from strbuf
	}
}

static void ProcessForegroundColour ( CString& strBuf )
{
	int n = strBuf.Find ( "<CF#" );
	if ( n >= 0 )
	{
		if ( strBuf.GetAt(n+10) == '>' )						// <CF#RRGGBB>
			strBuf.Delete ( n, 11 );							// remove <CF#RRGGBB> from strbuf
	}
}

static void ProcessBackgroundColour ( CString& strBuf )
{
	int n = strBuf.Find ( "<CB#" );
	if ( n >= 0 )
	{
		if ( strBuf.GetAt(n+10) == '>' )						// <CB#RRGGBB>
			strBuf.Delete ( n, 11 );							// remove <CB#RRGGBB> from strbuf
	}
}

/**********************************************************************/

static void ProcessAttributes ( CString& strBuf )
{
	ProcessTextAlign ( strBuf );
	ProcessForegroundColour ( strBuf );
	ProcessBackgroundColour ( strBuf );
}

/**********************************************************************/

void CReportFileDoc::SaveAsCSV ( const char* szFilename, CTabRuler& ruler )
{
	CSSFile file;

	if ( file.Open ( szFilename, "wb" ) == TRUE )
	{
		CCSV csvHeader;

		for ( int i = 0 ; i < ruler.GetSize() ; i++ )
		{
			int nOrder = ruler.GetOrder(i);

			if ( ruler.GetWidth(nOrder) > 0 )
			{
				if ( ruler.GetSubTabSize(nOrder) > 0 )
				{
					for ( int j = 0 ; j < ruler.GetSubTabSize(nOrder) ; j++ )
					{
						CString strText = ruler.GetText(nOrder);
						ProcessAttributes ( strText );		// discard colours and align

						CString str = strText;
						str += " ";

						strText = ruler.GetSubTabText(nOrder,j);
						ProcessAttributes ( strText );		// discard colours and align

						str += strText;
						csvHeader.Add ( str );
					}
				}
				else
				{
					CString strText = ruler.GetText(nOrder);
					ProcessAttributes ( strText );		// discard colours and align
					csvHeader.Add ( strText );
				}
			}
		}

		file.WriteLine ( csvHeader.GetLine() );

		for ( int n = 0 ; n < GetPrintTextSize() ; n++ )
		{
			CCSV csvIn ( GetPrintTextAt(n), '\t' );
			CCSV csvOut;

			for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
			{
				CString strText;

				if ( csvIn.GetSize() > 1 )		// found tab (must be table)
				{
					strText = csvIn.GetString(ruler.GetOrderedIndex(i));
				}
				else		// simple line (no tabs)
				{
					strText = ( i == 0 ) ? csvIn.GetString(0) : "";
				}

				ProcessAttributes ( strText );		// discard colours and align

				if ( strText.Left(1) == "<" )			// check for special formatting
				{
					if ( strText.Left(4) == "<..>" )
						strText = strText.Mid(4);	// keep rest of field
					else
						strText = "";				// discard field
				}

				csvOut.Add ( strText );
			}

			file.WriteLine ( csvOut.GetLine() );
		}

		file.Close();
	}
}

/**********************************************************************/
