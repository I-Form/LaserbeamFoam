IOobject phiHHeader
(
    "phiH",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ
);


surfaceScalarField* phiHPtr = nullptr;

if (phiHHeader.typeHeaderOk<surfaceScalarField>(true))
{
    Info<< "Reading face flux ";

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
