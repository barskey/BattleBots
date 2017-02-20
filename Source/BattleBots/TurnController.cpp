// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBots.h"
#include "TurnController.h"


void ATurnController::DrawPoints()
{
    UNavigationPath *tpath;
    UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
    
    FVector PathStart;
    FVector PathEnd;
    
    AActor* Actor = GetOwner();
    UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *(Actor->GetName()))

    /*
    tpath = NavSys->FindPathToLocationSynchronously(GetWorld(), PathStart, PathEnd, Actor);

    if (tpath != NULL)
    {
        for (int pointiter = 0; pointiter < tpath->PathPoints.Num(); pointiter++)
        {
            DrawDebugSphere(GetWorld(), tpath->PathPoints[pointiter], 10.0f, 12, FColor(255, 0, 0));
        }
    }
     */
    return;
}
