#pragma once
class Record
{

public:
	CString name;
		
	int hour;
	int minute;
	int second;
	int timeset;

	Record(void);
	Record(CString name, int hour, int minute, int second, int timeset);

	int CalculateTime();

	~Record(void);
};

