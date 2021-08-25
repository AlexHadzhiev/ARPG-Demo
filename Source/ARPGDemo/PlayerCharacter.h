#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"
#include "Ability.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class ARPGDEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	float Health;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	float SpeedModifier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	float Score;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	float MaxDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Name")
	FString CharacterName;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly)
	bool bIsUsingAbility;

	UPROPERTY()
	bool bIsWeaponInRange;

	UPROPERTY()
	bool bIsAbilityInRange;

	UPROPERTY()
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	UAbility* Ability;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbility> AbilityClass;

	UPROPERTY(EditAnywhere)
	USphereComponent* WeaponRangeSphere;

	UPROPERTY(EditAnywhere)
	USphereComponent* AbilityRangeSphere;

	UPROPERTY()
	AActor* EnemyTarget;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const
	{
		return TopDownCameraComponent;
	}

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

	FORCEINLINE class UDecalComponent* GetCursorToWorld()
	{
		return CursorToWorld;
	}

	FORCEINLINE class UDecalComponent* GetCursorToEnemy()
	{
		return CursorToEnemy;
	}

	FORCEINLINE class UDecalComponent* GetCursorAbility()
	{
		return CursorAbility;
	}

	void ActivateAbilty();
	
	UFUNCTION()
	void Attack(AActor* Target);

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);

	UFUNCTION()
	void WeaponInRangeOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AbilityInRangeOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDeath();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorAbility;
};
