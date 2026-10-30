#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsCube.generated.h"

UCLASS()
class ACTIONROOM_API APhysicsCube : public AActor
{
	GENERATED_BODY()
	
public:	
	APhysicsCube();
	
	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CubeMesh;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

public:	
	
};
