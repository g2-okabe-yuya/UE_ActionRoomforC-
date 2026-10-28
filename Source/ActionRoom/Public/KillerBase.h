#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillerBase.generated.h"

UCLASS()
class ACTIONROOM_API AKillerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AKillerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
