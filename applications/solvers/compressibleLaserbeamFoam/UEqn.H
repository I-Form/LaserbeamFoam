

if (laser.powderSim())
{
    DC =
        DarcyConstantlarge*Foam::pow((1.0-epsilon1mask),2)
       /(Foam::pow(epsilon1mask, 3) + DarcyConstantsmall);
}
else
{
    DC =
        DarcyConstantlarge*Foam::pow((1.0 - epsilon1), 2)
       /(Foam::pow(epsilon1, 3) + DarcyConstantsmall);
}


Marangoni = condensate*epsilon1*mixture.MarangoniForce(T);

    // Create local autopointer to reference UEqn outside
    // of the if statement.
    autoPtr<fvVectorMatrix> UEqnPtr;

    if (electromagnetics)
    {
        // Perform declarations
        volVectorField& H = HPtr();
        volVectorField& J = JPtr();
        volScalarField& magperm = magpermPtr();
        volVectorField& JcrossB = JcrossBPtr();
        // < declarations end

        JcrossB=(J ^ (magperm*H));//.oldTime()
        JcrossB.correctBoundaryConditions();

        UEqnPtr.reset
        (
            new fvVectorMatrix
            (
              fvm::ddt(rho, U)
            + fvm::div(mixture.rhoPhi(), U)
            + turbulence->divDevRhoReff(U)
            + fvm::Sp(DC, U)
            + Marangoni
            -JcrossB
            )
        );
    }
    else
    {
        UEqnPtr.reset
        (
            new fvVectorMatrix
            (
              fvm::ddt(rho, U)
            + fvm::div(mixture.rhoPhi(), U)
            + turbulence->divDevRhoReff(U)
            + fvm::Sp(DC, U)
            + Marangoni
            )
        );
    }

    // set reference as usual.
    fvVectorMatrix& UEqn = UEqnPtr();


    UEqn.relax();

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    mixture.surfaceTensionForce()
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
                ) * mesh.magSf()
            )
        );

        K = 0.5*magSqr(U);
    }
