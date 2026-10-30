// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowLight.h"

#include "MyThirdPersonCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFollowLight::AFollowLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFollowLight::BeginPlay()
{
	Super::BeginPlay();
	GetPlayer();
}

// Called every frame
void AFollowLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(Player))
	{
		Player->SetActorLocation(Player->GetActorLocation() + LocationZ);
	}
}

AActor* AFollowLight::GetPlayer()
{
	Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	return Player;
}
