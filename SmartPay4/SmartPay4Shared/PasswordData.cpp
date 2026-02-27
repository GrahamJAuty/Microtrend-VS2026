//$$******************************************************************
#include "PasswordData.h"
//$$******************************************************************
const char* szDEFAULTMASTEROPTIONS = "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1";
const char* szDEFAULTUSEROPTIONS   = "0,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,0,1";
//$$******************************************************************

CPasswordData::CPasswordData()
{
	m_strFilename = "";
	m_strMagicPassword = "?EmlLEt?";			// reversed
	m_nPasswordInUse = 0;						// 0=master 1-20 = passwords
	m_arrayPassword.SetSize(21);
	m_arrayUserName.SetSize(21);
	m_arrayOptions.SetSize(21);
	m_bUsingMagicPassword = FALSE;
}

//$$******************************************************************

void CPasswordData::ReLoad()
{
	Read(m_strFilename);
}

//$$******************************************************************

void CPasswordData::Read(CString strFilename)
{
	m_strFilename = strFilename;
	m_strMagicPassword.MakeUpper();

	CString strText = "T00";						// text
	CString strUser = "U00";						// user
	CString strOpts = "F00";						// flags

	CSSIniFile file(SS_INI_HEX);
	file.Read(m_strFilename);

	m_arrayPassword.SetAt(0, file.GetString(strText, ""));
	m_arrayUserName.SetAt(0, "Master");
	m_arrayOptions.SetAt(0, file.GetString(strOpts, szDEFAULTMASTEROPTIONS));

	for (int nPassword = 1; nPassword <= 20; nPassword++)
	{
		strText.Format("T%2.2d", nPassword);
		strUser.Format("U%2.2d", nPassword);
		strOpts.Format("F%2.2d", nPassword);

		m_arrayPassword.SetAt(nPassword, file.GetString(strText, ""));
		m_arrayUserName.SetAt(nPassword, file.GetString(strUser, ""));
		m_arrayOptions.SetAt(nPassword, file.GetString(strOpts, szDEFAULTUSEROPTIONS));
	}
}

//$$******************************************************************

bool CPasswordData::Save()
{
	CString strText = "T00";							// text
	CString strUser = "U00";							// user name
	CString strOpts = "F00";							// Flags

	CSSIniFile file(SS_INI_HEX);
	file.Set(strText, m_arrayPassword.GetAt(0));
	file.Set(strUser, "Master");
	file.Set(strOpts, szDEFAULTMASTEROPTIONS);

	for (int nPassword = 1; nPassword <= 20; nPassword++)
	{
		strText.Format("T%2.2d", nPassword);
		strUser.Format("U%2.2d", nPassword);
		strOpts.Format("F%2.2d", nPassword);

		file.Set(strText, m_arrayPassword.GetAt(nPassword));
		file.Set(strUser, m_arrayUserName.GetAt(nPassword));
		file.Set(strOpts, m_arrayOptions.GetAt(nPassword));
	}

	return file.Write(m_strFilename);
}

//$$******************************************************************

void CPasswordData::SetMasterPassword(CString strText)
{
	m_arrayPassword.SetAt(0, strText);
}

//$$******************************************************************

void CPasswordData::SetUserDefault(int nPasswordNo)
{
	m_arrayPassword.SetAt(nPasswordNo, "");
	m_arrayOptions.SetAt(nPasswordNo, szDEFAULTUSEROPTIONS);
}

//$$******************************************************************

void CPasswordData::SetUserPassword(int nPasswordNo, CString strText)
{
	if ((nPasswordNo >= 0) && (nPasswordNo <= 20))
	{
		m_arrayPassword.SetAt(nPasswordNo, strText);
	}
}

//$$******************************************************************

void CPasswordData::SetUserName(int nPasswordNo, CString strText)
{
	if ((nPasswordNo >= 0) && (nPasswordNo <= 20))
	{
		m_arrayUserName.SetAt(nPasswordNo, strText);
	}
}

//$$******************************************************************

CString CPasswordData::GetPassword(int nPasswordNo)
{
	if ((nPasswordNo >= 0) && (nPasswordNo <= 20))
	{
		ExtractOptions(nPasswordNo);						// get option ticks
		m_nPasswordInUse = nPasswordNo;
		return m_arrayPassword.GetAt(nPasswordNo);		// return password text
	}

	return "";
}

//$$******************************************************************

void CPasswordData::ExtractOptions(int nPasswordNo)
{
	if ((nPasswordNo >= 0) && (nPasswordNo <= 20))
	{
		CString strOptions;
		if (0 == nPasswordNo)
		{
			strOptions = szDEFAULTMASTEROPTIONS;
		}
		else
		{
			strOptions = m_arrayOptions.GetAt(nPasswordNo);
		}

		CCSV csv(strOptions);	// get option ticks

		m_csvOptions.RemoveAll();
		for (int i = 0; i < csv.GetSize(); i++)
		{
			m_csvOptions.Add(csv.GetBool(i));	// copy into password options
		}
	}
}

//$$******************************************************************

CString CPasswordData::GetUserName(int nPasswordNo)
{
	if (0 == nPasswordNo)
	{
		return "Master";
	}
	else if ((nPasswordNo >= 1) && (nPasswordNo <= 20))
	{
		return m_arrayUserName.GetAt(nPasswordNo);
	}

	return "";
}

//$$******************************************************************

int CPasswordData::IsValid(CString strPassword)
{
	m_bUsingMagicPassword = FALSE;

	CString strUserPassword = strPassword;

	if (strUserPassword == "")
	{
		return -1;
	}

	for (m_nPasswordInUse = 0; m_nPasswordInUse <= 20; m_nPasswordInUse++)
	{
		if (strUserPassword == m_arrayPassword.GetAt(m_nPasswordInUse))
		{
			ExtractOptions(m_nPasswordInUse);			// get option ticks
			return m_nPasswordInUse;
		}
	}

	strUserPassword.MakeUpper();
	if (strUserPassword == m_strMagicPassword.MakeReverse())
	{
		m_nPasswordInUse = 0;
		m_strMagicPassword.MakeReverse();					// reverse back
		ExtractOptions(m_nPasswordInUse);					// get option ticks
		m_bUsingMagicPassword = TRUE;						// set magic password flag
		return m_nPasswordInUse;
	}

	return -1;
}

//$$******************************************************************

void CPasswordData::CopyOptions(int nSrcPasswordNo, int nDstPasswordNo)
{
	if ((nSrcPasswordNo > 0) && (nSrcPasswordNo < m_arrayOptions.GetSize()) &&
		(nDstPasswordNo > 0) && (nDstPasswordNo < m_arrayOptions.GetSize()) &&
		(nSrcPasswordNo != nDstPasswordNo))
	{
		m_arrayOptions.SetAt(nDstPasswordNo, m_arrayOptions.GetAt(nSrcPasswordNo));
	}
}

//$$******************************************************************
