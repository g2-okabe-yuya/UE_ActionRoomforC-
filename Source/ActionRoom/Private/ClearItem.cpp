#include "ClearItem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"

AClearItem::AClearItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AClearItem::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision = FindComponentByClass<UBoxComponent>();
	
	if (!BoxCollision)
	{
		UE_LOG(LogTemp,Error,TEXT("ClearItem: Can't find box component"));
		return;
	}
	
	// Collisionのイベントに登録
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this,&AClearItem::OnOverlapBegin);
}

void AClearItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// アクターを回転させる
	AddActorLocalRotation(FRotator(0,RotationZ,0));
}


void AClearItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,     
		int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	if (Cast<ACharacter>(OtherActor))
	{
		/**
		 * 現在のレベル名を取得する
		 * レベル名はFString型
		 */ 
		FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
		FString RoomIDToString = UKismetStringLibrary::Right(LevelName,2);
		int RoomID = UKismetStringLibrary::Conv_StringToInt(RoomIDToString);
		RoomID += 01;
		FText RoomName = UKismetTextLibrary::Conv_IntToText(RoomID,false,false,2,100);
		LevelName = UKismetTextLibrary::Conv_TextToString(RoomName);
		FString NextLevelName = UKismetStringLibrary::Concat_StrStr("Room_",LevelName);
		UGameplayStatics::OpenLevel(GetWorld(),UKismetStringLibrary::Conv_StringToName(NextLevelName));
		K2_DestroyActor();
	}
}

