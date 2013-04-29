
import java.net.*;
import java.io.*;
import org.json.*;
import org.apache.http.client.*;
import processing.serial.*;
import ddf.minim.*;

AudioPlayer player_pumpup;
AudioPlayer player_focus;
AudioPlayer player_destress;
AudioPlayer player_hangout;
Minim minim_pumpup;
Minim minim_focus;
Minim minim_destress;
Minim minim_hangout;

//AudioRenderer radar, vortex, iso;
//AudioRenderer[] visuals;

static boolean DEBUG_NETWORK = true;
static boolean DEBUG_ARDUINO = true;
static boolean ENABLE_SERIAL = false;

Serial myPort;
int currentColor;
int bc_enable;
int aroma_enable;
int box_enable;
int ir_enable;
int music_player_enable;
String music_genre;
Minim current_minim;
AudioPlayer current_player;


void setup() {
  println(Serial.list());
  if(ENABLE_SERIAL) {
    String portName = Serial.list()[2];
    myPort = new Serial(this, portName, 9600);
  }
  
  bc_enable = aroma_enable = box_enable = ir_enable = 0;
  currentColor = 0;
  
  minim_pumpup = new Minim(this);
  player_pumpup = minim_pumpup.loadFile("music/pumpup/royksoppforever.mp3");
  
  minim_focus = new Minim(this);
  player_focus = minim_focus.loadFile("music/focus/reichlargeensemble.mp3");
  
  minim_destress = new Minim(this);
  player_destress = minim_destress.loadFile("music/relax/awalk.mp3");
  
  minim_hangout = new Minim(this);
  player_hangout = minim_hangout.loadFile("music/hangout/callmehome.mp3");
  
  music_player_enable = 0;
  current_player = player_pumpup;
  
 // size(displayWidth,displayHeight,P3D);
  
  background(50);
  
  // setup renderers
  //vortex = new VortexRenderer(groove);
  //radar = new RadarRenderer(groove);
  //iso = new IsometricRenderer(groove);
   
  //visuals = new AudioRenderer[] {radar, vortex,  iso};
   
  // activate first renderer in list
  //select = 0;
  //groove.addListener(visuals[select]);
  //visuals[select].setup();

}


boolean sketchFullScreen() {
  return true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

void poll_for_changes() {
  JSONObj outerObject = new JSONObj();
  JSONObj room2Object = new JSONObj();
  JSONObj dataObject = new JSONObj();
    
  dataObject.put("bubble_column_color","green"); // Blue
  dataObject.put("bubble_column_enable",false);
  dataObject.put("aroma_diffuser_enable",true);
  dataObject.put("music_player_genre","pumpup");  // Soothing
  dataObject.put("music_player_enable",false);
  dataObject.put("projector_screen_color","blue");  // Blue
  dataObject.put("music_visualizer_enable",true);
  room2Object.put("object_id",2);
  room2Object.put("action","GET_DATA");  // GET or PUT
  room2Object.put("data",dataObject);
  outerObject.put("room2",room2Object);
  
  if(DEBUG_NETWORK) {
    System.out.println("Beginning backend poll for device states, the JSON object looks like");
    System.out.println(outerObject);
  }
  
  URL myURL = null;
  HttpURLConnection connection = null;
  
  if(DEBUG_NETWORK) {
    System.out.println("Opening connection");
  }
  
  try {
    myURL = new URL("http","cmu-389815.andrew.cmu.edu",8080,"/tfh/");
  } catch(MalformedURLException e) { System.out.println(e); }
  
  try {
    connection = (HttpURLConnection)myURL.openConnection();
  } catch(IOException e) { System.out.println(e); }
  
  connection.setDoOutput(true);
  connection.setDoInput(true);
  try {
    connection.setRequestMethod("POST");
  } catch(ProtocolException e) { System.out.println(e); }
  connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
  connection.setRequestProperty("charset","utf-8");
  connection.setRequestProperty("Content-Length", "" + Integer.toString( outerObject.toString().length() ));
  
  OutputStreamWriter writer = null;
  BufferedReader reader = null;
  
  try {
    writer = new OutputStreamWriter(connection.getOutputStream());
    writer.write(outerObject.toString());
    writer.flush();
    writer.close();
  } catch(IOException e) { System.out.println(e); }
  
  String str = null;
  String accumulator = "";
  
  try {
    reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
    
    while (!reader.ready()) {
      try {
        if(DEBUG_NETWORK) {
          System.out.println("> reader not ready");
        }
        Thread.sleep(1000); // wait for stream to be ready.
      } catch(InterruptedException e) { System.out.println(e); }
    }
    
    while ( null != ((str = reader.readLine() )) ) {
      if(DEBUG_NETWORK) {
        System.out.println(str);
      }
      accumulator += str;
    }
    
    reader.close();
    
  } catch (IOException e) { System.out.println(e); }
  
  JSONObj responseJSON = null;
  if(accumulator.contains("ERROR") || accumulator.contains("SUCCESS")) {
    if(DEBUG_NETWORK) {
      System.out.println("Error in the network communication.");
    }
    return;
  }
  
  responseJSON = new JSONObj(accumulator);
  
  if(DEBUG_NETWORK) {
    System.out.println("Done with the network communication.");
  }
  
  generate_arduino_update(responseJSON);
}

//--------------------------------------------------------------------------------------
/***** serial message format:

byte 0: 'r' (the header, for "rapid") 
byte 1: a value from 0 to 6 representing the bubble column color 
byte 2: relay states, broken into: 
  bit 0-1 - bubble column current power state, desired power state (0 is off, 1 is on) 
  bit 2-3 - aromatherapy device current power state, desired power state 
  bit 4-5 - box fan current power state, desired power state 
  bit 6-7 - state of IR remote 
byte 3: 'p' (the footer, "for prototyping")

*****/
//--------------------------------------------------------------------------------------

void generate_arduino_update(JSONObj data) {
  String bc_color;
  int new_bc_enable;
  int new_aroma_enable;
  int desired_music_enable = 0;
  String screen_color;
  String desired_music_genre = "";
  int new_visualizer_enable;
  byte byteOne = 0;
  byte byteTwo = 0;
  byte byteThree = 0;
  byte byteFour = 0;
  
  if(DEBUG_ARDUINO) {
    System.out.println("Begin setting up and sending Arduino serial update");
  }
  
  if(bc_enable != 0)
    byteThree |= (1 << 5);
  if(aroma_enable != 0)
    byteThree |= (1 << 3);
  if(box_enable != 0)
    byteThree |= (1 << 1);
  if(ir_enable != 0)
    byteThree |= (1 << 7);
  
  byteOne = 'r';
  
  if(data.has("bubble_column_color")) {
    bc_color = data.getString("bubble_column_color");
      println(bc_color);
      if(bc_color.equals("red")) {
        byteTwo = 0;
        currentColor = 0;
      }
      else if(bc_color.equals("green")) {
        byteTwo = 1;
        currentColor = 1;
      }
      else if(bc_color.equals("blue")) {
        byteTwo = 2;
        currentColor = 2;
      }
      else if(bc_color.equals("yellow")) {
        byteTwo = 3;
        currentColor = 3;
      }
      else if(bc_color.equals("cyan")) {
        byteTwo = 4;
        currentColor = 4;
      }
      else if(bc_color.equals("magenta")) {
        byteTwo = 5;
        currentColor = 5;
      }
      else if(bc_color.equals("white")) {
        byteTwo = 6;
        currentColor = 6;
      }
      else if(bc_color.equals("black")) {
        byteTwo = 7;
        currentColor = 7;
      }
  }
  if(data.has("bubble_column_enable")) {
    new_bc_enable = (0 == data.getInt("bubble_column_enable") ? 0 : 1);
    byteThree |= (new_bc_enable << 4);
    bc_enable = new_bc_enable;
  }
  if(data.has("aroma_diffuser_enable")) {
    new_aroma_enable = (0==data.getInt("aroma_diffuser_enable") ? 0 : 1);
    byteThree |= (new_aroma_enable << 2);
    aroma_enable = new_aroma_enable;
  }
  
  if (data.has("music_player_enable")) {
    desired_music_enable = (0==data.getInt("music_player_enable") ? 0 : 1);
  }
  
  if(data.has("music_player_genre")) {
    desired_music_genre = data.getString("music_player_genre");
  }
  
  update_music_player(desired_music_genre, desired_music_enable);
  
  if(data.has("projector_screen_color"))
    screen_color = data.getString("projector_screen_color");
  //if(data.has("music_visualizer_enable")) {
    //new_visualizer_enable = 1 == data.getInt("music_visualizer_enable");
    //byteThree |= 1;
  //}
  
  byteFour = 'p';
  if(ENABLE_SERIAL) {
    myPort.write((byte)byteOne);
    myPort.write((byte)byteTwo);
    myPort.write((byte)byteThree);
    myPort.write((byte)byteFour);
  }
  
  if(DEBUG_ARDUINO) {
    print(binary(byteOne) + " ");
    print(binary(byteTwo) + " ");
    print(binary(byteThree) + " ");
    println(binary(byteFour));
  }
  
  
  if(DEBUG_ARDUINO) {
    System.out.println("Done setting up and sending Arduino serial update");
    System.out.println(); 
  }
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

void draw() {
  poll_for_changes();
  //delay(4000);
}



void update_music_player(String desired_music_genre, int desired_state) {
  if (desired_music_genre == null) return;
  
  /// if the desired player is not selected, switch the music player to the desired genre
  if (!desired_music_genre.equals(music_genre)) {
    
    /// stop the current music playing
    current_player.pause();
    music_player_enable = 0;
    
    /// set the new genre's player
    if ("pumpup".equals(desired_music_genre)) {
      current_player = player_pumpup;
    }
    else if ("focus".equals(desired_music_genre)) {
      current_player = player_focus;
    }
    else if ("destress".equals(desired_music_genre)) {
      current_player = player_destress;
    }
    else if ("hangout".equals(desired_music_genre)) {
      current_player = player_hangout;
    }
    else {
      /// otherwise something weird happened, so we stay on the current player
      /// return so that we don't update the selected music genre  
      return;
    }
    music_genre = desired_music_genre;
  }
  
  /// if the desired state is true, play the music
  if (desired_state == 1 && music_player_enable == 0) {
    current_player.loop();
    music_player_enable = 1;
  }
  else if (desired_state == 0) {
    current_player.pause();
    music_player_enable = 0;
  }
}

