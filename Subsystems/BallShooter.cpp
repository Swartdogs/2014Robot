#include "BallShooter.h"
#include "../Robotmap.h"
#include "../CommandBase.h"

BallShooter::BallShooter(RobotLog* log) : Subsystem("BallShooter") {
	f_triggerPosition = 920;
	f_releasePosition = 160;

	m_robotLog = log;
	
	m_shootMotor = new Victor(PWM_SHOOT_MOTOR);
	
	m_shootPot = new AnalogInput(AI_SHOOT_POT);
	m_shootPot->SetAverageBits(2);
	m_shootPot->SetOversampleBits(0);
	
	m_shootState = sIdle;
	m_maxReadyPosition = 0;
	
	m_shootPID = new PIDControl(0.005, 0.0, 0);
	m_shootPID->SetInputRange(0, 1000);       
	m_shootPID->SetOutputRange(-1.0, 1.0);
	
	m_ballSensor = new DigitalInput(DI_BALL_SHOOTER_SENSOR);
	
	m_hasBall = false;
	m_runMode = 0;
}
    
void BallShooter::Fire() {
	sprintf(m_log, "Shooter: Call to Fire with HasBall=%d", HasBall());
	m_robotLog->LogWrite(m_log);

	if(HasBall()) {
		m_shootState = sFire;
		sprintf(m_log, "Shooter: State=%s  MaxReady=%d", GetStateName(sFire).c_str(), m_maxReadyPosition);
		m_robotLog->LogWrite(m_log);

	} else if (m_runMode == 3) {					// Autonomous Mode
		m_shootState = sFireReady;
		m_robotLog->LogWrite("Shooter: State=FireReady");
	}
}

int32_t BallShooter::GetShooterPosition() {
	int32_t curReading = 0;
	static int32_t lastReading = 0;
	static bool potRollover = false;
	int32_t returnValue = 0;
	
	curReading = (int32_t)((float)m_shootPot->GetAverageValue() / 4 + 0.5);
	
	if(curReading > lastReading) {
		returnValue = curReading;
		potRollover = false;
	} else if(abs(curReading - lastReading) < 200) {
		returnValue = curReading;
	} else {
		returnValue = lastReading;
		if (!potRollover) {
			potRollover = true;
			m_robotLog->LogWrite("Shooter: Pot Rollover");
		}
	}
	
	lastReading = curReading;
	
	return returnValue;
}

BallShooter::ShootState BallShooter::GetShootState() {
	return m_shootState;
}

bool BallShooter::HasBall() {
	return m_hasBall;
}

bool BallShooter::HasBallRightNow() {
	return !m_ballSensor->Get();
}

void BallShooter::InitDefaultCommand() {
}

void BallShooter::Load() {
	if(m_shootState == sIdle) {
		m_shootState = sStart;
		sprintf(m_log, "Shooter: State=%s", GetStateName(sStart).c_str());
		m_robotLog->LogWrite(m_log);
	} else if (m_shootState == sFireReady) {
		m_shootState = sReady;
	}
}

void BallShooter::LogPosition(char* info) {
	sprintf(m_log, "BallShooter: %s Shooter Position=%d", info, GetShooterPosition());
	m_robotLog->LogWrite(m_log);
}

void BallShooter::Periodic(int runMode) {
	ShootState startState = m_shootState;
	float shootSpeed = 0;
	int32_t curPosition = GetShooterPosition();
	static int ballCount = 0;
	static int stallCount = 0;
	

	m_runMode = runMode;
	
	switch(m_shootState) {
	case sStart:
		m_hasBall = false;
		ballCount = 0;
		m_shootPID->SetPID(0.005, 0, 0); // 0.010, 0, 0
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) f_triggerPosition);
		m_shootState = sLoad;
		shootSpeed = 1.0;
		break;
	
	case sLoad:
		if(curPosition > f_triggerPosition || curPosition < 100) {
			shootSpeed = 1.0;
		} else {
			shootSpeed = m_shootPID->Calculate(curPosition);
			
			if(curPosition > (f_triggerPosition - 100)) {
				m_maxReadyPosition = curPosition;
				m_shootPID->SetPID(0.005, 0.0006, 0);    //  0.010  0.0006  0
				m_shootState = sReady;
				stallCount = 0;
			}
		}
		break;
	
	case sReady:
	case sFireReady:
		shootSpeed = m_shootPID->Calculate(curPosition);
		if (m_maxReadyPosition < curPosition) m_maxReadyPosition = curPosition;
		
		if (stallCount < 250) {
			if (shootSpeed > 0.5) {
				stallCount++;
			} else {
				stallCount = 0;
			}
		} else {
			if (stallCount == 150) m_robotLog->LogWrite("Shooter: Stall Detected"); 
			shootSpeed = 0.0;
		}
			
		if(!m_hasBall && (curPosition > (f_triggerPosition - 50))) {
			if(ballCount < 10) {
				if(!m_ballSensor->Get()) ballCount++;
				else ballCount = 0;
			} else {
				ballCount = 0;
				m_hasBall = true;
				m_robotLog->LogWrite("Shooter: Has Ball");
				if (m_shootState == sFireReady) {
					m_shootState = sFire;
					m_robotLog->LogWrite("Shooter: State=Fire");
				}
			}
		} else if(m_hasBall) {
			if(ballCount < 100) {
				if(m_ballSensor->Get()) ballCount++;
				else ballCount = 0;
			} else {
				ballCount = 0;
				ResetHasBall();
			}
		}
		break;
	
	case sFire:
		shootSpeed = m_shootPID->Calculate(curPosition);
		
		if(CommandBase::backPickup->ReadyForShot() && CommandBase::frontPickup->ReadyForShot()) {
			if(abs(curPosition - f_triggerPosition) < 100) {
				shootSpeed = 1.0;
			} else {
				m_shootState = sStart;
				m_robotLog->LogWrite("Shooter: Move to Trigger");
			}
		}
		break;
		
	case sRelease:
		shootSpeed = m_shootPID->Calculate(curPosition);
		if (curPosition < f_releasePosition + 10 || curPosition > f_triggerPosition + 10) m_shootState = sIdle;
		break;
		
	default:;
	}
	
	if (m_shootState != startState) {
		sprintf(m_log, "Shooter: State=%s", GetStateName(m_shootState).c_str());
		m_robotLog->LogWrite(m_log);
	}

	m_shootMotor->Set(shootSpeed);
}

void BallShooter::Release() {
	if (m_shootState == sReady) {
		m_shootPID->SetPID(0.010, 0, 0);
		m_shootPID->Reset();
		m_shootPID->SetSetpoint((float) f_releasePosition);
		m_shootState = sRelease;
		sprintf(m_log, "Shooter: State=%s", GetStateName(sRelease).c_str());
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::Reset() {
	if (m_shootState == sReady ) {
//	if (m_shootState == sReady && !HasBall()) {
		f_triggerPosition -= 1;					// -5
		m_shootState = sStart;
		sprintf(m_log, "Shooter: Reset to %d  MaxReady=%d", f_triggerPosition, m_maxReadyPosition);
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::ResetHasBall() {
	if(m_hasBall) m_robotLog->LogWrite("Shooter: Reset HasBall");
	m_hasBall = false;
}

void BallShooter::SetConstant(std::string key, int32_t value) {
	if(key == "triggerPosition") {
		f_triggerPosition = value;
		sprintf(m_log, "BallShooter: Set TriggerPosition=%d", value);
		m_robotLog->LogWrite(m_log);
	
	} else if(key == "releasePosition") {
		f_releasePosition = value;
		sprintf(m_log, "BallShooter: Set ReleasePosition=%d", value);
		m_robotLog->LogWrite(m_log);
	}
}

void BallShooter::StopMotors() {
	m_shootMotor->Set(0.0);
}


//  ******************** PRIVATE ********************


std::string BallShooter::GetStateName(ShootState state) {
	switch(state) {
	case sIdle:			return "Idle";
	case sStart:		return "Start";
	case sLoad:			return "Load";
	case sReady:		return "Ready";
	case sFire:			return "Fire";
	case sFireReady:  	return "FireReady";
	case sRelease:		return "Release";
	default:			return "?";
	}
}

