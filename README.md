# qto_xray_ortho

take xyz pointcloud csv data and render as an xray style orthoimage : color matches density

## usage 

cat xyz.csv  |  ./qto_xray_ortho  xmin xmax ymin ymax   ortho_image.png   m_per_pixel

## make test

make test will fetch a 60Mb xyz.csv point cloud
then generate two orthos, at 1cm and 5mm pixel size

## todo

 - windows release, exe or installer : maybe use mingw ? all static so no DLL deps
 - maybe  line arg for stdin or input csv file name to use : eg - or xyz_input.csv
 - more subtle xray count filtering / scaling : eg to remove low count noise
 - better image output library, and support for .tiff and especially .webp

## credits

brought to you by quato.xyz

works well with [Quato Plan Tracer](https://quato.xyz/plan_tracer/plan_tracer.html) web tool

A PRO subscription to quato will help fund open source projects like this.

gord @ quato.xyz

