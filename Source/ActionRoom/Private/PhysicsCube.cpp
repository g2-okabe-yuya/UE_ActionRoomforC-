#include "PhysicsCube.h"

#include "MyThirdPersonCharacter.h"

APhysicsCube::APhysicsCube()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APhysicsCube::BeginPlay()
{
	Super::BeginPlay();
	
	// 指定した型のコンポーネントをすべて取得
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	
	for (UStaticMeshComponent* Component : MeshComponents)
	{
		if (Component && Component->GetFName() == "Cube")
		{
			CubeMesh = Component;
			break;
		}
	}
	
	if (CubeMesh == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("CubeMesh is nullptr"));
		return;
	}
	
	CubeMesh->OnComponentHit.AddDynamic(this,&APhysicsCube::OnComponentHit);
}

void APhysicsCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhysicsCube::OnComponentHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AMyThirdPersonCharacter>(OtherActor))
	{
		OverlappedComp->AddImpulse(FVector(0,0,3000.0f));
	}
}