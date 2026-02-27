//****************************************************************************
#include "SystemData.h"
//****************************************************************************
#include "CommandFile.h"
//****************************************************************************

bool CCommandFile::Found()
{
	if (::FileExists(System.m_strCommandFilename) == FALSE)
	{
		return FALSE;
	}

	CSSFile file;
	CString strCommand = "";

	if (file.Open(System.m_strCommandFilename, "rb") == TRUE)
	{
		file.ReadString(strCommand);
		file.Close();

		::SendToRecycleBin(System.m_strCommandFilename);
	}

	if (strCommand == "SETUP")
	{
		return TRUE;
	}

	return FALSE;
}

//*******************************************************************
