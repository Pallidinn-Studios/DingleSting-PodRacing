// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "HoverComponent.h"
#include "PodRacer.generated.h"

UCLASS()
class PODRACING_API APodRacer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APodRacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetDirectionForce(UHoverComponent* HoverComp, FRotator Direction, float Force);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* PodMesh;
	UPROPERTY(BlueprintReadOnly) float PodSpeed;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
