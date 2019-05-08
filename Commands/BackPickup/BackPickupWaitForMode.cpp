#include "../AllCommands.h"

BackPickupWaitForMode::BackPickupWaitForMode(BackPickup::BackMode mode) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(backPickup);
	
	m_mode = mode;
}

// Called just before this Command runs the first time
void BackPickupWaitForMode::Initialize() {
	CommandBase::robotLog->LogWrite("Command: BackPickup WaitForMode");
}

// Called repeatedly when this Command is scheduled to run
void BackPickupWaitForMode::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupWaitForMode::IsFinished() {
	return backPickup->GetBackPickupMode() == m_mode;
}

// Called once after isFinished returns true
void BackPickupWaitForMode::End() {
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupWaitForMode::Interrupted() {
}
