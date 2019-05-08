#include "WPILib.h"
#include "IterativeRobot525.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "RobotMap.h"
#include "RobotLog.h"
#include "Commands/AllCommands.h"
#include <cstdlib>

class CommandBasedRobot : public IterativeRobot525, public RobotLog {
private:
	typedef enum {mInit, mStart, mDisabled, mAutonomous, mTeleop, mTest}RunMode;
	double const        m_brownoutShed = 7.0;
	double const        m_brownoutRestore = 8.5;
	
	CommandGroup*		m_autoCommand;
	
	int      			m_autoDelay;				// Autonomous Start Delay Count
	int					m_autoSelect;				// Autonomous Command Group Selection
	int                 m_brownoutLevel;
	char       			m_log[100];
	int32_t      		m_periodicCount;
	double     			m_periodicBeginTime;
	double     			m_periodicLastEnd;
	double     			m_periodicLastStart;
	double     			m_periodicTotalTime;
	RunMode    			m_runMode;
	
	Solenoid*          	m_sensorPower1;
	Solenoid*  			m_sensorPower2;

	void EndOfPeriodic() {										
		m_periodicCount++;										
		m_periodicLastEnd = GetClock() * 1000;					
		
		double runTime = m_periodicLastEnd - m_periodicLastStart;	
		
		m_periodicTotalTime += runTime;							
		
		if(runTime  > 10) {
			sprintf(m_log, "Long Periodic Duration=%f", runTime);
			LogWrite(m_log);
		}
	}
	
	void LogWrite(std::string logEntry){
		static FILE*	logFile;
		const char*		cLogEntry;

		if (m_runMode == mInit) return;
		
		cLogEntry = logEntry.c_str();

		if (m_periodicCount > 0) {
			printf("%5d  %5d: %s \n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), cLogEntry);
		} else {
			printf("%s \n", cLogEntry);
		}

		if (logFile == NULL) logFile = fopen("/home/lvuser/Log525.txt", "a");

		if (logFile != NULL) {
			if (m_periodicCount > 0) {
				fprintf(logFile, "%5d  %5d: %s \r\n", m_periodicCount, (int)(((GetClock()*1000) - m_periodicBeginTime) / 20), cLogEntry);
			} else {
				fprintf(logFile, "%s \r\n", cLogEntry);
			}

			if (m_runMode == mDisabled) {
				fclose(logFile);
				logFile = NULL;
			}
		}
	}

	void IniParser() {
		char 		temp[100];
		int   		subsystemIndex;
		std::string	subsystemName[3];
		int     	equalSign;
		std::string key;
		int32_t		keyValue;
		
		subsystemName[0] = "[BACKPICKUP]";
		subsystemName[1] = "[FRONTPICKUP]";
		subsystemName[2] = "[BALLSHOOTER]";
		
		FILE* iniFile = fopen("/home/lvuser/525Init.ini", "r");
		
		if (iniFile == NULL) {
			LogWrite("IniParser: File not found");
			fclose(iniFile);
			return;
		}
			
		while(!feof(iniFile)) {
			fgets(temp, 100, iniFile);
			std::string line(temp);
			line.erase(line.length() - 2, 2);
			
			if(line[0] == '[') {
				subsystemIndex = -1;
				
				for(int i = 0; i < 3; i++) {
					if (line == subsystemName[i]) {
						subsystemIndex = i;
						break;
					}
				}

			} else if(line.length() > 0 && line[0] != ' ' && line[0] != '!') {
				equalSign = line.find('=');
				if(equalSign != 0) {
					key = line.substr(0, equalSign);
					keyValue = atoi(line.substr(equalSign + 1).c_str());

					switch(subsystemIndex) {
					case 0:								// Back Pickup
						CommandBase::backPickup->SetConstant(key, keyValue);
						break;
					case 1:								// Front Pickup
						CommandBase::frontPickup->SetConstant(key, keyValue);
						break;
					case 2:								// Ball Shooter
						CommandBase::ballShooter->SetConstant(key, keyValue);
						break;
					default:;
					}
				}
			}
			
		}
		fclose(iniFile);
	}
	
	void StagedBrownout() {
		static int delayCount = 0;
		double voltage = DriverStation::GetInstance()->GetBatteryVoltage();

		if(voltage < m_brownoutShed) {
			if(m_brownoutLevel < 2) {
				m_brownoutLevel++;
				delayCount = 0;
				CommandBase::drive->SetBrownoutLevel(m_brownoutLevel);
				sprintf(m_log, "Brownout Stage=%d", m_brownoutLevel);
				LogWrite(m_log);
			}

		} else if(m_brownoutLevel > 0) {
			if(voltage > m_brownoutRestore) {
				delayCount++;
				if(delayCount > 10) {
					m_brownoutLevel--;
					delayCount = 0;
					CommandBase::drive->SetBrownoutLevel(m_brownoutLevel);
					sprintf(m_log, "Brownout Stage=%d", m_brownoutLevel);
					LogWrite(m_log);
				}
			} else {
				delayCount = 0;
			}
		}
	}

	virtual void RobotInit() {
		m_runMode = mInit;
		
		CommandBasedRobot::SetPeriod(0.02);
		
		m_sensorPower1 = new Solenoid(1, SOLENOID_SENSOR_POWER1);
		m_sensorPower2 = new Solenoid(1, SOLENOID_SENSOR_POWER2);
		
		m_runMode = mStart;
		m_autoDelay = m_autoSelect = m_periodicCount = 0;
		m_autoCommand = NULL;
		m_brownoutLevel = 0;

		CommandBase::Init(this);
		IniParser();
		CommandBase::pdp->SetEnabled(true);

		LogWrite("");
		LogWrite("Flik: Robot Init (Build 2014 CowTown)");
	}

	virtual void DisabledInit() {
		if(m_runMode != mStart){								// Log Usage at end of Autonomous and Telelop
			sprintf(m_log, "Periodic Usage=%5.1f %%", (m_periodicTotalTime / (GetClock() * 1000 - m_periodicBeginTime)) * 100);
			LogWrite(m_log);
		}

		if (m_runMode == mAutonomous) {
			if (m_autoCommand != NULL) m_autoCommand->Cancel();
		}

		CommandBase::StopMotors();
		CommandBase::drive->SetSafetyEnabled(false);
		CommandBase::drive->SetLEDRing(false);
		CommandBase::backPickup->SetPickupMode(BackPickup::bStore);
		CommandBase::db->SetRobotMode(1);
		CommandBase::pdp->SetLogEnabled(false);
		
		m_periodicCount = 0;
		m_runMode = mDisabled;

		LogWrite("Flik: Disabled Init");
	}

	virtual void DisabledPeriodic() {
		static int CameraStartDelay = 250;
		
		if (CameraStartDelay > 0) {										// Start Camera after delay
			CameraStartDelay--;											// (Waiting for completion of camera bootup)
			if (CameraStartDelay == 0) {
//				CommandBase::findTarget->StartCamera("10.5.25.9");		// Start Camera
//				LogWrite("Start Camera");
			}
		}

		if (CommandBase::oi->GetButtonPress(11)) IniParser();
		if (CommandBase::db->GetButtonPress(0, 1)) IniParser();
		if (CommandBase::db->GetButtonPress(0, 2)) CommandBase::pdp->ResetPeaks();

		CommandBase::db->SetRobotValue(32, (int32_t)(CommandBase::pdp->GetVoltage() * 100 + 0.5));
		CommandBase::db->SetRobotValue(33, (int32_t)(CommandBase::pdp->GetLowVoltage() * 100 + 0.5));
		CommandBase::db->SetRobotValue(34, (int32_t)(CommandBase::drive->GetGyroAngle() * 10 + 0.5));
		CommandBase::db->SetRobotValue(35, CommandBase::frontPickup->GetPosition(FrontPickup::pRight));
		CommandBase::db->SetRobotValue(36, CommandBase::frontPickup->GetPosition(FrontPickup::pLeft));
		CommandBase::db->SetRobotValue(37, CommandBase::ballShooter->GetShooterPosition());
		CommandBase::db->SetRobotValue(38, CommandBase::backPickup->GetPosition());
		CommandBase::db->SetRobotValue(39, (int32_t)(CommandBase::drive->GetRange() * 10 + 0.5));
	}
	
	virtual void AutonomousInit() {
		m_sensorPower1->Set(true);
		m_sensorPower2->Set(true);

		m_runMode = mAutonomous;								// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;
		m_autoSelect = m_autoDelay = 0;

		CommandBase::db->SetRobotMode(2);
		CommandBase::pdp->SetLogEnabled(CommandBase::db->GetButton(0, 3));

		m_autoSelect = CommandBase::db->GetValue(0);
		m_autoDelay = CommandBase::db->GetValue(1);

		switch(m_autoSelect) {									// Create instance of selected Autonomous Command Group
			case 1: m_autoCommand = new OneBallAuto((m_autoDelay / 4.0));
					break;
			case 2: m_autoCommand = new TwoBallHotAuto();
				    break;
			case 3: m_autoCommand = new TwoBallAuto();
					break;
			case 4: m_autoCommand = new TwoBallRightAuto();
			        break;
			case 5: m_autoCommand = new ZeroBallAuto((m_autoDelay / 4.0), 60.0);
					break;
			default: m_autoCommand = NULL;
		}

		CommandBase::drive->ResetGyro();
		CommandBase::ballShooter->Load();
		
		if (m_autoCommand != NULL) m_autoCommand->Start();		// Start Autonomous Command Group

		sprintf(m_log, "Flik: Autonomous Init  Command=%d  Delay=%d", m_autoSelect, m_autoDelay * 250);
		LogWrite(m_log);
	}
	
	virtual void AutonomousPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		CommandBase::Periodic(m_runMode);
		Scheduler::GetInstance()->Run();

		CommandBase::db->SetRobotValue(32, (int32_t)(CommandBase::pdp->GetVoltage() * 100 + 0.5));
		CommandBase::db->SetRobotValue(33, (int32_t)(CommandBase::pdp->GetLowVoltage() * 100 + 0.5));

		for (int i = 0; i < 16; i++) {
			CommandBase::db->SetRobotValue(i, (int32_t)(CommandBase::pdp->GetCurrent(i) * 10 + 0.5));
			CommandBase::db->SetRobotValue(i + 16, (int32_t)(CommandBase::pdp->GetPeak(i) * 10 + 0.5));
		}

		EndOfPeriodic();
	}
	
	virtual void TeleopInit() {
		if (m_autoCommand != NULL) m_autoCommand->Cancel();
		
		m_sensorPower1->Set(true);
		m_sensorPower2->Set(true);

		m_runMode = mTeleop;									// Set Run Mode and Initialize Variables
		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		CommandBase::db->SetRobotMode(3);
		CommandBase::pdp->SetLogEnabled(CommandBase::db->GetButton(0, 3));
		CommandBase::ballShooter->Load();
		
		LogWrite("Flik: Teleop Init");
	}
	
	virtual void TeleopPeriodic() {
		double timeNow = GetClock() * 1000;
		
		if((timeNow - m_periodicLastStart) > 100){				// Log Periodic() Intervals > 100ms
			sprintf(m_log, "Delay    Last Start=%f  Last End=%f", timeNow - m_periodicLastStart, timeNow - m_periodicLastEnd);
			LogWrite(m_log);
		}
		
		m_periodicLastStart = timeNow;							// Set Periodic Start Time

		CommandBase::Periodic(m_runMode);
		Scheduler::GetInstance()->Run();

		CommandBase::db->SetRobotValue(32, (int32_t)(CommandBase::pdp->GetVoltage() * 100 + 0.5));
		CommandBase::db->SetRobotValue(33, (int32_t)(CommandBase::pdp->GetLowVoltage() * 100 + 0.5));

		for (int i = 0; i < 16; i++) {
			CommandBase::db->SetRobotValue(i, (int32_t)(CommandBase::pdp->GetCurrent(i) * 10 + 0.5));
			CommandBase::db->SetRobotValue(i + 16, (int32_t)(CommandBase::pdp->GetPeak(i) * 10 + 0.5));
		}

//		StagedBrownout();

		EndOfPeriodic();
	}
	
	virtual void TestInit() {
		if (m_autoCommand != NULL) m_autoCommand->Cancel();
		Scheduler::GetInstance()->Run();
		
		m_sensorPower1->Set(true);
		m_sensorPower2->Set(true);
		m_runMode = mTest;										

		m_periodicCount = 0;
		m_periodicLastStart = GetClock() * 1000;
		m_periodicLastEnd = m_periodicLastStart;
		m_periodicBeginTime = m_periodicLastStart;
		m_periodicTotalTime = 0;

		CommandBase::db->SetRobotMode(4);
		CommandBase::StopMotors();

		LogWrite("Flik: Test Init");
	}

	virtual void TestPeriodic() {
		if (CommandBase::db->GetButtonPress(0, 0)) {
			LogWrite("Flik: Reset Robot");
			CommandBase::pdp->ResetPeaks();
			CommandBase::ballShooter->Release();
			CommandBase::frontPickup->SetPickupMode(FrontPickup::fStart);
			CommandBase::backPickup->SetPickupMode(BackPickup::bStart);
		}

		if (CommandBase::oi->GetButtonPress(11)) {				// Release the shooter
			CommandBase::ballShooter->Release();
		}
		
		if(CommandBase::oi->GetButtonPress(10)) {				// Put robot in starting configuration
			CommandBase::frontPickup->SetPickupMode(FrontPickup::fStart);
			CommandBase::backPickup->SetPickupMode(BackPickup::bStart);
		}
		
		CommandBase::Periodic(m_runMode);
		
//		printf("Shooter Has Ball=%d\n", CommandBase::ballShooter->HasBallRightNow());
//		printf("BackPickup ball detected %d\n", CommandBase::backPickup->HasBall());
	
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

