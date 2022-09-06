// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "M4.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AM4 : public AWeapon
{
	GENERATED_BODY()
	
public: 
	AM4();

	void InitiateReloadAnim();

	void InitiateFireSequence() override;

	bool SetCocked(bool Param_bCocked);

	UFUNCTION(BlueprintCallable)
	int32 FinaliseReload(int32 AmmoReserve);

protected:
	virtual void BeginPlay() override;

private:
	// Functions

};
