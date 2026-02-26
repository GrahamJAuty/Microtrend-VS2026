//**********************************************************************
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//**********************************************************************
#include "DbExportEditor.h"
#include "DbReportHandler.h"
#include "LoyaltyManager.h"
#include "PhotoIDListData.h"
#include "RecSort.h"
#include "RepmanHandler.h"
#include "WorkingDlg.h"
//**********************************************************************
#include "ReportPhotoIDList.h"
//**********************************************************************

CReportPhotoIDList::CReportPhotoIDList ( CWnd* pParent )
{
	m_pParent			= pParent;
	m_strReportLabel	= "R011";
}

/**********************************************************************/

bool CReportPhotoIDList::Display()
{
	CDbExportEditor editor(FALSE, NULL);
	if (editor.EditLabel(m_strReportLabel) == FALSE)
	{
		return FALSE;
	}

	CDbReportHandler reporter{};
	if (reporter.Read(m_strReportLabel) == FALSE)
	{
		return FALSE;
	}

	CString strTitle = reporter.GetReportTitle();
	CString strDataFilename = reporter.GetReportFilename();

	while (m_dlgList.DoModal() == IDOK)
	{
		CString strWhere = "";
		CSQLBuilderBind BuilderBind{};
		CSQLRepositoryAccount repoAccount;
		repoAccount.PrepareExportWhere(reporter, -1, strWhere, BuilderBind);

		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
		RecordSet.m_strSort = reporter.GetSQLOrderBy();
		
		int nCount = 0;
		int nChecked = 0;
		bool bGotData = FALSE;
		bool bCancelled = FALSE;
		CSSFile fileOut;
		CPhotoIDListData data;

		{
			CWorkingDlg WorkingDlg("Creating Photo ID List Report");
			WorkingDlg.Create();

			CSQLRowAccountFull RowAccount;
			while (RecordSet.StepSelectAll(RowAccount) == TRUE)
			{
				if (WorkingDlg.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
				{
					bCancelled = TRUE;
					break;
				}

				if (reporter.NonSQLFilter(RowAccount) == TRUE)
				{
					if (FALSE == bGotData)
					{
						if (fileOut.Open(strDataFilename, "wb") == FALSE)
						{
							CRepmanHandler repman(strTitle, m_pParent);
							repman.DisplayError(nREPORT_CREATEFAIL);
							break;
						}
						bGotData = TRUE;
					}

					CImageHandler photo;
					CString strFilename = photo.GetFirst(RowAccount.GetUserIDString());

					if ( (strFilename == "") && ( TRUE == m_dlgList.m_bSkipNoPhoto))
					{
						continue;
					}

					if (fileOut.WriteLine(data.GetReportData(RowAccount,strFilename)) == TRUE)
					{
						++nCount;
					}
				}
			}
		}

		if (TRUE == bGotData)
		{
			fileOut.Close();
		}

		if (TRUE == bCancelled)
		{
			break;
		}

		if (FALSE == bGotData)
		{
			CRepmanHandler repman(strTitle, m_pParent);
			repman.DisplayError(nREPORT_NOMATCH);
			break;
		}

		{
			int nErrorNo;
			CRepmanHandler repman(strTitle, m_pParent);
			if (nCount > 0)
			{
				nErrorNo = repman.ShowBarcode(strDataFilename, data.m_strLabelName, m_dlgList.m_bPrintReqd);
			}
			else
			{
				nErrorNo = nREPORT_NOMATCH;
			}

			repman.DisplayError(nErrorNo);
		}
	}

	return TRUE;
}

//**********************************************************************
