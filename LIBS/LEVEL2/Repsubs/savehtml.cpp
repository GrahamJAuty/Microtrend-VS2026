/**********************************************************************/

void CReportFileDoc::SaveAsHTML ( const char* szFilename )
{
	SaveAsHTML ( szFilename, m_Ruler );
}

/**********************************************************************/

static const char* GetAlignText ( int nAlign )
{
	switch ( nAlign )
	{
	case TA_LEFT:		return "left";
	case TA_RIGHT:		return "right";
	case TA_CENTER:	return "center";
	default:			return "left";
	}
}

/**********************************************************************/

static int CheckAlignOverride ( CString& strBuf )
{
	int n = strBuf.Find ( "<AL>" );	// align left
	if ( n >= 0 )
		return TA_LEFT;

	n = strBuf.Find ( "<AC>" );		// align centre
	if ( n >= 0 )
		return TA_CENTER;

	n = strBuf.Find ( "<AR>" );		// align right
	if ( n >= 0 )
		return TA_RIGHT;

	return -1;		// not found
}

/**********************************************************************/

int CalcColumnSpan ( int nCol, CString& strText, CCSV& csvIn, CTabRuler& ruler, int& nAlign )
{
	int nColSpan = 1;

	// need to check for align override - but dont discard ( required by GetStyle() )

	if (( nAlign = CheckAlignOverride ( strText )) == -1 )
		nAlign = ruler.GetOrderedAlign(nCol);

	if ( nAlign == TA_LEFT )
	{
		// find number of consecutive blank columns following to use as colspan

		int i;
		for ( i = nCol+1 ; i < ruler.GetOrderedSize() ; i++ )
		{
			CString str = csvIn.GetString(ruler.GetOrderedIndex(i));
			if ( str != "" )
				break;
		}
		nColSpan = i - nCol;
	}
	else if ( nAlign == TA_RIGHT )
	{
		// find previous number of consecutive blank columns to use as colspan

		int i;
		for ( i = nCol-1 ; i >= 0 ; i-- )
		{
			CString str = csvIn.GetString(ruler.GetOrderedIndex(i));
			if ( str != "" )
				break;
		}
		nColSpan = nCol - i;
	}
	else if ( nAlign == TA_CENTER )		// column spanning not implemented for centre
	{
	}

	return nColSpan;
}

/**********************************************************************/

static CString ProcessTextAlign ( CString& strBuf )
{
	CString strAlign;

	int n = strBuf.Find ( "<AL>" );	// align left
	if ( n >= 0 )
	{
		strAlign = "text-align:left;";
		strBuf.Delete ( n, 4 );		// remove <AL> from strbuf
		return strAlign;
	}

	n = strBuf.Find ( "<AC>" );		// align centre
	if ( n >= 0 )
	{
		strAlign = "text-align:center;";
		strBuf.Delete ( n, 4 );		// remove <AC> from strbuf
		return strAlign;
	}

	n = strBuf.Find ( "<AR>" );		// align right
	if ( n >= 0 )
	{
		strAlign = "text-align:right;";
		strBuf.Delete ( n, 4 );		// remove <AR> from strbuf
		return strAlign;
	}

	return strAlign;
}

/**********************************************************************/

static CString ProcessForegroundColour ( CString& strBuf )
{
	CString strColour;

	int n = strBuf.Find ( "<CF#" );
	if ( n >= 0 )
	{
		if ( strBuf.GetAt(n+10) == '>' )						// <CF#RRGGBB>
		{
			strColour.Format ( "color:#%s;", strBuf.Mid(n+4,6) );	// RRGGBB
			strBuf.Delete ( n, 11 );							// remove <CF#RRGGBB> from strbuf
		}
	}
	return strColour;
}

/**********************************************************************/

static CString ProcessBackgroundColour ( CString& strBuf )
{
	CString strColour;

	int n = strBuf.Find ( "<CB#" );
	if ( n >= 0 )
	{
		if ( strBuf.GetAt(n+10) == '>' )								// <CB#RRGGBB>
		{
			strColour.Format ( "background-color:#%s;", strBuf.Mid(n+4,6) );	// RRGGBB
			strBuf.Delete ( n, 11 );									// remove <CB#RRGGBB> from strbuf
		}
	}
	return strColour;
}

/**********************************************************************/

static CString GetStyle ( CString& strBuf )
{
	CString strStyle;

	CString strAlign = ProcessTextAlign ( strBuf );
	CString strFG = ProcessForegroundColour ( strBuf );
	CString strBG = ProcessBackgroundColour ( strBuf );

	if ( strAlign != "" || strFG != "" || strBG != "" )
		strStyle.Format ( " style=\"%s%s%s\"", strAlign, strFG, strBG );		// style="text-align:XXXX; color:#RRGGBB; background-color:#RRGGBB;"

	return strStyle;
}

/**********************************************************************/

void CReportFileDoc::SaveAsHTML ( const char* szFilename, CTabRuler& ruler ) 
{
	CSSFile file;

	if ( file.Open ( szFilename, "wb" ) == TRUE )
	{
		const char* szPadding = "padding-left:3px; padding-right:3px;";

		file.WriteLine ( "<!DOCTYPE html>" );
		file.WriteLine ( "<html>" );
		file.WriteLine ( "<head>" );

		CString strTitle = GetReportTitle();
		CString strTitleStyle = GetStyle ( strTitle );		// extract align and colour information

		CString str;
		str.Format ( "<title>%s</title>", strTitle );
		file.WriteLine ( str );

		file.WriteLine ( "<style type=\"text/css\">" );
		file.WriteLine ( "body { font-family:Arial,Helvetica,sans-serif; }" );
		file.WriteLine ( "p.title { font-size:12pt; font-weight:bold; }" );
		file.WriteLine ( "table.report { font-size:8pt; table-layout:fixed; }" );
		file.WriteLine ( "td { overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }" );

		int nTotalWidth = 0;
		bool bSubHeaderRequired = FALSE;
		bool bHeaderRequired = FALSE;

		for ( int i = 0 ; i < ruler.GetSize() ; i++ )
		{
			if ( ruler.GetWidth(i) > 0 )
			{
				if ( strlen ( ruler.GetText(i) ) > 0 )
					bHeaderRequired = TRUE;

				nTotalWidth += ruler.GetWidth(i);
			}
		}

		for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
		{
			if ( strlen ( ruler.GetOrderedSubTabText(i) ) > 0 )
				bSubHeaderRequired = TRUE;
		}

		CString strBuf;
		if( bHeaderRequired == TRUE && bSubHeaderRequired == TRUE )		// header + sub header
		{
			int n = 1;
			for ( int i = 0 ; i < ruler.GetSize() ; i++ )
			{
				int nOrder = ruler.GetOrder(i);
				if ( ruler.GetWidth(nOrder) > 0 )
				{
					int nOrder = ruler.GetOrder(i);
					strBuf.Format ( "th.cc%d { %s width:%dpx; text-align:%s; }", n++, szPadding, ruler.GetWidth(nOrder), GetAlignText(ruler.GetAlign(nOrder)) );
					file.WriteLine ( strBuf );
				}
			}

			for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
			{
				strBuf.Format ( "th.c%d { %s width:%dpx; text-align:%s; }", i+1, szPadding, ruler.GetOrderedWidth(i), GetAlignText(ruler.GetOrderedAlign(i)) );
				file.WriteLine ( strBuf );

				strBuf.Format ( "td.c%d { %s width:%dpx; text-align:%s; }", i+1, szPadding, ruler.GetOrderedWidth(i), GetAlignText(ruler.GetOrderedAlign(i)) );
				file.WriteLine ( strBuf );
			}
		}
		else			// header only or no header
		{
			int n = 1;
			for ( int i = 0 ; i < ruler.GetSize() ; i++ )
			{
				int nOrder = ruler.GetOrder(i);
				if ( ruler.GetWidth(nOrder) > 0 )
				{
					int nOrder = ruler.GetOrder(i);

					strBuf.Format ( "th.c%d { %s width:%dpx; text-align:%s; }", n, szPadding, ruler.GetWidth(nOrder), GetAlignText(ruler.GetAlign(nOrder)) );
					file.WriteLine ( strBuf );

					strBuf.Format ( "td.c%d { %s width:%dpx; text-align:%s; }", n, szPadding, ruler.GetWidth(nOrder), GetAlignText(ruler.GetAlign(nOrder)) );
					file.WriteLine ( strBuf );

					n++;
				}
			}
		}

		file.WriteLine ( "hr { height:1; }" );	// horizontal rule
		file.WriteLine ( "</style>" );

		file.WriteLine ( "</head>" );

		file.WriteLine ( "<body>" );

		strBuf.Format ( "<p class=\"title\"%s>%s</p>", strTitleStyle, strTitle );
		file.WriteLine ( strBuf );

		if( bHeaderRequired == TRUE && bSubHeaderRequired == TRUE )		// header + sub header
		{
			CString strTable;
			strTable.Format ( "<table class=\"report\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" width=\"%d\">", nTotalWidth );
			file.WriteLine ( strTable );

			file.WriteLine ( "<thead>" );
			file.WriteLine ( "<tr>" );

			int n = 1;
			for ( int i = 0 ; i < ruler.GetSize() ; i++ )
			{
				int nOrder = ruler.GetOrder(i);
				if ( ruler.GetWidth(nOrder) > 0 )
				{
					strBuf.Format ( "<th class=\"cc%d\">%s</th>", n++, ruler.GetText(nOrder) );
					file.WriteLine ( strBuf );
				}
			}

			file.WriteLine ( "</tr>" );
			file.WriteLine ( "</thead>" );
			file.WriteLine ( "</table>" );

			strTable.Format ( "<table class=\"report\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" width=\"%d\">", nTotalWidth );
			file.WriteLine ( strTable );

			file.WriteLine ( "<thead>" );
			file.WriteLine ( "<tr>" );

			for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
			{
				strBuf.Format ( "<th class=\"c%d\">%s</th>", i+1, ruler.GetOrderedSubTabText(i) );
				file.WriteLine ( strBuf );
			}

			file.WriteLine ( "</tr>" );
			file.WriteLine ( "</thead>" );
		}
		else			// header only or no header
		{
			CString strTable;
			strTable.Format ( "<table class=\"report\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" width=\"%d\">", nTotalWidth );
			file.WriteLine ( strTable );

			file.WriteLine ( "<thead>" );
			file.WriteLine ( "<tr>" );

			int n = 1;
			for ( int i = 0 ; i < ruler.GetSize() ; i++ )
			{
				int nOrder = ruler.GetOrder(i);
				if ( ruler.GetWidth(nOrder) > 0 )
				{
					CString strText = ruler.GetText(nOrder);
					CString strStyle = GetStyle ( strText );	// style="text-align:left; color:#RRGGBB; background-color:#RRGGBB";
					strBuf.Format ( "<th class=\"c%d\"%s>%s</th>", n++, strStyle, strText );
					file.WriteLine ( strBuf );
				}
			}

			file.WriteLine ( "</tr>" );
			file.WriteLine ( "</thead>" );
		}

		file.WriteLine ( "<tbody>" );

		for ( int nLine = 0 ; nLine < GetPrintTextSize() ; nLine++ )
		{
			if ( strlen ( GetPrintTextAt(nLine) ) == 0 )
			{
				file.WriteLine ( "<tr><td>&nbsp;</td></tr>" );	// blank row
			}
			else
			{
				CCSV csvIn ( GetPrintTextAt(nLine), '\t' );

				if ( nLine == 0 )	// if 1st line contains colspan - need to write blank row to set column widths
				{
					for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
					{
						CString strText = csvIn.GetString(ruler.GetOrderedIndex(i));
						if ( strText.Find ( "<..>" ) >= 0 )	// ignore column width
						{
							file.WriteLine ( "<tr>" );
							for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
							{
								strBuf.Format ( "<td class=\"c%d\">&nbsp;</td>", i+1 );
								file.WriteLine ( strBuf );
							}
							file.WriteLine ( "</tr>" );
							break;
						}
					}
				}

				file.WriteLine ( "<tr>" );

				if ( csvIn.GetSize() == 1 )		// simple line (no tabs in line)
				{
					int nColSpan = ruler.GetOrderedSize();	// span all columns

					CString strText = csvIn.GetString(0);

					int n = strText.Find ( "<..>" );		// ignore column width
					if ( n >= 0 )
						strText.Delete ( n, 4 );			// keep rest of field

					if ( strText.Left(4) == "<LI>" )
						strText = "<hr>";				// horizontal rule

					CString strStyle = GetStyle ( strText );	// style="text-align:left; color:#RRGGBB; background-color:#RRGGBB";

					if ( strStyle != "" && strText == "" )
						strText = "&nbsp;";					// force line to be displayed if there is style but no text
														// it may be a full line of background colour

					strBuf.Format ( "<td colspan=\"%d\"%s>%s</td>", nColSpan, strStyle, strText );	// use full width of table (span all columns)

					file.WriteLine ( strBuf );
				}
				else							// tab char found must be table
				{
					CStringArray arText;
					bool bLeftColSpanFound = FALSE;		// NB a right align colspan cannot immediately follow a left aligned colspan

					for ( int i = 0 ; i < ruler.GetOrderedSize() ; i++ )
					{
						int nColSpan = 1;
						int nAlign = 0;

						CString strText = csvIn.GetString(ruler.GetOrderedIndex(i));

						int n = strText.Find ( "<..>" );		// ignore column width (column spanning)
						if ( n >= 0 )
						{
							strText.Delete ( n, 4 );			// keep rest of field

							nColSpan = CalcColumnSpan ( i, strText, csvIn, ruler, nAlign );
						}

						CString strStyle = GetStyle ( strText );	// style="text-align:left; color:#RRGGBB; background-color:#RRGGBB";

						if ( strText.Left(4) == "<LI>" )
							strText = "<hr>";				// horizontal rule

						if ( nColSpan > 1 )
						{
							if ( nAlign == TA_LEFT )			// left align
							{
								bLeftColSpanFound = TRUE;

								strBuf.Format ( "<td class=\"c%d\" colspan=\"%d\"%s>%s</td>", i+1, nColSpan, strStyle, strText );

								i += (nColSpan-1);							// skip following blank fields
							}
							else if ( nAlign == TA_RIGHT )	// right align
							{
								if ( bLeftColSpanFound == FALSE )		// right colspan not allowed if already got left colspan (columns would clash)
								{
									for ( int j = 0 ; j < nColSpan-1 ; j++ )
										arText.RemoveAt ( arText.GetSize()-1 );		// discard previous blank fields

									strBuf.Format ( "<td class=\"c%d\" colspan=\"%d\"%s>%s</td>", i+1, nColSpan, strStyle, strText );
								}
								else
									strBuf.Format ( "<td class=\"c%d\"%s>%s</td>", i+1, strStyle, strText );

								bLeftColSpanFound = FALSE;	// clear 
							}
							else if ( nAlign == TA_CENTER )	// column spanning not implemented for centre
							{
								bLeftColSpanFound = FALSE;	// clear 
							}
						}
						else			// normal field
						{
							strBuf.Format ( "<td class=\"c%d\"%s>%s</td>", i+1, strStyle, strText );
							bLeftColSpanFound = FALSE;		// clear 
						}

						arText.Add ( strBuf );
					}

					for ( int i = 0 ; i < arText.GetSize() ; i++ )
					{
						file.WriteLine ( arText.GetAt(i) );
					}
				}

				file.WriteLine ( "</tr>" );
			}
		}

		file.WriteLine ( "</tbody>" );
		file.WriteLine ( "</table>" );
		file.WriteLine ( "</body>" );
		file.WriteLine ( "</html>" );

		file.Close();
	}
}

/**********************************************************************/
