
#include "MovementFloor.h"

AMovementFloor::AMovementFloor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMovementFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveActor();
}

void AMovementFloor::BeginPlay()
{
	Super::BeginPlay();
	OnDelayFinished();
}

void AMovementFloor::OnDelayFinished()
{
	switch (MoveState)
	{
	case EMoveState::UpMove:
		GetWorldTimerManager().SetTimer(
			StateLoopTimerHandle,
			this,
			&AMovementFloor::ToggleStateAndRestartLoop,
			1.0f,
			false
		);
		break;

	case EMoveState::DownMove:
		GetWorldTimerManager().SetTimer(
			StateLoopTimerHandle,
			this,
			&AMovementFloor::ToggleStateAndRestartLoop,
			1.0f,
			false
		);
		break;
	default: ;
	}
}

void AMovementFloor::ToggleStateAndRestartLoop()
{
	if (MoveState == EMoveState::UpMove)
	{
		MoveState = EMoveState::DownMove;
	}
	else
	{
		MoveState = EMoveState::UpMove;
	}
	
	GetWorldTimerManager().SetTimer(
		StateLoopTimerHandle,
		this,
		&AMovementFloor::OnDelayFinished,
		3.0f,
		false
	);
}

void AMovementFloor::MoveActor()
{
	switch (MoveState)
	{
	case EMoveState::Stop:
		break;
	case EMoveState::UpMove:
		MovementFloorMesh->AddActorLocalOffset(FVector(0.0f, 0.0f, 1.0f));
		break;
	case EMoveState::DownMove:
		MovementFloorMesh->AddActorLocalOffset(FVector(0.0f, 0.0f, -1.0f));
		break;
	}
}