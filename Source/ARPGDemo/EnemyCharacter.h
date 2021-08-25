#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class AWeapon;

UCLASS()
class ARPGDEMO_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float SpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	FString Name;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead;

	UPROPERTY(EditAnywhere)
	bool bIsBoss;

	UPROPERTY()
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath(AActor* Attacker);

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);
};
