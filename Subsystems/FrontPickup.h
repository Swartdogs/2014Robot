#ifndef FRONTPICKUP_H
#define FRONTPICKUP_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"
#include <string>

/**
 *
 *
 * @author Collin
 */
class FrontPickup: public Subsystem {
public:
	typedef enum {fUnknown, fDeploy, fWaitToStore, fStore, fWaitToLoad, fLoad, fDropInShooter, fShoot, fLowDeploy, fLowShoot, fAutoLoad, fCatch, fStart} FrontMode;
	typedef enum {pLeft, pRight} Pot;
	typedef enum {wIn, wOut, wOff} RollerMode;
	
	FrontPickup(RobotLog* log);
	
	FrontMode	GetFrontPickupMode();
	int32_t 	GetPosition(Pot pot);
	bool 		HasBall();
	void 		IncrementArm(Pot arm, bool up);
	void 		InitDefaultCommand();
	bool 		OnTarget();
	void 		Periodic();
	bool 		ReadyForShot();
	void        SetConstant(std::string key, int32_t value);
	void 		SetJoystickLeft(float joyLeft);
	void 		SetJoystickRight(float joyRight);
	void 		SetPickupMode(FrontMode mode);
	void 		SetRollers(RollerMode mode);
	void 		SetUseJoystickLeft(bool use);
	void 		SetUseJoystickRight(bool use);
	void        StopMotors();
	
private:
	Victor* 		m_rightArm;
	Victor* 		m_leftArm;
	
	Relay* 			m_rightWheels;
	Relay* 			m_leftWheels;
	
	AnalogInput*	m_leftArmPot;
	AnalogInput* 	m_rightArmPot;

	PIDControl* 	m_leftArmPID;
	PIDControl* 	m_rightArmPID;
	
	DigitalInput*   m_ballLoadedSensor;
	
	RobotLog*       m_robotLog;

	FrontMode 		m_frontMode;
	float   		m_joyLeft;
	float   		m_joyRight;
	int32_t 		m_leftArmTarget;
	bool 			m_leftOnTarget;
	char			m_log[100];
	int32_t 		m_rightArmTarget;
	bool 			m_rightOnTarget;
	bool 			m_useJoystickLeft;
	bool 			m_useJoystickRight;
	
	int32_t 		f_leftArmZeroOffset;
	int32_t			f_leftArmMaxPosition;
	int32_t			f_rightArmZeroOffset;
	int32_t			f_rightArmMaxPosition;
	int32_t			f_armTargetDeadband;
	int32_t			f_incrementValue;
	
	int32_t 		f_deploySetpoint;
	int32_t			f_storeSetpoint;
	int32_t			f_loadSetpoint;
	int32_t			f_dropInShooterLeftSetpoint;
	int32_t			f_dropInShooterRightSetpoint;
	int32_t			f_shootSetpoint;
	int32_t			f_lowDeploySetpoint;
	int32_t			f_catchSetpoint;
	int32_t			f_startSetpoint;
	
	std::string		GetModeName(FrontMode mode);
	int32_t 		LimitValue(Pot pot, int32_t position);
	void 			SetSetpoints(int32_t leftPosition, int32_t rightPosition);
};

#endif
