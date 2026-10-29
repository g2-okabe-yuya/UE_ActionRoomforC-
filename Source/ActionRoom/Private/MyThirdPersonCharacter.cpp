#include "MyThirdPersonCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KillerBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


AMyThirdPersonCharacter::AMyThirdPersonCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AMyThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent = GetMesh();
	SphereCollision = FindComponentByClass<UCapsuleComponent>();
	
	if (!SphereCollision)
	{
		UE_LOG(LogTemp, Error, TEXT("No sphere collision found"));
		return;
	}
	
	SphereCollision->OnComponentHit.AddDynamic(this, &AMyThirdPersonCharacter::OnComponentHit);
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
	}

}

void AMyThirdPersonCharacter::CreateWBPStageNameWidget(APlayerController* PlayerController)
{
	if (StageNameWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No stage name widget found"));
		return;
	}
	
	InitTransform = GetActorTransform();
	
	if (MeshComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh widget"));
		return;
	}
	
	InitMeshTransform = MeshComponent->GetRelativeTransform();
	
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

void AMyThirdPersonCharacter::GameOver()
{
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("Ragdoll",true);
	DisableInput(Cast<APlayerController>(GetController()));
	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AMyThirdPersonCharacter::Respawn,3.0f,false);
}

void AMyThirdPersonCharacter::Respawn()
{
	SetActorTransform(InitTransform);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionProfileName("CharacterMesh",true);
	K2_AttachToComponent(SphereCollision,"None",EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true);
	MeshComponent->SetRelativeTransform(InitMeshTransform);
	EnableInput(Cast<APlayerController>(GetController()));
}