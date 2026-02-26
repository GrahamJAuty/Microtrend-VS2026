//*******************************************************************
#include "AESHelpers.h"
#include "ChartwellsOptions.h"
#include "FileRemover.h"
#include "FilenameHandler.h"
//*******************************************************************
#include "SQLConnectionOptions.h"
//*******************************************************************

CSQLConnectionOptions::CSQLConnectionOptions(void)
{
	m_bLANConnect = FALSE;
	m_strLANServerName = "";
	m_bAlreadyRead = FALSE;
	m_strPasswordPT = "";
	m_strPasswordAES = "";
	m_nQueryTimeout = 0;
	m_nODBCDriverVersion = 17;
}

//*******************************************************************

bool CSQLConnectionOptions::Read()
{
	bool bReply = TRUE;

	if (FALSE == m_bAlreadyRead)
	{
		CString strFilename = SQLFilenames.GetSQLConnectionOptionsFilename();
		
		m_bAlreadyRead = TRUE;

		CSSIniFile file;
		bReply = file.Read(strFilename);
		m_bLANConnect = file.GetBool("LANConnect", FALSE);
		m_strLANServerName = file.GetString("LANServerName", "");
		m_strPasswordPT = file.GetString("PasswordPT", "");
		m_strPasswordAES = file.GetString("Password", "");
		m_nQueryTimeout = file.GetInt("QueryTimeout", 0);
		SetODBCDriverVersion(file.GetInt("ODBCDriverVersion", 17));

		if (m_strPasswordPT != "")
		{
			CAESHelpers::PlainTextToAESHex(m_strPasswordPT, m_strPasswordAES);
		}
		else if (m_strPasswordAES != "")
		{
			CAESHelpers::AESHexToPlainText(m_strPasswordAES, m_strPasswordPT);
		}
		else
		{
			m_strPasswordPT.Format("SHADwell%6.6d##",
				ChartwellsOptions.GetSmartPaySerialNo());
		}
	}

	return bReply;
}

//*******************************************************************

void CSQLConnectionOptions::CopyFrom(CSQLConnectionOptions& source )
{
	m_bLANConnect = source.m_bLANConnect;
	m_strLANServerName = source.m_strLANServerName;
	m_strPasswordPT = source.m_strPasswordPT;
	m_strPasswordAES = source.m_strPasswordAES;
	m_nQueryTimeout = source.m_nQueryTimeout;
	m_nODBCDriverVersion = source.m_nODBCDriverVersion;
}

//*******************************************************************
