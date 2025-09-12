# qto_xray_ortho

take xyz pointcloud csv data and render as an xray style orthoimage : color matches density

## usage 

cat xyz.csv  |  ./qto_xray_ortho  xmin xmax ymin ymax   ortho_image.png   m_per_pixel

## make test

make test will fetch a 60Mb xyz.csv point cloud
then generate two orthos, at 1cm and 5mm pixel size

