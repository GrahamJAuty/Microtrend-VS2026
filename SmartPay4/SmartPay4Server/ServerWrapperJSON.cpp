/**********************************************************************/
#include <chrono>
/**********************************************************************/
#include "CkJsonObject.h"
#include "CkJsonArray.h"
/**********************************************************************/
#include "..\SmartPay4Shared\FilenameHandler.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

static inline _int64 GetUnixEpochMillis()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/**********************************************************************/

CString CServerWrapper::GetIncomingJSONString()
{
	CString strJSON = "";
	for (int n = 0; n < m_LoyReqThreadManager.GetRequestLineCount(); n++)
	{
		strJSON += m_LoyReqThreadManager.GetRequestLine(n);
	}
	return strJSON;
}

/**********************************************************************/

void CServerWrapper::GetIncomingJSONParams(CkJsonObject& jsonRequest, CJSONIncomingParams& params)
{
	//VERSION
	CString strVersion = "";
	if (CheckJSONParam(&jsonRequest, "version", strVersion) == TRUE)
	{
		params.m_nVersion = atoi((const char*)strVersion);
	}

	//TIMESTAMP
	CString strTimeStamp = "";
	CheckJSONParam(&jsonRequest, "timeStamp", strTimeStamp);
	params.m_nTimeStamp = _atoi64(strTimeStamp);

	//SERIAL NUMBER
	CString strSerialNumber = "";
	if (CheckJSONParam(&jsonRequest, "serialNumber", strSerialNumber) == TRUE)
	{
		params.m_nSerialNumber = atoi(strSerialNumber);
	}

	//MESSAGE TYPE
	CheckJSONParam(&jsonRequest, "messageType", params.m_strMessageType);
	
	//SUBTYPE
	CheckJSONParam(&jsonRequest, "subType", params.m_strSubType);
	
	//DATA ENTITY
	CheckJSONParam(&jsonRequest, "dataEntity", params.m_strDataEntity);
}

/**********************************************************************/

bool CServerWrapper::ValidateIncomingJSONParams(CJSONIncomingParams& params, CJSONIncomingError& error)
{
	params.m_nSubType = 0;
	params.m_nDataEntity = 0;

	if (params.m_nVersion != 1)
	{
		error.m_nCode = 400;
		error.m_strText = "Unknown version number";
		return FALSE;
	}

	if (params.m_nTimeStamp <= 0)
	{
		error.m_nCode = 400;
		error.m_strText = "Missing or invalid timestamp";
		return FALSE;
	}

	if (params.m_nSerialNumber <= 0)
	{
		error.m_nCode = 400;
		error.m_strText = "Missing or invalid serial number";
		return FALSE;
	}

	{
		CString strMessageType = params.m_strMessageType;
		strMessageType.MakeLower();
		if (strMessageType != "request")
		{
			error.m_nCode = 400;
			error.m_strText = "Missing or invalid message type";
			return FALSE;
		}
	}

	{
		CString strSubType = params.m_strSubType;
		strSubType.MakeLower();

		if (strSubType == "data")
		{
			params.m_nSubType = 1;
		}
		else if (strSubType == "report")
		{
			params.m_nSubType = 2;
		}
		else
		{
			error.m_nCode = 400;
			error.m_strText = "Missing or invalid sub type";
			return FALSE;
		}
	}

	{
		CString strDataEntity = params.m_strDataEntity;
		strDataEntity.MakeLower();

		switch (params.m_nSubType)
		{
		case 1:
			if (strDataEntity.Right(11) == "datarequest")
			{
				CString strReportType = strDataEntity.Left(strDataEntity.GetLength() - 11);

				if (strReportType == "eodpreviousreports")
				{
					params.m_nDataEntity = 1;
				}
			}
			break;

		case 2:
			if (strDataEntity.Right(13) == "reportrequest")
			{
				CString strReportType = strDataEntity.Left(strDataEntity.GetLength() - 13);

				if (strReportType == "eodprevious")
				{
					params.m_nDataEntity = 1;
				}
				else if (strReportType == "eodconsolidated")
				{
					params.m_nDataEntity = 2;
				}
			}
			break;
		}

		if (0 == params.m_nDataEntity)
		{
			error.m_nCode = 400;
			error.m_strText = "Missing or invalid date entity type";
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CServerWrapper::CreateJSONErrorReply(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error)
{
	bool bResult = FALSE;

	CString strTimeStamp = "";
	strTimeStamp.Format("%I64d", GetUnixEpochMillis());

	jsonReply.AppendInt("version", 1); //MY VERSION
	jsonReply.AddNumberAt(-1, "timeStamp", strTimeStamp); //MY TIMESTAMP
	jsonReply.AppendInt("serialNumber", params.m_nSerialNumber); //ECHO BACK
	jsonReply.AppendString("messageType", "response");
	jsonReply.AppendString("subType", "error");
	jsonReply.AppendString("dataEntity", "error");

	CkJsonObject* jsonReplyData = jsonReply.AppendObject("data");

	if (jsonReplyData != NULL)
	{
		jsonObjectCollector.RememberJSONObject(jsonReplyData);

		jsonReplyData->AppendInt("code", error.m_nCode);
		jsonReplyData->AppendString("message", error.m_strText);

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CServerWrapper::ProcessJSONRequest()
{
	if (Server.GetLogAction2Flag() == TRUE)
	{
		CString strMsg = "";
		strMsg.Format("==> REQ: %s",
			(const char*)m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	{
		CJSONObjectCollector jsonObjectCollector;
		CJSONIncomingParams IncomingParams;
		CJSONIncomingError IncomingError;

		CkJsonObject jsonRequest;
		if (jsonRequest.Load(GetIncomingJSONString()) == FALSE)
		{
			IncomingError.m_nCode = 400;
			IncomingError.m_strText = "Unable to parse JSON";
		}

		if (0 == IncomingError.m_nCode)
		{
			GetIncomingJSONParams(jsonRequest, IncomingParams);

			CString strMsg = "";
			strMsg.Format("==> VS=%d, TS=%I64d, SN=%d, MT=%s, ST=%s, DE=%s",
				IncomingParams.m_nVersion,
				IncomingParams.m_nTimeStamp,
				IncomingParams.m_nSerialNumber,
				(const char*)IncomingParams.m_strMessageType,
				(const char*)IncomingParams.m_strSubType,
				(const char*)IncomingParams.m_strDataEntity);
			MessageLogger.LogSystemMessage(strMsg);

			ValidateIncomingJSONParams(IncomingParams, IncomingError);
		}

		if (0 == IncomingError.m_nCode)
		{
			IncomingError.m_nCode = 400;
			IncomingError.m_strText = "Unknown request type";

			CkJsonObject jsonReply;

			switch (IncomingParams.m_nSubType)
			{
			case 1: //DATA
				switch (IncomingParams.m_nDataEntity)
				{
				case 1: //EOD PREVIOUS REPORTS DATA REQUEST
					ProcessJSONDataRequestEODPrevious(jsonObjectCollector, jsonRequest, jsonReply, IncomingParams, IncomingError);
					break;

				default:
					break;
				}
				break;

			case 2: //REPORT
				switch (IncomingParams.m_nDataEntity)
				{
				case 1: //EOD PREVIOUS REPORT REQUEST
					ProcessJSONReportRequestEODPrevious(jsonObjectCollector, jsonRequest, jsonReply, IncomingParams, IncomingError);
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}

			if (0 == IncomingError.m_nCode)
			{
				SaveJSONReply(jsonReply);
			}
		}

		if ( 0 != IncomingError.m_nCode)
		{
			CkJsonObject jsonReply;
			if (CreateJSONErrorReply(jsonObjectCollector, jsonReply, IncomingParams, IncomingError) == FALSE)
			{
				m_LoyReqThreadManager.AddResponseLine("0");
			}
			else
			{
				SaveJSONReply(jsonReply);
			}
		}
	}

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		int nThreadIndex = m_LoyReqThreadManager.GetCurrentThread();

		m_LoyRspThreadManager.QueueResponseHeader(nThreadIndex);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(nThreadIndex, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(nThreadIndex);

		if (Server.GetLogAction2Flag() == TRUE)
		{
			CString strMsg = "";
			strMsg += "<== RSP: ";
			strMsg += m_LoyReqThreadManager.GetResponseLine(1);
			MessageLogger.LogSystemMessage(strMsg);
		}
	}
}

/**********************************************************************/

void CServerWrapper::ProcessJSONDataRequestEODPrevious(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonRequest, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error)
{
	error.m_nCode = 0;
	error.m_strText = "";

	CkJsonObject* jsonRequestData = jsonRequest.ObjectOf("data");

	if (jsonRequestData == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "No data section found";
		return;
	}
	jsonObjectCollector.RememberJSONObject(jsonRequestData);

	CString strDataName = jsonRequestData->stringOf("dataName");

	if (strDataName != "eodPreviousReports")
	{
		error.m_nCode = 400;
		error.m_strText = "Mismatched data name";
		return;
	}

	bool bGotLimit = FALSE;
	CString strError = "";
	int nLimit = 0;
	CheckJSONInt(jsonRequestData, "limit", 0, 9999, bGotLimit, nLimit, strError);

	if (strError != "")
	{
		error.m_nCode = 400;
		error.m_strText = strError;
		return;
	}
	else if (FALSE == bGotLimit)
	{
		error.m_nCode = 400;
		error.m_strText = "Missing limit parameter";
		return;
	}

	bool bGotOffset = FALSE;
	int nOffset = 0;
	CheckJSONInt(jsonRequestData, "offset", 0, 9999, bGotOffset, nOffset, strError);

	if (strError != "")
	{
		error.m_nCode = 400;
		error.m_strText = strError;
		return;
	}
	else if (FALSE == bGotOffset)
	{
		error.m_nCode = 400;
		error.m_strText = "Missing offset parameter";
		return;
	}

	int nTotalRows = 0;
	CArray<CEODListInfo, CEODListInfo> arrayEODListInfo;
	CReportEndOfDay::GetEODDateInfo(arrayEODListInfo, nTotalRows, nOffset, nLimit);

	CString strTimeStamp = "";
	strTimeStamp.Format("%I64d", GetUnixEpochMillis());

	jsonReply.AppendInt("version", 1); //MY VERSION
	jsonReply.AddNumberAt(-1, "timeStamp", strTimeStamp); //MY TIMESTAMP
	jsonReply.AppendInt("serialNumber", params.m_nSerialNumber); //ECHO BACK
	jsonReply.AppendString("messageType", "response");
	jsonReply.AppendString("subType", "data");
	jsonReply.AppendString("dataEntity", "eodPreviousReportsDataResponse");

	CkJsonObject* jsonReplyData = jsonReply.AppendObject("data");

	if (jsonReplyData == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "Unable to create data section for reply";
		return;
	}
	jsonObjectCollector.RememberJSONObject(jsonReplyData);

	jsonReplyData->AppendString("dataName", "eodPreviousReport");
	jsonReplyData->AppendInt("limit", nLimit);
	jsonReplyData->AppendInt("offset", nOffset);
	jsonReplyData->AppendInt("returnedRows", arrayEODListInfo.GetSize());
	jsonReplyData->AppendInt("totalRows", nTotalRows);

	if (jsonReplyData->AddArrayAt(-1, "rows") == FALSE)
	{
		error.m_nCode = 400;
		error.m_strText = "Unable to create reports array for reply";
		return;
	}

	CkJsonArray* jsonArrayReports = jsonReplyData->ArrayAt(jsonReplyData->get_Size() - 1);

	if (jsonArrayReports == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "Unable to create reports array for reply";
		return;

	}
	jsonObjectCollector.RememberJSONArray(jsonArrayReports);

	for (int n = 0; n < arrayEODListInfo.GetSize(); n++)
	{
		CEODListInfo eodListInfo = arrayEODListInfo.GetAt(n);

		CString strDisplayDate = eodListInfo.m_strDisplayDate;
		CString strDisplayTime = eodListInfo.m_strDisplayTime;

		if ((strDisplayDate.GetLength() != 10) || (strDisplayTime.GetLength() != 5))
		{
			continue;
		}

		CString strFilename = strDisplayDate.Mid(6, 4) + strDisplayDate.Mid(3, 2) + strDisplayDate.Left(2);

		if (::TestNumeric(strFilename) == FALSE)
		{
			continue;
		}

		COleDateTime oleDateTime = COleDateTime(
			atoi(strFilename.Left(4)),
			atoi(strFilename.Mid(4, 2)),
			atoi(strFilename.Right(2)),
			atoi(strDisplayTime.Left(2)),
			atoi(strDisplayTime.Right(2)),
			0);

		if (COleDateTime::valid != oleDateTime.m_status)
		{
			continue;
		}

		if (jsonArrayReports->AddObjectAt(-1) == FALSE)
		{
			error.m_nCode = 400;
			error.m_strText = "Unable to create report object for reply";
			return;
		}

		CkJsonObject* jsonReport = jsonArrayReports->ObjectAt(jsonArrayReports->get_Size() - 1);
		if (jsonReport == NULL)
		{
			error.m_nCode = 400;
			error.m_strText = "Unable to create report object for reply";
			return;
		}
		jsonObjectCollector.RememberJSONObject(jsonReport);

		CString strDateTime = "";
		::ConvertOleDateTimeTo8601Time(oleDateTime, strDateTime);

		jsonReport->AppendString("dateTime", strDateTime);
		jsonReport->AppendString("displayDate", eodListInfo.m_strDisplayDate);
		jsonReport->AppendString("displayTime", eodListInfo.m_strDisplayTime);
		jsonReport->AppendInt("balance1", int(floor(eodListInfo.m_dPurse1 * 100.0)));
		jsonReport->AppendInt("balance2", int(floor(eodListInfo.m_dPurse3 * 100.0)));
		jsonReport->AppendString("filename", strFilename + ".rep");
	}
}

/**********************************************************************/

void CServerWrapper::ProcessJSONReportRequestEODPrevious(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonRequest, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error)
{
	error.m_nCode = 0;
	error.m_strText = "";

	CkJsonObject* jsonRequestData = jsonRequest.ObjectOf("data");

	if (jsonRequestData == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "No data section found";
		return;
	}
	jsonObjectCollector.RememberJSONObject(jsonRequestData);

	CString strDataName = jsonRequestData->stringOf("reportName");

	if (strDataName != "eodPrevious")
	{
		error.m_nCode = 400;
		error.m_strText = "Mismatched report name";
		return;
	}

	CkJsonObject* jsonRequestParams = jsonRequestData->ObjectOf("params");

	if (jsonRequestParams == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "No params section found";
		return;
	}
	jsonObjectCollector.RememberJSONObject(jsonRequestParams);

	CString strFilename = jsonRequestParams->stringOf("filename");
	strFilename.MakeUpper();

	bool bValidFilename = FALSE;

	if (strFilename.GetLength() == 12)
	{
		if (strFilename.Right(4) == ".REP")
		{
			if (::TestNumeric(strFilename.Left(8)) == TRUE)
			{
				COleDateTime dateTest = COleDateTime(
					atoi(strFilename.Left(4)),
					atoi(strFilename.Mid(4,2)),
					atoi(strFilename.Mid(6,2)),
					0,
					0,
					0);
				
				if (COleDateTime::valid == dateTest.m_status)
				{
					bValidFilename = TRUE;
				}
			}
		}
	}
	
	if ( FALSE == bValidFilename)
	{
		error.m_nCode = 400;
		error.m_strText = "Invalid or missing filename parameter";
		return;
	}

	CSSFile fileEODReport;
	if (fileEODReport.Open(Filenames.GetEODReportPath(strFilename.Left(8)), "rb") == FALSE)
	{
		error.m_nCode = 400;
		error.m_strText = "Unable to open report file";
		return;
	}

	CString strTimeStamp = "";
	strTimeStamp.Format("%I64d", GetUnixEpochMillis());

	jsonReply.AppendInt("version", 1); //MY VERSION
	jsonReply.AddNumberAt(-1, "timeStamp", strTimeStamp); //MY TIMESTAMP
	jsonReply.AppendInt("serialNumber", params.m_nSerialNumber); //ECHO BACK
	jsonReply.AppendString("messageType", "response");
	jsonReply.AppendString("subType", "report");
	jsonReply.AppendString("dataEntity", "repmanResponse");

	CkJsonObject* jsonReplyData = jsonReply.AppendObject("data");

	if (jsonReplyData == NULL)
	{
		error.m_nCode = 400;
		error.m_strText = "Unable to create data section for reply";
		return;
	}
	jsonObjectCollector.RememberJSONObject(jsonReplyData);

	CString strData = "";
	CString strBuffer = "";
	while (fileEODReport.ReadString(strBuffer) == TRUE)
	{
		strData += strBuffer + "\n";
	}

	fileEODReport.Close();

	jsonReplyData->AppendString("data", strData);
}

/**********************************************************************/

bool CServerWrapper::CheckJSONParam(CkJsonObject* jsonParent, CString strLabel, CString& strValue)
{
	strValue = "";
	bool bResult = FALSE;

	if (jsonParent->IndexOf(strLabel) != -1)
	{
		bResult = TRUE;
		strValue = jsonParent->stringOf(strLabel);
	}

	return bResult;
}

/**********************************************************************/

void CServerWrapper::ApplyJSONError(CkJsonObject* jsonReplyData, int nErrorCode, CString strErrorText)
{
	jsonReplyData->AppendInt("code", nErrorCode);
	jsonReplyData->AppendString("message", strErrorText);
}

/**********************************************************************/

void CServerWrapper::SaveJSONReply(CkJsonObject& jsonReply)
{
	jsonReply.put_EmitCompact(FALSE);
	CString strReply = jsonReply.emit();
	m_LoyReqThreadManager.AddResponseLine(strReply);
}

/**********************************************************************/

void CServerWrapper::CheckJSONInt(CkJsonObject* jsonParent, const CString strLabel, const int nMin, const int nMax, bool& bGotValue, int& nValue, CString& strError)
{
	nValue = 0;
	bGotValue = FALSE;
	strError = "";

	if (jsonParent->IndexOf(strLabel) != -1)
	{
		bGotValue = TRUE;
		nValue = jsonParent->IntOf(strLabel);

		if ((nValue < nMin) || (nValue > nMax))
		{
			strError.Format("%s out of range",
				(const char*)strLabel);
		}
	}
}

/**********************************************************************/