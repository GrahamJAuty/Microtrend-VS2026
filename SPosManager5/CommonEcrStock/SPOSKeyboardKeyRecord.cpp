/**********************************************************************/
#include "SPOSKeyboardDefines.h"
/**********************************************************************/
#include "SPOSKeyboardKeyRecord.h"
/**********************************************************************/

CSPOSKeyboardKeyRecord::CSPOSKeyboardKeyRecord()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::Reset()
{
	m_nListNo = 1;
	m_nPageNo = 1;
	m_nKeyNo = 1;
	m_nType = KEY_TYPE_EMPTY;
	m_nValue = 0;
	m_strKeyText = "";
	m_nForeground = 1;
	m_nBackground = 4;
	m_nLegacyTextSize = 1;
	m_nRowSize = 0;
	m_nColSize = 0;

	m_nFontSize = 8;
	m_bBold = FALSE;
	m_nAlignmentH = 1;
	m_nAlignmentV = 1;
	m_strImageFilename = "";
	m_nImageScale = 100;
	m_bHideText = FALSE;
	m_bDefaultTextAlways = FALSE;
	m_bGJANoSort = FALSE;
}

/**********************************************************************/

__int64 CSPOSKeyboardKeyRecord::GetLookUpKey()
{
	return ( m_nListNo * 0x10000 ) + ( m_nPageNo * 0x100 ) + m_nKeyNo;
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::GetDisplayTexts(CStringArray& Texts, bool bForPluSort)
{
	Texts.RemoveAll();

	CCSV csv(m_strKeyText, '|', '"');

	int nTotalLen = 0;
	for (int n = 0; n < csv.GetSize(); n++)
	{
		CString strOriginal = csv.GetString(n);
		CString strDisplay = strOriginal;

		if (TRUE == bForPluSort)
		{
			::TrimSpaces(strDisplay, FALSE);
		}

		Texts.Add(strDisplay);

		nTotalLen += strOriginal.GetLength() + 2;
		if (nTotalLen >= 80)
		{
			break;
		}
	}

	while (Texts.GetSize() != 0)
	{
		CString strText = Texts.GetAt(0);

		if (strText == "")
		{
			Texts.RemoveAt(0);
		}
		else
		{
			break;
		}
	}

	while (Texts.GetSize() != 0)
	{
		int nPos = Texts.GetSize() - 1;

		CString strText = Texts.GetAt(nPos);

		if (strText == "")
		{
			Texts.RemoveAt(nPos);
		}
		else
		{
			break;
		}
	}

	if (FALSE == bForPluSort)
	{
		CString strPluNo = "";
		if (SPOSKeyboardManager.GetLayoutOptions()->GetShowPluNoFlag() == TRUE)
		{
			if (m_nType == KEY_TYPE_PLUNUMBER)
			{
				strPluNo.Format("%I64d", m_nValue);
			}
		}

		Texts.InsertAt(0, strPluNo);
	}
}

/**********************************************************************/

const char* CSPOSKeyboardKeyRecord::GetTrimmedKeyText()
{
	int nMaxLen = m_strKeyText.GetLength() * 2;

	if ( nMaxLen <= 80 )
	{
		m_strTrimmedKeyText = m_strKeyText;
		return m_strTrimmedKeyText;
	}

	int nLenSoFar = 0;

	for ( int n = 0; n < m_strKeyText.GetLength(); n++ )
	{
		if ( m_strKeyText.GetAt(n) == '|' )
		{
			nLenSoFar += 2;

			if ( nLenSoFar == 80 )
			{
				m_strTrimmedKeyText = m_strKeyText.Left( n + 1 );
				return m_strTrimmedKeyText;
			}

			if ( nLenSoFar == 81 )
			{
				m_strTrimmedKeyText = m_strKeyText.Left( n );
				return m_strTrimmedKeyText;
			}
		}
		else
		{
			nLenSoFar += 1;

			if ( nLenSoFar == 80 )
			{
				m_strTrimmedKeyText = m_strKeyText.Left( n + 1 );
				return m_strTrimmedKeyText;
			}
		}
	}

	m_strTrimmedKeyText = m_strKeyText;
	return m_strTrimmedKeyText;
}

/**********************************************************************/

const char* CSPOSKeyboardKeyRecord::GetEditableKeyText()
{
	int nLenSoFar = 0;
	m_strEditableKeyText = "";

	for (int n = 0; n < m_strKeyText.GetLength(); n++)
	{
		char c = m_strKeyText.GetAt(n);

		if (c == '|')
		{
			if (nLenSoFar > 78)
			{
				break;
			}

			if (nLenSoFar == 78)
			{
				m_strEditableKeyText += "\r\n";
				break;
			}

			m_strEditableKeyText += "\r\n";
			nLenSoFar += 2;
		}
		else
		{
			m_strEditableKeyText += c;

			nLenSoFar++;
			if (nLenSoFar >= 80)
			{
				break;
			}
		}
	}

	return m_strEditableKeyText;
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetEditableKeyText(const char* szText)
{
	m_strKeyText = "";

	CString strText = szText;
	int nLength = strText.GetLength();
	for (int nPos = 0; nPos < nLength; nPos++)
	{
		if (nPos == nLength - 1)
		{
			m_strKeyText += strText.GetAt(nPos);
		}
		else
		{
			if (strText.Mid(nPos, 2) == "\r\n")
			{
				m_strKeyText += "|";
				nPos++;
			}
			else
			{
				m_strKeyText += strText.GetAt(nPos);
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetListNo(__int64 n)
{
	if ((n >= 1) && (n <= MAX_LIST_NO))
	{
		m_nListNo = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetPageNo(int n)
{
	if ((n >= 1) && (n <= MAX_LIST_PAGES))
	{
		m_nPageNo = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetKeyNo(int n)
{
	if ((n >= 1) && (n <= POS_KEY_COUNT))
	{
		m_nKeyNo = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetType(int n)
{
	switch (n)
	{
	case KEY_TYPE_EMPTY:
	case KEY_TYPE_FUNCTION:
	case KEY_TYPE_PAYMENT:
	case KEY_TYPE_MODIFIER:
	case KEY_TYPE_CONTROL:
	case KEY_TYPE_LIST:
	case KEY_TYPE_SVRPRESET:
	case KEY_TYPE_MACRO:
	case KEY_TYPE_PLUNUMBER:
		m_nType = n;
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetValue( __int64 n )
{
	m_nValue = n;
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetLegacyTextSize(int n)
{
	if ((n >= 1) && (n <= 2))
	{
		m_nLegacyTextSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetForeground(int n)
{
	if ((n >= 1) && (n <= 64))
	{
		m_nForeground = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetBackground(int n)
{
	if ((n >= 1) && (n <= 64))
	{
		m_nBackground = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetColSize(int n)
{
	if ((n >= 0) && (n <= 10))
	{
		m_nColSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetRowSize(int n)
{
	if ((n >= 0) && (n <= 12))
	{
		m_nRowSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetFontSize( int n )
{
	if ( CSPOSKeyboardKeyRecord::IsValidFontSize(n) == TRUE )
	{ 
		m_nFontSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetAlignmentH(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nAlignmentH = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetAlignmentV(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nAlignmentV = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetImageFilename(const char* sz)
{
	CString str = sz;
	int nLength = str.GetLength();

	if (nLength >= 5 && nLength <= MAX_BITMAP_FILENAME + 4)
	{
		CString strTest = str.Right(4);
		strTest.MakeUpper();

		if (strTest == ".BMP" || strTest == ".GIF" || strTest == ".JPG")
		{
			m_strImageFilename = str;
		}
	}
	else if (nLength == 0)
	{
		m_strImageFilename = "";
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::SetImageScale(int n)
{
	if ((n >= 0) && (n <= 100))
	{
		m_nImageScale = n;
	}
}

/**********************************************************************/

bool CSPOSKeyboardKeyRecord::IsValidFontSize(int n)
{
	bool bResult = FALSE;

	switch (n)
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
	case 16:
	case 20:
	case 24:
	case 26:
	case 28:
	case 36:
	case 48:
	case 72:
	case 120:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardKeyRecord::GetFontSizeList(CWordArray& arrayList)
{
	arrayList.RemoveAll();
	arrayList.Add(8);
	arrayList.Add(10);
	arrayList.Add(11);
	arrayList.Add(12);
	arrayList.Add(14);
	arrayList.Add(16);
	arrayList.Add(20);
	arrayList.Add(24);
	arrayList.Add(26);
	arrayList.Add(28);
	arrayList.Add(36);
	arrayList.Add(48);
	arrayList.Add(72);
	arrayList.Add(120);
}

/**********************************************************************/
