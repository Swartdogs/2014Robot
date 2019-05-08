#ifndef DRIVE_H
#define DRIVE_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "PIDControl.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Srinu
 */
class Drive: public Subsystem {
public:
	Drive(RobotLog *robotLog);
	
	typedef enum {tOff, tArm, tDetect} TapeMode;
	typedef enum {rOff, rOn} RangeMode;
	
	bool 	CrossedTape();
	void 	DriveArcade(float move, float rotate);
	void    TapeDetectExecute(float move, float rotate);
	void    RangeDetectExecute(float move, float rotate);
	void 	ExecuteDistance();
	void 	ExecuteRotate();
	float   GetGyroAngle();
	double	GetRange();
	void 	InitDefaultCommand();
	void 	InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance);
	void 	InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float absoluteAngle);
	void 	InitDistance(double targetDistance, float maxSpeed, bool resetEncoders, double fireDistance, float relativeAngle, bool setRelativeZero);
	void 	InitRotate(float absoluteAngle);
	void 	InitRotate(float relativeAngle, bool setRelativeZero);
	bool 	OnTarget();
	void    SetBrownoutLevel(int level);
	void 	SetPID(float kP, float kI, float kD, bool drive);
	void 	SetSafetyEnabled(bool enabled);
	void 	StopEncoders();
	void 	StopMotors();
	void    ResetGyro();
	void    SetLEDRing(bool on);
	
	TapeMode  GetTapeMode();
	void      SetTapeMode(TapeMode mode);
	
	RangeMode GetRangeMode();
	void      SetRangeMode(RangeMode mode);
	
	void      EncoderTestEnabled(bool state);
	
private:
	Victor*         m_leftFront;
	Victor* 	    m_leftCenterRear;
	Victor*	   	    m_rightFront;
	Victor*	   	    m_rightCenterRear;
	Gyro*           m_gyro;
	Encoder*        m_leftEncoder;
	Encoder*        m_rightEncoder;
	PIDControl*     m_drivePID;
	PIDControl*	    m_rotatePID;
	RobotLog*       m_robotLog;
	DigitalInput*   m_tapeSensor;
	AnalogInput*    m_rangefinder;
	Solenoid*	    m_tapeDetectLED;
	
	TapeMode        m_tapeMode;
	RangeMode       m_rangeMode;
	
	bool            m_brakeApplied;
	bool            m_onTarget;
	bool            m_rampDone;
	bool		    m_useGyro;
	bool		    m_startCollection;
	bool			m_tapeDetected;
	char	        m_log[100];

	double          m_lastDistance;
	double          m_targetDistance;
	double          m_fireDistance;
	
	int32_t  		m_startLeftDistance;
	int32_t         m_startRightDistance;
	int32_t         m_endLeftDistance;
	int32_t		    m_endRightDistance;
	
	int             m_brownoutLevel;

	float		    m_driveSpeed;
	float           m_maxSpeed;
	float           m_relativeZero;
	float           m_targetAngle;
	
	double EncoderDistance(double val1, double val2);
	bool   RampSpeed(float& curSpeed, float pidSpeed);
	float  RotateError();
	
};

#endif
