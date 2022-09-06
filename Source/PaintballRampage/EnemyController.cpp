// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense_Damage.h"
#include "GenericTeamAgentInterface.h"
#include "NavigationSystem.h"
#include "Protagonist.h"
#include "Library.h"



AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");

	SetGenericTeamId(FGenericTeamId((uint8)ENEMY_TEAM_ID));
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OnPossess()")));

	if (InPawn == nullptr) return;
	
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(InPawn);

	if (BaseEnemy)
	{
		if (BaseEnemy->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*(BaseEnemy->GetBehaviorTree()->BlackboardAsset));
		}

		BlackboardComponent->SetValueAsVector(KEY_SPAWN_LOCATION, BaseEnemy->GetActorLocation());
		BlackboardComponent->SetValueAsBool(KEY_CAN_ATTACK, true);
		RunBehaviorTree(BaseEnemy->GetBehaviorTree());
	}

}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("BeginPlay()")));

}

ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);

	if (OtherPawn)
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == ENEMY_TEAM_ID) return ETeamAttitude::Neutral;

			else return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}

void AEnemyController::OnPerceptionUpdatedFunction(const TArray<AActor*>& UpdatedActors)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PerceptionUpdateFunction()")));

	const FAISenseID ID_Sight{ UAISense::GetSenseID<UAISense_Sight>() };
	const FAISenseID ID_Hearing{ UAISense::GetSenseID<UAISense_Hearing>() };
	const FAISenseID ID_Damage{ UAISense::GetSenseID<UAISense_Damage>() };

	for (AActor* Actor : UpdatedActors)
	{
		if (GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Neutral || GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Friendly) continue;

		FActorPerceptionBlueprintInfo ActorPerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(Actor, ActorPerceptionInfo);

		for (FAIStimulus Stimulus : ActorPerceptionInfo.LastSensedStimuli)
		{
			if (Stimulus.Type == ID_Sight)			ProcessLastSightStimuli(Actor, Stimulus);
			else if (Stimulus.Type == ID_Hearing)	ProcessLastHearingStimuli(Actor, Stimulus);
			else if (Stimulus.Type == ID_Damage)	ProcessLastDamageStimuli(Actor, Stimulus);
			// else ONSCREEN_DEBUG("Invalid stimuli ID", 5)
		}
	}
}

void AEnemyController::ProcessLastSightStimuli(AActor* ActorSensed, FAIStimulus Stimulus)
{
	if (Cast<AProtagonist>(ActorSensed))
	{
		SetStateAggro(ActorSensed);
	}
}

void AEnemyController::ProcessLastHearingStimuli(AActor* ActorSensed, FAIStimulus Stimulus)
{
	// ONSCREEN_DEBUG("Process last hearing", 1)
	if (!(BlackboardComponent->GetValueAsEnum(KEY_STATE) == (uint8)2))
	{
		// ONSCREEN_DEBUG("Heared stimulus processed", 1)

		if (Stimulus.SensingSucceeded == FAIStimulus::SensingSucceeded) {
			// ONSCREEN_DEBUG("Sensing succeeded", 1)

			if (Stimulus.Tag == TAG_GUNSHOT)
			{
				// ONSCREEN_DEBUG("SensingSucceeded", 1)
				SetStateAlert(UNavigationSystemV1::ProjectPointToNavigation(GetWorld(), Stimulus.StimulusLocation));
			}
		}
	}
	else
	{
		// ONSCREEN_DEBUG("Heared stimulus ignored", 1)
	}
}

void AEnemyController::ProcessLastDamageStimuli(AActor* ActorSensed, FAIStimulus Stimulus)
{

}

void AEnemyController::SetStateNeutral()
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetStateNeutral()")));
	State = EEnemyState::EES_Neutral;
	BlackboardComponent->SetValueAsEnum(KEY_STATE, (uint8)State);
}

void AEnemyController::SetStateAlert(FVector AlertLocation)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetStateAlert()")));
	State = EEnemyState::EES_Alert;
	BlackboardComponent->SetValueAsEnum(KEY_STATE, (uint8)State);
	BlackboardComponent->SetValueAsVector(KEY_POI, AlertLocation);
}

void AEnemyController::SetStateAggro(AActor* TargetActor)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetStateAggro()")));
	State = EEnemyState::EES_Aggro;
	BlackboardComponent->SetValueAsEnum(KEY_STATE, (uint8)State);
	
	if(TargetActor) BlackboardComponent->SetValueAsObject(KEY_TARGET_ENEMY, TargetActor);
}

void AEnemyController::SetStateDead()
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetStateDead()")));
	State = EEnemyState::EES_Dead;
	BlackboardComponent->SetValueAsEnum(KEY_STATE, (uint8)State);
}

