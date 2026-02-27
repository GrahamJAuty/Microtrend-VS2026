//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "DbReportFields.h"
//$$******************************************************************

CDbReportFieldInfo::CDbReportFieldInfo()
{
	Reset();
}

/**********************************************************************/

void CDbReportFieldInfo::Reset()
{
	m_strLabel = "";
	m_strHeaderText = "";
	m_nFieldType = 0;
	m_nColumnType = DBREPORTFIELD_COLTYPE_NORMAL;
	m_nColumnPos = -1;
	m_nAlign = TA_LEFT;
	m_nWidth = 200;
}

/**********************************************************************/

void CDbReportFieldInfo::Add(CDbReportFieldInfo& source)
{
}

/**********************************************************************/

int CDbReportFieldInfo::Compare(CDbReportFieldInfo& source, int nHint)
{
	if (m_strLabel != source.m_strLabel)
		return (m_strLabel > source.m_strLabel ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CDbReportFields::CDbReportFields()
{
	AddField(Account::UserID.Label, DBREPORTFIELD_USERID, "", TA_LEFT, 210);
	AddField(Account::Username.Label, DBREPORTFIELD_USERNAME, "", TA_LEFT, 360);
	AddField(Account::Forename.Label, DBREPORTFIELD_FORENAME, "", TA_LEFT, 360);
	AddField(Account::Surname.Label, DBREPORTFIELD_SURNAME, "", TA_LEFT, 360);
	AddField(Account::GroupNo.Label, DBREPORTFIELD_GROUPNO, "Grp", TA_RIGHT, 100);
	AddField("EmptyField", DBREPORTFIELD_EMPTYFIELD, " ", TA_LEFT, 200);
	AddField(Account::GroupShiftStatus.Label, DBREPORTFIELD_GROUPSHIFT_STATUS, "Group Shift", TA_LEFT, 300);
	AddField(Account::GroupShiftStartDate.Label, DBREPORTFIELD_GROUPSHIFT_STARTDATE, "Start Date", TA_LEFT, 300);
	AddField(Account::GroupShiftEndDate.Label, DBREPORTFIELD_GROUPSHIFT_ENDDATE, "End Date", TA_LEFT, 300);
	AddField(Account::GroupShiftFutureGroup.Label, DBREPORTFIELD_GROUPSHIFT_FUTUREGROUP, "Switch Group", TA_LEFT, 300);
	AddField(Account::DOB.Label, DBREPORTFIELD_DOB, "", TA_LEFT, 220);
	AddField(Account::Gender.Label, DBREPORTFIELD_GENDER, "Sx", TA_CENTER, 100);
	AddField(Account::ExpiryDate.Label, DBREPORTFIELD_EXPIRY, "", TA_LEFT, 220);
	AddField(Account::DayGroups.Label, DBREPORTFIELD_DAYGROUPS, "DayGrps", TA_LEFT, 400);
	AddField(Account::AlertCode.Label, DBREPORTFIELD_ALERTCODE, "Al.No", TA_LEFT, 140);
	AddField(Account::AlertDate.Label, DBREPORTFIELD_ALERTDATE, "Al.Date", TA_LEFT, 220);
	AddField(Account::AlertTime.Label, DBREPORTFIELD_ALERTTIME, "Al.Time", TA_LEFT, 160);
	AddField(Account::Inactive.Label, DBREPORTFIELD_INACTIVE, "In", TA_CENTER, 90);
	AddField(Account::AllergyCodes.Label, DBREPORTFIELD_ALLERGYCODES, "ACode", TA_LEFT, 160);
	AddField(Account::Dietary.Label, DBREPORTFIELD_DIETARY, "", TA_LEFT, 400);
	AddField(Account::OwnMaxSpend.Label, DBREPORTFIELD_OWNMAXSPEND, "Own.Max.Sp", TA_LEFT, 250);
	AddField(Account::Purse1Allow.Label, DBREPORTFIELD_P1ALLOW, "P1.Allow", TA_LEFT, 250);
	AddField(Account::Purse2Allow.Label, DBREPORTFIELD_P2ALLOW, "P2.Allow", TA_LEFT, 250);
	AddField(Account::Purse3Allow.Label, DBREPORTFIELD_P3ALLOW, "P3.Allow", TA_LEFT, 250);
	AddField(Account::MaxSpendPurse1Period1.Label, DBREPORTFIELD_P1MAXSPEND1, "Max.Sp.P1.1", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse1Period2.Label, DBREPORTFIELD_P1MAXSPEND2, "Max.Sp.P1.2", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse1Period3.Label, DBREPORTFIELD_P1MAXSPEND3, "Max.Sp.P1.3", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse1Period4.Label, DBREPORTFIELD_P1MAXSPEND4, "Max.Sp.P1.4", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse1Period5.Label, DBREPORTFIELD_P1MAXSPEND5, "Max.Sp.P1.5", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse1Period5.Label, DBREPORTFIELD_P1MAXSPEND6, "Max.Sp.P1.6", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period1.Label, DBREPORTFIELD_P1OVERSPEND1, "Over.Sp.P1.1", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period2.Label, DBREPORTFIELD_P1OVERSPEND2, "Over.Sp.P1.2", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period3.Label, DBREPORTFIELD_P1OVERSPEND3, "Over.Sp.P1.3", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period4.Label, DBREPORTFIELD_P1OVERSPEND4, "Over.Sp.P1.4", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period5.Label, DBREPORTFIELD_P1OVERSPEND5, "Over.Sp.P1.5", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse1Period5.Label, DBREPORTFIELD_P1OVERSPEND6, "Over.Sp.P1.6", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period1.Label, DBREPORTFIELD_P3MAXSPEND1, "Max.Sp.P3.1", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period2.Label, DBREPORTFIELD_P3MAXSPEND2, "Max.Sp.P3.2", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period3.Label, DBREPORTFIELD_P3MAXSPEND3, "Max.Sp.P3.3", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period4.Label, DBREPORTFIELD_P3MAXSPEND4, "Max.Sp.P3.4", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period5.Label, DBREPORTFIELD_P3MAXSPEND5, "Max.Sp.P3.5", TA_RIGHT, 300);
	AddField(Account::MaxSpendPurse3Period6.Label, DBREPORTFIELD_P3MAXSPEND6, "Max.Sp.P3.6", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period1.Label, DBREPORTFIELD_P3OVERSPEND1, "Over.Sp.P3.1", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period2.Label, DBREPORTFIELD_P3OVERSPEND2, "Over.Sp.P3.2", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period3.Label, DBREPORTFIELD_P3OVERSPEND3, "Over.Sp.P3.3", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period4.Label, DBREPORTFIELD_P3OVERSPEND4, "Over.Sp.P3.4", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period5.Label, DBREPORTFIELD_P3OVERSPEND5, "Over.Sp.P3.5", TA_RIGHT, 300);
	AddField(Account::MaxOverdraftPurse3Period6.Label, DBREPORTFIELD_P3OVERSPEND6, "Over.Sp.P3.6", TA_RIGHT, 300);
	AddField(Account::Purse1Liability.Label, DBREPORTFIELD_P1LIABILITY, "", TA_RIGHT, 300);
	AddField(Account::Purse1Credit.Label, DBREPORTFIELD_P1CREDIT, "", TA_RIGHT, 250);
	AddField(Account::Purse1LastSpendDate.Label, DBREPORTFIELD_P1DATE, "P1.Date", TA_LEFT, 220);
	AddField(Account::Purse1LastSpendTime.Label, DBREPORTFIELD_P1TIME, "P1.Time", TA_LEFT, 200);
	AddField(Account::Purse1LastSpend.Label, DBREPORTFIELD_P1LASTSPEND, "P1.Trans", TA_RIGHT, 210);
	AddField(Account::Purse1SpendToDate.Label, DBREPORTFIELD_P1SPENDTD, "P1.SpendTD", TA_RIGHT, 260);
	AddField(Account::Purse2Balance.Label, DBREPORTFIELD_P2BALANCE, "", TA_RIGHT, 200);
	AddField(Account::Purse2LastSpendDate.Label, DBREPORTFIELD_P2DATE, "P2.Date", TA_LEFT, 220);
	AddField(Account::Purse2LastSpendTime.Label, DBREPORTFIELD_P2TIME, "P2.Time", TA_LEFT, 200);
	AddField(Account::Purse2LastSpend.Label, DBREPORTFIELD_P2LASTSPEND, "P2.Trans", TA_RIGHT, 210);
	AddField(Account::Purse2SpendToDate.Label, DBREPORTFIELD_P2SPENDTD, "P2.SpendTD", TA_RIGHT, 260);
	AddField(Account::Purse2RefreshFlags.Label, DBREPORTFIELD_REFRESHREF, "Rf.Days", TA_LEFT, 300);
	AddField(Account::Purse2RefreshedDate.Label, DBREPORTFIELD_REFRESHDATE, "Rf.Date", TA_LEFT, 220);
	AddField(Account::Purse2RefreshedTime.Label, DBREPORTFIELD_REFRESHTIME, "Rf.Time", TA_LEFT, 160);
	AddField(Account::Purse3Liability.Label, DBREPORTFIELD_P3LIABILITY, "", TA_RIGHT, 300);
	AddField(Account::Purse3Credit.Label, DBREPORTFIELD_P3CREDIT, "", TA_RIGHT, 250);
	AddField(Account::Purse3LastSpendDate.Label, DBREPORTFIELD_P3DATE, "P3.Date", TA_LEFT, 220);
	AddField(Account::Purse3LastSpendTime.Label, DBREPORTFIELD_P3TIME, "P3.Time", TA_LEFT, 200);
	AddField(Account::Purse3LastSpend.Label, DBREPORTFIELD_P3LASTSPEND, "P3.Trans", TA_RIGHT, 210);
	AddField(Account::Purse3SpendToDate.Label, DBREPORTFIELD_P3SPENDTD, "P3.SpendTD", TA_RIGHT, 260);
	AddField(Account::Points.Label, DBREPORTFIELD_POINTS, "", TA_RIGHT, 160);
	AddField(Account::PointsToDate.Label, DBREPORTFIELD_POINTSTD, "Pts.TD", TA_RIGHT, 190);
	AddField(Account::CashSpendToDate.Label, DBREPORTFIELD_CASHTD, "Cash.Spend", TA_RIGHT, 250);
	AddField(Account::MemberID.Label, DBREPORTFIELD_MEMBERID, "", TA_LEFT, 360);
	AddField(Account::Email.Label, DBREPORTFIELD_EMAIL, "", TA_LEFT, 360);
	AddField(Account::Info1.Label, DBREPORTFIELD_INFO1, "", TA_LEFT, 300);
	AddField(Account::Info2.Label, DBREPORTFIELD_INFO2, "", TA_LEFT, 300);
	AddField(Account::Info3.Label, DBREPORTFIELD_INFO3, "", TA_LEFT, 300);
	AddField(Account::Info4.Label, DBREPORTFIELD_INFO4, "", TA_LEFT, 300);
	AddField(Account::Info5.Label, DBREPORTFIELD_INFO5, "", TA_LEFT, 300);
	AddField(Account::Info6.Label, DBREPORTFIELD_INFO6, "", TA_LEFT, 300);
	AddField(Account::Info7.Label, DBREPORTFIELD_INFO7, "", TA_LEFT, 300);
	AddField(Account::Info8.Label, DBREPORTFIELD_INFO8, "", TA_LEFT, 300);
	AddField(Account::Tick1.Label, DBREPORTFIELD_TICK1, "T1", TA_CENTER, 90);
	AddField(Account::Tick2.Label, DBREPORTFIELD_TICK2, "T2", TA_CENTER, 90);
	AddField(Account::Tick3.Label, DBREPORTFIELD_TICK3, "T3", TA_CENTER, 90);
	AddField(Account::Tick4.Label, DBREPORTFIELD_TICK4, "T4", TA_CENTER, 90);
	AddField(Account::TotalSpendToDate.Label, DBREPORTFIELD_TOTALSPENDTD, "TL.SpendTD", TA_RIGHT, 250);
	AddField(Account::DOBNextAge.Label, DBREPORTFIELD_DOB_NEXTAGE, "NxtAge", TA_RIGHT, 170);
	AddField(Account::DOBDay.Label, DBREPORTFIELD_DOB_BDAY, "Day", TA_LEFT, 120);
	AddField(Account::AlertText.Label, DBREPORTFIELD_ALERTTEXT, "Alert", TA_LEFT, 200);
	AddField(Account::AllergyText.Label, DBREPORTFIELD_ALLERGYTEXT, "Allergies", TA_LEFT, 400);
	AddField(Account::ExternalRef1.Label, DBREPORTFIELD_EXTREF, "", TA_LEFT, 360);
	AddField(Account::ExternalRef2.Label, DBREPORTFIELD_EXTREF2, "", TA_LEFT, 360);
	AddField(Account::DeletedDate.Label, DBREPORTFIELD_DELETEDDATE, "", TA_LEFT, 220);
	AddField(Account::DeletedTime.Label, DBREPORTFIELD_DELETEDTIME, "", TA_LEFT, 200);
	AddField(PINNumber::PINNumber.Label, DBREPORTFIELD_PIN, "PIN Number", TA_LEFT, 250);
	AddField(DbReportFieldsSpecial::AccuRank, DBREPORTFIELD_ACCURANK, "Rank", TA_LEFT, 150);
	AddField(DbReportFieldsSpecial::AccuPoints, DBREPORTFIELD_ACCUPOINTS, "Points", TA_RIGHT, 210);
	AddField(DbReportFieldsSpecial::AccuTrans, DBREPORTFIELD_ACCUTRANS, "Trans.", TA_RIGHT, 210);
	AddField(DbReportFieldsSpecial::AccuSpend, DBREPORTFIELD_ACCUSPEND, "Spend", TA_RIGHT, 210);
	AddField(DbReportFieldsSpecial::StartBalLastUsed, DBREPORTFIELD_STARTBAL_LASTUSED, "LastUsed", TA_LEFT, 220);
	AddField(DbReportFieldsSpecial::StartBalNextUsed, DBREPORTFIELD_STARTBAL_NEXTUSED, "NextUsed", TA_LEFT, 220);
	AddField(DbReportFieldsSpecial::StartBalSource, DBREPORTFIELD_STARTBAL_SOURCE, "Source", TA_LEFT, 250);
	AddField(DbReportFieldsSpecial::StartBalPoints, DBREPORTFIELD_STARTBAL_POINTS, "Points", TA_RIGHT, 180);
	AddField(DbReportFieldsSpecial::StartBalPurse1, DBREPORTFIELD_STARTBAL_PURSE1, "Purse1", TA_RIGHT, 200);
	AddField(DbReportFieldsSpecial::StartBalPurse3, DBREPORTFIELD_STARTBAL_PURSE3, "Purse3", TA_RIGHT, 200);
}

//$$******************************************************************

void CDbReportFields::AddField(CString strLabel, int nFieldType, CString strHeaderText, int nAlign, int nWidth)
{
	CDbReportFieldInfo info{};
	info.m_strLabel = strLabel;
	info.m_strHeaderText = strHeaderText;
	info.m_nFieldType = nFieldType;
	info.m_nAlign = nAlign;
	info.m_nWidth = nWidth;
	info.m_nColumnPos = -1;
	m_arrayFields.Consolidate(info);
}

//$$******************************************************************

void CDbReportFields::ResetColumns()
{
	for (int n = 0; n < m_arrayFields.GetSize(); n++)
	{
		CDbReportFieldInfo info{};
		m_arrayFields.GetAt(n, info);
		info.m_nColumnPos = -1;
		info.m_nColumnType = DBREPORTFIELD_COLTYPE_NORMAL;
		m_arrayFields.SetAt(n, info);
	}

	m_arrayColumns.RemoveAll();
}

//$$******************************************************************

void CDbReportFields::ResetColumnsFromUserText()
{
	CSQLRepositoryUserText Repo;

	ResetColumns();
	SetHeaderText(Account::DOB.Label, Repo.GetDOBText(Account::DOB.Label));
	SetHeaderText(Account::MemberID.Label, Repo.GetMemberIDText(Account::MemberID.Label));
	SetHeaderText(Account::Info1.Label, Repo.GetInfo1Text());
	SetHeaderText(Account::Info2.Label, Repo.GetInfo2Text());
	SetHeaderText(Account::Info3.Label, Repo.GetInfo3Text());
	SetHeaderText(Account::Info4.Label, Repo.GetInfo4Text());
	SetHeaderText(Account::Info5.Label, Repo.GetInfo5Text());
	SetHeaderText(Account::Info6.Label, Repo.GetInfo6Text());
	SetHeaderText(Account::Info7.Label, Repo.GetInfo7Text());
	SetHeaderText(Account::Info8.Label, Repo.GetInfo8Text());
	SetHeaderText(Account::ExternalRef1.Label, Repo.GetExtRef1Text());
	SetHeaderText(Account::ExternalRef2.Label, Repo.GetExtRef2Text());
}

//$$******************************************************************

bool CDbReportFields::AddColumn( CString strLabel, int nColumnType )
{
	bool bResult = FALSE;

	CDbReportFieldInfo info{};
	info.m_strLabel = strLabel;

	int nPos = 0;
	if (m_arrayFields.Find(info, nPos) == TRUE)
	{
		m_arrayFields.GetAt(nPos, info);
		if (info.m_nColumnPos == -1)
		{
			info.m_nColumnPos = m_arrayColumns.GetSize();
			info.m_nColumnType = nColumnType;
			m_arrayFields.SetAt(nPos, info);
			m_arrayColumns.Add(info);
			bResult = TRUE;
		}
	}

	return bResult;
}

//$$******************************************************************

void CDbReportFields::CreateReportHeader(CReportFileWithEndText* pReport)
{
	for (int n = 0; n < m_arrayColumns.GetSize(); n++)
	{
		CDbReportFieldInfo info = m_arrayColumns.GetAt(n);
		
		CString strText = info.m_strHeaderText;
		if (strText == "")
		{
			strText = info.m_strLabel;
		}
		
		pReport->AddColumn(strText, info.m_nAlign, info.m_nWidth);
	}
}

//$$******************************************************************

int CDbReportFields::GetFieldType(int n)
{
	if ((n >= 0) && (n < m_arrayColumns.GetSize()))
	{
		return m_arrayColumns[n].m_nFieldType;
	}
	else
	{
		return -1;
	}
}

//$$******************************************************************

int CDbReportFields::GetColType(int n)
{
	if ((n >= 0) && (n < m_arrayColumns.GetSize()))
	{
		return m_arrayColumns[n].m_nColumnType;
	}
	else
	{
		return -1;
	}
}

//$$******************************************************************

void CDbReportFields::SetHeaderText(CString strLabel, CString strText)
{
	CDbReportFieldInfo info{};
	info.m_strLabel = strLabel;

	int nPos = 0;
	if (m_arrayFields.Find(info, nPos) == TRUE)
	{
		m_arrayFields.GetAt(nPos, info);
		info.m_strHeaderText = strText;
		m_arrayFields.SetAt(nPos, info);
	}
}

//$$******************************************************************

void CDbReportFields::CreateColumnListFromHeader(CString strHeader)
{
	ResetColumns();
	
	CCSV csvHeader(strHeader);

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strLabel = csvHeader.GetString(n);
		if (AddColumn(strLabel, DBREPORTFIELD_COLTYPE_NORMAL) == FALSE)
		{
			//ADD PLACEHOLDER COLUMN IN CASE OF UNKNOWN FIELD
			CDbReportFieldInfo info;
			info.m_strLabel = "";
			info.m_strHeaderText = "";
			info.m_nColumnType = DBREPORTFIELD_COLTYPE_NORMAL;
			info.m_nFieldType = DBREPORTFIELD_BLANK;
			info.m_nAlign = TA_LEFT;
			info.m_nWidth = 250;
			info.m_nColumnPos = m_arrayColumns.GetSize();
			m_arrayColumns.Add(info);
		}
	}
}

//$$******************************************************************
