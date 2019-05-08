#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid_s;
  PID pid_t;
  /**
   * TODO: Initialize the pid_s variable.
   */
  //pid_s.Init(0.134611, 0.000270736, 3.05349);
  //pid_s.Init(0.2, 0.000270736, 3.05349);
  //pid_t.Init(0.316731, 0.0000, 0.0226185);

  //pid_s.Init(0.3345, 0.0011011, 2.662); //your init parameters
  
  //pid_s.Init(0.134611, 0.000270736, 3.05349); //OK
  //pid_t.Init(0.316731, 0.0000, 0.0226185);
#if 0
  pid_t.Init(0.2, 0.0000, 0.01);
#endif
  
  //pid_s.Init(0.3345, 0.0011011, 2.662); // OK
  // pid_s.Init(0.15, 0.0011011, 2.662);  // OK
  //pid_s.Init(0.15, 0.0003, 1.35);   // OK
  //pid_s.Init(0.2, 0.0003, 1.35);   // OK
  //pid_s.Init(0.25, 0.0003, 1.35);   // OK
  //pid_s.Init(0.25, 0.0003, 2);   // OK
  pid_s.Init(0.25, 0.0003, 2.5);   // OK
  

  h.onMessage([&pid_s, &pid_t](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          double throttle_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          ///////////////////////////////////////////////////////////////////////

          pid_s.UpdateError(cte);
          //steer_value = -pid_s.Kp * pid_s.p_error - pid_s.Kd * pid_s.d_error - pid_s.Ki * pid_s.i_error;
          steer_value = pid_s.TotalError();
            
#if 0          
          pid_t.UpdateError(fabs(cte));     // |cte|
          //pid_t.UpdateError(pow(cte, 2));   // cte^2
          //throttle_value = 0.75 - pid_t.Kp * pid_t.p_error - pid_t.Kd * pid_t.d_error  - pid_t.Ki * pid_t.i_error;
          throttle_value = 0.75 + pid_t.TotalError();
#endif          
          ///////////////////////////////////////////////////////////////////////
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    << ", Throttle Value: " << throttle_value
                    << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3; //dthrottle_value; //0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}