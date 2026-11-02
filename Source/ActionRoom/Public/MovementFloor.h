// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementFloor.generated.h"

UENUM()
enum class EMoveState : uint8
{
	Stop,
	UpMove,
	DownMove,
};

UCLASS()
class ACTIONROOM_API AMovementFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovementFloor();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	TObjectPtr<AActor> MovementFloorMesh;

protected:
	virtual void BeginPlay() override;
	void OnDelayFinished();
	void ToggleStateAndRestartLoop();
	void MoveActor();

private:
	EMoveState MoveState = EMoveState::UpMove;
	FTimerHandle StateLoopTimerHandle;
};
