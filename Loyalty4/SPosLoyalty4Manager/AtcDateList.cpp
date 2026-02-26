/**********************************************************************/
#include "WorkingDlg.h"
#include "AuditRecord.h"
/**********************************************************************/
#include "AtcDateList.h"
/**********************************************************************/

CAtcDateList::CAtcDateList(const char* szFilename)
{
	m_strLastDate = "";
	m_strDate = "";
	m_nCount = 0;;

	CSSFile file;
	if (file.Open(szFilename, "rb") == TRUE)
	{
		CWorkingDlg WorkingDlg("Scanning Audit File", TRUE);
		WorkingDlg.Create();

		CAuditRecord atc;
		while (atc.ReadLine(&file) == TRUE)
		{
			if ( WorkingDlg.SetCaption2PercentDone(file.GetPercentPosition(), TRUE) == TRUE )
			{
				break;
			}

			if (atc.GetDateFiled() == m_strLastDate)
			{
				++m_nCount;
			}
			else
			{
				Flush();						// flush date into array
				m_strLastDate = atc.GetDateFiled();
				m_nCount = 1;					// new date

				CString strStatus = "";
				strStatus.Format("Scanning Audit File - %s", 
					(const char*)m_strLastDate);

				WorkingDlg.SetCaption1(strStatus);
			}
		}
		Flush();								// flush date into array

		file.Close();
	}
}

/**********************************************************************/

void CAtcDateList::Flush()
{
	if ( m_nCount > 0 )
	{
		CCSV csv;
		csv.Add ( m_strLastDate );
		csv.Add ( m_nCount );

		m_arrayDates.Add ( csv.GetLine() );
	}
}

/**********************************************************************/
	
const char* CAtcDateList::GetAtDate ( int nIndex )
{
	CCSV csv ( m_arrayDates.GetAt(nIndex) );
	m_strDate = csv.GetString(0);
	return m_strDate;
}

int CAtcDateList::GetAtCount ( int nIndex )
{
	CCSV csv ( m_arrayDates.GetAt(nIndex) );
	return csv.GetInt(1);
}

//*********************************************************************
// remove entries from list upto & including passed date

void CAtcDateList::RemoveEntries ( CSSDate& pDate )
{
	while ( TRUE )
	{
		if ( m_arrayDates.GetSize() == 0 )			// check if have any entries
			break;

		CSSDate date;
		date.SetDate ( GetAtDate(0) );				// get date at head if list

		if ( date > pDate )							// see if our of range
			break;

		m_arrayDates.RemoveAt(0);					// delete head of list
	}
}

//*******************************************************************