// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy_Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/SwordFightingGameCharacter.h"
#include "Data/EnemyKeys.h"
#include "AI/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "General/GlobalManager.h"
#include "General/AudioManager.h"

AEnemy_Controller::AEnemy_Controller(FObjectInitializer const& a_pObjectInit)
{
	// Init the tree and blackboard components
	m_pBehaviorTreeComponent = a_pObjectInit.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree Component"));
	m_pBlackboard = a_pObjectInit.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));

	// Init Perception
	SetupPerception();
}

void AEnemy_Controller::BeginPlay()
{
	Super::BeginPlay();

	// Run the behavior tree
	RunBehaviorTree(m_pBehaviorTree);
	m_pBehaviorTreeComponent->StartTree(*m_pBehaviorTree);
}

void AEnemy_Controller::OnPossess(APawn* a_pPawn)
{
	// Run default possess method
	Super::OnPossess(a_pPawn);

	// Init blackboard
	if (m_pBlackboard)
	{
		m_pBlackboard->InitializeBlackboard(*m_pBehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemy_Controller::GetBlackboard() const
{
	return m_pBlackboard;
}

void AEnemy_Controller::HandleTargetDetection(AActor* a_pActor, FAIStimulus a_sStimulus)
{
	// Cast the actor to the player
	ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(a_pActor);

	// If the cast is valid, then set the blackboard key
	if (pPlayer)
	{
		GetBlackboard()->SetValueAsBool(EnemyKeys::canSeePlayer, a_sStimulus.WasSuccessfullySensed());
		AEnemy* pEnemy = Cast<AEnemy>(GetPawn());

		// Check for initial combat status
		if (!pEnemy->m_bInCombat)
		{
			pEnemy->m_bInCombat = true;
		}
	}
}

void AEnemy_Controller::SetupPerception()
{
	// Create and init sight config object
	m_pSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component"));

	// Configure sight options
	m_pSightConfig->SightRadius = 1200.0f;
	m_pSightConfig->LoseSightRadius = m_pSightConfig->SightRadius + 50.0f;
	m_pSightConfig->PeripheralVisionAngleDegrees = 360.0f;
	m_pSightConfig->SetMaxAge(5.0);
	m_pSightConfig->AutoSuccessRangeFromLastSeenLocation = 500.0f;
	m_pSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_pSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	m_pSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Attach the sight configuration to the perception component
	GetPerceptionComponent()->SetDominantSense(*m_pSightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_Controller::HandleTargetDetection);
	GetPerceptionComponent()->ConfigureSense(*m_pSightConfig);
}
