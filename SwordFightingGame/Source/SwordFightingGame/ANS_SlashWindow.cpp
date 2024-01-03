// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SlashWindow.h"
#include "CombatComponent.h"

void UANS_SlashWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// Get the combat component from the owning actor
	AActor* owningActor = MeshComp->GetOwner();
	UCombatComponent* combatComp = Cast<UCombatComponent>(owningActor->GetComponentByClass(UCombatComponent::StaticClass()));

	// Hit location from socket
	FVector pLocation = MeshComp->GetSocketLocation(FName(m_sSocketName));

	// Generate a hit sphere at the slash location (if not continuous)
	if(!m_bContinuousTracing)
		combatComp->GenerateHitSphere(pLocation, m_fRadius, m_sAttackStats, m_bDebugMode);

	// Set the slash begin to the socket location
	vSlashBegin = pLocation;

	// Set time between checks based on total duration
	fTimeBetweenChecks = TotalDuration / static_cast<float>(m_iNumTraceChecks);
	fCheckTimer = fTimeBetweenChecks;
}

void UANS_SlashWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	// Update timer
	fCheckTimer -= FrameDeltaTime;
	if (fCheckTimer <= 0)
	{
		// Get the combat component from the owning actor
		AActor* owningActor = MeshComp->GetOwner();
		UCombatComponent* combatComp = Cast<UCombatComponent>(owningActor->GetComponentByClass(UCombatComponent::StaticClass()));

		// Hit location from socket
		FVector pLocation = MeshComp->GetSocketLocation(FName(m_sSocketName));
		vSlashEnd = pLocation;

		// Generate the hit capsule 
		if(m_bContinuousTracing)
			combatComp->GenerateHitCapsule(vSlashBegin, vSlashEnd, m_fRadius, m_sAttackStats, m_bDebugMode);

		// Update the begin location
		vSlashBegin = pLocation;

		// Reset the timer
		fCheckTimer = fTimeBetweenChecks;
	}
}

void UANS_SlashWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get the combat component from the owning actor
	AActor* owningActor = MeshComp->GetOwner();
	UCombatComponent* combatComp = Cast<UCombatComponent>(owningActor->GetComponentByClass(UCombatComponent::StaticClass()));

	// Hit location from socket
	FVector pLocation = MeshComp->GetSocketLocation(FName(m_sSocketName));
	vSlashEnd = pLocation;

	// Generate the hit capsule 
	if (m_bContinuousTracing)
		combatComp->GenerateHitCapsule(vSlashBegin, vSlashEnd, m_fRadius, m_sAttackStats, m_bDebugMode);

	// Clear set of damaged actors
	combatComp->m_sDamagedActors.Empty();
}
