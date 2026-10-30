#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyThirdPersonCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

// GameModeに設定しているのでDefaultで呼び出される
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
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	UInputAction* IA_Pause;
	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> StageNameWidget;
	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> PauseWidget;
	UPROPERTY(EditAnywhere, Category = "Game Settings")
	float GameOverDelay;
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentPauseWidget;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	// ステージ開始時にステージ名が浮かび上がる
	void CreateWBPStageNameWidget(APlayerController* PlayerController);
	// GameOverが呼び出されるとRespawnもセットで呼び出される
	void GameOver();
	void Respawn();
	
	// Bindする関数
	void OnLookInput(const FInputActionValue& Value);
	void OnMovementInput(const FInputActionValue& Value);
	void OnJumpInput(const FInputActionValue& Value);
	void OnJumpCompleted(const FInputActionValue& Value);
	void OnPauseInput(const FInputActionValue& Value);
	
	// Colに紐づける衝突判定
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	
	FTransform InitTransform;
	FTransform InitMeshTransform;
	FTimerHandle RespawnTimerHandle;
	bool IsGamePaused;
};
