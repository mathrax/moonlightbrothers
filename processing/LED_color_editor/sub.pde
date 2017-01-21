//DRAG & DROP
String[] extensions = {
  ".bmp", ".gif", ".png", 
  ".BMP", ".GIF", ".PNG"
};

int loadDone;
String imgname="";

void dropEvent(DropEvent theDropEvent) {

  loadDone=0;
  now_r = new int[0];
  now_g = new int[0];
  now_b = new int[0];

  next_r = new int[0];
  next_g = new int[0];
  next_b = new int[0];

  diff_r = new int[0];
  diff_g = new int[0];
  diff_b = new int[0];

  set_r = new int[0];
  set_g = new int[0];
  set_b = new int[0];

  if (theDropEvent.isFile()) {

    File myFile = theDropEvent.file();
    //File[] files = myFile.listFiles();

    if (myFile.isFile()) {

      imgname = myFile.getName();
      //println(myFile.getName());


      for (String extension : extensions) {
        if (myFile.getPath().endsWith(extension)) {
          image  = loadImage(myFile.getAbsolutePath());
        }
      }
    }
  }
  loadDone=1;
}