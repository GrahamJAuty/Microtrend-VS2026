//#include "SystemData.h"
#include "GroupPicker.h"

CGroupPicker::CGroupPicker ( const char* szFilename )
	: m_strFilename(szFilename)
{
	Reset();
}

void CGroupPicker::Reset()
{
	m_array.RemoveAll();									// force read on next validate
}

//**************************************************************

void CGroupPicker::LoadList()
{
	if ( m_array.GetSize() == 0 )								// see if file has been read yet
	{
		CSSFile file;
		if ( file.Open ( m_strFilename, "rb" ) == TRUE )		// no so read data
		{
			CString strLine;
			while ( file.ReadString(strLine) == TRUE )
				m_array.Add ( atoi ( strLine ) );

			file.Close();
		}
	}
}

//**************************************************************

int CGroupPicker::GetCount()
{
	LoadList();													// load list if not already done

	return m_array.GetSize();
}

//**************************************************************

int CGroupPicker::GetSingleGroupNo()
{
	LoadList();													// load list if not already done

	return ( GetCount() == 1 ) ? m_array.GetAt(0) : 0;			// see if only 1 group seleected
}

//**************************************************************

bool CGroupPicker::IsValid ( int nGroupNo )
{
	LoadList();													// load list if not alreday done

	bool bReply = FALSE;
	for ( int i = 0 ; i < m_array.GetSize() ; i++ )
	{
		if ( m_array.GetAt ( i ) == nGroupNo )
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

//*******************************************************************
