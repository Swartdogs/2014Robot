/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "IterativeRobot525.h"

#include "NetworkCommunication/UsageReporting.h"
#include "SmartDashboard/SmartDashboard.h"
#include "LiveWindow/LiveWindow.h"
#include "networktables/NetworkTable.h"

IterativeRobot525::IterativeRobot525()
	: m_disabledInitialized (false)
	, m_autonomousInitialized (false)
	, m_teleopInitialized (false)
	, m_testInitialized (false)
	, m_period (0.02)
{
}

IterativeRobot525::~IterativeRobot525()
{
}

void IterativeRobot525::SetPeriod(double period)
{
	if (period <= 0) period = 0.02;
	m_mainLoopTimer.Reset();
	m_mainLoopTimer.Start();
	m_period = period;
}

double IterativeRobot525::GetPeriod()
{
	return m_period;
}

double IterativeRobot525::GetLoopsPerSec()
{
	return 1.0 / m_period;
}

void IterativeRobot525::StartCompetition()
{
	static bool waitForNext = false;
	
//	nUsageReporting::report(nUsageReporting::kResourceType_Framework, nUsageReporting::kFramework_Iterative);

	RobotInit();

	while (true)
	{
		if (IsDisabled())
		{
			if(!m_disabledInitialized)
			{
				DisabledInit();
				m_disabledInitialized = true;
				m_autonomousInitialized = false;
                m_teleopInitialized = false;
                m_testInitialized = false;
			}
			if (NextPeriodReady())
			{
				DisabledPeriodic();
				waitForNext = true;
			}
		}
		else if (IsAutonomous())
		{
			if(!m_autonomousInitialized)
			{
				AutonomousInit();
				m_autonomousInitialized = true;
				m_disabledInitialized = false;
                m_teleopInitialized = false;
                m_testInitialized = false;
			}
			if (NextPeriodReady())
			{
				AutonomousPeriodic();
				waitForNext = true;
			}
		}
        else if (IsTest())
        {
            if(!m_testInitialized)
            {
                TestInit();
                m_testInitialized = true;
                m_disabledInitialized = false;
                m_autonomousInitialized = false;
                m_teleopInitialized = false;
            }
            if (NextPeriodReady())
            {
                TestPeriodic();
                waitForNext = true;
            }
        }
		else
		{
			if(!m_teleopInitialized)
			{
				TeleopInit();
				m_teleopInitialized = true;
				m_disabledInitialized = false;
                m_autonomousInitialized = false;
                m_testInitialized = false;
                Scheduler::GetInstance()->SetEnabled(true);
			}
			if (NextPeriodReady())
			{
				TeleopPeriodic();
				waitForNext = true;
			}
		}

		if(waitForNext) {
			waitForNext = false;
			
			if (m_period > 0.0) {
				Wait(m_period - m_mainLoopTimer.Get());
			}
		}
	}	
}


bool IterativeRobot525::NextPeriodReady()
{
	return m_mainLoopTimer.HasPeriodPassed(m_period);
}

void IterativeRobot525::RobotInit()
{
}

void IterativeRobot525::DisabledInit()
{
}

void IterativeRobot525::AutonomousInit()
{
}

void IterativeRobot525::TeleopInit()
{
}

void IterativeRobot525::TestInit()
{
}

void IterativeRobot525::DisabledPeriodic()
{
}

void IterativeRobot525::AutonomousPeriodic()
{
}

void IterativeRobot525::TeleopPeriodic()
{
}

void IterativeRobot525::TestPeriodic()
{
}

