// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EAmmoColor : uint8
{
	EAC_Purple UMETA(DisplayName = "Purple"),
	EAC_Yellow UMETA(DisplayName = "Yellow"),
	EAC_Green UMETA(DisplayName = "Green"),
	EAC_Blue UMETA(DisplayName = "Blue"),
	EAC_Red UMETA(DisplayName = "Red")
};

UENUM(BlueprintType)
enum class EGunState : uint8
{
	EGS_Equipped UMETA(DisplayName = "Equipped"),
	EGS_Highlighted UMETA(DisplayName = "Highlighted"),
	EGS_PickUp UMETA(DisplayName = "PickUp")
};

UCLASS()
class PAINTBALLRAMPAGE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire();

	virtual void PlayFireAnimation();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnDecalAtHitLocation(FHitResult Param_Hit);

	void ApplyDamageIfDamagable(FHitResult Param_Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnTracer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnImpactFX(FVector ImpactLocation, FVector ImpactNormal);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Firing related
	virtual void InitiateFireSequence();
	FORCEINLINE void SetTriggerPulled(bool Param_bTriggerPulled) { bTriggerPulled = Param_bTriggerPulled; }

	// Control fire
	FORCEINLINE bool SetReloading(bool Param_bReloading) { return bReloading = Param_bReloading; }
	
	UFUNCTION(BlueprintCallable)
	bool CalculateCanFire();

	void InitiateReload();

	int32 FillMag(int32 Param_AmmoInMag, int32 Empty);

	int32 RemoveAmmoInMag();


protected:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleSceneComponent;

	// Color data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Mode", meta = (AllowPrivateAccess = "true"))
	EAmmoColor AmmoColor; // Enum that indicates weapon related color

	UPROPERTY(EditAnywhere, Category = "Decal Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PaintMaterial;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicPaintMaterial;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor LinearColor;

	// Animation Assets
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwapAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwapBackwardsAnimMontage;

	// Variables
	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	int32 MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FVector DecalSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float Damage;

	// Runtime variables
	bool bTriggerPulled;
	int32 AmmoInMag;
	bool bIdle;
	bool bCanFire;
	bool bReloading;
	bool bCocked;

	FHitResult HitResultFromCharacter;

public:

	UFUNCTION(BlueprintCallable)
	virtual bool SetCocked(bool Param_bCocked);

	void PlayUnequipAnimation();

	void PlayEquipAnimation();

	FORCEINLINE void SetIfIdle(bool Param_bIdle) { bIdle = Param_bIdle; }

	FORCEINLINE void SetAmmoInMag(float NewAmmo) { AmmoInMag = NewAmmo; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIdle() { return bIdle; }

	FORCEINLINE bool GetIsReloading() { return bReloading; }

	FORCEINLINE bool GetCanFire() { return bCanFire; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAmmo() { return AmmoInMag; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DecreaseAmmo() { AmmoInMag -= 1; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMagSize() { return MagSize; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetWeaponName() { return WeaponName; }

	FORCEINLINE EAmmoColor GetAmmoColor() { return AmmoColor; }

	FORCEINLINE void SetHitResultFromCharacter(FHitResult Param_Hit) { HitResultFromCharacter = Param_Hit; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FHitResult GetHitResultFromCharacter() { return HitResultFromCharacter; }

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() { return SkeletalMeshComponent; }

private:
	// Functions
	void BeginPlaySetup();
	//
};


