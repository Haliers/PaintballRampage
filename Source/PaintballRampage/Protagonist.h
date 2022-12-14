// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protagonist.generated.h"


UCLASS()
class PAINTBALLRAMPAGE_API AProtagonist : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProtagonist();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Val);
	void MoveRight(float Val);
	void LookVertical(float Val);
	void LookHorizontal(float Val);

	void BeginPlaySetup();

	void GetHitResultFromLineTraceComponent();

	void JumpAbility();
	void JumpCounter(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	void ReleaseTrigger();

	void InitiateReloadFromCharacter();

	UFUNCTION(BlueprintCallable)
	void FinaliseReloadFromCharacter();

	UFUNCTION(BlueprintCallable)
	class AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass);

	// void TryPickUpWeapon();
	void EquipWeaponByCurrentIndex();

	UFUNCTION(BlueprintCallable)
	void CockWeapon();

	void SetWeaponMovement();

	UFUNCTION(BlueprintNativeEvent)
	void DeathSetupBlueprint();

	int32 GetAmmoReserveForAmmoColor();
	void SetAmmoReserveForAmmoColor(float Reserve);

	UFUNCTION(BlueprintCallable)
	void ChangeWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DeathSetup();

	// Getters
	UFUNCTION(BlueprintCallable)
	AWeapon* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void Die();

	class UAnimInstance* GetAnimInstance();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentReserveAmmo();

	void ProgressMilestone();

private:
	// Class variables declared here
	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float StartingHP;

	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float HP;

	UPROPERTY(EditAnywhere, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float LookSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	float JumpAbilityDamage;

	// Class components declared here
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* GunSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LineTraceSceneComponent;	

	// Weapon related
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	TArray<AWeapon*> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	TArray<AWeapon*> WeaponsAvailable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWeaponIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AWeapon* WeaponTraced;

	// Milestone related
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 KillMilestoneStart;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 KillMilestone;

	// Runtime variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 PurpleAmmoReserve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 YellowAmmoReserve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 GreenAmmoReserve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 BlueAmmoReserve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 RedAmmoReserve;

	FString CommandText;
	FString KeyText;

	FHitResult HitUnderLineTraceComponent;

	int32 Milestone;
	bool MilestoneKillCounterSwitch;
	FString CounterText;
	bool bCanJump;
	float JumpCoolDown;
	float JumpCoolDownCounter;
	float JumpCoolDownPercentage;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddPurpleAmmoReserve(int32 Amount) { PurpleAmmoReserve += Amount; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddYellowAmmoReserve(int32 Amount) { YellowAmmoReserve += Amount; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddGreenAmmoReserve(int32 Amount) { GreenAmmoReserve += Amount; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddBlueAmmoReserve(int32 Amount) { BlueAmmoReserve += Amount;  }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddRedAmmoReserve(int32 Amount) { RedAmmoReserve += Amount; }

	// Getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPurpleAmmoReserve() { return PurpleAmmoReserve;  }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetYellowAmmoReserve() { return YellowAmmoReserve; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGreenAmmoReserve() { return GreenAmmoReserve; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetBlueAmmoReserve() { return BlueAmmoReserve; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRedAmmoReserve() { return RedAmmoReserve; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetCommandText() { return CommandText; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetKeyText() { return KeyText; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHPPercentage() { return HP / StartingHP; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMilestoneKills() { return KillMilestone; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMilestone() { return Milestone; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetCounterText() { return CounterText; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetJumpCoolDownPercentage() { return JumpCoolDownPercentage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetCanJump() { return bCanJump; }
};
