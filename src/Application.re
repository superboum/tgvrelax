[@bs.module] external request : 
string => ('err => 'response => 'body => unit) => 'd = "request";

Js.log("Hello, BuckleScript and Reason!");

request(
  "http://monip.org", 
  (_, _, body) => Js.log(body));
