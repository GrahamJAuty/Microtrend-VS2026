//*******************************************************************
#include "AESHelpers.h"
#include "FileRemover.h"
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
	m_strODBCDriverVersion.Format("%d", DEF_OBDC_DRIVER_VERSION);
	m_nPortNumber = 0;
}

//*******************************************************************

bool CSQLConnectionOptions::Read()
{
	bool bReply = TRUE;

	if (FALSE == m_bAlreadyRead)
	{
		CString strFilenameOld = Filenames.GetSQLConnectionOptionsFilenameOld();
		CString strFilenameNew = Filenames.GetSQLConnectionOptionsFilenameNew();

		if (::FileExists(strFilenameNew) == FALSE)
		{
			if (::FileExists(strFilenameOld) == TRUE)
			{
				CopyFile(strFilenameOld, strFilenameNew, TRUE);
			}
		}

		CFileRemover FileRemover(strFilenameOld);

		m_bAlreadyRead = TRUE;

		CSSIniFile file;
		bReply = file.Read(strFilenameNew);
		m_bLANConnect = file.GetBool("LANConnect", FALSE);
		m_strLANServerName = file.GetString("LANServerName", "");
		m_strPasswordPT = file.GetString("PasswordPT", "");
		m_strPasswordAES = file.GetString("Password", "");
		m_nQueryTimeout = file.GetInt("QueryTimeout", 0);
		m_strODBCDriverVersion = file.GetString("ODBCDriverVersion", "");
		m_nPortNumber = file.GetInt("PortNumber", 0);
		
		if (m_strPasswordPT != "")
		{
			CAESHelpers::PlainTextToAESHex(m_strPasswordPT, m_strPasswordAES);
			Write();
		}
		else if (m_strPasswordAES != "")
		{
			CAESHelpers::AESHexToPlainText(m_strPasswordAES, m_strPasswordPT);
		}
		else
		{
			m_strPasswordPT.Format("SHADwell%6.6d##",
				Sysset.GetSerialNo());
		}
	}

	return bReply;
}

//*******************************************************************

bool CSQLConnectionOptions::Write()
{
	CSSIniFile file;
	file.Set("LANConnect", m_bLANConnect);
	file.Set("LANServerName", m_strLANServerName);

	if (m_strPasswordAES != "")
	{
		file.Set("Password", m_strPasswordAES);
	}

	file.Set("QueryTimeout", m_nQueryTimeout);
	file.Set("ODBCDriverVersion", m_strODBCDriverVersion);
	file.Set("PortNumber", m_nPortNumber);

	return file.Write(Filenames.GetSQLConnectionOptionsFilenameNew());
}

//*******************************************************************

void CSQLConnectionOptions::CopyFrom(CSQLConnectionOptions& source )
{
	m_bLANConnect = source.m_bLANConnect;
	m_strLANServerName = source.m_strLANServerName;
	m_strPasswordPT = source.m_strPasswordPT;
	m_strPasswordAES = source.m_strPasswordAES;
	m_nQueryTimeout = source.m_nQueryTimeout;
	m_strODBCDriverVersion = source.m_strODBCDriverVersion;
	m_nPortNumber = source.m_nPortNumber;
}

//*******************************************************************

void CSQLConnectionOptions::GetODBCDriverVersions(CReportUniqueMap<CMapKeyInt, CMapDataEmpty>& mapVersions)
{
	mapVersions.RemoveAll();
	CCSV csv(m_strODBCDriverVersion);

	for (int n = 0; n < csv.GetSize(); n++)
	{
		int nVer = csv.GetInt(n);
		if ((nVer >= 0) && (nVer <= 99))
		{
			mapVersions.Insert(CMapKeyInt(nVer), CMapDataEmpty());
		}
	}
}

//*******************************************************************

CString CSQLConnectionOptions::GetODBCDriverString(int nVer)
{
	if ((nVer >= 1) && (nVer <= 99))
	{
		m_strDriverConnectionString.Format("ODBC Driver %d for SQL Server", nVer);
	}
	else
	{
		m_strDriverConnectionString = "SQL Server";
	}

	return m_strDriverConnectionString;
}

//*******************************************************************