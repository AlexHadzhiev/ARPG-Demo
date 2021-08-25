#include "Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Attack(AActor* Target, AActor* PlayerActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Basic Weapon Attacked"));

}