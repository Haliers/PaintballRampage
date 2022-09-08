// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Glock.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AGlock : public AWeapon
{
	GENERATED_BODY()
	

public:
	AGlock();

protected:
	virtual void BeginPlay() override;

};
