#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "MyThirdPersonCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;


UCLASS()
class ACTIONROOM_API AMyThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyThirdPersonCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> StageNameWidget;
	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> PauseWidget;
	UPROPERTY(EditAnywhere, Category = "Game Settings")
	float GameOverDelay;
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentPauseWidget;
	
	void CreateWBPStageNameWidget(APlayerController* PlayerController);
	void OnLookInput(const FInputActionValue& Value);
	void OnMovementInput(const FInputActionValue& Value);
	void OnJumpInput(const FInputActionValue& Value);
	void OnJumpCompleted(const FInputActionValue& Value);
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	void GameOver();
	void Respawn();

	FTransform InitTransform;
	FTransform InitMeshTransform;
	FTimerHandle RespawnTimerHandle;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
};
