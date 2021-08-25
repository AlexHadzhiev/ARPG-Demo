#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "Weapon.generated.h"

UCLASS()
class ARPGDEMO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShootsProjectiles;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Attack(AActor* Target, AActor* PlayerActor);
};
