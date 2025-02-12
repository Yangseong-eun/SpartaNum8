#include "Num6Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Num6PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TextBlock.h"
#include "Num6GameState.h"
// Sets default values
ANum6Character::ANum6Character()
	: CurrentJumpCount(0), MaxJumpCount(2) // 점프 횟수 초기화
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMuliplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMuliplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// 점프 높이를 설정 (값이 클수록 더 높이 점프)
	GetCharacterMovement()->JumpZVelocity = 800.0f; // 기본값: 600.0f


	MaxHealth = 100;
	Health = MaxHealth;


}

// Called to bind functionality to input
void ANum6Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ANum6PlayerController* PlayerController = Cast<ANum6PlayerController>(GetController()))
		{
			// Move Action
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ANum6Character::Move
				);
			}

			// Jump Action
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ANum6Character::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ANum6Character::StopJump
				);
			}

			// Sprint Action
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ANum6Character::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ANum6Character::StopSprint
				);
			}

			// Look Action
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ANum6Character::Look
				);
			}
		}
	}
}

float ANum6Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("TakeDamage Called! DamageAmount: %f"), DamageAmount);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f) {
		OnDeath();
	}

	return ActualDamage;
}


// 점프 시작
void ANum6Character::StartJump(const FInputActionValue& Value)
{
	// 점프 입력 확인 및 점프 처리
	if (Value.Get<bool>() && CurrentJumpCount < MaxJumpCount)
	{
		ACharacter::Jump(); // 부모 클래스의 Jump 호출
		CurrentJumpCount++;
	}
}
// 점프 종료
void ANum6Character::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		ACharacter::StopJumping(); // 부모 클래스의 StopJumping 호출
	}
}

// 착지 시 점프 횟수 초기화
void ANum6Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CurrentJumpCount = 0; // 착지하면 점프 횟수 초기화
}

float ANum6Character::GetHealth() const
{
	return Health;
}

void ANum6Character::AddHealth(float Amount)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	UE_LOG(LogTemp, Warning, TEXT("Health increased from %.0f to %.0f"), OldHealth, Health);
}


// 이동 처리
void ANum6Character::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

// 보기 처리
void ANum6Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

// 달리기 시작
void ANum6Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// 달리기 종료
void ANum6Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}


// Called when the game starts or when spawned
void ANum6Character::BeginPlay()
{
	Super::BeginPlay();

	// JumpHeight 확인 후 JumpZVelocity에 설정
	UpdateOverheadHP();
}


// Called every frame
void ANum6Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 매 프레임 업데이트 로직이 필요한 경우 여기에 작성
}


void ANum6Character::OnDeath()
{
	ANum6GameState* Num6GameState = GetWorld() ? GetWorld()->GetGameState<ANum6GameState>() : nullptr;
	if (Num6GameState)
	{
		Num6GameState->OnGameOver();
	}
}

void ANum6Character::UpdateOverheadHP()
{
	if (!OverheadWidget) return;
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}
