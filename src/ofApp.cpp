#include "ofApp.h"

bool mapView   =false;
bool clickPos  =false;
double lat,lon,zoom,szoom;
int W,H;
ofVec2f XPosition;
ofFbo fbo;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    W=ofGetScreenWidth();
    H=ofGetScreenHeight();

    lat=29.4880;
    lon=-98.3987;
    zoom=12;
    szoom=4;

    //streetview
    streetview.setLatLon(lat,lon);
    streetview.setZoom(szoom);
    texture = streetview.getTextureReference();

    //openstreetmap
    map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
    map.setGeoLocationCenter(GeoLocation(lat,lon));
    map.setZoom(zoom);

    //fbo pointcloud 
    fbo.allocate(W/2,H,GL_RGBA);

    //gui
    gui = new ofxUISuperCanvas("TraceRoute", 5, 280, 240, 100, OFX_UI_FONT_SMALL);
    textInput = gui->addTextInput("ip", "openframeworks.cc");
    textInput->setAutoUnfocus(true);
    gui->addButton("search",    false);
    gui->addButton("fullscreen",false);

    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    cam.lookAt(ofVec3f(0.901754, 0.067921, -0.42688));
    cam.setOrientation(ofVec3f(0.295778, -1.41493, 57.1845));
    cam.setFarClip(100000);  
    cam.setDistance(75);  
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("fps: "+ofToString(ofGetFrameRate()));
    streetview.update();

    fbo.begin();
      ofClear(0,0,0,255);
      cam.begin();
         ofEnableDepthTest();
         glPointSize(4);
         streetview.draw();
         ofDisableDepthTest();
      cam.end();
    fbo.end();
}

ofVec2f mousePosition;

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(15),ofColor(200));

    map.draw();                                                    //disegno mappa
    texture.draw(0,0,texture.getWidth()/6, texture.getHeight()/6); //streetview image
    fbo.draw( (W-fbo.getWidth()) , 0 );                            //fbo pointcloud

    mousePosition.set(mouseX, mouseY);
    if(mouseX<=(W-fbo.getWidth()))
       ofDrawBitmapStringHighlight(ofToString(map.pointToGeolocation(mousePosition)), 10, H-20);

    if(clickPos){
         ofPushStyle();
         ofSetColor(5,5,255);
         ofNoFill();
         ofDrawBox(XPosition,20,20,20);
	 ofSetColor(255,5,0);
       	 ofFill();
         ofPushMatrix();
	 ofDrawCone(XPosition,3,8);
         ofPopMatrix();
         ofPopStyle();
    }
}

void ofApp::guiEvent(ofxUIEventArgs &e) {
    string name = e.getName();
    int kind = e.getKind();
    if(name == "search") {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if( button->getValue() == true ){
	        ofxUITextInput *textInput = (ofxUITextInput *) gui->getWidget("ip");
	        string value = textInput->getTextString();  
	        string t = thread.setHost(value);
	        thread.start();
		cout<<"thread-Host::"<<t<<endl;
	}
    }
    if(name == "fullscreen") {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if( button->getValue() == true ){
		ofToggleFullscreen();
        }
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	cout<<msg.message<<endl;
	string ms = msg.message;
	if(ms!=""){
	     vector<string> p = ofSplitString(ms,",");
             lat=ofToDouble(p[0]);
             lon=ofToDouble(p[1]);
	     streetview.setLatLon( lat, lon );
	     streetview.setZoom(szoom);
	     texture = streetview.getTextureReference();

	     map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
	     map.setGeoLocationCenter(GeoLocation(lat,lon));
	     map.setZoom(zoom);
	}
}

void ofApp::setCamera(){
//	cam.setPosition();
//	cam.setScale();
//	cam.setTransformMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

      if(key == '2'){
	cout<<"globorien:"<<cam.getGlobalOrientation()<<endl;
	cout<<"globpos"<<cam.getGlobalPosition()<<endl;
	cout<<"gloscale"<<cam.getGlobalScale()<<endl;
	cout<<"glomatrix"<<cam.getGlobalTransformMatrix()<<endl;
	cout<<"heading"<<cam.getHeading()<<endl;
	cout<<"LocalMatrix"<<cam.getLocalTransformMatrix()<<endl;
	cout<<"lookAt"<<cam.getLookAtDir()<<endl;
	cout<<"orienEuler"<<cam.getOrientationEuler()<<endl;
	cout<<"OrienQuat"<<cam.getOrientationQuat()<<endl;
	cout<<"parent"<<cam.getParent()<<endl;
	cout<<"pitch"<<cam.getPitch()<<endl;
	cout<<"position"<<cam.getPosition()<<endl;
	cout<<"roll"<<cam.getRoll()<<endl;
	cout<<"scale"<<cam.getScale()<<endl;
	cout<<"sidedir"<<cam.getSideDir()<<endl;
	cout<<"upDir"<<cam.getUpDir()<<endl;
	cout<<"getX"<<cam.getX()<<endl;
	cout<<"XAxis"<<cam.getXAxis()<<endl;
	cout<<"getY"<<cam.getY()<<endl;
	cout<<"YAxis"<<cam.getYAxis()<<endl;
	cout<<"getZ"<<cam.getZ()<<endl;
	cout<<"ZAxis"<<cam.getZAxis()<<endl;
      }

      if(key == '1')
	mapView=!mapView;
      if(key == OF_KEY_UP) {
         lat+=0.1;
         map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
	 map.setGeoLocationCenter(GeoLocation(lat,lon));
	 map.setZoom(zoom);
      }
      if(key == OF_KEY_DOWN) {
         lat-=0.1;
         map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
         map.setGeoLocationCenter(GeoLocation(lat,lon));
	 map.setZoom(zoom);
      }
      if(key == OF_KEY_RIGHT) {
         lon+=0.1;
         map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
	 map.setGeoLocationCenter(GeoLocation(lat,lon));
	 map.setZoom(zoom);
      }
      if(key == OF_KEY_LEFT) {
         lon-=0.1;
         map.setup(std::shared_ptr<OpenStreetMapProvider>(new OpenStreetMapProvider()), ofGetWidth(),ofGetHeight());
         map.setGeoLocationCenter(GeoLocation(lat,lon));
	 map.setZoom(zoom);
      }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(mapView && mouseX<=(W-fbo.getWidth())){
	XPosition=mousePosition;
        ofVec2d mousePosition(mouseX, mouseY);
	ofPoint p = map.pointToGeolocation(mousePosition);
	streetview.setLatLon( p.x, p.y );
	streetview.setZoom(szoom);
	texture=streetview.getTextureReference();
	clickPos=true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
