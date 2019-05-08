#include "../AllCommands.h"

TwoBallHotDrive::TwoBallHotDrive() {
	m_decision = dRight;
	Requires(drive);
}

// Called just before this Command runs the first time
void TwoBallHotDrive::Initialize() {
	CommandBase::robotLog->LogWrite("Command: TwoBallHotDrive");

	if(findTarget->GetHotTarget()) {
		m_decision = dRight;
		drive->InitDistance(153, 0.8, true, 72, 23);
	} else {
		m_decision = dLeft;
		drive->InitDistance(132, 0.7, true, 0, 0);
	}
}

// Called repeatedly when this Command is scheduled to run
void TwoBallHotDrive::Execute() {
	drive->ExecuteDistance();
}

// Make this return true when this Command no longer needs to run execute()
bool TwoBallHotDrive::IsFinished() {
//	return drive->OnTarget();
	static int count = 0;

	if(m_decision == dLeft) {
		if(drive->OnTarget()) {
			return (++count > 20);
		} else {
			return false;
		}
	} else {
		return drive->OnTarget();
	}
}

// Called once after isFinished returns true
void TwoBallHotDrive::End() {
	if(m_decision == dLeft) {
		ballShooter->Fire();
	}
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TwoBallHotDrive::Interrupted() {
}
