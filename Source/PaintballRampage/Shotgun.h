// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	
protected:
	// Sets default values for this actor's properties
	AShotgun();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void Fire()	override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InitiateFireSequence() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPellets() { return Pellets; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Pellets;

};
