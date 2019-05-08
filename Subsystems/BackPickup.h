#ifndef BACKPICKUP_H
#define BACKPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

class BackPickup: public Subsystem {
public:
	typedef enum{bUnknown, bStore, bDeploy, bWaitToDeploy, bPass, bShoot, bCatch, bStart, bManualPass} BackMode;
	typedef enum{rIn, rManualIn, rOut, rOff} RollerMode;
	typedef enum{aUp, aDown} AdjustMode;
	
	BackPickup(RobotLog* log);
	BackMode	GetBackPickupMode();
	int32_t		GetPosition();
	bool    	HasBall();
	void		IncrementArm(AdjustMode mode);
	void 		InitDefaultCommand();
	bool 		OnTarget();
	void 		Periodic();
	bool        ReadyForShot();
	void        SetConstant(std::string key, int32_t value);
	void 		SetJoystickSpeed(float speed);
	void 		SetPickupMode(BackMode mode);
	void 		SetRollerMode(RollerMode mode);
	void 		SetUseJoystick(bool use);
	void        StopMotors();
	
private:
	std::string	GetModeName(BackMode mode);
	void 		SetSetpoint(int32_t target);
	bool 		ShooterHasBall();
	int32_t 	LimitValue(int32_t position);
	
	Victor* 		m_baseMotor;
	Victor* 		m_rollers;
	
	DigitalInput*   m_ballSensor;
	AnalogInput* 	m_baseMotorPot;
	PIDControl*    	m_baseMotorPID;
	RobotLog*      	m_robotLog;
	
	BackMode 	m_backMode;
	RollerMode  m_rollerMode;
	int32_t		m_baseTarget;
	float 		m_joySpeed;
	char		m_log[100];
	bool  		m_onTarget;
	bool		m_useJoystick;
	
	int32_t f_baseMotorDeadband;
	int32_t f_baseZeroOffset;
	int32_t f_baseMaxPosition;
	int32_t f_incrementValue;
	
	int32_t f_storeSetpoint;
	int32_t f_deploySetpoint;
	int32_t f_passSetpoint;
	int32_t f_shootSetpoint;
	int32_t f_catchSetpoint;
	int32_t f_startSetpoint;
};

#endif
