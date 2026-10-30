#include "Switch_Base.h"
#include "MyThirdPersonCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ASwitch_Base::ASwitch_Base()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASwitch_Base::BeginPlay()
{
	Super::BeginPlay();
	BindTimeline();
	
	// 指定した型のコンポーネントをすべて取得
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	
	for (UStaticMeshComponent* Component : MeshComponents)
	{
		if (Component && Component->GetFName() == "SwitchButton")
		{
			Button = Component;
			break;
		}
	}
	
	if (Button == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("No button selected"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("No button selected"));
		Button->OnComponentHit.AddDynamic(this,&ASwitch_Base::OnComponentHit);
	}
}

void ASwitch_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ButtonUpDawn.TickTimeline(DeltaTime);
}

void ASwitch_Base::BindTimeline()
{
	// Timelineの更新処理と終了時の処理をBind
	FOnTimelineFloat UpdateProgress;
	UpdateProgress.BindUFunction(this, "HandleTimelineUpdate");
	ButtonUpDawn.AddInterpFloat(MovementCurve,UpdateProgress);
	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, "HandleTimelineFinished");
	ButtonUpDawn.SetTimelineFinishedFunc(FinishedEvent);
}

void ASwitch_Base::OnComponentHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AMyThirdPersonCharacter>(OtherActor))
	{
		if (IsPressed == false)
		{
			IsPressed = false;
			// タイムラインの再生
			ButtonUpDawn.PlayFromStart(); 
		}
	}
}

void ASwitch_Base::HandleTimelineUpdate(float Height)
{
	float ClampedVector = UKismetMathLibrary::MapRangeClamped(Height,0,1,35,15);
	FVector MakeVector = UKismetMathLibrary::MakeVector(0.0f,0.0f,ClampedVector);
	Button->SetRelativeLocation(MakeVector);
}

void ASwitch_Base::HandleTimelineFinished(ETimelineDirection::Type Direction)
{
	if (Direction == ETimelineDirection::Forward)
	{
		OnPressedButton();
	}
	else
	{
		OnReleasedButton();
	}
}

void ASwitch_Base::OnPressedButton(){}

void ASwitch_Base::OnReleasedButton()
{
	IsPressed = false;
}