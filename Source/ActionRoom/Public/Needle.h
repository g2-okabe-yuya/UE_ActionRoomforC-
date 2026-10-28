#pragma once

#include "CoreMinimal.h"
#include "KillerBase.h"
#include "Needle.generated.h"

// Playerにダメージを与える棘をつくる
UCLASS()
class ACTIONROOM_API ANeedle : public AKillerBase
{
	GENERATED_BODY()
public:
	ANeedle();
	/** 
	 * @brief PIE中に限らず配置生成されるたびに呼び出される 
	 * @detail NodeのConstructionScriptと同じ
	 */
	virtual void OnConstruction(const FTransform& Transform) override;
	
private:
	UPROPERTY(EditAnywhere,category="Needle")
	int LastIndexX = 1;
	UPROPERTY(EditAnywhere,category="Needle")
	int LastIndexY = 1;
	UPROPERTY()
	int MultiplyIndex = 100;

	/**
	 *  @brief 生成するMeshとMaterialを設定する
	 */
	UPROPERTY(EditAnywhere,category="Needle")
	TObjectPtr<UStaticMesh> ConeStaticMesh;
	UPROPERTY(EditAnywhere,category="Materials")
	UMaterialInterface* ConeMaterial;
};
