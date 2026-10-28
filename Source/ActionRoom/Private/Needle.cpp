#include "Needle.h"
#include "Kismet/KismetMathLibrary.h"

ANeedle::ANeedle()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void ANeedle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (int i = 1; i <= LastIndexX; ++i)
	{
		int TransformX = UKismetMathLibrary::Subtract_IntInt(i,1);
		TransformX = TransformX * MultiplyIndex;
		for (int j = 1; j <= LastIndexY; ++j)
		{
			int TransformY = UKismetMathLibrary::Subtract_IntInt(j,1);
			TransformY = TransformY * MultiplyIndex;
			FTransform RelativeTransform = UKismetMathLibrary::MakeTransform(FVector(TransformX, TransformY,0.0f),FRotator(0.0f,0.0f,0.0f),FVector(1,1,1));
			
			// StaticMeshComponentを作成する
			UStaticMeshComponent* NewMeshComponent = Cast<UStaticMeshComponent>(
				AddComponentByClass(UStaticMeshComponent::StaticClass(),false,RelativeTransform,false));
			
			if (NewMeshComponent != nullptr)
			{
				NewMeshComponent->SetStaticMesh(ConeStaticMesh);
				NewMeshComponent->SetMaterial(0,ConeMaterial);
			}
		}
	}
}
