#include "SwitchRotateActor.h"

void ASwitchRotateActor::BeginPlay()
{
	Super::BeginPlay();
	BindTimeline();
}

void ASwitchRotateActor::BindTimeline()
{
	// Timelineの更新処理をBind
	FOnTimelineFloat UpdateProgress;
	UpdateProgress.BindUFunction(this, "HandleRotationTimelineUpdate");
	RotateTimeline.AddInterpFloat(RotateCurve, UpdateProgress);
}

void ASwitchRotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateTimeline.TickTimeline(DeltaTime);
}

void ASwitchRotateActor::OnPressedButton()
{
	Super::OnPressedButton();

	StartRotation = RotateActor->GetActorRotation();
	StartAngle = StartRotation.Yaw;
	FinishAngle = StartAngle + 90.0f;

	RotateTimeline.PlayFromStart();
}

void ASwitchRotateActor::HandleRotationTimelineUpdate(float Angle)
{
	float RotationAngle = FMath::Lerp(StartAngle, FinishAngle, Angle);
	FRotator NewRotation = StartRotation;
	NewRotation.Yaw = RotationAngle;
	RotateActor->SetActorRotation(NewRotation);
}
