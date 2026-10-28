#include "KillerBase.h"

AKillerBase::AKillerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKillerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKillerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}