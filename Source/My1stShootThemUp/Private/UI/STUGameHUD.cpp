// My 1st Unreal Shoot Them Up Game


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"


void ASTUGameHUD::DrawHUD() 
{
    Super::DrawHUD();

    //this->DrawCrossHair();
}

void ASTUGameHUD::BeginPlay() 
{
    Super::BeginPlay();

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(this->GetWorld(), PlayerHUDWidgetClass);
    
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void ASTUGameHUD::DrawCrossHair() 
{
    const TInterval<float> Center(this->Canvas->SizeX * 0.5, this->Canvas->SizeY * 0.5);
    
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;
    
    this->DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    this->DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
