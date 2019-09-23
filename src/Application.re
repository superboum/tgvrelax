[@bs.module] external request : 
  string => 
  (Js.Nullable.t(exn) => Js.Nullable.t('res) => string => unit) 
  => unit 
  = "request";


[@bs.obj] external params : (
  ~uri: string, 
  ~qs: Js.t({..}),
  unit
) => _ = "";
[@bs.module] external request2 : 'a => ('err => 'response => string => unit) => unit = "request";

[@bs.deriving abstract]
type cheerio_element;

[@bs.send] external cheerio_text : cheerio_element => Js.Nullable.t(string) = "text"; 
[@bs.send] external cheerio_attr : cheerio_element => string => Js.Nullable.t(string) = "attr"; 
[@bs.module "cheerio"][@bs.val] external cheerio_load : string => [@bs] (string => cheerio_element) = "load"; 

Js.log("Hello, BuckleScript and Reason!");

let get_homepage = Js.Promise.make((~resolve, ~reject) =>
  request(
    "https://simulateur.tgvmax.fr/VSC/", 
    (err, _, body) => switch(Js.Nullable.toOption(err)) {
      | Some(e) => reject(. e)
      | None => resolve(. body)
    }));

exception TokenNotFound;

let extract_token = (body) => Js.Promise.make((~resolve, ~reject) => {
  let res = body
    ->cheerio_load(. "#hiddenToken")
    ->cheerio_attr("value");

  switch(Js.Nullable.toOption(res)) {
    | Some(token) => resolve(. token)
    | None => reject(. TokenNotFound)
  }
})

get_homepage
  |> Js.Promise.then_(token => extract_token(token))
  |> Js.Promise.then_(value => { Js.log(value); Js.Promise.resolve() })
  |> Js.Promise.catch(err => { Js.log(err); Js.Promise.resolve() })

/*
//let sms_conf = {"user": "00000000", "pass": "xxxxxxxxx", "msg": "plop ddd22é"}
[@bs.module] external sms_conf : 'a = "../sms.json";

request2(
  params(~uri="https://smsapi.free-mobile.fr/sendmsg", ~qs=sms_conf, ()),
  (err,res,body) => Js.log3(err,res##statusCode,body)
);
*/