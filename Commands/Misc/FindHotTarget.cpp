#include "../AllCommands.h"

FindHotTarget::FindHotTarget() {
	Requires(findTarget);
}

// Called just before this Command runs the first time
void FindHotTarget::Initialize() {
	CommandBase::robotLog->LogWrite("Command: FindHotTarget");
	findTarget->Init();
}

// Called repeatedly when this Command is scheduled to run
void FindHotTarget::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FindHotTarget::IsFinished() {
	return findTarget->IsFinished();
}

// Called once after isFinished returns true
void FindHotTarget::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FindHotTarget::Interrupted() {
}
