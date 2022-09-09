// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "OfficerDunkin.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AOfficerDunkin : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ThrowDonut();

private:
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ThrowSceneComponent;

	UPROPERTY(EditAnywhere, Category = "Spawned", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAProjectile> DonutClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FVector DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicPaintMaterial;
};
