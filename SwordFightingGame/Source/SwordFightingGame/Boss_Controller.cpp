// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SwordFightingGameCharacter.h"

ABoss_Controller::ABoss_Controller(FObjectInitializer const& a_pObjectInit)
{
	// Get the behavior tree from reference
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> treeFinder(TEXT("BehaviorTree'/Game/SwordFightingGame/AI/BT_BossBT.BT_BossBT'"));
	if (treeFinder.Succeeded())
	{
		m_pBehaviorTree = treeFinder.Object;
	}

	// Init the tree and blackboard components
	m_pBehaviorTreeComponent = a_pObjectInit.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree Component"));
	m_pBlackboard = a_pObjectInit.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));
}

void ABoss_Controller::BeginPlay()
{
	Super::BeginPlay();

	// Run the behavior tree
	RunBehaviorTree(m_pBehaviorTree);
	m_pBehaviorTreeComponent->StartTree(*m_pBehaviorTree);
}

void ABoss_Controller::OnPossess(APawn* a_pPawn)
{
	// Run default possess method
	Super::OnPossess(a_pPawn);

	// Init blackboard
	if (m_pBlackboard)
	{
		m_pBlackboard->InitializeBlackboard(*m_pBehaviorTree->BlackboardAsset);
	}
}

void ABoss_Controller::SetActive(bool a_bActive)
{
	// Set blackboard key based on activity
	if (a_bActive)
		m_pBlackboard->SetValueAsBool(FName("IsActive"), true);
	else
		m_pBlackboard->ClearValue(FName("IsActive"));
}

UBlackboardComponent* ABoss_Controller::GetBlackboard() const
{
	return m_pBlackboard;
}
