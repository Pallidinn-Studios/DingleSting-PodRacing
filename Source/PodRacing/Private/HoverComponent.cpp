// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "HoverComponent.h"

// Sets default values for this component's properties
UHoverComponent::UHoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Create debug directional arrow
	DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + GetUpVector() * 100, 5, FColor::Blue);
	UArrowComponent* DebugArrow = CreateDefaultSubobject<UArrowComponent>("DebugArrow");
	DebugArrow->SetupAttachment(this);
	DebugArrow->SetRelativeRotation(GetUpVector().Rotation() * -1);
	
}


// Called when the game starts
void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();

	MainBody = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}


// Called every frame
void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PodGroundCheck();
}

void UHoverComponent::PodGroundCheck() {
	//Adds force on impact if hover component requires hit
	if (RequireHit) {
		//Calculate start and end pos
		Start = GetComponentLocation();
		//End = Start + (-GetUpVector() * TraceLenght);
		End = Start + (FVector(0,0,-TraceLenght));

		//Do line trace
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility)) {
			//Debug
			if (DebugEnabled) {
				DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
				GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red, "Hit");
			}
			
			//Calculate and add force
			//FVector const CalculatedForce = FMath::Lerp(HoverForce, 0, (HitResult.Location - GetComponentLocation()).Length() / TraceLenght) * HitResult.ImpactNormal;
			//MainBody->AddImpulseAtLocation(CalculatedForce, GetComponentLocation());
			
			GetOwner()->SetActorLocation(HitResult.Location + (GetUpVector() * 300), false, nullptr, ETeleportType::TeleportPhysics);
			//FRotator test = FRotationMatrix::MakeFromXY(HitResult.ImpactNormal, GetOwner()->GetActorForwardVector());
			//GetOwner()->SetActorRotation(FRotationMatrix::MakeFromXY(HitResult.ImpactNormal, GetOwner()->GetActorForwardVector()));
			MainBody->SetLinearDamping(3);
			MainBody->SetAngularDamping(5);
			MainBody->SetEnableGravity(false);
		}
		else {
			MainBody->SetLinearDamping(0);
			MainBody->SetAngularDamping(0);
			MainBody->SetEnableGravity(true);
		}
	}
	else {
		//MainBody->AddImpulseAtLocation(GetUpVector() * HoverForce, GetComponentLocation());
		MainBody->AddImpulse(GetUpVector() * HoverForce);

	}
}


