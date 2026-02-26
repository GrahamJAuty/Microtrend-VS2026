/******************************************************************************/
#include "..\SPosLoyalty4Manager\WorkingDlg.h"
/******************************************************************************/
#include "AuditRecord_base.h"
/******************************************************************************/
#include "LoyaltyAuditAppend.h"
/******************************************************************************/

CLoyaltyAuditAppend::CLoyaltyAuditAppend(const char* szFilename)
{
	m_strAuditFile = szFilename;
}

/******************************************************************************/

bool CLoyaltyAuditAppend::AppendFile ( const char* szFilename )
{
	bool bReply = FALSE;

	if (m_strAuditFile == "")
	{
		m_strAuditFile = Filenames.GetAuditFilename();
	}

	CSSFile fileOut;
	if ( ( bReply = fileOut.Open ( m_strAuditFile, "ab" ) ) == TRUE )		// main audit file
	{
		CSSFile fileIn;
		bReply = FALSE;

		if ( fileIn.Open ( szFilename, "rb" ) == TRUE )	// new file
		{
			CAuditRecord_base atc;
			while ( atc.ReadLine ( &fileIn ) == TRUE )			// read line from new file
			{
				bReply = atc.SaveLineToFile( &fileOut, FALSE );		// line being saved is not new
			}	
			fileIn.Close();
		}
		fileOut.Close();
	}

	return bReply;
}

//**********************************************************************

bool CLoyaltyAuditAppend::AppendArray(CStringArray& array)
{
	bool bReply = FALSE;

	if (m_strAuditFile == "")
	{
		m_strAuditFile = Filenames.GetAuditFilename();
	}

	CSSFile fileOut;
	if ((bReply = fileOut.Open(m_strAuditFile, "ab")) == TRUE)		// main audit file
	{

#ifndef LOYSERVER
		CWorkingDlg WorkingDlg("Appending Audit Lines", FALSE);
		WorkingDlg.Create();
#endif

		CAuditRecord_base atc;
		for (int n = 0; n < array.GetSize(); n++)
		{

#ifndef LOYSERVER

			if ((n % 10) == 0)
			{
				WorkingDlg.SetCaption2RecordsOfTotal(n, array.GetSize());
			}
#endif

			CString str = array.GetAt(n);

			if (str != "")
			{
				atc.ReadFromString(str);
				bReply = atc.SaveLineToFile(&fileOut, FALSE);		// line being saved is not new
			}
		}

		fileOut.Close();
	}

	return bReply;
}

//******************************************************************************
