// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/ANS_JumpCurve.h"

void UANS_JumpCurve::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	m_fDuration = TotalDuration;
}

void UANS_JumpCurve::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	// Get location and rotational values to set for owner
	FVector vMeshLoc = MeshComp->GetOwner()->GetActorLocation();
	FVector vCurveVec = m_pCurve->GetVectorValue(FrameDeltaTime/m_fDuration);
	FRotator rMeshRot = MeshComp->GetOwner()->GetActorRotation();

	// Calculate location offset based on vector curve values
	FVector vUpward = MeshComp->GetOwner()->GetActorUpVector() * vCurveVec.Z * m_fHeight;
	FVector vForward = MeshComp->GetOwner()->GetActorForwardVector() * vCurveVec.X * m_fForward;
	FVector vRightward = MeshComp->GetOwner()->GetActorRightVector() * vCurveVec.Y * m_fRightward;

	// Combine all vectors
	FVector vCurveLoc = vForward + vRightward + vUpward;

	// Teleport owner with calculated values
	MeshComp->GetOwner()->TeleportTo(vMeshLoc + vCurveLoc, rMeshRot);
}
