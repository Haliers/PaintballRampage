// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Materials/MaterialInterface.h"
#include "NavigationSystem.h"
#include "BaseEnemy.h"
#include "Weapon.h"
#include "Protagonist.h"
#include "Kismet/GameplayStatics.h"
#include "Library.h"


// Sets default values
AEnemySpawner::AEnemySpawner() :
	StartingSpawnRate(5.f),
	MaxSpawnRate(0.5f),
	NumEnemy(0),
	bSpawnEnabled(true),
	ElapsedTimeFromLastSpawn(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	Protagonist = Cast<AProtagonist>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	SpawnRate = StartingSpawnRate;
	
	SpawnEnemy();
}

ABaseEnemy* AEnemySpawner::SpawnEnemy()
{
	int32 CurrentMilestone {1};

	if (Protagonist)
	{
		CurrentMilestone = Protagonist->GetMilestone();
	}
	
	int32 RandIntColor = FMath::RandRange((int32)1, (int32)CurrentMilestone);
	UMaterialInterface* RandomPrimaryMaterial{};
	UMaterialInterface* RandomDeathDecal{};
	EEnemyColor SpawnedEnemyColor{};

	switch (RandIntColor)
	{
	case 1:
		RandomPrimaryMaterial = PurpleMaterial;
		RandomDeathDecal = PurpleDecal;
		SpawnedEnemyColor = EEnemyColor::EEC_Purple;
		break;
	case 2:
		RandomPrimaryMaterial = YellowMaterial;
		RandomDeathDecal = YellowDecal;
		SpawnedEnemyColor = EEnemyColor::EEC_Yellow;
		break;
	case 3:
		RandomPrimaryMaterial = BlueMaterial;
		RandomDeathDecal = BlueDecal;
		SpawnedEnemyColor = EEnemyColor::EEC_Blue;
		break;
	case 4:
		RandomPrimaryMaterial = GreenMaterial;
		RandomDeathDecal = GreenDecal;
		SpawnedEnemyColor = EEnemyColor::EEC_Green;
		break;
	case 5:
		RandomPrimaryMaterial = RedMaterial;
		RandomDeathDecal = RedDecal;
		SpawnedEnemyColor = EEnemyColor::EEC_Red;
		break;
	}

	// FVector RandomLocationInNavMesh{ UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), FVector(0.f,0.f,0.f), 200.f) };

	FTransform EnemySpawnTransform{ GetAppropriateRandomTransform() };
	FActorSpawnParameters SpawnParams{};
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(GetRandomEnemyClass(), EnemySpawnTransform, SpawnParams);

	if (SpawnedEnemy)
	{
		// ONSCREEN_DEBUG("Enemy correctly spawned", 2)
		SpawnedEnemy->GetMesh()->SetMaterial(
			SpawnedEnemy->GetMesh()->GetMaterialIndex(FName("Primary")),
			RandomPrimaryMaterial);
		SpawnedEnemy->SetEnemyColor(SpawnedEnemyColor);
		SpawnedEnemy->SetEnemyColorData(RandomPrimaryMaterial, RandomDeathDecal);

		// Set delegate relationship with spawned enemy
		SpawnedEnemy->OnEnemyDeath.AddDynamic(this, &AEnemySpawner::EnemyDead);
		//
		
		NumEnemy++;
		ElapsedTimeFromLastSpawn = 0.f;

		return SpawnedEnemy;
	}

	ONSCREEN_DEBUG("Enemy null", 2)
	return nullptr;
}

void AEnemySpawner::EnemyDead()
{
	NumEnemy--;

	if (Protagonist)
	{
		Protagonist->ProgressMilestone();
	}
	
	if (SpawnRate > MaxSpawnRate)
	{
		SpawnRate -= 0.125f;
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawnEnabled)
	{
		ElapsedTimeFromLastSpawn += DeltaTime;

		if (ElapsedTimeFromLastSpawn > SpawnRate)
		{
			SpawnEnemy();
		}
	}
}

TSubclassOf<ABaseEnemy> AEnemySpawner::GetRandomEnemyClass()
{
	int32 RandInt = FMath::RandRange((int32)0, ((int32)EnemyClasses.Num() - 1));

	return EnemyClasses[RandInt];
}

FTransform AEnemySpawner::GetAppropriateRandomTransform_Implementation()
{
	return FTransform();
}
