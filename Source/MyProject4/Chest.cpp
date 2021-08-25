#include "Chest.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	Score = 100.0f;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetBoxExtent(FVector(75.0f, 60.0f, 60.0f));

	RootComponent = BoxCollision;

	SM_Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Chest"));
	SM_Chest->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SM_Chest->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnBeginOverlap);
}

void AChest::BeginPlay()
{
	Super::BeginPlay();

}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChest::DestroyChest()
{
	this->Destroy();
}

void AChest::OnBeginOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (IsValid(Player) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AChest::OnBeginOverlap IsValid(Player) == false"));
			return;
		}

		Player->Score += this->Score;

		UE_LOG(LogTemp, Warning, TEXT("Chest Looted"));
		UE_LOG(LogTemp, Warning, TEXT("Score: %.1f"), Player->Score);

		this->DestroyChest();
	}
}