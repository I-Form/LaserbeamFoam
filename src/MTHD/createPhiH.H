
// RB: Move if (switch) inside this code, leaving a reference to the point 
// in the main scope. Note, we must reference the pointer at point-of-use
// when used after outside this `if` statement.

surfaceScalarField* phiHPtr = nullptr;

if (electromagnetics)
{
    // Reference H field
    volVectorField& H = HPtr();


    IOobject phiHHeader
    (
        "phiH",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
    );

    if (phiHHeader.typeHeaderOk<surfaceScalarField>(true))
    {
        Info<< "Reading face flux ";

        // Check: Alex Skillen - change `phiHPtr` to `autoPtr` or add formal 
        // delete at end of main() (latter if needed). 
        phiHPtr = new surfaceScalarField
        (
            IOobject
            (
                "phiH",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );
    }
    else
    {
        Info<< "Calculating face flux ";

        phiHPtr = new surfaceScalarField
        (
            IOobject
            (
                "phiH",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            fvc::flux(H)
        );
    }

    surfaceScalarField& phiH = *phiHPtr;

    Info<< phiH.name() << nl << endl;
}
