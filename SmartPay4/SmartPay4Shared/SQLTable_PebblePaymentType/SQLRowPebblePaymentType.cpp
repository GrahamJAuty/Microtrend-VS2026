//**********************************************************************
#include "SQLRowPebblePaymentType.h"
//**********************************************************************

CSQLRowPebblePaymentType::CSQLRowPebblePaymentType()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebblePaymentType::Reset()
{
	m_nSPOSPaymentType = 1;
	m_strPebblePaymentName = "";
	m_nPebbleTranType = 0;
}

//**********************************************************************

CString CSQLRowPebblePaymentType::GetPebbleTranTypeName(int n)
{
	switch (n)
	{
	case 0:
		return "cash";
		break;

	case 1:
		return "cheque";
		break;

	case 2:
		return "ePayment";
		break;

	default:
		return "unknown";
		break;
	}
}

//**********************************************************************

bool CSQLRowPebblePaymentType::IsValidSPOSPaymentType(int nType, bool bTopup)
{
	bool bResult = FALSE;

	switch (nType)
	{
	case 0:	//CASH
	case 1:	//CHEQUE
	case 2: //EFT
	case 3: //EFT CNP
	case 9:	//EFT ORDER
		bResult = TRUE;
		break;

	case 6: //LOYALTY
		bResult = (FALSE == bTopup);
		break;
	}

	return bResult;
}

//**********************************************************************
