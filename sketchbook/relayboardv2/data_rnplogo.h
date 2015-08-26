/* MIME TYPES */
const char MIME_png[] PROGMEM = "image/png";
const char MIME_css[] PROGMEM = "text/css";
const char MIME_javascript[] PROGMEM = "application/x-javascript";
const char MIME_html[] PROGMEM = "text/html";


/* Stylesheet */
const char DATA_css[] PROGMEM = R"=====(
body {margin: 0px; padding: 0px; font-family: "Open Sans", sans-serif; font-color: #000; font-size: 14px; background-color: #F2F2F2;}
div.header {position: absolute; left: 0px; right: 0px; top: 0px; background-color: #363636; color: #FFF; margin: 0px; padding: 0px; padding-bottom: 0px; height: 55px;}
div.header h1 {padding: 0px; margin: 0px; padding-top: 5px;}
div.header h1 a {color: #FFFFFF; text-decoration: none;}
div.header .float_left {float: left; padding-top: 5px;padding-left: 10px;}
div.header .float_left h1 {margin: 0px; float: left; padding: 0px; padding-left: 5px; padding-right: 10px; font-size: 35px; font-weight: bold;}
div.header .float_left h1 .logo1 {color: #00AEEF;}
div.header .float_left h2 {margin: 0px; float: left; padding: 0px; padding-left: 5px; padding-right: 10px; font-size: 20px; padding-top: 10px;}
div.header .float_left form {float: left; margin-top: 3px;}
div.header .float_right {float: right; margin: 0px; margin-right: 20px; padding: 0px; height: 100%;}
.mobile_menu {width: 100%; margin-top: 50px; font-size: 30px; text-align: center; }
.mobile_menu a {display: block; width: 100%; color: #FFFFFF; height: 40px; background-color: #363636; text-decoration: none; }
.mobile_menu a:hover {background-color: #00AEEF;}
div.header .float_right a {display: inline-block; color: #FFF; text-decoration: none; padding-bottom: 0px; padding-top: 20px; padding-left: 10px; padding-right: 10px; margin: 0px; height: 35px;}
div.header .float_right a:hover {background-color: #00AEEF;}
.main{width: 600px; margin: 0 auto; margin-top: 100px; padding: 15px; border-color:#000000; border-width: 1px; border-style: solid; background-color: #FFFFFF;}
.main table { border-collapse: true; border-width: 0px; }
.main td { border-collapse: true; border-width: 1px; border-style: solid; border-color: #CCCCCC;}
.home a { display: block; height: 20px; width: auto; text-decoration: none; background-color: #363636; color: #FFFFFF; padding: 5px; }
.home a:hover { background-color: #00AEEF; }
.row {width: 100%; height: 40px; font-size: 15px; margin: 0px; }
.row .left { float: left; padding-top: 7px; }
.row .right {float: right; }
hr{ color: #000; background-color: #000; height: 1px; border: none; width: 100%;}
.center { float: center; width: 100%; text-align: center;}
.center table {text-align: left; width: 150px; margin-left: 75px;}
@media (min-width: 500px) {.home{ width: 300px; margin: 0 auto; padding: 15px; border-color:#DADADA; border-width: 1px; border-style: solid; background-color: #FFFFFF;} .wide{ width: 500px;}}
@media (max-width: 499px) {.home{ left: 3px; right: 3px; margin: 0px; padding: 0px; border-color:#DADADA; border-width: 1px; border-style: solid; background-color: #FFFFFF;}} 
@media (max-width: 699px) {div.header .float_right { visibility: hidden;}}
@media (min-width: 700px) {.mobile_menu {visibility: hidden; height: 0px;}}
)=====";
/*
const char DATA_logo[] PROGMEM = { //PNG
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, //Logo
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x29,
  0x08, 0x03, 0x00, 0x00, 0x00, 0x40, 0xf0, 0x8a, 0xdb, 0x00, 0x00, 0x00,
  0x03, 0x73, 0x42, 0x49, 0x54, 0x08, 0x08, 0x08, 0xdb, 0xe1, 0x4f, 0xe0,
  0x00, 0x00, 0x03, 0x00, 0x50, 0x4c, 0x54, 0x45, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x33, 0x00, 0x00, 0x66, 0x00, 0x00, 0x99, 0x00, 0x00, 0xcc, 0x00,
  0x00, 0xff, 0x00, 0x33, 0x00, 0x00, 0x33, 0x33, 0x00, 0x33, 0x66, 0x00,
  0x33, 0x99, 0x00, 0x33, 0xcc, 0x00, 0x33, 0xff, 0x00, 0x66, 0x00, 0x00,
  0x66, 0x33, 0x00, 0x66, 0x66, 0x00, 0x66, 0x99, 0x00, 0x66, 0xcc, 0x00,
  0x66, 0xff, 0x00, 0x99, 0x00, 0x00, 0x99, 0x33, 0x00, 0x99, 0x66, 0x00,
  0x99, 0x99, 0x00, 0x99, 0xcc, 0x00, 0x99, 0xff, 0x00, 0xcc, 0x00, 0x00,
  0xcc, 0x33, 0x00, 0xcc, 0x66, 0x00, 0xcc, 0x99, 0x00, 0xcc, 0xcc, 0x00,
  0xcc, 0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x33, 0x00, 0xff, 0x66, 0x00,
  0xff, 0x99, 0x00, 0xff, 0xcc, 0x00, 0xff, 0xff, 0x33, 0x00, 0x00, 0x33,
  0x00, 0x33, 0x33, 0x00, 0x66, 0x33, 0x00, 0x99, 0x33, 0x00, 0xcc, 0x33,
  0x00, 0xff, 0x33, 0x33, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x66, 0x33,
  0x33, 0x99, 0x33, 0x33, 0xcc, 0x33, 0x33, 0xff, 0x33, 0x66, 0x00, 0x33,
  0x66, 0x33, 0x33, 0x66, 0x66, 0x33, 0x66, 0x99, 0x33, 0x66, 0xcc, 0x33,
  0x66, 0xff, 0x33, 0x99, 0x00, 0x33, 0x99, 0x33, 0x33, 0x99, 0x66, 0x33,
  0x99, 0x99, 0x33, 0x99, 0xcc, 0x33, 0x99, 0xff, 0x33, 0xcc, 0x00, 0x33,
  0xcc, 0x33, 0x33, 0xcc, 0x66, 0x33, 0xcc, 0x99, 0x33, 0xcc, 0xcc, 0x33,
  0xcc, 0xff, 0x33, 0xff, 0x00, 0x33, 0xff, 0x33, 0x33, 0xff, 0x66, 0x33,
  0xff, 0x99, 0x33, 0xff, 0xcc, 0x33, 0xff, 0xff, 0x66, 0x00, 0x00, 0x66,
  0x00, 0x33, 0x66, 0x00, 0x66, 0x66, 0x00, 0x99, 0x66, 0x00, 0xcc, 0x66,
  0x00, 0xff, 0x66, 0x33, 0x00, 0x66, 0x33, 0x33, 0x66, 0x33, 0x66, 0x66,
  0x33, 0x99, 0x66, 0x33, 0xcc, 0x66, 0x33, 0xff, 0x66, 0x66, 0x00, 0x66,
  0x66, 0x33, 0x66, 0x66, 0x66, 0x66, 0x66, 0x99, 0x66, 0x66, 0xcc, 0x66,
  0x66, 0xff, 0x66, 0x99, 0x00, 0x66, 0x99, 0x33, 0x66, 0x99, 0x66, 0x66,
  0x99, 0x99, 0x66, 0x99, 0xcc, 0x66, 0x99, 0xff, 0x66, 0xcc, 0x00, 0x66,
  0xcc, 0x33, 0x66, 0xcc, 0x66, 0x66, 0xcc, 0x99, 0x66, 0xcc, 0xcc, 0x66,
  0xcc, 0xff, 0x66, 0xff, 0x00, 0x66, 0xff, 0x33, 0x66, 0xff, 0x66, 0x66,
  0xff, 0x99, 0x66, 0xff, 0xcc, 0x66, 0xff, 0xff, 0x99, 0x00, 0x00, 0x99,
  0x00, 0x33, 0x99, 0x00, 0x66, 0x99, 0x00, 0x99, 0x99, 0x00, 0xcc, 0x99,
  0x00, 0xff, 0x99, 0x33, 0x00, 0x99, 0x33, 0x33, 0x99, 0x33, 0x66, 0x99,
  0x33, 0x99, 0x99, 0x33, 0xcc, 0x99, 0x33, 0xff, 0x99, 0x66, 0x00, 0x99,
  0x66, 0x33, 0x99, 0x66, 0x66, 0x99, 0x66, 0x99, 0x99, 0x66, 0xcc, 0x99,
  0x66, 0xff, 0x99, 0x99, 0x00, 0x99, 0x99, 0x33, 0x99, 0x99, 0x66, 0x99,
  0x99, 0x99, 0x99, 0x99, 0xcc, 0x99, 0x99, 0xff, 0x99, 0xcc, 0x00, 0x99,
  0xcc, 0x33, 0x99, 0xcc, 0x66, 0x99, 0xcc, 0x99, 0x99, 0xcc, 0xcc, 0x99,
  0xcc, 0xff, 0x99, 0xff, 0x00, 0x99, 0xff, 0x33, 0x99, 0xff, 0x66, 0x99,
  0xff, 0x99, 0x99, 0xff, 0xcc, 0x99, 0xff, 0xff, 0xcc, 0x00, 0x00, 0xcc,
  0x00, 0x33, 0xcc, 0x00, 0x66, 0xcc, 0x00, 0x99, 0xcc, 0x00, 0xcc, 0xcc,
  0x00, 0xff, 0xcc, 0x33, 0x00, 0xcc, 0x33, 0x33, 0xcc, 0x33, 0x66, 0xcc,
  0x33, 0x99, 0xcc, 0x33, 0xcc, 0xcc, 0x33, 0xff, 0xcc, 0x66, 0x00, 0xcc,
  0x66, 0x33, 0xcc, 0x66, 0x66, 0xcc, 0x66, 0x99, 0xcc, 0x66, 0xcc, 0xcc,
  0x66, 0xff, 0xcc, 0x99, 0x00, 0xcc, 0x99, 0x33, 0xcc, 0x99, 0x66, 0xcc,
  0x99, 0x99, 0xcc, 0x99, 0xcc, 0xcc, 0x99, 0xff, 0xcc, 0xcc, 0x00, 0xcc,
  0xcc, 0x33, 0xcc, 0xcc, 0x66, 0xcc, 0xcc, 0x99, 0xcc, 0xcc, 0xcc, 0xcc,
  0xcc, 0xff, 0xcc, 0xff, 0x00, 0xcc, 0xff, 0x33, 0xcc, 0xff, 0x66, 0xcc,
  0xff, 0x99, 0xcc, 0xff, 0xcc, 0xcc, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff,
  0x00, 0x33, 0xff, 0x00, 0x66, 0xff, 0x00, 0x99, 0xff, 0x00, 0xcc, 0xff,
  0x00, 0xff, 0xff, 0x33, 0x00, 0xff, 0x33, 0x33, 0xff, 0x33, 0x66, 0xff,
  0x33, 0x99, 0xff, 0x33, 0xcc, 0xff, 0x33, 0xff, 0xff, 0x66, 0x00, 0xff,
  0x66, 0x33, 0xff, 0x66, 0x66, 0xff, 0x66, 0x99, 0xff, 0x66, 0xcc, 0xff,
  0x66, 0xff, 0xff, 0x99, 0x00, 0xff, 0x99, 0x33, 0xff, 0x99, 0x66, 0xff,
  0x99, 0x99, 0xff, 0x99, 0xcc, 0xff, 0x99, 0xff, 0xff, 0xcc, 0x00, 0xff,
  0xcc, 0x33, 0xff, 0xcc, 0x66, 0xff, 0xcc, 0x99, 0xff, 0xcc, 0xcc, 0xff,
  0xcc, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x33, 0xff, 0xff, 0x66, 0xff,
  0xff, 0x99, 0xff, 0xff, 0xcc, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x7a, 0x7c, 0x1d,
  0x00, 0x00, 0x01, 0x00, 0x74, 0x52, 0x4e, 0x53, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1b, 0xc9, 0x99, 0x6a, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73,
  0x00, 0x00, 0x0e, 0xc4, 0x00, 0x00, 0x0e, 0xc4, 0x01, 0x95, 0x2b, 0x0e,
  0x1b, 0x00, 0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6f, 0x66,
  0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x67, 0x6e, 0x6f, 0x6d, 0x65, 0x2d,
  0x73, 0x63, 0x72, 0x65, 0x65, 0x6e, 0x73, 0x68, 0x6f, 0x74, 0xef, 0x03,
  0xbf, 0x3e, 0x00, 0x00, 0x01, 0x52, 0x49, 0x44, 0x41, 0x54, 0x48, 0x89,
  0x8d, 0xd5, 0xdd, 0xb5, 0x83, 0x20, 0x0c, 0x00, 0x60, 0x36, 0xf0, 0xc9,
  0xb8, 0x07, 0xee, 0xc2, 0x3e, 0xec, 0xc3, 0x3e, 0x64, 0x9c, 0x6b, 0x40,
  0x9a, 0x1f, 0x08, 0x5e, 0xce, 0xe9, 0x43, 0x21, 0x9f, 0x84, 0x90, 0xd6,
  0x80, 0xde, 0x00, 0x6f, 0x21, 0xb8, 0xe2, 0xf2, 0x8c, 0x47, 0xe0, 0xba,
  0x3c, 0xe3, 0x10, 0x12, 0x9e, 0x59, 0x93, 0xb6, 0x87, 0x67, 0x96, 0xa4,
  0x47, 0x7b, 0x66, 0x45, 0x46, 0xac, 0x63, 0x16, 0x84, 0x23, 0xd7, 0x66,
  0x26, 0x32, 0x8e, 0x6a, 0x70, 0x7e, 0x12, 0xfd, 0xe4, 0x95, 0xb1, 0xc4,
  0xe6, 0x42, 0xe6, 0xd8, 0x92, 0x29, 0xfb, 0x38, 0x9b, 0xb0, 0x17, 0xaf,
  0xb9, 0x67, 0xd2, 0x2e, 0x1b, 0xee, 0x65, 0x85, 0x26, 0x43, 0xe4, 0xbc,
  0x78, 0x2c, 0xee, 0xc1, 0x9a, 0x30, 0xfa, 0xc9, 0x15, 0x88, 0xb7, 0x36,
  0x01, 0x8f, 0x76, 0xc0, 0x88, 0xeb, 0xb4, 0x30, 0x0d, 0x13, 0x99, 0x70,
  0x20, 0xd8, 0x83, 0x3e, 0xa3, 0xd6, 0xc9, 0x04, 0xfa, 0x36, 0xd6, 0x61,
  0xda, 0xa6, 0x76, 0xd2, 0x53, 0x79, 0x4d, 0x90, 0x61, 0x92, 0x2b, 0xd1,
  0x2b, 0x84, 0x4c, 0xb8, 0x23, 0x0c, 0xf9, 0x89, 0x43, 0x16, 0x40, 0x91,
  0xa8, 0x89, 0x16, 0x7c, 0xfc, 0x53, 0x24, 0x76, 0x28, 0xa2, 0x05, 0xcf,
  0x87, 0x28, 0xb6, 0x54, 0x55, 0x9e, 0x44, 0x2a, 0xb5, 0x66, 0x22, 0xa2,
  0xb4, 0x20, 0x1f, 0x36, 0x65, 0x95, 0x6b, 0x1b, 0xe9, 0x21, 0xad, 0x1f,
  0x00, 0x4e, 0x50, 0xbf, 0x8d, 0x32, 0xc4, 0xcd, 0xa2, 0x64, 0x62, 0xd4,
  0x30, 0xf7, 0xa2, 0x5f, 0x6a, 0xcf, 0x41, 0x5c, 0xe2, 0x23, 0xfa, 0x42,
  0xa6, 0xb6, 0x8c, 0xa3, 0xcb, 0x0e, 0x5f, 0x3c, 0x53, 0x6d, 0x22, 0xd7,
  0xd2, 0x9b, 0x3f, 0x9e, 0x00, 0xc0, 0xbd, 0x92, 0xc6, 0x41, 0x44, 0xa3,
  0x58, 0x62, 0xc7, 0x6b, 0x64, 0x6b, 0xbd, 0xfb, 0x56, 0x8f, 0x0c, 0x03,
  0x7c, 0x83, 0xcf, 0xc9, 0x73, 0xa2, 0x3a, 0x27, 0xef, 0x6f, 0x3c, 0xf5,
  0xe3, 0x70, 0xb6, 0xf5, 0x1d, 0xd9, 0x7f, 0x59, 0xa4, 0x5f, 0x09, 0x86,
  0x28, 0xf4, 0x49, 0x9b, 0xf7, 0x8b, 0x28, 0x1b, 0x8a, 0x8b, 0xc5, 0x2d,
  0x11, 0x46, 0x8a, 0x2d, 0x69, 0xd9, 0x4c, 0x62, 0x4f, 0xa8, 0x6d, 0x8a,
  0x15, 0x1f, 0xa4, 0x19, 0x23, 0xbe, 0x08, 0x19, 0x23, 0x3e, 0x09, 0x16,
  0x55, 0xeb, 0x7f, 0x11, 0x2c, 0x46, 0xe0, 0x1f, 0x40, 0x6d, 0x94, 0xb4,
  0x98, 0xd7, 0xe0, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44,
  0xae, 0x42, 0x60, 0x82 };
*//*
const char DATA_switchery[] PROGMEM = R"=====(
(function(){function require(name){var module=require.modules[name];if(!module)throw new Error('failed to require "'+name+'"');if(!("exports"in module)&&typeof module.definition==="function"){module.client=module.component=true;module.definition.call(this,module.exports={},module);delete module.definition}return module.exports}require.loader="component";require.helper={};require.helper.semVerSort=function(a,b){var aArray=a.version.split(".");var bArray=b.version.split(".");for(var i=0;i<aArray.length;++i){var aInt=parseInt(aArray[i],10);var bInt=parseInt(bArray[i],10);if(aInt===bInt){var aLex=aArray[i].substr((""+aInt).length);var bLex=bArray[i].substr((""+bInt).length);if(aLex===""&&bLex!=="")return 1;if(aLex!==""&&bLex==="")return-1;if(aLex!==""&&bLex!=="")return aLex>bLex?1:-1;continue}else if(aInt>bInt){return 1}else{return-1}}return 0};require.latest=function(name,returnPath){function showError(name){throw new Error('failed to find latest module of "'+name+'"')}var versionRegexp=/(.*)~(.*)@v?(\d+\.\d+\.\d+[^\/]*)$/;var remoteRegexp=/(.*)~(.*)/;if(!remoteRegexp.test(name))showError(name);var moduleNames=Object.keys(require.modules);var semVerCandidates=[];var otherCandidates=[];for(var i=0;i<moduleNames.length;i++){var moduleName=moduleNames[i];if(new RegExp(name+"@").test(moduleName)){var version=moduleName.substr(name.length+1);var semVerMatch=versionRegexp.exec(moduleName);if(semVerMatch!=null){semVerCandidates.push({version:version,name:moduleName})}else{otherCandidates.push({version:version,name:moduleName})}}}if(semVerCandidates.concat(otherCandidates).length===0){showError(name)}if(semVerCandidates.length>0){var module=semVerCandidates.sort(require.helper.semVerSort).pop().name;if(returnPath===true){return module}return require(module)}var module=otherCandidates.pop().name;if(returnPath===true){return module}return require(module)};require.modules={};require.register=function(name,definition){require.modules[name]={definition:definition}};require.define=function(name,exports){require.modules[name]={exports:exports}};require.register("abpetkov~transitionize@0.0.3",function(exports,module){module.exports=Transitionize;function Transitionize(element,props){if(!(this instanceof Transitionize))return new Transitionize(element,props);this.element=element;this.props=props||{};this.init()}Transitionize.prototype.isSafari=function(){return/Safari/.test(navigator.userAgent)&&/Apple Computer/.test(navigator.vendor)};Transitionize.prototype.init=function(){var transitions=[];for(var key in this.props){transitions.push(key+" "+this.props[key])}this.element.style.transition=transitions.join(", ");if(this.isSafari())this.element.style.webkitTransition=transitions.join(", ")}});require.register("ftlabs~fastclick@v0.6.11",function(exports,module){function FastClick(layer){"use strict";var oldOnClick,self=this;this.trackingClick=false;this.trackingClickStart=0;this.targetElement=null;this.touchStartX=0;this.touchStartY=0;this.lastTouchIdentifier=0;this.touchBoundary=10;this.layer=layer;if(!layer||!layer.nodeType){throw new TypeError("Layer must be a document node")}this.onClick=function(){return FastClick.prototype.onClick.apply(self,arguments)};this.onMouse=function(){return FastClick.prototype.onMouse.apply(self,arguments)};this.onTouchStart=function(){return FastClick.prototype.onTouchStart.apply(self,arguments)};this.onTouchMove=function(){return FastClick.prototype.onTouchMove.apply(self,arguments)};this.onTouchEnd=function(){return FastClick.prototype.onTouchEnd.apply(self,arguments)};this.onTouchCancel=function(){return FastClick.prototype.onTouchCancel.apply(self,arguments)};if(FastClick.notNeeded(layer)){return}if(this.deviceIsAndroid){layer.addEventListener("mouseover",this.onMouse,true);layer.addEventListener("mousedown",this.onMouse,true);layer.addEventListener("mouseup",this.onMouse,true)}layer.addEventListener("click",this.onClick,true);layer.addEventListener("touchstart",this.onTouchStart,false);layer.addEventListener("touchmove",this.onTouchMove,false);layer.addEventListener("touchend",this.onTouchEnd,false);layer.addEventListener("touchcancel",this.onTouchCancel,false);if(!Event.prototype.stopImmediatePropagation){layer.removeEventListener=function(type,callback,capture){var rmv=Node.prototype.removeEventListener;if(type==="click"){rmv.call(layer,type,callback.hijacked||callback,capture)}else{rmv.call(layer,type,callback,capture)}};layer.addEventListener=function(type,callback,capture){var adv=Node.prototype.addEventListener;if(type==="click"){adv.call(layer,type,callback.hijacked||(callback.hijacked=function(event){if(!event.propagationStopped){callback(event)}}),capture)}else{adv.call(layer,type,callback,capture)}}}if(typeof layer.onclick==="function"){oldOnClick=layer.onclick;layer.addEventListener("click",function(event){oldOnClick(event)},false);layer.onclick=null}}FastClick.prototype.deviceIsAndroid=navigator.userAgent.indexOf("Android")>0;FastClick.prototype.deviceIsIOS=/iP(ad|hone|od)/.test(navigator.userAgent);FastClick.prototype.deviceIsIOS4=FastClick.prototype.deviceIsIOS&&/OS 4_\d(_\d)?/.test(navigator.userAgent);FastClick.prototype.deviceIsIOSWithBadTarget=FastClick.prototype.deviceIsIOS&&/OS ([6-9]|\d{2})_\d/.test(navigator.userAgent);FastClick.prototype.needsClick=function(target){"use strict";switch(target.nodeName.toLowerCase()){case"button":case"select":case"textarea":if(target.disabled){return true}break;case"input":if(this.deviceIsIOS&&target.type==="file"||target.disabled){return true}break;case"label":case"video":return true}return/\bneedsclick\b/.test(target.className)};FastClick.prototype.needsFocus=function(target){"use strict";switch(target.nodeName.toLowerCase()){case"textarea":return true;case"select":return!this.deviceIsAndroid;case"input":switch(target.type){case"button":case"checkbox":case"file":case"image":case"radio":case"submit":return false}return!target.disabled&&!target.readOnly;default:return/\bneedsfocus\b/.test(target.className)}};FastClick.prototype.sendClick=function(targetElement,event){"use strict";var clickEvent,touch;if(document.activeElement&&document.activeElement!==targetElement){document.activeElement.blur()}touch=event.changedTouches[0];clickEvent=document.createEvent("MouseEvents");clickEvent.initMouseEvent(this.determineEventType(targetElement),true,true,window,1,touch.screenX,touch.screenY,touch.clientX,touch.clientY,false,false,false,false,0,null);clickEvent.forwardedTouchEvent=true;targetElement.dispatchEvent(clickEvent)};FastClick.prototype.determineEventType=function(targetElement){"use strict";if(this.deviceIsAndroid&&targetElement.tagName.toLowerCase()==="select"){return"mousedown"}return"click"};FastClick.prototype.focus=function(targetElement){"use strict";var length;if(this.deviceIsIOS&&targetElement.setSelectionRange&&targetElement.type.indexOf("date")!==0&&targetElement.type!=="time"){length=targetElement.value.length;targetElement.setSelectionRange(length,length)}else{targetElement.focus()}};FastClick.prototype.updateScrollParent=function(targetElement){"use strict";var scrollParent,parentElement;scrollParent=targetElement.fastClickScrollParent;if(!scrollParent||!scrollParent.contains(targetElement)){parentElement=targetElement;do{if(parentElement.scrollHeight>parentElement.offsetHeight){scrollParent=parentElement;targetElement.fastClickScrollParent=parentElement;break}parentElement=parentElement.parentElement}while(parentElement)}if(scrollParent){scrollParent.fastClickLastScrollTop=scrollParent.scrollTop}};FastClick.prototype.getTargetElementFromEventTarget=function(eventTarget){"use strict";if(eventTarget.nodeType===Node.TEXT_NODE){return eventTarget.parentNode}return eventTarget};FastClick.prototype.onTouchStart=function(event){"use strict";var targetElement,touch,selection;if(event.targetTouches.length>1){return true}targetElement=this.getTargetElementFromEventTarget(event.target);touch=event.targetTouches[0];if(this.deviceIsIOS){selection=window.getSelection();if(selection.rangeCount&&!selection.isCollapsed){return true}if(!this.deviceIsIOS4){if(touch.identifier===this.lastTouchIdentifier){event.preventDefault();return false}this.lastTouchIdentifier=touch.identifier;this.updateScrollParent(targetElement)}}this.trackingClick=true;this.trackingClickStart=event.timeStamp;this.targetElement=targetElement;this.touchStartX=touch.pageX;this.touchStartY=touch.pageY;if(event.timeStamp-this.lastClickTime<200){event.preventDefault()}return true};FastClick.prototype.touchHasMoved=function(event){"use strict";var touch=event.changedTouches[0],boundary=this.touchBoundary;if(Math.abs(touch.pageX-this.touchStartX)>boundary||Math.abs(touch.pageY-this.touchStartY)>boundary){return true}return false};FastClick.prototype.onTouchMove=function(event){"use strict";if(!this.trackingClick){return true}if(this.targetElement!==this.getTargetElementFromEventTarget(event.target)||this.touchHasMoved(event)){this.trackingClick=false;this.targetElement=null}return true};FastClick.prototype.findControl=function(labelElement){"use strict";if(labelElement.control!==undefined){return labelElement.control}if(labelElement.htmlFor){return document.getElementById(labelElement.htmlFor)}return labelElement.querySelector("button, input:not([type=hidden]), keygen, meter, output, progress, select, textarea")};FastClick.prototype.onTouchEnd=function(event){"use strict";var forElement,trackingClickStart,targetTagName,scrollParent,touch,targetElement=this.targetElement;if(!this.trackingClick){return true}if(event.timeStamp-this.lastClickTime<200){this.cancelNextClick=true;return true}this.cancelNextClick=false;this.lastClickTime=event.timeStamp;trackingClickStart=this.trackingClickStart;this.trackingClick=false;this.trackingClickStart=0;if(this.deviceIsIOSWithBadTarget){touch=event.changedTouches[0];targetElement=document.elementFromPoint(touch.pageX-window.pageXOffset,touch.pageY-window.pageYOffset)||targetElement;targetElement.fastClickScrollParent=this.targetElement.fastClickScrollParent}targetTagName=targetElement.tagName.toLowerCase();if(targetTagName==="label"){forElement=this.findControl(targetElement);if(forElement){this.focus(targetElement);if(this.deviceIsAndroid){return false}targetElement=forElement}}else if(this.needsFocus(targetElement)){if(event.timeStamp-trackingClickStart>100||this.deviceIsIOS&&window.top!==window&&targetTagName==="input"){this.targetElement=null;return false}this.focus(targetElement);if(!this.deviceIsIOS4||targetTagName!=="select"){this.targetElement=null;event.preventDefault()}return false}if(this.deviceIsIOS&&!this.deviceIsIOS4){scrollParent=targetElement.fastClickScrollParent;if(scrollParent&&scrollParent.fastClickLastScrollTop!==scrollParent.scrollTop){return true}}if(!this.needsClick(targetElement)){event.preventDefault();this.sendClick(targetElement,event)}return false};FastClick.prototype.onTouchCancel=function(){"use strict";this.trackingClick=false;this.targetElement=null};FastClick.prototype.onMouse=function(event){"use strict";if(!this.targetElement){return true}if(event.forwardedTouchEvent){return true}if(!event.cancelable){return true}if(!this.needsClick(this.targetElement)||this.cancelNextClick){if(event.stopImmediatePropagation){event.stopImmediatePropagation()}else{event.propagationStopped=true}event.stopPropagation();event.preventDefault();return false}return true};FastClick.prototype.onClick=function(event){"use strict";var permitted;if(this.trackingClick){this.targetElement=null;this.trackingClick=false;return true}if(event.target.type==="submit"&&event.detail===0){return true}permitted=this.onMouse(event);if(!permitted){this.targetElement=null}return permitted};FastClick.prototype.destroy=function(){"use strict";var layer=this.layer;if(this.deviceIsAndroid){layer.removeEventListener("mouseover",this.onMouse,true);layer.removeEventListener("mousedown",this.onMouse,true);layer.removeEventListener("mouseup",this.onMouse,true)}layer.removeEventListener("click",this.onClick,true);layer.removeEventListener("touchstart",this.onTouchStart,false);layer.removeEventListener("touchmove",this.onTouchMove,false);layer.removeEventListener("touchend",this.onTouchEnd,false);layer.removeEventListener("touchcancel",this.onTouchCancel,false)};FastClick.notNeeded=function(layer){"use strict";var metaViewport;var chromeVersion;if(typeof window.ontouchstart==="undefined"){return true}chromeVersion=+(/Chrome\/([0-9]+)/.exec(navigator.userAgent)||[,0])[1];if(chromeVersion){if(FastClick.prototype.deviceIsAndroid){metaViewport=document.querySelector("meta[name=viewport]");if(metaViewport){if(metaViewport.content.indexOf("user-scalable=no")!==-1){return true}if(chromeVersion>31&&window.innerWidth<=window.screen.width){return true}}}else{return true}}if(layer.style.msTouchAction==="none"){return true}return false};FastClick.attach=function(layer){"use strict";return new FastClick(layer)};if(typeof define!=="undefined"&&define.amd){define(function(){"use strict";return FastClick})}else if(typeof module!=="undefined"&&module.exports){module.exports=FastClick.attach;module.exports.FastClick=FastClick}else{window.FastClick=FastClick}});require.register("switchery",function(exports,module){var transitionize=require("abpetkov~transitionize@0.0.3"),fastclick=require("ftlabs~fastclick@v0.6.11");module.exports=Switchery;var defaults={color:"#64bd63",secondaryColor:"#dfdfdf",jackColor:"#fff",className:"switchery",disabled:false,disabledOpacity:.5,speed:"0.4s"};function Switchery(element,options){if(!(this instanceof Switchery))return new Switchery(element,options);this.element=element;this.options=options||{};for(var i in defaults){if(this.options[i]==null){this.options[i]=defaults[i]}}if(this.element!=null&&this.element.type=="checkbox")this.init()}Switchery.prototype.hide=function(){this.element.style.display="none"};Switchery.prototype.show=function(){var switcher=this.create();this.insertAfter(this.element,switcher)};Switchery.prototype.create=function(){this.switcher=document.createElement("span");this.jack=document.createElement("small");this.switcher.appendChild(this.jack);this.switcher.className=this.options.className;return this.switcher};Switchery.prototype.insertAfter=function(reference,target){reference.parentNode.insertBefore(target,reference.nextSibling)};Switchery.prototype.isChecked=function(){return this.element.checked};Switchery.prototype.isDisabled=function(){return this.options.disabled||this.element.disabled};Switchery.prototype.setPosition=function(clicked){var checked=this.isChecked(),switcher=this.switcher,jack=this.jack;if(clicked&&checked)checked=false;else if(clicked&&!checked)checked=true;if(checked===true){this.element.checked=true;if(window.getComputedStyle)jack.style.left=parseInt(window.getComputedStyle(switcher).width)-parseInt(window.getComputedStyle(jack).width)+"px";else jack.style.left=parseInt(switcher.currentStyle["width"])-parseInt(jack.currentStyle["width"])+"px";if(this.options.color)this.colorize();this.setSpeed()}else{jack.style.left=0;this.element.checked=false;this.switcher.style.boxShadow="inset 0 0 0 0 "+this.options.secondaryColor;this.switcher.style.borderColor=this.options.secondaryColor;this.switcher.style.backgroundColor=this.options.secondaryColor!==defaults.secondaryColor?this.options.secondaryColor:"#fff";this.jack.style.backgroundColor=this.options.jackColor;this.setSpeed()}};Switchery.prototype.setSpeed=function(){var switcherProp={},jackProp={left:this.options.speed.replace(/[a-z]/,"")/2+"s"};if(this.isChecked()){switcherProp={border:this.options.speed,"box-shadow":this.options.speed,"background-color":this.options.speed.replace(/[a-z]/,"")*3+"s"}}else{switcherProp={border:this.options.speed,"box-shadow":this.options.speed}}transitionize(this.switcher,switcherProp);transitionize(this.jack,jackProp)};Switchery.prototype.colorize=function(){this.switcher.style.backgroundColor=this.options.color;this.switcher.style.borderColor=this.options.color;this.switcher.style.boxShadow="inset 0 0 0 16px "+this.options.color;this.jack.style.backgroundColor=this.options.jackColor};Switchery.prototype.handleOnchange=function(state){if(document.dispatchEvent){var event=document.createEvent("HTMLEvents");event.initEvent("change",true,true);this.element.dispatchEvent(event)}else{this.element.fireEvent("onchange")}};Switchery.prototype.handleChange=function(){var self=this,el=this.element;if(el.addEventListener){el.addEventListener("change",function(){self.setPosition()})}else{el.attachEvent("onchange",function(){self.setPosition()})}};Switchery.prototype.handleClick=function(){var self=this,switcher=this.switcher,parent=self.element.parentNode.tagName.toLowerCase(),labelParent=parent==="label"?false:true;if(this.isDisabled()===false){fastclick(switcher);if(switcher.addEventListener){switcher.addEventListener("click",function(){self.setPosition(labelParent);self.handleOnchange(self.element.checked)})}else{switcher.attachEvent("onclick",function(){self.setPosition(labelParent);self.handleOnchange(self.element.checked)})}}else{this.element.disabled=true;this.switcher.style.opacity=this.options.disabledOpacity}};Switchery.prototype.markAsSwitched=function(){this.element.setAttribute("data-switchery",true)};Switchery.prototype.markedAsSwitched=function(){return this.element.getAttribute("data-switchery")};Switchery.prototype.init=function(){this.hide();this.show();this.setPosition();this.markAsSwitched();this.handleChange();this.handleClick()}});if(typeof exports=="object"){module.exports=require("switchery")}else if(typeof define=="function"&&define.amd){define("Switchery",[],function(){return require("switchery")})}else{(this||window)["Switchery"]=require("switchery")}})();
)=====";
*/
const char DATA_upgrade[] PROGMEM = R"=====(
<form action="upgrade_do" method="post" enctype="multipart/form-data">
    Select image to upload:
    <input type="file" name="firmware" id="firmware">
    <input type="submit" value="Flash firmware" name="submit">
</form>
)=====";
