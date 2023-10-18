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

	//Visual
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* PodMesh;

	//Player settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float GlobalControlSensitivity;

	//Flight settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float GroundDrag = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float GroundAngularDrag = 4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float AirDrag = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float AirAngularDrag = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float RideHeight = 300;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float ForwardForce = 3000;

	//Misc
	UPROPERTY(BlueprintReadOnly) float PodSpeed;
	UPROPERTY(BlueprintReadOnly) bool IsGrounded;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
