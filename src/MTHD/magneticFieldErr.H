// Info<< "B magnetic flux divergence error = "
//     << runTime.deltaTValue()
//       *mag(fvc::div(phiB))().weightedAverage(mesh.V()).value()
//     << endl;

Info<< "H magnetic flux divergence error {div(mu H)}= "
    << runTime.deltaTValue()
      *mag(fvc::div(fvc::interpolate(magperm)*phiH))().weightedAverage(mesh.V()).value()
    << endl;

    // fvc::div(fvc::interpolate(magperm)*phiH)

    //magperm*fvc::div(phiH)+((fvc::grad(magperm) & H))