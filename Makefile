
test : qto_xray_ortho xyz.csv
	cat xyz.csv | ./qto_xray_ortho  -26.00 -8.00  -7 9   ortho_010.png 
	cat xyz.csv | ./qto_xray_ortho  -26.00 -8.00  -7 9   ortho_005.png  0.005

qto_xray_ortho : qto_xray_ortho.cpp
	g++ -std=c++0x qto_xray_ortho.cpp -o qto_xray_ortho

xyz.csv :
	wget https://quato.blob.core.windows.net/uploads/pub/xyz.csv

clean : 
	rm -f ortho_010.png  ortho_005.png ortho.png qto_xray_ortho

    


