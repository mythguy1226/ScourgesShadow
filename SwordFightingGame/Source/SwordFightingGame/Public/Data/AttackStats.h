// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackStats.generated.h"

USTRUCT(BlueprintType)
struct FAttackStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool sKnockback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sKnockbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sStunDamage;
};
