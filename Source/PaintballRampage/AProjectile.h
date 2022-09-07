// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AProjectile.generated.h"

UCLASS()
class PAINTBALLRAMPAGE_API AAProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDamageRadius() { return DamageRadius; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDamageRadius(float Param_DamageRadius) { DamageRadius = Param_DamageRadius; }
};
