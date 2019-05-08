#include "../AllCommands.h"

CheckForHot::CheckForHot() {
	Requires(findTarget);
}

// Called just before this Command runs the first time
void CheckForHot::Initialize() {
	CommandBase::robotLog->LogWrite("Command: CheckForHot");
	if (findTarget->GetHotTarget()) SetTimeout(0.5);
	else SetTimeout(4.0);
}

// Called repeatedly when this Command is scheduled to run
void CheckForHot::Execute() {
}

// Make this return true when this Command no longer needs to run execute()
bool CheckForHot::IsFinished() {
	return IsTimedOut();
}

// Called once after isFinished returns true
void CheckForHot::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CheckForHot::Interrupted() {
}
