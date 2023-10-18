// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float GlobalControlSensitivity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Drag") float GroundDrag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Drag") float GroundAngularDrag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Drag") float AirDrag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Drag") float AirAngularDrag;

	UPROPERTY(BlueprintReadOnly) float PodSpeed;
	UPROPERTY(BlueprintReadOnly) bool IsGrounded;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
