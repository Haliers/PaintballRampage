// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Animation/AnimInstance.h"
#include "EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "Protagonist.h"
#include "Animation/AnimMontage.h"
#include "Weapon.h"
#include "AmmoDrop.h"
#include "Components/SphereComponent.h"
#include "Library.h"


// Sets default values
ABaseEnemy::ABaseEnemy() :
	AttackAnimations(0),
	Damage(30.f),
	ProtagonistInRange(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetAnimationCount();

	HP = StartingHP;

	ProtagonistReference = Cast<AProtagonist>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::ExecuteDeathProcess_Implementation()
{
}

void ABaseEnemy::PlayAttackAnimation()
{
	UAnimMontage* AttackAnimation{ nullptr };
	uint8 RandomAnimationNumber = FMath::RandRange(1, AttackAnimations);

	switch (RandomAnimationNumber)
	{
	case 1:
		AttackAnimation = AttackAnimation1;
		break;
	case 2:
		AttackAnimation = AttackAnimation2;
		break;
	case 3:
		AttackAnimation = AttackAnimation3;
		break;
	case 4:
		AttackAnimation = AttackAnimation4;
		break;
	case 5:
		AttackAnimation = AttackAnimation5;
		break;
	case 6:
		AttackAnimation = AttackAnimation6;
		break;
	}

	UAnimInstance* AnimInstance = GetAnimInstance();

	if (AnimInstance && AttackAnimation)
	{
		AnimInstance->Montage_Play(AttackAnimation);
		// ONSCREEN_DEBUG("Attack animation played", 5);
	}
}

void ABaseEnemy::ApplyDamageToProtagonist()
{
	if (ProtagonistReference && ProtagonistInRange)
	{
		ProtagonistReference->ApplyDamage(Damage);
	}
}

void ABaseEnemy::Drop()
{
	FTransform SpawnTransform = GetActorTransform();

	
		FActorSpawnParameters SpawnParams{};
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = SpawnTransform.GetLocation();
		SpawnLocation.Z = 100.f;
		SpawnTransform.SetLocation(SpawnLocation);

		AAmmoDrop* AmmoDrop = GetWorld()->SpawnActor<AAmmoDrop>(AmmoDropClass, SpawnTransform, SpawnParams);

		if (AmmoDrop)
		{
			AmmoDrop->GetMesh()->SetMaterial(0, PrimaryColor);

			switch (EnemyColor)
			{
			case EEnemyColor::EEC_Purple:
				AmmoDrop->SetDropColor(EDropColor::EDC_Purple);
				break;
			case EEnemyColor::EEC_Yellow:
				AmmoDrop->SetDropColor(EDropColor::EDC_Yellow);
				break;
			case EEnemyColor::EEC_Green:
				AmmoDrop->SetDropColor(EDropColor::EDC_Green);
				break;
			case EEnemyColor::EEC_Blue:
				AmmoDrop->SetDropColor(EDropColor::EDC_Blue);
				break;
			case EEnemyColor::EEC_Red:
				AmmoDrop->SetDropColor(EDropColor::EDC_Red);
				break;
			default:
				AmmoDrop->SetDropColor(EDropColor::EDC_Purple);
				break;
			}
			AmmoDrop->SetupAmmoData();
		}
	
}

void ABaseEnemy::SetAnimationCount()
{
	if (AttackAnimation1) AttackAnimations++;
	if (AttackAnimation2) AttackAnimations++;
	if (AttackAnimation3) AttackAnimations++;
	if (AttackAnimation4) AttackAnimations++;
	if (AttackAnimation5) AttackAnimations++;
	if (AttackAnimation6) AttackAnimations++;
}

void ABaseEnemy::UpdateMovementSpeed()
{
	switch (Cast<AEnemyController>(GetController())->GetState())
	{
	case EEnemyState::EES_Neutral:
		GetCharacterMovement()->MaxWalkSpeed = NeutralSpeed;
		break;
	case EEnemyState::EES_Alert:
		GetCharacterMovement()->MaxWalkSpeed = AlertSpeed;
		break;
	case EEnemyState::EES_Aggro:
		GetCharacterMovement()->MaxWalkSpeed = AggroSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = 0;
		break;
	}
}

bool ABaseEnemy::GetAIActive()
{
	bool bAIActive = true;

	if (bIsDead) bAIActive = false;

	return bAIActive;
}

UAnimInstance* ABaseEnemy::GetAnimInstance()
{
	if (GetMesh())
	{
		return GetMesh()->GetAnimInstance();
	}
	else
	{
		return nullptr;
	}
}

void ABaseEnemy::SetEnemyColorData(UMaterialInterface* PrimaryMaterial, UMaterialInterface* Param_DeathDecal)
{
	PrimaryColor = PrimaryMaterial;
	DeathDecal = Param_DeathDecal;

	switch (EnemyColor)
	{
	case EEnemyColor::EEC_Purple:
		EnemyLinearColor = PURPLE;
		break;
	case EEnemyColor::EEC_Yellow:
		EnemyLinearColor = YELLOW;
		break;
	case EEnemyColor::EEC_Green:
		EnemyLinearColor = GREEN;
		break;
	case EEnemyColor::EEC_Blue:
		EnemyLinearColor = BLUE;
		break;
	case EEnemyColor::EEC_Red:
		EnemyLinearColor = RED;
		break;
	default:
		break;
	}
}

float ABaseEnemy::ApplyDamage(float Param_Damage)
{
	HP -= Param_Damage;

	if (HP <= 0)
	{
		HP = 0;
		InitiateDeath();
		return 0;
	}

	UAnimInstance* AnimInstance{ GetMesh()->GetAnimInstance() };

	if (AnimInstance && FlinchAnimation)
	{
		AnimInstance->Montage_Play(FlinchAnimation);
		bCanAttack = false;
	}

	return HP;
}

void ABaseEnemy::InitiateDeath()
{
	UAnimInstance* AnimInstance{ GetMesh()->GetAnimInstance() };

	if (AnimInstance && DeathAnimation)
	{
		AnimInstance->Montage_Play(DeathAnimation);
	}

	OnEnemyDeath.Broadcast();
}



