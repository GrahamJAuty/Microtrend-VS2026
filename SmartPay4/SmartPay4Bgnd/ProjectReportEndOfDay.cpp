//*****************************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*****************************************************************************
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
//*****************************************************************************

CReportEndOfDay::CReportEndOfDay ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strBackgroundError = "";
	Init();
}

//********************************************************************
// CALLED FROM BACKGROND PROCESSOR
// returns nEOD_FALSE / nEOD_PERFORMED / nEOD_EXISTS / nEOD_INPROGRESS

int CReportEndOfDay::Background()
{
	m_strBackgroundError = SetEODInProgress();								// try & set EOD in progress
	if (m_strBackgroundError != "")											// already running
	{
		return nEOD_INPROGRESS;												// EOD in progress
	}

	int nReply = nEOD_FALSE;
	m_strTitle = "EOD Report";											// 14/04/2015 - v1.06c
	m_nEODReportType = nEOD_EXECUTE;											// 14/04/2015 - v1.06c 

	bool bHaveEOD = FALSE;
	CString strEODTime = "";

	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		
		CString strKeyDate = "";
		strKeyDate.Format("%4.4d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay());

		CSQLRowClosingBalance RowCB;
		RowCB.SetKeyDate(strKeyDate);

		CSQLRepositoryClosingBalance RepoClosing;
		if (RepoClosing.SelectRow(RowCB, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			bHaveEOD = TRUE;
			strEODTime = RowCB.GetTime();
		}
	}

	if (bHaveEOD == FALSE)												// see if EOD been done today 
	{
		bool bHadLock = FALSE;
		int nError = nREPORT_NOERROR;
		CString strReportFile = "";

		{
			CSmartPayLockRequest Request;
			Request.m_bWantLockServerInner = TRUE;
			
			CSmartPayLockManager LockManager;
			int nLockError = LockManager.AcquireLocks(Request);		
			
			// pause server as report may take a few minutes
			if (LOCKMANAGER_OK == nLockError)
			{
				// do EOD report
				bHadLock = TRUE;
				strReportFile = Filenames.GetReportFilename(m_strReportLabel);
				nError = EndOfDayReport(strReportFile, TRUE);
			}
		}

		if (TRUE == bHadLock)
		{
			CRepmanHandler repman;											// in case need to translate error number to text
			if (nError == nREPORT_NOERROR)
			{
				nReply = nEOD_PERFORMED;									// all went ok

				int nSaveAsType = m_texts.GetSaveAsType();
				if (nSaveAsType != nSAVEAS_NONE && m_strEODSaveFilename != "")
				{
					repman.SetupSaveAs(nSaveAsType, m_texts.GetSaveAsFilename(m_strEODSaveFilename));		// nSAVEAS_CSV / nSAVEAS_HTML
					repman.SaveReportAs(strReportFile);
				}
			}
			else
			{
				m_strBackgroundError = repman.GetRepmanHandlerError(nError);			// get report error text so it can be saved in background log
			}
		}
	}
	else
	{
		m_strBackgroundError.Format("Already performed @ %s", (const char*) strEODTime);
		nReply = nEOD_EXISTS;												// already done	today //21/09/2016
	}

	ClearEODInProgress();														// clear in progress indicator

	return nReply;
}

//***********************************************************************
