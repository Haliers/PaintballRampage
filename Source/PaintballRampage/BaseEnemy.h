// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

UENUM(BlueprintType)
enum class EEnemyColor : uint8
{
	EEC_Purple UMETA(DisplayName = "Purple"),
	EEC_Yellow UMETA(DisplayName = "Yellow"),
	EEC_Green UMETA(DisplayName = "Green"),
	EEC_Blue UMETA(DisplayName = "Blue"),
	EEC_Red UMETA(DisplayName = "Red")
};

UCLASS()
class PAINTBALLRAMPAGE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable)
	void Drop();

private:
	void SetAnimationCount();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeath;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExecuteDeathProcess();

	UFUNCTION(BlueprintCallable)
	void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToProtagonist();
	
	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeed();

	UFUNCTION(BlueprintCallable)
	bool GetAIActive();

	class UAnimInstance* GetAnimInstance();

	void SetEnemyColorData(UMaterialInterface* PrimaryMaterial, UMaterialInterface* Param_DeathDecal);


protected:
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NeutralSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AlertSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AggroSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PrimaryColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DeathDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Color", meta = (AllowPrivateAccess = "true"))
	FLinearColor EnemyLinearColor;

	UPROPERTY(EditAnywhere, Category = "Drops", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAmmoDrop> AmmoDropClass;

	UPROPERTY(VisibleAnywhere, Category = "Drops", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> WeaponDropClass;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation1;
	
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation2;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation3;
	
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation4;
	
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation5;
	
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation6;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FlinchAnimation;

	int8 AttackAnimations;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EEnemyColor EnemyColor;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AProtagonist* ProtagonistReference;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ProtagonistInRange;

	// Health related

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float StartingHP;

	UPROPERTY(VisibleAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float HP;

	bool bCanAttack;
	bool bIsDead;

public:
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float Param_Damage);

	void InitiateDeath();

	// Getters
	FORCEINLINE float GetHealth() { return HP; }
	FORCEINLINE bool GetIsDead() { return bIsDead; }
	FORCEINLINE bool GetCanAttack() { return bCanAttack; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMaterialInterface* GetPrimaryColor() { return PrimaryColor; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() { return HP / StartingHP; }

	// Setters
	FORCEINLINE void SetEnemyColor(EEnemyColor Color) { EnemyColor = Color; }
	FORCEINLINE void SetEnemyColor(FLinearColor Param_Color) { EnemyLinearColor = Param_Color; }
};
