//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetChartwellExportPayment.h"
//**********************************************************************

CSQLRecordSetMetadataChartwellExportPayment::CSQLRecordSetMetadataChartwellExportPayment() : CSQLRecordSetMetadata()
{
	AddInt64Field(&m_nUserID, ChartwellExportPayment::UserID.Label, 1);
	AddInt64Field(&m_nTransactionID, ChartwellExportPayment::TransactionID.Label, 1);
	AddIntField(&m_nTerminalNo, ChartwellExportPayment::TerminalNo.Label, 1);
	AddTextField(&m_strDate, ChartwellExportPayment::Date.Label);
	AddTextField(&m_strCreateDateTime, ChartwellExportPayment::CreateDateTime.Label);
	AddIntField(&m_nGroupNo, ChartwellExportPayment::GroupNo.Label, 1);
	AddDecimalField(&m_strPaymentPurse1, ChartwellExportPayment::PaymentPurse1.Label);
	AddDecimalField(&m_strPaymentPurse2, ChartwellExportPayment::PaymentPurse2.Label);
	AddDecimalField(&m_strPaymentPurse3, ChartwellExportPayment::PaymentPurse3.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataChartwellExportPayment::AddParamsByTranDetail(const CString strUserID, const CString strTransactionID, const int nTerminalNo, const CString strDate)
{
	AddTextParam(ChartwellExportPayment::UserID.Label, strUserID);
	AddTextParam(ChartwellExportPayment::TransactionID.Label, strTransactionID);
	AddIntParam(ChartwellExportPayment::TerminalNo.Label, nTerminalNo);
	AddTextParam(ChartwellExportPayment::Date.Label, strDate);
}

//**********************************************************************

CSQLRecordSetChartwellExportPayment::CSQLRecordSetChartwellExportPayment(CDatabase* pDatabase, RSParams_ChartwellExportPayment_NormalByTranDetail params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	m_MD.AddParamsByTranDetail(params.strUserId, params.strTransactionId, params.nTerminalNo, params.strDate);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetChartwellExportPayment::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetBasicFilterString();
}

//**********************************************************************

CString CSQLRecordSetChartwellExportPayment::GetDefaultSQL()
{
    return SQLTableNames::ChartwellExportPayments;
}

//**********************************************************************

void CSQLRecordSetChartwellExportPayment::LoadFromChartwellExportPaymentRow(CSQLRowChartwellExportPayment& RowPB)
{
    m_MD.m_nUserID = RowPB.GetUserID();
	m_MD.m_nTransactionID = RowPB.GetTransactionID();
	m_MD.m_nTerminalNo = RowPB.GetTerminalNo();
	m_MD.m_strDate = RowPB.GetDate();
	m_MD.m_strCreateDateTime = RowPB.GetCreateDateTime();
	m_MD.m_nGroupNo = RowPB.GetGroupNo();
	SetDoubleString(m_MD.m_strPaymentPurse1, RowPB.GetPaymentPurse1());
	SetDoubleString(m_MD.m_strPaymentPurse2, RowPB.GetPaymentPurse2());
	SetDoubleString(m_MD.m_strPaymentPurse3, RowPB.GetPaymentPurse3());
}

//**********************************************************************

void CSQLRecordSetChartwellExportPayment::SaveToChartwellExportPaymentRow(CSQLRowChartwellExportPayment& RowPB)
{
    RowPB.SetUserID(m_MD.m_nUserID);
	RowPB.SetTransactionID(m_MD.m_nTransactionID);
	RowPB.SetTerminalNo(m_MD.m_nTerminalNo);
	RowPB.SetDate(m_MD.m_strDate);
    RowPB.SetCreateDateTime(m_MD.m_strCreateDateTime);
	RowPB.SetGroupNo(m_MD.m_nGroupNo);
	RowPB.SetPaymentPurse1(atof(m_MD.m_strPaymentPurse1));
	RowPB.SetPaymentPurse2(atof(m_MD.m_strPaymentPurse2));
    RowPB.SetPaymentPurse3(atof(m_MD.m_strPaymentPurse3));
}

//**********************************************************************

bool CSQLRecordSetChartwellExportPayment::StepSelectAll(CSQLRowChartwellExportPayment& RowPB)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToChartwellExportPaymentRow(RowPB);
    MoveNext();

    return TRUE;
}

//**********************************************************************
