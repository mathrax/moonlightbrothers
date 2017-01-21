//Moon imageSize = 30x480
//Mutta&Hibit imageSize = 24x480

import drop.*;
SDrop drop;

PImage image;

//MOON
int maxColor=30;

//Mutta&Hibit
//int maxColor=24;

int imageWidth;
int imageHeight;
int maxPixelColor;

int[] sortLED = {
  29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 
  19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 
  9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};

int[] now_r = {};
int[] now_g = {};
int[] now_b = {};

int[] next_r = {};
int[] next_g = {};
int[] next_b = {};

int[] diff_r = {};
int[] diff_g = {};
int[] diff_b = {};

int[] set_r = {};
int[] set_g = {};
int[] set_b = {};

int[] pixelColor = {};

PrintWriter output;

int count=0;
int nextCount = count+1;
int framePreScale;
float maxPreScale=30;
float lineCount;

int sendCount=-1;

void setup() {
  size(640, 600); 
  colorMode(RGB);
  noSmooth();
  noStroke();
  frameRate(60);
  drop = new SDrop(this);
}


void draw() {

  background(10);
  tint(255);
  if (image!=null) {

    if (loadDone==1) {

      imageWidth = image.width;
      imageHeight = image.height;


      maxPixelColor=imageWidth*imageHeight;

      now_r = expand(now_r, maxPixelColor);
      now_g = expand(now_g, maxPixelColor);
      now_b = expand(now_b, maxPixelColor);

      next_r = expand(next_r, maxPixelColor);
      next_g = expand(next_g, maxPixelColor);
      next_b = expand(next_b, maxPixelColor);

      diff_r = expand(diff_r, maxPixelColor);
      diff_g = expand(diff_g, maxPixelColor);
      diff_b = expand(diff_b, maxPixelColor);

      set_r = expand(set_r, maxPixelColor);
      set_g = expand(set_g, maxPixelColor);
      set_b = expand(set_b, maxPixelColor);

      pixelColor = expand(pixelColor, maxPixelColor);

      loadDone=2;
    } else if (loadDone==2) {

      framePreScale++;

      if (framePreScale>maxPreScale) {
        framePreScale=0;

        image.loadPixels();

        count++;

        if (count<(image.width-1)) {
          nextCount = count+1;
        } else if (count>(image.width-1)) {
          count=0;
          nextCount = count+1;
        } else if (count==(image.width-1)) {
          nextCount = 0;
        }

        for (int i=0; i<imageHeight; i++) {
          pixelColor[i] = image.pixels[i*image.width+count];

          now_r[i]=(pixelColor[i]>>16)&0xFF;
          now_g[i]=(pixelColor[i]>>8)&0xFF;
          now_b[i]=(pixelColor[i])&0xFF;

          pixelColor[i] = image.pixels[i*image.width+nextCount];
          next_r[i]=(pixelColor[i]>>16)&0xFF;
          next_g[i]=(pixelColor[i]>>8)&0xFF;
          next_b[i]=(pixelColor[i])&0xFF;
        }
      }

      imageMode(CORNER);
      noSmooth();
      tint(255);
      image(image, 0, 0, 640, 30);

      lineCount = framePreScale*((nextCount*(640/image.width)-count*(640/image.width))/maxPreScale);

      stroke(255, 255, 0);
      line(width*(count/(float)image.width), 0, width*(count/(float)image.width), 30);


      for (int i=0; i<maxColor; i++) {
        diff_r[i] = next_r[i] - now_r[i];
        diff_g[i] = next_g[i] - now_g[i];
        diff_b[i] = next_b[i] - now_b[i];

        set_r[i] = (int)(now_r[i] + framePreScale*(diff_r[i]/maxPreScale));
        set_g[i] = (int)(now_g[i] + framePreScale*(diff_g[i]/maxPreScale));
        set_b[i] = (int)(now_b[i] + framePreScale*(diff_b[i]/maxPreScale));
      }
      roundCircles();
      //fill(255);
      //text(640*(count/(float)image.width), 100, 100);
    }
  }
}


void roundCircles() {
  pushMatrix();
  translate(width/2, height/2);

  for (int i=0; i<maxColor; i++) {
    rotate(radians(-360/maxColor));
    noStroke();
    fill(set_r[i], set_g[i], set_b[i]);
    ellipse(0, -200, 20, 20);
  }

  popMatrix();
}

void mousePressed() {
  saveData();
}


void saveData() {
  output = createWriter("picdata.txt");


  output.flush();

  for (int j=0; j<imageHeight; j++) {

    for (int i=0; i<imageWidth; i++) {
      int r;
      int g;
      int b;
      color pcolor;

      pcolor = image.pixels[j*image.width+i];

      r=(pcolor>>16)&0xFF;
      g=(pcolor>>8)&0xFF;
      b=(pcolor)&0xFF;

      if (r>=255)r=254;
      if (g>=255)g=254;
      if (b>=255)b=254;

      if (i%10==0) {
        output.println("");
      }

      output.print("0x"+ hex((r<<16) + (g<<8) + b, 6)  + " , " );
    }

    output.println("");
    output.println("//"+j);
    output.println("");
  }
  output.close();  // Finishes the file
}