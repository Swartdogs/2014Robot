#include "../AllCommands.h"

BallShooterReset::BallShooterReset() {
	Requires(ballShooter);
}

void BallShooterReset::Initialize() {
	CommandBase::robotLog->LogWrite("Command: BallShooter Reset");
	ballShooter->Reset();
}

void BallShooterReset::Execute() {
}

bool BallShooterReset::IsFinished() {
	return true;
}

void BallShooterReset::End() {
}

void BallShooterReset::Interrupted() {
}
