//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportPayment.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleExportPayment::CSQLRecordSetMetadataPebbleExportPayment(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nUserID, PebbleExportPayment::UserID.Label, 1);
        AddTextField(&m_strDate, PebbleExportPayment::Date.Label);
        AddTextField(&m_strTime, PebbleExportPayment::Time.Label);
        AddTextField(&m_strCreateDateTime, PebbleExportPayment::CreateDateTime.Label);
        AddIntField(&m_nCCNo, PebbleExportPayment::CCNo.Label, 1);
        AddIntField(&m_nPaymentType[0], PebbleExportPayment::PaymentType1.Label);
        AddDecimalField(&m_strPaymentAmount[0], PebbleExportPayment::PaymentAmount1.Label);
        AddIntField(&m_nPaymentType[1], PebbleExportPayment::PaymentType2.Label);
        AddDecimalField(&m_strPaymentAmount[1], PebbleExportPayment::PaymentAmount2.Label);
        AddIntField(&m_nPaymentType[2], PebbleExportPayment::PaymentType3.Label);
        AddDecimalField(&m_strPaymentAmount[2], PebbleExportPayment::PaymentAmount3.Label);
        AddIntField(&m_nPaymentType[3], PebbleExportPayment::PaymentType4.Label);
        AddDecimalField(&m_strPaymentAmount[3], PebbleExportPayment::PaymentAmount4.Label);
        AddIntField(&m_nPaymentType[4], PebbleExportPayment::PaymentType5.Label);
        AddDecimalField(&m_strPaymentAmount[4], PebbleExportPayment::PaymentAmount5.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleExportPayment::AddParamsFull(const CString strUserID, const CString strDate, const CString strTime, const int nCCNo)
{
	AddTextParam(PebbleExportPayment::UserID.Label, strUserID);
	AddTextParam(PebbleExportPayment::Date.Label, strDate);
	AddTextParam(PebbleExportPayment::Time.Label, strTime);
	AddIntParam(PebbleExportPayment::CCNo.Label, nCCNo);
}

//**********************************************************************

CSQLRecordSetPebbleExportPayment::CSQLRecordSetPebbleExportPayment(CDatabase* pDatabase, RSParams_PebbleExportPayment_NormalNoParams params) :
    CSQLRecordSetConnectionPool( pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleExportPayment::CSQLRecordSetPebbleExportPayment(CDatabase* pDatabase, RSParams_PebbleExportPayment_NormalByFullParams params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsFull(params.strUserId, params.strDate, params.strTime, params.nCCNo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleExportPayment::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleExportPayment::GetDefaultSQL()
{
    return SQLTableNames::PebbleExportPayments;
}

//**********************************************************************

void CSQLRecordSetPebbleExportPayment::LoadFromPebbleExportPaymentRow(CSQLRowPebbleExportPayment& RowPB)
{
    m_MD.m_nUserID = RowPB.GetUserID();
    m_MD.m_strDate = RowPB.GetDate();
    m_MD.m_strTime = RowPB.GetTime();
    m_MD.m_strCreateDateTime = RowPB.GetCreateDateTime();
    m_MD.m_nCCNo = RowPB.GetCCNo();

    for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
    {
        m_MD.m_nPaymentType[n] = RowPB.GetPaymentType(n);
		SetDoubleString(m_MD.m_strPaymentAmount[n], RowPB.GetPaymentAmount(n));
    }
}

//**********************************************************************

void CSQLRecordSetPebbleExportPayment::SaveToPebbleExportPaymentRow(CSQLRowPebbleExportPayment& RowPB)
{
    RowPB.SetUserID(m_MD.m_nUserID);
    RowPB.SetDate(m_MD.m_strDate);
    RowPB.SetTime(m_MD.m_strTime);
    RowPB.SetCreateDateTime(m_MD.m_strCreateDateTime);
    RowPB.SetCCNo(m_MD.m_nCCNo);

    for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
    {
        RowPB.SetPaymentType(n, m_MD.m_nPaymentType[n]);
        RowPB.SetPaymentAmount(n, atof(m_MD.m_strPaymentAmount[n]));
    }
}

//**********************************************************************

bool CSQLRecordSetPebbleExportPayment::StepSelectAll(CSQLRowPebbleExportPayment& RowPB)
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

    SaveToPebbleExportPaymentRow(RowPB);
    MoveNext();

    return TRUE;
}

//**********************************************************************
