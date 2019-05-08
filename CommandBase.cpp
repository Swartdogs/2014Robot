#include "CommandBase.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

Drive* 			CommandBase::drive = NULL;
FindTarget* 	CommandBase::findTarget = NULL;
FrontPickup*	CommandBase::frontPickup = NULL;
BackPickup*		CommandBase::backPickup = NULL;
BallShooter* 	CommandBase::ballShooter = NULL;
OI* 			CommandBase::oi = NULL;
TcpHost*		CommandBase::db = NULL;
PdpData*		CommandBase::pdp = NULL;
RobotLog*		CommandBase::robotLog = NULL;

void CommandBase::Init(RobotLog* logDelegate) {
	drive = 		new Drive(logDelegate);
	findTarget = 	new FindTarget(logDelegate);
	ballShooter = 	new BallShooter(logDelegate);
	frontPickup = 	new FrontPickup(logDelegate);
	backPickup = 	new BackPickup(logDelegate);
	oi = 			new OI(drive);
	db = 			new TcpHost(logDelegate, 40, 2, 1);		// Robot Values, Dashboard Values, Dashboard Button Groups
	pdp = 			new PdpData(logDelegate);
	robotLog = 		logDelegate;
}

void CommandBase::Periodic(int runMode) {
	frontPickup->Periodic();
	backPickup->Periodic();
	ballShooter->Periodic(runMode);
	oi->Periodic();
}

void CommandBase::StopMotors() {
	drive->StopMotors();
	frontPickup->StopMotors();
	backPickup->StopMotors();
	ballShooter->StopMotors();
}
