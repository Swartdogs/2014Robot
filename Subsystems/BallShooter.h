#ifndef BALLSHOOTER_H
#define BALLSHOOTER_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"
#include <string>

/**
 *
 *
 * @author Srinu
 */
class BallShooter: public Subsystem {

public:
	typedef enum {sIdle, sStart, sLoad, sReady, sFire, sFireReady, sRelease} ShootState;
	
	BallShooter(RobotLog* log);

	void  		Fire();
	int32_t 	GetShooterPosition();
	ShootState	GetShootState();
	bool  		HasBall();
	bool 		HasBallRightNow();
	void  		InitDefaultCommand();
	void  		Load();
	void        LogPosition(char* info);
	void  		Periodic(int runMode);
	void		Release();
	void        Reset();
	void        ResetHasBall();
	void		SetConstant(std::string key, int32_t value);
	void		StopMotors();
	
private:
	Victor*        m_shootMotor;
	AnalogInput*   m_shootPot;
	
	PIDControl*    m_shootPID;
	ShootState     m_shootState;
	
	DigitalInput*  m_ballSensor;
	
	RobotLog*	   m_robotLog;
	char		   m_log[100];
	
	std::string	   GetStateName(ShootState state);
	
	int32_t f_triggerPosition;
	int32_t f_releasePosition;
	
	int32_t m_maxReadyPosition;
	
	bool m_hasBall;
	int  m_runMode;
};

#endif
