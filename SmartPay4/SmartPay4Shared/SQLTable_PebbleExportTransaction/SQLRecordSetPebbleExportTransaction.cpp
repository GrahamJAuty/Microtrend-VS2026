//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportTransaction.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleExportTransaction::CSQLRecordSetMetadataPebbleExportTransaction(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, PebbleExportTransaction::LineID.Label);
        AddInt64Field(&m_nUserID, PebbleExportTransaction::UserID.Label);
        AddInt64Field(&m_nXFerUserID, PebbleExportTransaction::XFerUserID.Label);
        AddTextField(&m_strMemberID, PebbleExportTransaction::MemberID.Label);
        AddTextField(&m_strDate, PebbleExportTransaction::Date.Label);
        AddTextField(&m_strTime, PebbleExportTransaction::Time.Label);
        AddIntField(&m_nCCNo, PebbleExportTransaction::CCNo.Label);
        AddIntField(&m_nSourceType, PebbleExportTransaction::SourceType.Label);
        AddIntField(&m_nAppNo, PebbleExportTransaction::AppNo.Label);
        AddDecimalField(&m_strAmount, PebbleExportTransaction::Amount.Label);
        AddDecimalField(&m_strPurse1, PebbleExportTransaction::Purse1.Label);
        AddDecimalField(&m_strPurse2, PebbleExportTransaction::Purse2.Label);
        AddDecimalField(&m_strCash, PebbleExportTransaction::Cash.Label);
        AddTextField(&m_strWebComment, PebbleExportTransaction::WebComment.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleExportTransaction::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(PebbleExportTransaction::LineID.Label, strLineID);
}

//**********************************************************************

CSQLRecordSetPebbleExportTransaction::CSQLRecordSetPebbleExportTransaction(CDatabase* pDatabase, RSParams_PebbleExportTransaction_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleExportTransaction::CSQLRecordSetPebbleExportTransaction(CDatabase* pDatabase, RSParams_PebbleExportTransaction_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.strLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransaction::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleExportTransaction::GetDefaultSQL()
{
    return SQLTableNames::PebbleExportTransactions;
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransaction::LoadFromPebbleExportTransactionRow(CSQLRowPebbleExportTransaction& RowPB)
{
    m_MD.m_nLineID = RowPB.GetLineID();
    m_MD.m_nUserID = RowPB.GetUserID();
    m_MD.m_nXFerUserID = RowPB.GetXFerUserID();
    m_MD.m_strMemberID = RowPB.GetMemberID();
    m_MD.m_strDate = RowPB.GetDate();
    m_MD.m_strTime = RowPB.GetTime();
    m_MD.m_nCCNo = RowPB.GetCCNo();
    m_MD.m_nSourceType = RowPB.GetSourceType();
    m_MD.m_nAppNo = RowPB.GetAppNo();
	SetDoubleString(m_MD.m_strAmount, RowPB.GetAmount());
	SetDoubleString(m_MD.m_strPurse1, RowPB.GetPurse1());
	SetDoubleString(m_MD.m_strPurse2, RowPB.GetPurse2());
	SetDoubleString(m_MD.m_strCash, RowPB.GetCash());
    m_MD.m_strWebComment = RowPB.GetWebComment();
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransaction::SaveToPebbleExportTransactionRow(CSQLRowPebbleExportTransaction& RowPB)
{
    RowPB.SetLineID(m_MD.m_nLineID);
    RowPB.SetUserID(m_MD.m_nUserID);
	RowPB.SetXFerUserID(m_MD.m_nXFerUserID);
    RowPB.SetMemberID(m_MD.m_strMemberID);
    RowPB.SetDate(m_MD.m_strDate);
    RowPB.SetTime(m_MD.m_strTime);
    RowPB.SetCCNo(m_MD.m_nCCNo);
    RowPB.SetSourceType(m_MD.m_nSourceType);
    RowPB.SetAppNo(m_MD.m_nAppNo);
	RowPB.SetAmount(atof(m_MD.m_strAmount));
	RowPB.SetPurse1(atof(m_MD.m_strPurse1));
	RowPB.SetPurse2(atof(m_MD.m_strPurse2));
	RowPB.SetCash(atof(m_MD.m_strCash));
    RowPB.SetWebComment(m_MD.m_strWebComment);
}

//**********************************************************************

bool CSQLRecordSetPebbleExportTransaction::StepSelectAll(CSQLRowPebbleExportTransaction& RowBC)
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

    SaveToPebbleExportTransactionRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
