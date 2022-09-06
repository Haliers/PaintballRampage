// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

#define ENEMY_TEAM_ID 1

#define KEY_SPAWN_LOCATION FName("SpawnLocation")
#define KEY_STATE FName("State")
#define KEY_TARGET_ENEMY FName("TargetEnemy")
#define KEY_NEW_LOCATION FName("MoveTo")
#define KEY_POI FName("POI")
#define KEY_CAN_ATTACK FName("CanAttack")

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Neutral UMETA(DisplayName = "Neutral"),
	EES_Alert UMETA(DisplayName = "Alert"),
	EES_Aggro UMETA(DisplayName = "Aggro"),
	EES_Dead UMETA(DisplayName = "Dead")
};

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdatedFunction(const TArray<AActor*>& SensedActors);

	void ProcessLastSightStimuli(AActor* ActorSensed, struct FAIStimulus Stimulus);
	void ProcessLastHearingStimuli(AActor* ActorSensed, FAIStimulus Stimulus);
	void ProcessLastDamageStimuli(AActor* ActorSensed, FAIStimulus Stimulus);

	// VARIABLES	
	EEnemyState State;

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EEnemyState GetState() const { return State; }
	
	void SetStateNeutral();
	void SetStateAlert(FVector AlertLocation);
	void SetStateAggro(AActor* TargetActor);
	void SetStateDead();

};
