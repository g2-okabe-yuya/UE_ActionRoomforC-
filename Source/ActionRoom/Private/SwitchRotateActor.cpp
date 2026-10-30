#include "SwitchRotateActor.h"

#include "Kismet/KismetMathLibrary.h"

void ASwitchRotateActor::BeginPlay()
{
	Super::BeginPlay();
	BindTimeline();
}

void ASwitchRotateActor::BindTimeline()
{
	// Timelineの更新処理と終了時の処理をBind
	FOnTimelineFloat UpdateProgress;
	UpdateProgress.BindUFunction(this, "HandleRotationTimelineUpdate");
	RotateTimeline.AddInterpFloat(RotateCurve,UpdateProgress);
	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, "HandleRotationTimelineFinished");
	ButtonUpDawn.SetTimelineFinishedFunc(FinishedEvent);
}

void ASwitchRotateActor::OnPressedButton()
{
	Super::OnPressedButton();
	
	FRotator Rotation = RotateActor->GetActorRotation();
	
	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
	StartAngle = Yaw;
	UKismetMathLibrary::BreakRotator(Rotation,Roll,Pitch,Yaw);
	Yaw += 90.0f;
	FinishAngle = Yaw;
	
	RotateTimeline.PlayFromStart();
}

void ASwitchRotateActor::HandleRotationTimelineUpdate(float Angle)
{
	float RotationAngle = FMath::Lerp(StartAngle,FinishAngle,Angle);
	FRotator NewRotation = UKismetMathLibrary::MakeRotator(0.0f,0.0f,RotationAngle);
	RotateActor->SetActorRotation(NewRotation);
}

void ASwitchRotateActor::HandleRotationTimelineFinished()
{
	ButtonUpDawn.ReverseFromEnd();
}

