//*********************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//*********************************************************************
#include "BromCom.hpp"
//*********************************************************************

static const char* szIPADDRESS_LIVE = "cloudmis.bromcom.com";
static const char* szOBJECTNAME_RDS	= "Nucleus/WebServices/ThirdParty/TPReadOnlyDataService.asmx";
static const char* szOBJECTNAME_WDS = "Nucleus/WebServices/ThirdParty/TPWriteDataService.asmx";
static const char* szNODE_NEWDATASET = "NewDataSet";

//*********************************************************************

CSSBromCom::CSSBromCom( CBromComData* pData )
{
	m_pData = pData;
}

//**********************************************************************
// Get Payments
//**********************************************************************

CString CSSBromCom::GetBromComPaymentOrders(const char* szLoginReply, const char* szCsvFilename, const char* szHeader, CWorkingDlg& WorkingDlg)
{	
	CString strError = "";

	CString strFilter = "";
	strFilter += "PaymentModifiedDate > '";
	strFilter += m_pData->GetSQLFilterDateTime();
	strFilter += "'";
	
	CSSSoap s("1.2"); //SOAP VERSION
	s.Attribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	s.Attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	s.SetSoapAction("http://tempuri.org/getEntityDataBySchoolID");
	s.BeginSoap("Body");
	s.Begin("getEntityDataBySchoolID");
	s.Attribute("xmlns", "http://tempuri.org/");
	s.Element("schoolID", m_pData->GetSchoolIDForConnection());
	s.Element("entityName", "OnlinePaymentOrders");
	s.Element("entityFilter", strFilter);
	s.Element("username", m_pData->GetUsernameForConnection());
	s.Element("password", m_pData->GetPasswordForConnection());
	s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetPaymentOrdersLogin.xml"), TRUE);
	}

	WorkingDlg.SetCaption1("Reading Orders from BromCom API");

	CString strIPAddress = szIPADDRESS_LIVE;

	strError = s.SendHttps(strIPAddress, szOBJECTNAME_RDS, szLoginReply, FALSE, System.GetInternetAgentString());			// no XML declaration check

	if (strError == "")
	{
		CXMLNodeArray* p1 = s.FindNode(szNODE_NEWDATASET);				// get new Batch ID
		if (p1 != NULL)
		{
			CXMLNodeArray arFound;
			if (s.FindNodes(arFound, "Table") > 0)
			{
				CSSUnsortedDatabase db;
				if (db.CreateSSDatabase(szCsvFilename, "", szHeader, 10) == DB_ERR_NONE)		// create new db & open in Read\Write.
				{
					for (int n = 0; n < arFound.GetSize(); n++)	// number of records
					{
						WorkingDlg.SetCaption2RecordsChecked(n, FALSE);

						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);

						for (int i = 0; i < node->GetSize(); i++)
						{
							if (node->GetAt(i)->IsText() == TRUE)
							{
								db.Set(node->GetAt(i)->GetName(), node->GetAt(i)->GetValue());
							}
						}
					}

					if (db.GetRecordCount() == 0 || m_pData->m_bKeepXmlFiles == FALSE)	//02/06/2017 - v1.15x
					{
						CFileRecycler::SendToRecycleBin(szLoginReply);								// delete original xml file
					}

					db.Close();
				}
				else
				{
					strError = db.GetError();
				}
			}
		}
	}
	return strError;
}

//**********************************************************************

CString CSSBromCom::GetBromComPaymentOrderItems(const char* szLoginReply, const char* szCsvFilename, const char* szHeader, CReportConsolidationArray<CSortedIntItem>& arrayOrderID, CWorkingDlg& WorkingDlg, bool& bNoItemsButOK)
{
	CString strError = "";
	bNoItemsButOK = FALSE;

	CCSV csvOrderID{};
	for (int n = 0; n < arrayOrderID.GetSize(); n++)
	{
		CSortedIntItem item{};
		arrayOrderID.GetAt(n, item);
		csvOrderID.Add(item.m_nItem);
	}

	CString strFilter = "";
	strFilter.Format("IsDinnerPayment = 'true' AND OrderID IN (%s)", 
		(const char*) csvOrderID.GetLine());

	CSSSoap s("1.2"); //SOAP VERSION
	s.Attribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	s.Attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	s.SetSoapAction("http://tempuri.org/getEntityDataBySchoolID");
	s.BeginSoap("Body");
	s.Begin("getEntityDataBySchoolID");
	s.Attribute("xmlns", "http://tempuri.org/");
	s.Element("schoolID", m_pData->GetSchoolIDForConnection());
	s.Element("entityName", "OnlinePaymentOrderItems");
	s.Element("entityFilter", strFilter);
	s.Element("username", m_pData->GetUsernameForConnection());
	s.Element("password", m_pData->GetPasswordForConnection());
	s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetPaymentOrderItemsLogin.xml"), TRUE);
	}

	WorkingDlg.SetCaption1("Reading Order Items from BromCom API");

	CString strIPAddress = szIPADDRESS_LIVE;

	strError = s.SendHttps(strIPAddress, szOBJECTNAME_RDS, szLoginReply, FALSE, System.GetInternetAgentString());			// no XML declaration check
	bool bGotItem = FALSE;

	if (strError == "")
	{
		CXMLNodeArray* p1 = s.FindNode(szNODE_NEWDATASET);				// get new Batch ID
		if (p1 != NULL)
		{
			CXMLNodeArray arFound;
			if (s.FindNodes(arFound, "Table") > 0)
			{
				CSSUnsortedDatabase db;
				if (db.CreateSSDatabase(szCsvFilename, "", szHeader, 10) == DB_ERR_NONE)		// create new db & open in Read\Write.
				{
					for (int n = 0; n < arFound.GetSize(); n++)	// number of records
					{
						WorkingDlg.SetCaption2RecordsChecked(n, FALSE);

						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);

						for (int i = 0; i < node->GetSize(); i++)
						{
							if (node->GetAt(i)->IsText() == TRUE)
							{
								db.Set(node->GetAt(i)->GetName(), node->GetAt(i)->GetValue());
							}
						}
					}

					bGotItem = (db.GetRecordCount() > 0);

					db.Close();
				}
				else
				{
					strError = db.GetError();
				}
			}
		}
		else
		{
			bNoItemsButOK = TRUE;
		}
	}

	if ((FALSE == bGotItem) || (FALSE == m_pData->m_bKeepXmlFiles))
	{
		// delete original xml file
		CFileRecycler::SendToRecycleBin(szLoginReply);								
	}

	return strError;
}

//**********************************************************************

CString CSSBromCom::SendBromComBalance(const char* szLoginReply, int nPersonId, double dBalance)
{
	CString strPersonId = "";
	strPersonId.Format("%d", nPersonId);

	CString strBalance = "";
	strBalance.Format("%.2f", dBalance);

	CSSSoap s("1.2"); //SOAP VERSION
	s.Attribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	s.Attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	s.SetSoapAction("http://tempuri.org/updateDinnerBalance");
	s.BeginSoap("Body");
	s.Begin("updateDinnerBalance");
	s.Attribute("xmlns", "http://tempuri.org/");
	s.Element("schoolId", m_pData->GetSchoolIDForConnection());
	s.Element("username", m_pData->GetUsernameForConnection());
	s.Element("password", m_pData->GetPasswordForConnection());
	s.Element("personId", strPersonId);
	s.Element("amount", strBalance);
	s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("UpdateDinnerBalanceLogin.xml"), TRUE);
	}

	// no XML declaration check
	CString strIPAddress = szIPADDRESS_LIVE;
	CString strError = s.SendHttps(strIPAddress, szOBJECTNAME_WDS, szLoginReply, FALSE, System.GetInternetAgentString());
	return strError;
}

//************************************** ********************************

CString CSSBromCom::SendBromComTransaction(const char* szLoginReply, CSQLRowBromComTransaction& RowTran )
{
	CString strPersonId = "";
	strPersonId.Format("%d", RowTran.GetPersonID());

	CString strBalance = "";
	strBalance.Format("%.2f", RowTran.GetBalance());

	CString strAmount = "";
	strAmount.Format("%.2f", RowTran.GetAmount());

	CString strTranId = "";
	strTranId.Format("%d", RowTran.GetTransactionID());

	CSSSoap s("1.2"); //SOAP VERSION
	s.Attribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	s.Attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	s.SetSoapAction("http://tempuri.org/saveDinnerTransaction");
	s.BeginSoap("Body");
	s.Begin("saveDinnerTransaction");
	s.Attribute("xmlns", "http://tempuri.org/");
	s.Element("schoolId", m_pData->GetSchoolIDForConnection());
	s.Element("username", m_pData->GetUsernameForConnection());
	s.Element("password", m_pData->GetPasswordForConnection());
	s.Element("personId", strPersonId);
	s.Element("transactionDetails", RowTran.GetWebComment());
	s.Element("amount", strAmount);
	s.Element("remainingBalance", strBalance);
	s.Element("instant", RowTran.GetDateTime());
	s.Element("transactionType", RowTran.GetType());
	s.Element("externalId", strTranId);
	s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("saveDinnerTransactionLogin.xml"), TRUE);
	}

	// no XML declaration check
	CString strIPAddress = szIPADDRESS_LIVE;
	CString strError = s.SendHttps(strIPAddress, szOBJECTNAME_WDS, szLoginReply, FALSE, System.GetInternetAgentString());
	return strError;
}

//**********************************************************************
