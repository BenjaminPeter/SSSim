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


mkdir fig3.ibd.m
for m in "${mRates[@]}"; do for i in `seq 100`; do nice ./sssim $defaultNReplicates  --ibd $defaultWidth $defaultHeight $defaultDemeSize $m $ascSample  -o fig3.ibd.m/ibd.m${m}_${i} 2>fig3.ibd.m.${m}_${i}.txt; done & done

mkdir fig3.t
 for tau in "${tauV[@]}"; do   for i in `seq 100`; do tStart=$(((defaultWidth-defaultStartX-1)*tau)); ./sssim $defaultNReplicates  --see $defaultWidth $defaultHeight $defaultDemeSize $defaultFounderSize  $defaultMigrationRate $defaultStartX $defaultStartY ${tStart}.001 $tau $ascSample  -o fig3.t/t${tau}_${i} 2>fig3.tau.${tau}_${i}.txt;    done &  done

mkdir fig3.m
for m in "${mRates[@]}"; do   for i in `seq 100`; do ./sssim $defaultNReplicates  --see $defaultWidth $defaultHeight $defaultDemeSize $defaultFounderSize  $m $defaultStartX $defaultStartY ${defaultTStart}.001 $defaultTau $ascSample  -o fig3.m/m${m}_${i} 2>fig3.m.${m}_${i}.txt;    done &  done
