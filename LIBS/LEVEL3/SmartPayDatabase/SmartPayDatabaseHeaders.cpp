/**********************************************************************/

//static const char* szHEADER_V1 = "UserID,GroupNo,Forename,Surname,DOB,Gender,Expiry,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,Points,PointsTD,CashSpendTD,MemberID,Info1,Info2,Info3,Info4";
//static const char* szHEADER_V2 = "UserID,GroupNo,Forename,Surname,DOB,Gender,Expiry,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,Points,PointsTD,CashSpendTD,MemberID,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8";
//static const char* szHEADER_V3 = "UserID,GroupNo,Forename,Surname,DOB,Gender,Expiry,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshRef,RefreshDate,RefreshTime,Purse3,Purse3Date,Purse3Time,Purse3Spend,Purse3SpendTD,Points,PointsTD,CashSpendTD,MemberID,Email,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8";
//static const char* szHEADER_V4 = "UserID,GroupNo,Forename,Surname,DOB,Gender,Expiry,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshRef,RefreshDate,RefreshTime,Purse3,Purse3Date,Purse3Time,Purse3Spend,Purse3SpendTD,Points,PointsTD,CashSpendTD,MemberID,ExtRef,Email,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8";
//static const char* szHEADER_V5 = "UserID,UserName,GroupNo,Forename,Surname,DOB,Gender,Expiry,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,MaxSpendP3,OverSpendP3,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshRef,RefreshDate,RefreshTime,Purse3,Purse3Date,Purse3Time,Purse3Spend,Purse3SpendTD,R3Date,R3Spend,Points,PointsTD,CashSpendTD,MemberID,ExtRef,Email,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8,Tick1,Tick2,Tick3,Tick4";
//static const char* szHEADER_V6  = "UserID,UserName,GroupNo,Forename,Surname,DOB,Gender,Expiry,DayGroups,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,MaxSpend,OverSpend,MaxSpendP3,OverSpendP3,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshRef,RefreshDate,RefreshTime,Purse3,Purse3Date,Purse3Time,Purse3Spend,Purse3SpendTD,R3Date,R3Spend,Points,PointsTD,CashSpendTD,MemberID,ExtRef,Email,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8,Tick1,Tick2,Tick3,Tick4";
//static const char* szHEADER_V7  = "UserID,UserName,GroupNo,Forename,Surname,DOB,Gender,Expiry,DayGroups,Alert,AlertDate,AlertTime,NextNo,PreviousNo,Inactive,Allergy,Dietary,MaxSpend,OverSpend,MaxSpendP3,OverSpendP3,Purse1,Purse1Date,Purse1Time,Purse1Spend,Purse1SpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshRef,RefreshDate,RefreshTime,Purse3,Purse3Date,Purse3Time,Purse3Spend,Purse3SpendTD,R3Date,R3Spend,Points,PointsTD,CashSpendTD,MemberID,ExtRef,ExtRef2,Email,Info1,Info2,Info3,Info4,Info5,Info6,Info7,Info8,Tick1,Tick2,Tick3,Tick4";

void CSmartPayDatabase::CreateHeaders()
{
	m_strHeader = "";
	m_strBioHeader = "";

	AddHeaderField("UserID");
	AddHeaderField("UserName");
	AddHeaderField("GroupNo");
	AddHeaderField("Forename");
	AddHeaderField("Surname");
	AddHeaderField("DOB");
	AddHeaderField("Gender");
	AddHeaderField("Expiry");
	AddHeaderField("DayGroups");
	AddHeaderField("Alert");
	AddHeaderField("AlertDate");
	AddHeaderField("AlertTime");
	AddHeaderField("NextNo", FALSE);
	AddHeaderField("PreviousNo", FALSE);
	AddHeaderField("Inactive");
	AddHeaderField("Allergy");
	AddHeaderField("Dietary");
	AddHeaderField("OwnMaxSpend");	//NEW TO HEADER V8
	AddHeaderField("P1Allow");		//NEW TO HEADER V8
	AddHeaderField("P2Allow");		//NEW TO HEADER V8
	AddHeaderField("P3Allow");		//NEW TO HEADER V8
	AddHeaderField("MaxSpend");
	AddHeaderField("MSV2");			//NEW TO HEADER V8
	AddHeaderField("MSV3");			//NEW TO HEADER V8
	AddHeaderField("MSV4");			//NEW TO HEADER V8
	AddHeaderField("MSV5");			//NEW TO HEADER V8
	AddHeaderField("MSV6");			//NEW TO HEADER V8
	AddHeaderField("OverSpend");
	AddHeaderField("OSV2");			//NEW TO HEADER V8
	AddHeaderField("OSV3");			//NEW TO HEADER V8
	AddHeaderField("OSV4");			//NEW TO HEADER V8
	AddHeaderField("OSV5");			//NEW TO HEADER V8
	AddHeaderField("OSV6");			//NEW TO HEADER V8
	AddHeaderField("MaxSpendP3");
	AddHeaderField("MSV32");			//NEW TO HEADER V8
	AddHeaderField("MSV33");			//NEW TO HEADER V8
	AddHeaderField("MSV34");			//NEW TO HEADER V8
	AddHeaderField("MSV35");			//NEW TO HEADER V8
	AddHeaderField("MSV36");			//NEW TO HEADER V8
	AddHeaderField("OverSpendP3");
	AddHeaderField("OSV32");			//NEW TO HEADER V8
	AddHeaderField("OSV33");			//NEW TO HEADER V8
	AddHeaderField("OSV34");			//NEW TO HEADER V8
	AddHeaderField("OSV35");			//NEW TO HEADER V8
	AddHeaderField("OSV36");			//NEW TO HEADER V8
	AddHeaderField("Purse1");
	AddHeaderField("Purse1Credit");		//NEW TO HEADER V9
	AddHeaderField("Purse1Date");
	AddHeaderField("Purse1Time");
	AddHeaderField("Purse1Spend");
	AddHeaderField("Purse1SpendTD");
	AddHeaderField("Purse2");
	AddHeaderField("Purse2Date");
	AddHeaderField("Purse2Time");
	AddHeaderField("Purse2Spend");
	AddHeaderField("Purse2SpendTD");
	AddHeaderField("RefreshRef");
	AddHeaderField("RefreshDate");
	AddHeaderField("RefreshTime");
	AddHeaderField("Purse3");
	AddHeaderField("Purse3Credit");		//NEW TO HEADER V9
	AddHeaderField("Purse3Date");
	AddHeaderField("Purse3Time");
	AddHeaderField("Purse3Spend");
	AddHeaderField("Purse3SpendTD");
	AddHeaderField("R3Date");
	AddHeaderField("R3Spend");
	AddHeaderField("Points");
	AddHeaderField("PointsTD");
	AddHeaderField("CashSpendTD");
	AddHeaderField("MemberID");
	AddHeaderField("ExtRef", FALSE);
	AddHeaderField("ExtRef2", FALSE);
	AddHeaderField("Email");
	AddHeaderField("Info1");
	AddHeaderField("Info2");
	AddHeaderField("Info3");
	AddHeaderField("Info4");
	AddHeaderField("Info5");
	AddHeaderField("Info6");
	AddHeaderField("Info7");
	AddHeaderField("Info8");
	AddHeaderField("Tick1");
	AddHeaderField("Tick2");
	AddHeaderField("Tick3");
	AddHeaderField("Tick4");
}

/**********************************************************************/

void CSmartPayDatabase::AddHeaderField( const char* szField, bool bAddToBioHeader )
{
	if (m_strHeader != "")
	{
		m_strHeader += ",";
	}

	m_strHeader += szField;

	if ( TRUE == bAddToBioHeader )
	{
		if (m_strBioHeader != "")
		{
			m_strBioHeader += ",";
		}

		m_strBioHeader += szField;
	}
}

/**********************************************************************/
