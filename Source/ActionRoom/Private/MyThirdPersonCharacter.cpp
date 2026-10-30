#include "MyThirdPersonCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KillerBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AMyThirdPersonCharacter::AMyThirdPersonCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AMyThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitTransform = GetActorTransform();
	InitMeshTransform = MeshComponent->GetRelativeTransform();
	if (MeshComponent == nullptr)
	{
		MeshComponent = GetMesh();
	}
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	
	if (!CapsuleComp)
	{
		UE_LOG(LogTemp, Error, TEXT("No sphere collision found"));
		return;
	}
	
	CapsuleComp->OnComponentHit.AddDynamic(this, &AMyThirdPersonCharacter::OnComponentHit);
}

void AMyThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext,0);
				CreateWBPStageNameWidget(PlayerController);
			}
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Look)
		{
			EnhancedInputComponent->BindAction(IA_Look,ETriggerEvent::Triggered,this, &AMyThirdPersonCharacter::OnLookInput);
		}
		
		if (IA_Move) 
		{
			EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,this, &AMyThirdPersonCharacter::OnMovementInput);
		}
		
		if (IA_Jump)
		{
			EnhancedInputComponent->BindAction(IA_Jump,ETriggerEvent::Started,this, &AMyThirdPersonCharacter::OnJumpInput);
			EnhancedInputComponent->BindAction(IA_Jump,ETriggerEvent::Completed,this, &AMyThirdPersonCharacter::OnJumpCompleted);
		}
		if (IA_Pause)
		{
			EnhancedInputComponent->BindAction(IA_Pause,ETriggerEvent::Triggered,this, &AMyThirdPersonCharacter::OnPauseInput);
		}
	}

}

void AMyThirdPersonCharacter::CreateWBPStageNameWidget(APlayerController* PlayerController)
{
	if (StageNameWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No stage name widget found"));
		return;
	}
	
	if (MeshComponent == nullptr)
	{
		MeshComponent = GetMesh();
	}
	
	UUserWidget* CreateStageNameWidget = UWidgetBlueprintLibrary::Create(GetWorld(),StageNameWidget,PlayerController);
	CreateStageNameWidget->AddToViewport(0);
	
	if (!CurrentPauseWidget)
	{
		CurrentPauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(),PauseWidget,PlayerController);
	}
}

void AMyThirdPersonCharacter::OnLookInput(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyThirdPersonCharacter::OnMovementInput(const FInputActionValue& Value)
{
	FRotator Rotation = GetControlRotation();
	const FRotator YawOnlyRotation(0.0f,Rotation.Yaw,0.0f);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawOnlyRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawOnlyRotation);
	
	AddMovementInput(ForwardVector, Value.Get<FVector2D>().Y);
	AddMovementInput(RightVector, Value.Get<FVector2D>().X);
}

void AMyThirdPersonCharacter::OnJumpInput(const FInputActionValue& Value)
{
	Jump();
}

void AMyThirdPersonCharacter::OnJumpCompleted(const FInputActionValue& Value)
{
	StopJumping();
}

void AMyThirdPersonCharacter::OnComponentHit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{
	if (Cast<AKillerBase>(OtherActor))
	{
		GameOver();
	}
}

void AMyThirdPersonCharacter::OnPauseInput(const FInputActionValue& Value)
{
	if (IsGamePaused)
	{
		IsGamePaused = false;
	}
	else
	{
		IsGamePaused = true;
	}
	
	UGameplayStatics::SetGamePaused(GetWorld(),IsGamePaused);
	if (IsGamePaused)
	{
		CurrentPauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(),PauseWidget,Cast<APlayerController>(GetController()));
		CurrentPauseWidget->AddToViewport(0);
	}
	else
	{
		CurrentPauseWidget->RemoveFromParent();
	}
}

void AMyThirdPersonCharacter::GameOver()
{
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("Ragdoll",true);
	DisableInput(Cast<APlayerController>(GetController()));
	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AMyThirdPersonCharacter::Respawn,3.0f,false);
}

void AMyThirdPersonCharacter::Respawn()
{
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionProfileName("CharacterMesh",true);
	MeshComponent->AttachToComponent(GetCapsuleComponent(),FAttachmentTransformRules::KeepRelativeTransform,NAME_None);
	SetActorTransform(InitTransform);
	MeshComponent->SetRelativeTransform(InitMeshTransform);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		EnableInput(PlayerController);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Can't find player controller"));
	}
}