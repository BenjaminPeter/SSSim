defaultNReplicates=100
defaultWidth=101
defaultHeight=101
defaultDemeSize=10000
defaultMigrationRate=1
defaultFounderSize=500
defaultStartX=50
defaultStartY=50
defaultTau=100
defaultTStart=$(((defaultWidth-defaultStartX-1)*defaultTau))
defaultSampleSize=100
s=$defaultSampleSize
defaultSample="-s 50 50 $s -s 40 50 $s -s 30 50 $s -s 20 50 $s -s 10 50 $s -s 0 50 $s"
ascSample="-s 50 50 $s -s 40 50 $s -s 30 50 $s -s 20 50 $s -s 10 50 $s -s 0 50 $s -s 50 60 100 -s 35 60 100"
defaultSamp1="-s 50 50 $defaultSampleSize"
defaultSamp2="-s 0 50 $defaultSampleSize"
defaultTheta=1e-5


mkdir fig3.s
for s in "${sampleSize[@]}"; do   for i in `seq 100`; do nice ./sssim  $defaultNReplicates  -t $defaultTheta --see $defaultWidth $defaultHeight $defaultDemeSize $defaultFounderSize  $defaultMigrationRate $defaultStartX $defaultStartY    ${defaultTStart}.001 $defaultTau  -s 50 50 $s -s 40 50 $s -s 30 50 $s -s 20 50 $s -s 10 50 $s -s 0 50 $s -s 50 60 $s -s 35 60 $s -o fig3.s/s${s}_${i} 2>fig3.s.${s}_${i}.txt;  done &  done

mkdir fig3.ibd.m
for m in "${mRates[@]}"; do for i in `seq 100`; do nice ./sssim $defaultNReplicates -t $defaultTheta --ibd $defaultWidth $defaultHeight $defaultDemeSize $m $ascSample  -o fig3.ibd.m/ibd.m${m}_${i} 2>fig3.ibd.m.${m}_${i}.txt; done & done

mkdir fig3.k
for k in "${founderSizes[@]}"; do   for i in `seq 100`; do nice ./sssim $defaultNReplicates  -t $defaultTheta --see $defaultWidth $defaultHeight $defaultDemeSize $k  $defaultMigrationRate $defaultStartX $defaultStartY ${defaultTStart}.001 $defaultTau  $ascSample  -o fig3.k/k${k}_${i} 2>fig3.k.${k}_${i}.txt;   done &  done
