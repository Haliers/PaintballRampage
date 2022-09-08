// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Intervention.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AIntervention : public AWeapon
{
	GENERATED_BODY()

public:
	AIntervention();

	void InitiateFireSequence() override;

protected:
	virtual void BeginPlay() override;

	void Fire() override;
};
