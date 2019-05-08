#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include "WPILib.h"

class RobotLog {
public:
	virtual void LogWrite(std::string logEntry);

protected:
	RobotLog();
	virtual ~RobotLog();
};

#endif
