[@bs.module] external request : 
string => ('err => 'response => 'body => unit) => 'd = "request";

[@bs.deriving abstract]
type cheerio_element;

[@bs.send] external cheerio_text : cheerio_element => string = "text"; 
[@bs.send] external cheerio_attr : cheerio_element => string => string = "attr"; 
[@bs.module "cheerio"][@bs.val] external cheerio_load : string =>  [@bs] (string => cheerio_element) = "load"; 

Js.log("Hello, BuckleScript and Reason!");

request(
  "https://simulateur.tgvmax.fr/VSC/", 
  (_, _, body) => {
    let html_instance = cheerio_load(body);
    let input = html_instance(. "#hiddenToken");
    let a = input->cheerio_attr("value");
    Js.log(a);
  });