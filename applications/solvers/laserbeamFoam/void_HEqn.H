{
       while (bpiso.correct())
       {




    oneoversigma = 1.0/(eleccond);
    oneoversigma.correctBoundaryConditions();



surfaceScalarField muPhiH = fvc::interpolate(magperm)*phiH;
surfaceScalarField muPhi = fvc::interpolate(magperm)*phi;



volTensorField TgradH = fvc::grad(H)().T();
TgradH.correctBoundaryConditions();


fvVectorMatrix HEqn
            (
                fvm::ddt(magperm,H)

              + fvm::div(muPhi, H)
              - fvc::div(muPhiH, U)//**************

                -fvm::laplacian(oneoversigma,H)
                 +fvc::div(oneoversigma*TgradH)//**************

            );



              HEqn.relax();

            //   fvConstraints.constrain(HEqn);

            HEqn.solve();

H.correctBoundaryConditions();



            volScalarField rAB(1.0/(HEqn.A()));
            rAB.correctBoundaryConditions();
            surfaceScalarField rABf("rABf", fvc::interpolate(rAB));

            volVectorField HbyAH("HbyAH", H);
            HbyAH = rAB*HEqn.H();
            surfaceScalarField phiHbyAH
    (
        "phiHbyAH",
        fvc::flux(HbyAH)

    );


            while (bpiso.correctNonOrthogonal())//NonOrthogonal
            {



                fvScalarMatrix pHEqn
                (

                        fvm::laplacian(rABf,pH) ==fvc::div(fvc::interpolate(magperm)*phiHbyAH)


                );

                pHEqn.solve();
                pH.correctBoundaryConditions();

                if (bpiso.finalNonOrthogonalIter())
                {
                    

                    phiH =phiHbyAH - (pHEqn.flux())/fvc::interpolate(magperm);

                    H.correctBoundaryConditions();




                }

            }



surfaceScalarField phiB = phiH*fvc::interpolate(magperm);
#include "magneticFieldErr.H"



B=magperm*H;
J=fvc::curl(H);
J.correctBoundaryConditions();
Joulesource=(1.0/eleccond)*magSqr(J);
Joulesource.correctBoundaryConditions();
divB=fvc::div(fvc::interpolate(magperm)*phiH);//magperm*fvc::div(phiH)+((fvc::grad(magperm) & H)) ;//
divB.correctBoundaryConditions();

}


}