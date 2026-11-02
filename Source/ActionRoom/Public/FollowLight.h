#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowLight.generated.h"

UCLASS()
class ACTIONROOM_API AFollowLight : public AActor
{
	GENERATED_BODY()
	
public:	
	AFollowLight();
	virtual void Tick(float DeltaTime) override;
	AActor* GetPlayer();
	UPROPERTY(EditAnywhere,Category = "Follow Light")
	float LocationZ = 150.0f;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	AActor* Player;
};
