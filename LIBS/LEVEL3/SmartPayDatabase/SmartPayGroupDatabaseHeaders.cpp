/**********************************************************************/

//static const char* szHEADER_V1	= "GroupNo,Name,UType,SetNo,PLevel,RFType,RFAllow,RFVal,P1Allow,P2Allow,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal,LYFlag,LYType,LYRfp,LYTrp,LYPts,LYVal,LYTrg" ;
//static const char* szHEADER_V2	= "GroupNo,Name,UType,SetNo,PLevel,VShift,RFType,RFAllow,RFVal,P1Allow,P2Allow,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,RF1,RF2,RF3,RF4,RF5,RF6,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal,LYFlag,LYType,LYRfp,LYTrp,LYPts,LYVal,LYTrg" ;
//static const char* szHEADER_V3	= "GroupNo,Name,UType,SetNo,PLevel,VShift,RFType,RFAllow,RFVal,P1Allow,P2Allow,P3Allow,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,MSType3,MSV31,MSV32,MSV33,MSV34,MSV35,MSV36,OSV31,OSV32,OSV33,OSV34,OSV35,OSV36,RF1,RF2,RF3,RF4,RF5,RF6,DLP0,DLP1,DLP2,DLP3,P3List,P3Max,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal,LYFlag,LYType,LYRfp,LYTrp,LYPts,LYVal,LYTrg";
//static const char* szHEADER_V4	= "GroupNo,Name,UType,SetNo,PLevel,VShift,RFType,RFAllow,RFVal,P1Allow,P2Allow,P3Allow,LBP1,LBP2,LBP3,PAL1,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,MSType3,MSV31,MSV32,MSV33,MSV34,MSV35,MSV36,OSV31,OSV32,OSV33,OSV34,OSV35,OSV36,RF1,RF2,RF3,RF4,RF5,RF6,DLP0,DLP1,DLP2,DLP3,P3List,P3Max,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal,LYFlag,LYType,LYRfp,LYTrp,LYPts,LYVal,LYTrg";
//static const char* szHEADER_V5	= "GroupNo,Name,UType,SetNo,PLevel,VShift,RFType,RFAllow,RFVal,P1Allow,P2Allow,P3Allow,LBP1,LBP2,LBP3,PAL1,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,MSType3,MSV31,MSV32,MSV33,MSV34,MSV35,MSV36,OSV31,OSV32,OSV33,OSV34,OSV35,OSV36,RF1,RF2,RF3,RF4,RF5,RF6,RFT1,RFT2,RFT3,RFT4,RFT5,RFT6,DLP0,DLP1,DLP2,DLP3,P3List,P3Max,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal";
//static const char* szHEADER_V6	= "GroupNo,Name,UType,SetNo,PLevel,VShift,RFType,RFAllow,RFVal,RFAppend,P1Allow,P2Allow,P3Allow,LBP1,LBP2,LBP3,PAL1,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,OSV1,OSV2,OSV3,OSV4,OSV5,OSV6,MSType3,MSV31,MSV32,MSV33,MSV34,MSV35,MSV36,OSV31,OSV32,OSV33,OSV34,OSV35,OSV36,RF1,RF2,RF3,RF4,RF5,RF6,RFT1,RFT2,RFT3,RFT4,RFT5,RFT6,DLP0,DLP1,DLP2,DLP3,P3List,P3Max,RVFlag,RVType,RVTrp1,RVTrp2,RVPts,RVVal";

void CSmartPayGroupDatabase::CreateHeaders()
{
	AddHeaderField( "GroupNo" );
	AddHeaderField( "Name" );
	AddHeaderField( "UType" );
	AddHeaderField( "SetNo" );
	AddHeaderField( "PLevel" );
	AddHeaderField( "VShift" );
	AddHeaderField( "RFType" );
	AddHeaderField( "RFAllow" );
	AddHeaderField( "RFVal" );
	AddHeaderField( "RFAppend" );
	AddHeaderField( "P1Allow" );
	AddHeaderField( "P2Allow" );
	AddHeaderField( "P3Allow" );
	AddHeaderField( "LBP1" );
	AddHeaderField( "LBP2" );
	AddHeaderField( "LBP3" );
	AddHeaderField( "PAL1" );
	AddHeaderField( "MSType" );
	AddHeaderField( "MSV1" );
	AddHeaderField( "MSV2" );
	AddHeaderField( "MSV3" );
	AddHeaderField( "MSV4" );
	AddHeaderField( "MSV5" );
	AddHeaderField( "MSV6" );
	AddHeaderField( "OSV1" );
	AddHeaderField( "OSV2" );
	AddHeaderField( "OSV3" );
	AddHeaderField( "OSV4" );
	AddHeaderField( "OSV5" );
	AddHeaderField( "OSV6" );
	AddHeaderField( "MSType3" );
	AddHeaderField( "MSV31" );
	AddHeaderField( "MSV32" );
	AddHeaderField( "MSV33" );
	AddHeaderField( "MSV34" );
	AddHeaderField( "MSV35" );
	AddHeaderField( "MSV36" );
	AddHeaderField( "OSV31" );
	AddHeaderField( "OSV32" );
	AddHeaderField( "OSV33" );
	AddHeaderField( "OSV34" );
	AddHeaderField( "OSV35" );
	AddHeaderField( "OSV36" );
	AddHeaderField( "RF1" );
	AddHeaderField( "RF2" );
	AddHeaderField( "RF3" );
	AddHeaderField( "RF4" );
	AddHeaderField( "RF5" );
	AddHeaderField( "RF6" );
	AddHeaderField( "RFT1" );
	AddHeaderField( "RFT2" );
	AddHeaderField( "RFT3" );
	AddHeaderField( "RFT4" );
	AddHeaderField( "RFT5" );
	AddHeaderField( "RFT6" );
	AddHeaderField( "DLP0" );
	AddHeaderField( "DLP1" );
	AddHeaderField( "DLP2" );
	AddHeaderField( "DLP3" );
	AddHeaderField( "P3List" );
	AddHeaderField( "P3Max" );
	AddHeaderField( "RVFlag" );
	AddHeaderField( "RVType" );
	AddHeaderField( "RVTrp1" );
	AddHeaderField( "RVTrp2" );
	AddHeaderField( "RVPts" );
	AddHeaderField( "RVVal" );
	AddHeaderField( "RVDis" );		//NEW TO HEADER V7
	AddHeaderField( "LYFlag" );		//NEW TO HEADER V7
	AddHeaderField( "LYType" );		//NEW TO HEADER V7
	AddHeaderField( "LYTrp" );		//NEW TO HEADER V7
	AddHeaderField( "LYPts" );		//NEW TO HEADER V7
	AddHeaderField( "LYVal" );		//NEW TO HEADER V7
	AddHeaderField( "LYLmt" );		//NEW TO HEADER V7
	AddHeaderField( "LYTrg" );		//NEW TO HEADER V7
	AddHeaderField( "LYDis" );		//NEW TO HEADER V7
	AddHeaderField( "RDFlag" );		//NEW TO HEADER V7
	AddHeaderField( "RDTrp" );		//NEW TO HEADER V7
	AddHeaderField( "RDVal" );		//NEW TO HEADER V7
	AddHeaderField( "RDPur" );		//NEW TO HEADER V7
	AddHeaderField( "RDDis" );		//NEW TO HEADER V7
}

/**********************************************************************/

void CSmartPayGroupDatabase::AddHeaderField( const char* szField )
{
	if ( m_strHeader != "" )
		m_strHeader += ",";

	m_strHeader += szField;
}

/**********************************************************************/
