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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnImpactFX(FVector ImpactLocation, FVector ImpactNormal);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnDecalAtHitLocation(FVector ImpactLocation, FVector ImpactNormal, UPrimitiveComponent* AttachComponent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetColorData(FLinearColor P_LinearColor, UMaterialInstanceDynamic* P_DynamicMaterial);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicPaintMaterial;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor LinearColor;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector DecalSize;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDamageRadius() { return DamageRadius; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDamage() { return Damage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDamageRadius(float Param_DamageRadius) { DamageRadius = Param_DamageRadius; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDamage(float Param_Damage) { Damage = Param_Damage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDecalSize(FVector P_DecalSize) { DecalSize = P_DecalSize; }

	FORCEINLINE void SetDynamicMaterial(UMaterialInstanceDynamic* P_DynamicPaintMaterial) { DynamicPaintMaterial = P_DynamicPaintMaterial; }
};
