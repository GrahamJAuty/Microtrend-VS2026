/**********************************************************************/
#include "local.h"
/**********************************************************************/

CLegacyEmbeddedSerialNoFind::CLegacyEmbeddedSerialNoFind(const char* szExeFilename)
{
	m_strExeFilename = szExeFilename;
	m_lOffset = -1L;				// offset to start of user registration data
}

/**********************************************************************/

bool CLegacyEmbeddedSerialNoFind::LocateDetails()
{
	CSSFile file;

	if (file.Open(m_strExeFilename, "rb") == TRUE)
	{
		CString strMatch = CLegacyEmbeddedSerialNo::GetMatchString();
		int nMatchLen = strMatch.GetLength();
		int nMatchCount = 0;
		int n;

		while ((n = file.GetChar()) != EOF)
		{
			if (n == strMatch.GetAt(nMatchCount))
			{
				if (++nMatchCount == nMatchLen)
				{
					m_lOffset = file.Tell() - (long)nMatchCount;	// offset to registration data

					file.Seek(-(long)nMatchCount, SEEK_CUR);

					char buf[SYSSET_DATA_LEN + 1];

					if (file.Read(buf, 1, SYSSET_DATA_LEN) == SYSSET_DATA_LEN)
					{
						buf[SYSSET_DATA_LEN] = 0;
						file.Close();

						return SetData(buf);
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				if (nMatchCount > 0)
				{
					file.Seek(-(long)nMatchCount, SEEK_CUR);
					nMatchCount = 0;
				}
			}
		}
		file.Close();
	}

	m_lOffset = -1L;		// registration data not found

	return FALSE;
}

/**********************************************************************/

bool CLegacyEmbeddedSerialNoFind::WriteDetails()
{
	if (m_lOffset == -1L)			// registration data not found
	{
		return FALSE;
	}

	CFileStatus OldFileStatus;

	BOOL bOldStatus = CFile::GetStatus(m_strExeFilename, OldFileStatus);

	CSSFile file;

	if (file.Open(m_strExeFilename, "r+b") == TRUE)
	{
		file.Seek(m_lOffset, SEEK_SET);

		CString strData = GetData();

		if (file.Write(strData, 1, SYSSET_DATA_LEN) != SYSSET_DATA_LEN)
		{
			return FALSE;
		}

		file.Close();
	}

	if (bOldStatus == TRUE)
	{
		CFileStatus NewFileStatus;

		if (CFile::GetStatus(m_strExeFilename, NewFileStatus) == TRUE)
		{
			NewFileStatus.m_mtime = OldFileStatus.m_mtime;	// use original modified date and time
			CFile::SetStatus(m_strExeFilename, NewFileStatus);
		}
	}

	return TRUE;
}

/**********************************************************************/
