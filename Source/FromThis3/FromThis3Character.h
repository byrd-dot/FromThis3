// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TargetSystemTargetableInterface.h"
#include "FromThis3Character.generated.h"

class UCharacterStateComponent;
class UAnimMontage;
class UTargetSystemComponent;
class UHealthComponent;
class AWeapon;

UCLASS(config=Game)
class AFromThis3Character : public ACharacter, public ITargetSystemTargetableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AFromThis3Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FName WeaponSocket = "WeaponSocket";

	//handles player state
	//must successfully enter correct state before event state is entered
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCharacterStateComponent * CharacterStateComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UTargetSystemComponent * TargetSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UHealthComponent * HealthComp;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TArray<TSubclassOf<AWeapon>> WeaponInventory;



protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// initiate melee attack on input
	void Attack();

	// initiate avoiding move
	void Roll();

	// attempt to target or untarget enemy
	void TargetToggle();

	//TODO: Encapsulate in weapon component
	UPROPERTY(EditDefaultsOnly, Category = Default)
	TArray<UAnimMontage*> AttackMontages;

	//TODO: Encapsulate to roll event
	UPROPERTY(EditDefaultsOnly, Category = Default)
	UAnimMontage * ForwardRoll;

	UPROPERTY(EditDefaultsOnly, Category = Default)
	UAnimMontage * LeftRoll;

	UPROPERTY(EditDefaultsOnly, Category = Default)
	UAnimMontage * RightRoll;

	UPROPERTY(EditDefaultsOnly, Category = Default)
	UAnimMontage * BackRoll;

	UPROPERTY(EditDefaultsOnly, Category = Default)
	UAnimMontage * BackStep;

	UPROPERTY()
	int32 AttackNum = 0;

protected:
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	bool IsTargetable() const;
	virtual bool IsTargetable_Implementation() const { return true; }

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


public:
	//Utility functions to get info about character. TODO: Move somewhere else?

	//find input relative to camera
	float GetWeightedForwardRotation();

	float GetWeightedRightRotation();
};