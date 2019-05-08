#include "../AllCommands.h"

OneBallHotDrive::OneBallHotDrive() {
	Requires(drive);
}

void OneBallHotDrive::Initialize() {
	CommandBase::robotLog->LogWrite("Command: OneBallHotDrive");
	drive->InitDistance(120, 0.7, true, 60, 0);
}

void OneBallHotDrive::Execute() {
	drive->ExecuteDistance();
}

bool OneBallHotDrive::IsFinished() {
	return drive->OnTarget();
}

void OneBallHotDrive::End() {
}

void OneBallHotDrive::Interrupted() {
}
