#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"

#include "ArrowProjectile.generated.h"

UCLASS()
class ARPGDEMO_API AArrowProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AArrowProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnBeginOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
