#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;


//   USAGE :    cat xyz.csv | ./test2  xmin xmax ymin ymax

//   BUILD :    g++ -std=c++0x qto_xray_ortho.cpp -o qto_xray_ortho

//   RUN   :    cat xyz.csv | ./qto_xray_ortho -26.00 -8.00  -7 9


int main(int argc, char* argv[])
{
    float pix_per_m = 0.010;   // 1cm pixels
    const float m_per_pix = 1.0 / pix_per_m;

    if (argc<5)
    {
        cout << "USAGE : ./test xmin xmax ymin ymax" << endl;
        exit(-1);
    }
    float ll = stof(argv[1]);
    float rr = stof(argv[2]);
    float bb = stof(argv[3]);
    float tt = stof(argv[4]);

    cout << "l r b t : " << fixed << setprecision(3) << ll << " " << rr << " " << bb << " " << tt << endl;

    float ww = (rr-ll);
    float hh = (tt-bb);
    cout << "ww x hh m : " << ww << " x " << hh << endl;

    int W = ceil(ww / pix_per_m);
    int H = ceil(hh / pix_per_m);

    cout << "W x H pix : " << W << " x " << H << endl;

    vector<uint16_t> buf(W*H, 0);
    if (0){
    for (int j=0;j<H;j++)
    {
        for (int i=0;i<W;i++)
        {
            int ix = j*W+i;
            buf[ix]=0;
        }
    }
    }

    // stream and count pts per x y pixel bin

    {

    string line;
    int n=0;
    int oob=0;
    while (getline(cin, line)) 
    {
        stringstream ss(line);
        string value;
        vector<float> row;
        n++;

        while (getline(ss, value, ',')) 
        {
            row.push_back(stof(value));
        }
    
        float x = row[0];
        float y = row[1];
        //cout << "x y : " << fixed << setprecision(3) << x << " " << y << endl;

        if (x<ll || x>rr || y<bb || y>tt)
        {
            oob++;
        }
        else
        {
            int i = round((x-ll) * m_per_pix );
            int j = round((y-bb) * m_per_pix);

            if (i<0 || j<0 || i>=W || j>=H)
                cout << "BAD index" << endl;
            
            // cout << "i j : " << i << " " << j << endl; 
            
            int ix = j*W+i;
            // cout << " ix : " << ix << endl;

            buf[ix]++;
        }

    }
    cout << "ignored oobs : " << oob << endl;
        
    }

    // get max count so we can normalize to 0..255 color range

    uint16_t npmax=0;
    {
    for (int j=0;j<H;j++)
    {
        for (int i=0;i<W;i++)
        {
            int ix = j*W+i;
            uint16_t np = buf[ix];
            if (np>npmax)
                npmax=np;
        }
    }
    cout << "max cnt : " << npmax << endl;
    }

    // normalize [0..npmax] -> 0..255 in some color gradient scheme 
    // write rgb array, save as image


    /// write rgb buffer, save to png file

{
    int width = W;
    int height = H;
    std::vector<unsigned char> data(width * height * 3); // R, G, B for each pixel

    for (int j=0;j<H;j++)
    {
        for (int i=0;i<W;i++)
        {
            int ix = j*W+i;
            uint16_t np = buf[ix];

            int cg = floor(255*np/npmax);

            int jj = H-j-1;                     // image y inverted
            int cix = 3*(jj*W+i);

            data[cix+0]=cg;
            data[cix+1]=cg;
            data[cix+2]=cg;
        }
    }

    stbi_write_png("output.png", width, height, 3, data.data(), width * 3);
}
}

        



