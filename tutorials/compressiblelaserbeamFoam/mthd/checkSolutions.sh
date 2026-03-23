#!/bin/bash

# Run check using diff and diffstat to see if solutions mutated
# folders=("0.001" "0.002" "0.003" "0.004" "0.005" "0.01" "0.02" "0.03" "0.04" "0.05" "0.06" "0.07" "0.08" "0.09")
fields=("U" "T" "H" "p_rgh" "alphas" "divB")

fpSource=~/Documents/openfoam/v2506/laserbeamfoam_dev_branches/fix_divzero_error/tutorials/laserbeamFoam/Plate2D
#$PWD
#$PWD / ./richter2dOrig 
fpReference=~/Documents/openfoam/v2506/laserbeamfoam_dev_branches/fix_divzero_error/tutorials/laserbeamFoam/Plate2D_MTHD_original
#Plate2D_orig
#../richter2dAutoPtr/



# fpSource=~/Documents/openfoam/v2506/laserbeamfoam_dev_branches/fix_divzero_error/tutorials/compressiblelaserbeamFoam/mthd/richter2d_rerun2
# #$PWD
# #$PWD / ./richter2dOrig 
# fpReference=../richter2D_MTHD_original


folders=$(foamListTimes -case ${fpSource}) # could pass `-time 0:``


# Quick list
## Richter 2D:
# AutoPtr, Orig = Match
# `==` old, Orig = Match
# `==` Recompile, Orig = 
#
## Plate 2D:
#  AutoPtr (T+U), orig = Match
#  AutoPtr (T only), orig = Match
#  With `==`

fpReport=./solution\_check.log
echo "Checking files" > ${fpReport}
# myArray=("cat" "dog" "mouse" "frog"))
echo "fpSource = ${fpSource}" >> ${fpReport}
echo "fpReference = ${fpReference}" >> ${fpReport}

for folder in ${folders[@]}; do
    echo "(${folder})"
    for field in ${fields[@]}; do
        if [ -d "${fpSource}/${folder}" ]; then
            echo "(${folder}, ${field})"
            #^checks if directory exists

            fileRef=${fpReference}/${folder}/${field}
            fileSrc=${fpSource}/${folder}/${field}
            
            # Report the differences, if any
            echo "${folder}/${field}:" >> ${fpReport}
            diff -u ${fileRef} ${fileSrc} | diffstat 2>&1 | tee -a ${fpReport}
        fi
    done
done

# for folder in ${folders[@]}; do
#     echo "(${folder})"
#     for field in ${fields[@]}; do
#         if [ -d "${fpSource}/${folder}" ]; then
#             echo "(${folder}, ${field})"
#             #^checks if directory exists

#             fileRef=${fpReference}/${folder}/${field}
#             fileSrc=${fpSource}/${folder}/${field}
            
#             # Report the differences, if any
#             echo "${folder}/${field}:" >> ${fpReport}
#             diff -u ${fileRef} ${fileSrc} >> ${fpReport}
#         fi
#     done
# done

echo "[checkSolutions.sh] Done."