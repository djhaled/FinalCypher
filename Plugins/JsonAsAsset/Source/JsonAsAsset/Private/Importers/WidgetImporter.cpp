// Copyright Epic Games, Inc. All Rights Reserved.

#include "Importers/WidgetImporter.h"

#include "Dom/JsonObject.h"
#include "Factories/CurveTableFactory.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintCompilationManager.h"
#include "Components/Overlay.h"
#include "Utilities/AssetUtilities.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include <UMGEditor/Public/WidgetBlueprint.h>

bool UWidgetImporter::ImportData()
{
    WidgetBlueprint = CastChecked<UWidgetBlueprint>(FKismetEditorUtilities::CreateBlueprint(UUserWidget::StaticClass(), Package, *FileName, EBlueprintType::BPTYPE_Normal, UWidgetBlueprint::StaticClass(), UWidgetBlueprintGeneratedClass::StaticClass()));
    WidgetTree = WidgetBlueprint->WidgetTree.Get();
    UObject* WidgetTreeObject = Cast<UObject>(WidgetTree);
    try
    {
        TSharedPtr<FJsonObject> Properties = JsonObject->GetObjectField("Properties");
        TSharedPtr<FJsonObject> WidgetTreeProperties = Properties->GetObjectField("WidgetTree");
        Properties->RemoveField("UberGraphFunction");
        // RootWidgetExport Section
        auto RootWidgetExport = GetExportByObjectPath(WidgetTreeProperties);
        auto RootWidgetProperties = RootWidgetExport->AsObject()->GetObjectField("Properties")->GetObjectField("RootWidget");
        UWidget* RootWidgetObject = CreateContentWidget(RootWidgetProperties);
        auto RootWidgetProperties_v = RootWidgetExport->AsObject()->GetObjectField("Properties");
        WidgetTree->RootWidget = RootWidgetObject;
        FBlueprintCompilationManager::CompileSynchronously(FBPCompileRequest(WidgetBlueprint, EBlueprintCompileOptions::None, NULL));
        // Handle edit changes, and add it to the content browser
        if (!HandleAssetCreation(WidgetBlueprint))
        {
            return false;
        }
    }
    catch (const char* Exception)
    {
        UE_LOG(LogJson, Error, TEXT("%s"), *FString(Exception));
        return false;
    }

    return true;
}

UObject* UWidgetImporter::CreateEmptyWidget(UObject* Parent, FName Name, FName Type)
{
    if (Type == "CommonTextBlock") {
        Type = "TextBlock";
    }
    if (Type == "LazyImage_ResourceIcon")
    {
        Type = "Image";
    }
    if (Type == "FortLazyImage")
    {
        Type = "Image";
    }
    float RenderOpacity_Fallback = 1.0f;
    if (Type.ToString().EndsWith("_C"))
    {
        //Type = "TeamLoadout";
        int bk = 3;
    }
    const UClass* WidgetClass = FindObject<UClass>(ANY_PACKAGE, *Type.ToString());
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("JAA: Class type '%s' doesn't exist! Creating a OverlayWidget just for Reference"), *Type.ToString());
        WidgetClass = UOverlay::StaticClass();
        Name = FName(FString::Printf(TEXT("FALLBACK_%s_%s"), *Name.ToString(), *Type.ToString()));
        RenderOpacity_Fallback = 0.0f;
    }
    UWidget* Widget = WidgetTree->ConstructWidget<UWidget>(WidgetClass->GetDefaultObject()->GetClass(), Name);
    Widget->SetRenderOpacity(RenderOpacity_Fallback);
    UObject* WidgetObject = Cast<UObject>(Widget);
    return WidgetObject;
}



UObject* UWidgetImporter::CreateEmptySlot(UObject* Parent, FName Name, FName Type)
{
    const UClass* SlotClass = FindObject<UClass>(ANY_PACKAGE, *Type.ToString());
    EObjectFlags NewObjectFlags = RF_Transactional;
    UPanelSlot* PanelSlot = NewObject<UPanelSlot>(WidgetTree, SlotClass->GetDefaultObject()->GetClass(), Name, NewObjectFlags);
    return PanelSlot;
}
UPanelSlot* UWidgetImporter::CreateSlot(TSharedPtr<FJsonValue> SlotProp, UObject* Parent,UPanelWidget* PanWid)
{
    FString SlotType;
    FName SlotName;
    TSharedPtr<FJsonObject> SlotProperties;
    //
    TSharedPtr<FJsonObject> SlotObject = SlotProp->AsObject();
    ProcessWidgetProperties(SlotObject, SlotType, SlotName, SlotProperties);
    UObject* SlotPanelObject = CreateEmptySlot(Parent, SlotName, FName(SlotType));
    UPanelSlot* PanelSlot = Cast<UPanelSlot>(SlotPanelObject);
    UWidget* ContentWidget = CreateContentWidget(SlotProperties.Get()->GetObjectField("Content"));
    UPanelSlot* pWid = PanWid->AddChild(ContentWidget);
    SlotProperties.Get()->RemoveField("Parent");
    SlotProperties.Get()->RemoveField("Content");
    GetObjectSerializer()->DeserializeObjectProperties(SlotProperties, pWid);
    return pWid;
}
UWidget* UWidgetImporter::CreateContentWidget(TSharedPtr<FJsonObject> ContentProp)
{
    auto ContentWidgetExport = GetExportByObjectPath(ContentProp);
    auto RootWidgetProperties = ContentWidgetExport->AsObject()->GetObjectField("Properties");
    // Declare
    FString WidgetType;
    FName WidgetName;
    TSharedPtr<FJsonObject> ObjectProperties;
    //
    ProcessWidgetProperties(ContentProp, WidgetType, WidgetName, ObjectProperties);
    //if (WidgetType.EndsWith("_C"))
    //{
    //    auto idk = ContentWidgetExport->AsObject()->GetStringField("Class");
    //    idk.RemoveFromEnd("'");
    //    idk.RemoveFromStart("'");
    //    idk = idk.Replace(TEXT("WidgetBlueprintGeneratedClass'"), TEXT(""));
    //    TObjectPtr<UObject> Object = NULL;
    //    LoadObject(idk, ContentWidgetExport->AsObject()->GetStringField("Type"), Object);
    //}
    UObject* RootWidgetObject = CreateEmptyWidget(WidgetBlueprint, WidgetName, FName(WidgetType));
    GetObjectSerializer()->DeserializeObjectProperties(ObjectProperties, RootWidgetObject);
    UWidget* ContentWidget = Cast<UWidget>(RootWidgetObject);
    UPanelWidget* PanelWidget = Cast<UPanelWidget>(RootWidgetObject);
    if (!PanelWidget) { return ContentWidget; }
    auto SlotsArray = ObjectProperties->GetArrayField("Slots");
    auto AllSlots = PanelWidget->GetSlots();
    PanelWidget->PostLoad();
    for (size_t SlotIndex = 0; SlotIndex < SlotsArray.Num(); SlotIndex++)
    {
        CreateSlot(SlotsArray[SlotIndex], RootWidgetObject,PanelWidget);
        int bk = 3;
    }
    return ContentWidget;
}

void UWidgetImporter::test()
{
}

void UWidgetImporter::ProcessWidgetProperties(TSharedPtr<FJsonObject> WidgetProperties, FString& WidgetType, FName& WidgetName, TSharedPtr<FJsonObject>& WidgetPropertiesObject)
{
    FJsonValue* WidgetValue = GetExportByObjectPath(WidgetProperties).Get();

    auto WidgetJson = WidgetValue->AsObject().Get();
    WidgetType = WidgetJson->GetStringField("Type");
    FString TempWidgetName = WidgetJson->GetStringField("Name");
    auto test = WidgetJson->GetObjectField("Properties");
    WidgetPropertiesObject = WidgetJson->GetObjectField("Properties");
    WidgetName = FName(TempWidgetName);
}

