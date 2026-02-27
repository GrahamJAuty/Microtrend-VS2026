//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

CCommentHandler::CCommentHandler(const char* szFilename)
{
	ReadComments(szFilename);
	m_nMaxLength = 0;
}

//*******************************************************************

CCommentHandler::CCommentHandler()
{
	m_nMaxLength = 0;
}

//*******************************************************************

void CCommentHandler::ReadComments(const char* szFilename)
{
	m_strFilename = szFilename;

	m_nMaxLength = 20;
	m_arrayComments.RemoveAll();								// may have been read defore, so start again
	m_arrayComments.Read(m_strFilename, 30);					// read lines
}

//*******************************************************************

void CCommentHandler::Delete(const char* szComment)
{
	CString strComment = szComment;

	for (int i = 0; i < m_arrayComments.GetSize(); i++)		// read previous comments
	{
		if (m_arrayComments.GetAt(i) == strComment)
		{
			m_arrayComments.RemoveAt(i);
			SaveFile();
			break;
		}
	}
}

//*******************************************************************

bool CCommentHandler::Save(const char* szComment)
{
	CString strComment = szComment;

	if (strComment == "")
	{
		return TRUE;
	}

	if (strComment.GetLength() > m_nMaxLength)
	{
		strComment = strComment.Left(m_nMaxLength);
	}

	Delete(strComment);								// delete if found
	m_arrayComments.InsertAt(0, strComment);			// add comment at start

	return SaveFile();
}

//*******************************************************************

bool CCommentHandler::SaveFile()
{
	return m_arrayComments.Write(m_strFilename);
}

//*******************************************************************
// Special functions used when list is used to hold Epos Terminal number lists

void CCommentHandler::ExtractTNos(const char* szTerminalList, CUIntArray* ptrArrayTNo)
{
	CCSV csv(szTerminalList);

	for (int i = 0; i < csv.GetSize(); i++)
	{
		if (csv.IsEmpty(i) == FALSE)
		{
			CString strItem = csv.GetString(i);

			int nPtr = 0;
			if ((nPtr = strItem.Find('-')) != -1)		// index to '-' in '1-12' ie range 1 to 12
			{
				CString strStart = strItem.Left(nPtr);
				++nPtr;
				CString strEnd = strItem.Mid(nPtr, strItem.GetLength() - nPtr);

				if (strStart != "" && strEnd != "")
				{
					for (int nTNo = atoi(strStart); nTNo <= atoi(strEnd); nTNo++)
					{
						ptrArrayTNo->Add(nTNo);
					}
				}
			}
			else
			{
				int nTNo = csv.GetInt(i);
				if (nTNo >= 0)
				{
					ptrArrayTNo->Add(nTNo);
				}
			}
		}
	}
}

//*******************************************************************
