// WARNING: this header has a dependency on `phasedictionary`.

const Switch electromagnetics(phasedictionary.lookupOrDefault<Switch>("electromagnetics", false));

// EM should be independent from others => Moving to pointer system.

autoPtr<volVectorField> HPtr;
autoPtr<volScalarField> pHPtr;
autoPtr<volVectorField> BPtr;
autoPtr<volVectorField> JPtr;
autoPtr<volScalarField> JoulesourcePtr;
autoPtr<volVectorField> JcrossBPtr;
autoPtr<volScalarField> divBPtr;
// createPhiH.H now called after `if` statement
autoPtr<volScalarField> eleccondPtr;
autoPtr<volScalarField> magpermPtr;
autoPtr<volScalarField> oneoversigmaPtr;
autoPtr<volTensorField> gradHPtr;

if (electromagnetics)
{
    HPtr.reset
    (
        new volVectorField
        (
            IOobject
            (
                "H",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh//,
        )
    );

    pHPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "pH",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        )
    );

    BPtr.reset
    (
        new volVectorField
        (
        IOobject
        (
            "B",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector("B",dimensionSet(1,0,-2,0,0),vector::zero)
        )
    );

    JPtr.reset
    (
        new volVectorField
        (
            IOobject
            (
                "J",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedVector("J", dimensionSet(0, -2, 0, 0, 0), vector::zero)
            // dimensionedScalar("TRHS",dimensionSet(0, 2, -3, 0, 0),scalar(0.0))
        )
    );

    // J=(1.0/magperm)*fvc::curl(B);
    // J.correctBoundaryConditions();

    JoulesourcePtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "Joulesource",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("Joulesource",dimensionSet(1, -1, -3, 0, 0),0.0)
        )
    );


    JcrossBPtr.reset
    (
        new volVectorField
        (
            IOobject
            (
                "JcrossB",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
                mesh,
                dimensionedVector("JcrossB", dimensionSet(1, -2, -2, 0, 0), vector::zero),
                zeroGradientFvPatchVectorField::typeName
                // dimensionedScalar("TRHS",dimensionSet(0, 2, -3, 0, 0),scalar(0.0))
        )
    );



    divBPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "divB",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            // dimensionedScalar("divB",dimensionSet(0,-2,0,0,0),0.0)
            dimensionedScalar("divB",dimensionSet(1,-1,-2,0,0),0.0)
        )
    );

    //     volVectorField divoneoversigmaTgradH
    // (
    //     IOobject
    //     (
    //         "divoneoversigmaTgradH",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::READ_IF_PRESENT,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh,
    //     dimensionedVector("divoneoversigmaTgradH",dimensionSet(1, 0, -3, 0, 0),vector::zero)//,
    // 	    // zeroGradientFvPatchVectorField::typeName
    // );

    // RB: moving this after the Ptr reference assignment:
    // #include "createPhiH.H"

    eleccondPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "eleccond",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("eleccond",dimensionSet(-1, -3, 3, 0, 0),0.0),
                    zeroGradientFvPatchScalarField::typeName
        )
    );

    magpermPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "magperm",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("magperm",dimensionSet(1, 1, -2, 0, 0),0.0),
                    zeroGradientFvPatchScalarField::typeName
        )
    );

    oneoversigmaPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "oneoversigma",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("oneoversigma",dimensionSet(1, 3, -3, 0, 0),0.0),
            zeroGradientFvPatchScalarField::typeName
        )
    );

    gradHPtr.reset
    (
        new volTensorField
        (
            IOobject
            (
                "gradH",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedTensor("gradH",dimensionSet(0, -2, 0, 0, 0),tensor::zero),
            zeroGradientFvPatchTensorField::typeName       
        )
    );

    volVectorField& H = HPtr();
    volScalarField& pH = pHPtr();
    volVectorField& B = BPtr();
    H.correctBoundaryConditions();

    pH.correctBoundaryConditions();

    B.write();
}

// Referenece declarations - AVOID, call references ONLY at point of use.
// volVectorField& H = HPtr();
// volScalarField& pH = pHPtr();
// volVectorField& B = BPtr();
// volVectorField& J = JPtr();
// volScalarField& Joulesource = JoulesourcePtr();
// volVectorField& JcrossB = JcrossBPtr();
// volScalarField& divB = divBPtr();
// // FD within createPhi.H
// // n/done
// volScalarField& eleccond = eleccondPtr();
// volScalarField& magperm = magpermPtr();
// volScalarField& oneoversigma = oneoversigmaPtr();
// volTensorField& gradH = gradHPtr();

// H.correctBoundaryConditions();

// pH.correctBoundaryConditions();

// B.write();

// Call createPhiH.H as is, has internal if-switch.
#include "createPhiH.H"


// volTensorField gradH
// (
//     IOobject
//     (
//         "gradH",
//         runTime.timeName(),
//         mesh,
//         IOobject::READ_IF_PRESENT,
//         IOobject::AUTO_WRITE
//     ),
//     mesh,
//     dimensionedTensor("gradH",dimensionSet(0, -2, 0, 0, 0),tensor::zero),
//     zeroGradientFvPatchTensorField::typeName
// );
// }
/*
    Unfortunately this needs to be changed to autoPtr<type> ptrName; in fwd-decl.
    then you do the assignments

    autoPtr<volScalarField> additionalFieldPtr;

    if (switch)
{
    additionalFieldPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "additionalField",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("zero", dimless, 0.0)
        )
    );
}
    //then
    if (additionalFieldPtr.valid() && switch)
    {
        UEqn += additionalFieldPtr();
    }
*/
// //MAGNETIC STUFF (End)