/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "..\CommonEcrStock\PropertySheetSystemDataList.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnDatabaseEditData() 
{
	/*
	CReportConsolidationArray<CSortedIntItem> arraySerialNo;
	CReportConsolidationArray<CSortedIntItem> arrayLineNo;
	
	{
		CString strFileIn = ".\\ecrset.dbs";
		CString strFileOut1 = ".\\ecrset1.dbs";
		CString strFileOut2 = ".\\ecrset2.dbs";

		CSSFile fileIn;
		fileIn.Open(strFileIn, "rb ");

		CSSFile fileOut1;
		fileOut1.Open(strFileOut1, "wb ");

		CSSFile fileOut2;
		fileOut2.Open(strFileOut2, "wb ");

		CString strBuffer;
		while (fileIn.ReadString(strBuffer) == TRUE)
		{
			CCSV csv(strBuffer);

			{
				CSortedIntItem item;
				item.m_nItem = csv.GetInt(0);
				arrayLineNo.Consolidate(item);
			}

			int nSerialNo = csv.GetInt(1);
			CString strTest = csv.GetString(2);

			if ( (strTest == "Demonstration System") && ( nSerialNo > 100 ) && ( nSerialNo <= 9000 ) && ( nSerialNo != 8291 ) && ( nSerialNo != 8997 ) )
			{
				CSortedIntItem item;
				item.m_nItem = nSerialNo;
				arraySerialNo.Consolidate(item);
				fileOut2.WriteLine(strBuffer);
			}
			else
			{
				fileOut1.WriteLine(strBuffer);
			}
		}
	}

	{
		CString strFileIn = ".\\register.dbs";
		CString strFileOut1 = ".\\register1.dbs";
		CString strFileOut2 = ".\\register2.dbs";
		CString strFileOut3 = ".\\register3.dbs";

		CSSFile fileIn;
		fileIn.Open(strFileIn, "rb ");

		CSSFile fileOut1;
		fileOut1.Open(strFileOut1, "wb ");

		CSSFile fileOut2;
		fileOut2.Open(strFileOut2, "wb ");

		CSSFile fileOut3;
		fileOut3.Open(strFileOut3, "wb ");

		CString strBuffer;
		while (fileIn.ReadString(strBuffer) == TRUE)
		{
			CCSV csv(strBuffer);

			CSortedIntItem item;
			item.m_nItem = csv.GetInt(0);
			
			int nPos;
			if ( arraySerialNo.Find(item, nPos ) == TRUE )
			{
				fileOut2.WriteLine(strBuffer);
			}
			else
			{
				CSortedIntItem item;
				item.m_nItem = csv.GetInt(1);
				if ( arrayLineNo.Find( item, nPos ) == TRUE )
				{
					fileOut1.WriteLine(strBuffer);
				}
				else
				{
					fileOut3.WriteLine(strBuffer);
				}
			}
		}
	}

	return;
	*/

	AutoExpireHistory.ExpireReportsNow();

	CPropertySheetSystemDataList propSheet( this );
	propSheet.DoModal();

	if ( propSheet.GetDataValidationErrorFlag() == TRUE )
	{
		DatabaseValidator.SetErrorFlag( TRUE );
		PostMessage( WM_CLOSE );
	}
}

/**********************************************************************/
