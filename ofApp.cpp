#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(1280, 720);
	ofSetFrameRate(this->cap.get(CV_CAP_PROP_FPS));

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame, src, blend_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		cap.set(CV_CAP_PROP_POS_FRAMES, 1);
		return;
	}
	cv::resize(cap_frame, src, this->cap_size);
	cv::flip(src, src, 1);
	cv::cvtColor(src, src, CV_RGB2BGR);

	blend_frame = cv::Mat::zeros(this->frame.size(), this->frame.type());

	int gap = 0;
	int tmp_y = 0;

	for (int x = 0; x < src.cols; x++) {

		if (x % 35 == 0) {

			gap = ofMap(ofNoise(x * 0.005, ofGetFrameNum() * 0.005), 0, 1, -80, 80);
		}

		for (int y = 0; y < src.rows; y++) {

			tmp_y = y + gap;

			if (tmp_y >= 0 && tmp_y < src.rows) {

				blend_frame.at<cv::Vec3b>(tmp_y, x) = src.at<cv::Vec3b>(y, x);
			}
		}
	}

	blend_frame.copyTo(this->frame);
	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}