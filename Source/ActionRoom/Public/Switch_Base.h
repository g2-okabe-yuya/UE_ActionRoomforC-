#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Switch_Base.generated.h"

UCLASS()
class ACTIONROOM_API ASwitch_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	ASwitch_Base();
	virtual void Tick(float DeltaTime) override;
	void BindTimeline();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Switch Base")
	TObjectPtr<UStaticMeshComponent> Button;
	FTimeline ButtonUpDawn;

protected:
	virtual void BeginPlay() override;
	virtual void OnPressedButton();

private:	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	UFUNCTION()
	void HandleTimelineUpdate(float Height);
	UFUNCTION()
	void HandleTimelineFinished();
	UFUNCTION()
	void OnReleasedButton();
	bool IsPressed;
	UPROPERTY(EditAnywhere,Category="Switch Base")
	TObjectPtr<UCurveFloat> MovementCurve;
};
