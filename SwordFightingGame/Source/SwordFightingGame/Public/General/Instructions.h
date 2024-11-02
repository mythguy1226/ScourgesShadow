// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Instructions.generated.h"

USTRUCT(BlueprintType)
struct FInstructions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString sText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* sIcon;
};
