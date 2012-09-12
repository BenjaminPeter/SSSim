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


mkdir fig3.e
for tau in "${tAfterE[@]}"; do   for i in `seq 100`; do nice ./sssim $defaultNReplicates  --see $defaultWidth $defaultHeight $defaultDemeSize $defaultFounderSize  $defaultMigrationRate $defaultStartX $defaultStartY $((defaultTStart*tau)).001 $defaultTau $ascSample  -o fig3.e/e${tau}_$i 2>fig3.e.${tau}_${i}.txt;   done &  done

mkdir fig3.ibd.m
for m in "${mRates[@]}"; do for i in `seq 100`; do nice ./sssim $defaultNReplicates  --ibd $defaultWidth $defaultHeight $defaultDemeSize $m $ascSample  -o fig3.ibd.m/ibd.m${m}_${i} 2>fig3.ibd.m.${m}_${i}.txt; done & done

mkdir fig3.ibd.s
for s in "${sampleSize[@]}"; do   for i in `seq 100`; do nice ./sssim $defaultNReplicates  --ibd $defaultWidth $defaultHeight $defaultDemeSize $defaultMigrationRate  -s 50 50 $s -s 40 50 $s -s 30 50 $s -s 20 50 $s -s 10 50 $s -s 0 50 $s -s 50 60 $s -s 35 60 $s  -o fig3.ibd.s/ibd.s${s}_${i} 2>fig3.ibd.s.${s}_${i}.txt;   done& done;
