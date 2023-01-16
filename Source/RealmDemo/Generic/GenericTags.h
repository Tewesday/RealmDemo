

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"
#include "UObject/NoExportTypes.h"

#include "GenericTags.generated.h"

UENUM()
enum class EGenericTags {
    IsDead,
    NeedsCleanup,
    CleanupProcessor,
    GenericTags
};

USTRUCT()
struct REALMDEMO_API FIsDeadTag : public FMassTag {
    GENERATED_BODY()
};

USTRUCT()
struct REALMDEMO_API FNeedsCleanup : public FMassTag {
    GENERATED_BODY()
};

// This tag exists to allow for processors that should run without specific entities needing to exist (ex: cleanup processors)
USTRUCT()
struct REALMDEMO_API FCleanupProcessor : public FMassTag {
    GENERATED_BODY()
};