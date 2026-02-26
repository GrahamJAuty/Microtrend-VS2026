#pragma once
/**********************************************************************/

struct CCashRSPImporterLinePosItem
{
public:
	CCashRSPImporterLinePosItem( int nLinePos = 0, bool b15Digit = FALSE );

public:
	int m_nLinePos;
	bool m_b15Digit;
};

/**********************************************************************/