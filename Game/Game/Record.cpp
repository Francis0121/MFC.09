#include "stdafx.h"
#include "Record.h"


Record::Record(void){
	name = _T("");
	hour = 0;
	minute = 0;
	second = 0;
	timeset = 0;
}

Record::Record(CString name, int hour, int minute, int second, int timeset){
	this->name = name;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
	this->timeset = timeset;
}


Record::~Record(void)
{

}

int Record::CalculateTime(){
	return hour * 60 * 60 * 10 + minute * 60 * 10 +  second * 10 + timeset;
}
