#ifndef LASERBEAMFOAM_HELPER_FUNCTIONS
#define LASERBEAMFOAM_HELPER_FUNCTIONS
// Description:
// Contains definitions for local helper functions (currently just 1). 
// 
// Presently, only used for reading electrical conductivities and where a check 
// is required against the depreciated `elec_resistivity` keyword.
// 
// In future this likely will be replaced with something simpler.
//
// Mostly exists for 'Do Not Repeat yourself'.
// 
// (Note to other devs: originally this was going to be templated, but
//  the types have unique constructors, so this one just focuses on
//  the MTHD properties. If it's decided to strip this down or out, that
//  is fine. - RB)
// 
// (end).


namespace Foam
{

void lookupConstDimensionedScalarWithDepreciationCheck
(
    autoPtr<const dimensionedScalar>& objectPtr,
    const dimensionSet& dimensionSetValue,
    dictionary& targetDictionary,
    const word oldKeyword,
    const word newKeyword
)
/*
Looks up `oldKeyword` and `newKeyword` in `targetDictionary` and then
handles the logical conditions. It then assigns the `dimensionedScalar`
via `objectPtr.reset( new ... );`.

Example inputs:
```
lookupWithDepreciationCheck<dimensionedScalar>
(
    eleccond1Ptr,               // autoPtr<dimensionedScalar>& objectPtr
    dimensionSet(-1, -3, 3, 0, 0), // const dimensionSet& dimensionSetValue
    transportPropertiesMetal,   // dictionary& targetDictionary
    "elec_resistivity",         // const word oldKeyword
    "Elec_conductivity"         // const word newKeyword
);
```
*/
{
    if 
    (
        targetDictionary.found(newKeyword)
     && targetDictionary.found(oldKeyword)
    )
    {
        Info<< "`createFields.H`:\n\tWarning: using depreciated keyword `" 
            << oldKeyword << "`. "
            << "Please use `" << newKeyword << "` in future.\n"
            << "\tWarning: detected both "
            << oldKeyword << " and " << newKeyword<< "\n"
            << "\t> Prioritising " << newKeyword <<". "
            << "Check your dictionary for redundancies(!), at:\n" 
            << targetDictionary.name()
            << endl;

        objectPtr.reset
        (
            new const dimensionedScalar
            (
                newKeyword,
                dimensionSetValue,
                targetDictionary
            )
        );

        scalar oldScalarValue =
            targetDictionary.get<scalar>(oldKeyword);

        
        // Optional: check tolerances on inputs
        // - user may not have realised. Throw if out of tolerance
        constexpr scalar toleranceForCheck = 10.0 * Foam::SMALL;
        // - (SMALL is 1e-15 when compiled in double precision)

        if 
        (
            mag(objectPtr().value() - (1.0/oldScalarValue)) 
            <= toleranceForCheck
        )
        {
            Info<< "\t> Note: (" << newKeyword << " & " << oldKeyword 
                << " are within a "
                << toleranceForCheck
                << " tolerance)."
                << "\n\t    " << oldKeyword << "^-1\t\t= " 
                << 1.0/oldScalarValue
                << "\n\t    " << newKeyword << "\t\t= " 
                << objectPtr().value()
                << endl;
        }
        else
        {
            FatalErrorInFunction
                << "\tCAUTION: Your " << oldKeyword << " and " 
                << newKeyword << " "
                << "values are dissimilar!"
                << "\n\tTolerance   = " << toleranceForCheck
                << "\n\t" << oldKeyword << "^-1\t\t= " << 1.0/oldScalarValue
                << "\n\t" << newKeyword << "\t\t= " << objectPtr().value()
                << "\n\tStopping solver!"
                << nl
                << exit(FatalError);
        }
    }

    if
    (
        (!targetDictionary.found(newKeyword))
     &&   targetDictionary.found(oldKeyword)
    )
    {
        Info<< "Warning: using depreciated keyword `" 
            << oldKeyword << "`. "
            << "Please use `" << newKeyword << "` in future.\n"
            << endl;

        // read resistivity as a scalar then invert at `.reset`
        scalar oldScalarValue = 
            targetDictionary.get<scalar>(oldKeyword);

        // oldScalarValue += SMALL; // add safety for case of 0?

        objectPtr.reset
        (
            new const dimensionedScalar
            (
                newKeyword,
                dimensionSetValue,
                1.0 / oldScalarValue
            )
        );
    }
    else
    {
        // Logic: assume `newKeyword` is provided.
        objectPtr.reset
        (
            new const dimensionedScalar
            (
                newKeyword,
                dimensionSetValue,
                targetDictionary
            )
        );
    }
}
// Backwards compatability with `oldKeyword` key word (end)

}

#endif
