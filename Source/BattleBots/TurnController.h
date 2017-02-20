// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TurnController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BATTLEBOTS_API ATurnController : public AAIController
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category="Nav")
    void DrawPoints();
	
	
};
