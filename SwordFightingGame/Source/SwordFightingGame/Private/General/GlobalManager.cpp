// Fill out your copyright notice in the Description page of Project Settings.


#include "General/GlobalManager.h"


void UGlobalManager::Init()
{
	// Load all services into array
	LoadServices();
}

void UGlobalManager::LoadServices()
{
	// Get all global services and load them into services array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGlobalService::StaticClass(), m_aServices);
}
