// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Path.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEBOTS_API UPath : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPath();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
    UFUNCTION(BlueprintCallable, Category="Nav")
    void DrawNavPath(FVector PathEnd);
    
    // Returns the length of the move to path
    UFUNCTION(BlueprintCallable, Category="Nav")
    float GetPathLength();
    
    class USplineComponent* MoveToSpline = nullptr;
    
    UPROPERTY(EditAnywhere, Category="Nav")
    UStaticMesh* MoveToLineMesh;
    
    UPROPERTY(EditAnywhere, Category="Nav")
    UStaticMesh* PathPointDisc;
    
    UPROPERTY(EditAnywhere, Category="Nav")
    UMaterialInterface* LineMeshMaterial;
    
    UPROPERTY(EditAnywhere, Category="Nav")
    UMaterialInterface* PathPointMaterial;
    
    UPROPERTY(EditAnywhere, Category="Nav")
    float MoveToLineHeight = 20;
    
private:
    TArray<class USplineMeshComponent*> SplineMeshPtrArray;
    
    TArray<class UStaticMeshComponent*> StaticMeshPtrArray;
};

