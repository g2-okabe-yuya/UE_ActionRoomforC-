#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ClearItem.generated.h"

UCLASS()
class ACTIONROOM_API AClearItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AClearItem();
	virtual void Tick(float DeltaTime) override;
	// PlayerがItemにぶつかった時のみClearとする
	UFUNCTION()
	/* @brief ぶつかったActorを判定してPlayerなら次のレベルに遷移する
	 * @param OverlappedComp 自分側のコリジョン
	 * @param OtherActor 触れてきたアクタ
	 * @param OtherComp 触れてきた側のコンポーネント
	 * @param OtherBodyIndex ボディ番号
	 * @param bFromSweep sweep移動での接触か
	 * @param SweepResult 当たり判定の詳細情報
	 * @detail Levelの後ろの数値を足す(01 => 02) Level名の後ろ2文字にID必須
	*/
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	
	// どのくらい回転するか
	UPROPERTY(EditAnywhere,Category="ItemSettings")
	float RotationZ = 0.5f;
	// 衝突判定で使用するコライダー
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="ItemSettings")
	TObjectPtr<UBoxComponent> BoxCollision;

protected:
	virtual void BeginPlay() override;
};
