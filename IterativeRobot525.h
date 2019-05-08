/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef ITERATIVE_ROBOT_525_H_
#define ITERATIVE_ROBOT_525_H_

#include "Timer.h"
#include "RobotBase.h"


class IterativeRobot525 : public RobotBase {
public:
	virtual void StartCompetition();

	virtual void RobotInit();
	virtual void DisabledInit();
	virtual void AutonomousInit();
    virtual void TeleopInit();
    virtual void TestInit();

	virtual void DisabledPeriodic();
	virtual void AutonomousPeriodic();
    virtual void TeleopPeriodic();
    virtual void TestPeriodic();

	void SetPeriod(double period);
	double GetPeriod();
	double GetLoopsPerSec();

protected:
	virtual ~IterativeRobot525();
	IterativeRobot525();

private:
	bool NextPeriodReady();

	bool m_disabledInitialized;
	bool m_autonomousInitialized;
    bool m_teleopInitialized;
    bool m_testInitialized;
	double m_period;
	Timer m_mainLoopTimer;
};

#endif

