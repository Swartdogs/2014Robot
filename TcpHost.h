#ifndef TCPHOST_H_
#define TCPHOST_H_

#include "WPILib.h"
#include "RobotLog.h"

class TcpHost
{
public:
	TcpHost(RobotLog* robotLog, int robotDataCount, int dbDataCount, int dbButtonCount);
	~TcpHost();

	RobotLog*	m_robotLog;

	std::string CountReply();                                       // Returns count of Robot, DbData, DbButton values
	std::string	DataString(int32_t number);							// Returns string with number followed by comma

	bool        GetButton(int group, int button);					// Returns current state of the Dashboard Button
	bool		GetButtonPress(int group, int button);				// Returns pressed state of the Dashboard Button
	std::string GetReply(int beginIndex, int endIndex);				// Returns GET reply for all Robot values in the specified range
	int32_t     GetValue(int index);								// Returns Dashboard Value at specified index

	bool        SetDashboardButton(int index, int32_t value);		// Sets Dashboard Button group at specified index
	bool        SetDashboardValue(int index, int32_t value);		// Sets Dashboard Value at specified index
	bool 		SetRobotValue(int index, int32_t value);			// Sets Robot Value at specified index
	void 		SetRobotMode(int mode);								// Sets current Robot Mode

private:
	struct button {
		int32_t state;
		int32_t pressed;
	};

	Task		m_task;
	int         m_dashboardButtonCount;
	int         m_dashboardValueCount;
	int			m_robotValueCount;
	int 		m_robotMode;

	button*		m_dashboardButton;
	int32_t*	m_dashboardValue;
	int32_t*	m_robotValue;
};

#endif
