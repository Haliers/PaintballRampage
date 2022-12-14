// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"


UCLASS()
class PAINTBALLRAMPAGE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ABaseEnemy* SpawnEnemy();

	UFUNCTION(BlueprintNativeEvent)
	FTransform GetAppropriateRandomTransform();

	UFUNCTION()
	void EnemyDead();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<ABaseEnemy> GetRandomEnemyClass();
	
	// Components
	 
	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* PurpleMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* YellowMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* GreenMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BlueMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* RedMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PurpleDecal;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* YellowDecal;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* GreenDecal;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BlueDecal;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* RedDecal;

	UPROPERTY(EditAnywhere, Category = "Spawned Enemies", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseEnemy>> EnemyClasses;

	UPROPERTY()
	class AProtagonist* Protagonist;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawnedEnemies; // Time between spawning enemies in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	float StartingSpawnRate; // Time between spawning enemies in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	float MaxSpawnRate; // Time at maximum spawn rate

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	float SpawnRate;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 NumEnemy;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	bool bSpawnEnabled;

	float ElapsedTimeFromLastSpawn;
};
