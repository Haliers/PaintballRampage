// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Thumper.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AThumper : public AWeapon
{
	GENERATED_BODY()

protected:
	void Fire() override;
	void InitiateFireSequence() override;

private:
	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAProjectile> ProjectileClass;
};
