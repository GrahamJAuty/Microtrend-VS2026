//**********************************************************************
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//**********************************************************************
#include "DbExportEditor.h"
#include "PhotoIDListData.h"
//**********************************************************************
#include "ReportPhotoIDList.h"
//**********************************************************************

CReportPhotoIDList::CReportPhotoIDList ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strReportLabel	= "R011";
}

/**********************************************************************/

bool CReportPhotoIDList::Display()
{
	CDbExportEditor editor{};
	if (editor.EditExportLabel(m_strReportLabel) == FALSE)
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
		CWorkingDlg WorkingDlg("Filtering Database Records");
		WorkingDlg.Create();
		bool bCancelButton = FALSE;

		// scan database

		CArray<__int64, __int64> arrayUserID;

		CString strWhere = "";
		CSQLBuilderBind BuilderBind{};
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.PrepareExportWhere(reporter, -1, strWhere, BuilderBind);

		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
		RecordSet.m_strSort = reporter.GetSQLOrderBy();
		
		int nChecked = 0;
		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(++nChecked) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			if (reporter.NonSQLFilter(RowAccount) == TRUE)
			{
				arrayUserID.Add(RowAccount.GetUserID());
			}
		}

		if (bCancelButton == TRUE)
		{
			break;
		}

		if (arrayUserID.GetSize() == 0)
		{
			CRepmanHandler repman(strTitle, m_pParent);
			repman.DisplayError(nREPORT_NOMATCH);
			break;
		}

		CSSFile fileOut;
		if (fileOut.Open(strDataFilename, "wb") == FALSE)
		{
			CRepmanHandler repman(strTitle, m_pParent);
			repman.DisplayError(nREPORT_CREATEFAIL);
			break;
		}

		int nCount = 0;
		CPhotoIDListData data;

		WorkingDlg.SetCaption1("Finding Photo Filenames");

		for (int nIndex = 0; nIndex < arrayUserID.GetSize(); nIndex++)
		{
			if (WorkingDlg.SetCaption2PercentDone(50 + ((nIndex * 50L) / arrayUserID.GetSize())) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CString strUserID;
			strUserID.Format("%I64d", arrayUserID.GetAt(nIndex));

			CImageHandler image(FALSE);
			CString strFilename = image.GetImageFilepath(strUserID);

			if (strFilename == "" && m_dlgList.m_bSkipNoPhoto == TRUE)
			{
				continue;
			}

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(arrayUserID.GetAt(nIndex));

			if (RepoAccount.SelectRow(RowAccount,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				if (fileOut.WriteLine(data.GetReportData(RowAccount, strFilename)) == TRUE)
				{
					++nCount;
				}
			}
		}

		fileOut.Close();
		WorkingDlg.DestroyWindow();

		if (FALSE == bCancelButton)
		{
			int nErrorNo = 0;
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
