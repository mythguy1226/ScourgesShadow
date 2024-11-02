// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvasionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UEvasionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEvasionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Reference to owning character
	ACharacter* m_pOwningCharacter;

	// Dodge montage references
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pDodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pFallDodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeForwardLMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeForwardRMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeBackLMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pLockDodgeBackRMontage;

	// *** Handle Dodging ***
	void Dodge(bool a_bSprinting = false, bool a_bTargetLocked = false);
	bool m_bIsDodging = false;
	bool m_bCanAttackAfterDodge = false;
	bool m_bAttackingAfterDodge = false;
	FVector dodgeDir;
};
