#pragma once

#include "CoreMinimal.h"
#include "Switch_Base.h"
#include "SwitchRotateActor.generated.h"

UCLASS()
class ACTIONROOM_API ASwitchRotateActor : public ASwitch_Base
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RotateFloor")
	TObjectPtr<AActor> RotateActor;
	void HandleRotationTimelineUpdate(float Angle);
	void HandleRotationTimelineFinished();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPressedButton() override;
	void BindTimeline();

private:
	float StartAngle = 0.0f;
	float FinishAngle = 0.0f;
	FTimeline RotateTimeline;
	UPROPERTY(EditAnywhere,Category="Timeline")
	TObjectPtr<UCurveFloat> RotateCurve;
};
