[@bs.module] external request : 
  string => 
  (Js.Nullable.t(exn) => Js.Nullable.t('res) => string => unit) 
  => unit 
  = "request";

[@bs.obj] external params : (
  ~uri: string, 
  ~qs: Js.t({..})=?,
  ~headers: Js.t({..})=?,
  unit
) => _ = "";
[@bs.module] external request2 : 'params => (Js.Nullable.t(exn) => Js.Nullable.t('res) => string => unit) => unit = "request";

[@bs.deriving abstract]
type cheerio_element;

[@bs.send] external cheerio_text : cheerio_element => Js.Nullable.t(string) = "text"; 
[@bs.send] external cheerio_attr : cheerio_element => string => Js.Nullable.t(string) = "attr"; 
[@bs.module "cheerio"][@bs.val] external cheerio_load : string => [@bs] (string => cheerio_element) = "load"; 

[@bs.deriving abstract]
type travel = {
  originCode: string,
  originName: string,
  destinationCode: string,
  destinationName: string,
  departureDateTime: string,
  arrivalDateTime: string,
  train: string,
  availableSeatCount: int,
  axe: string
};

[@bs.scope "JSON"] [@bs.val]
external parseIntoTravelList : string => list(travel) = "parse";

[@bs.deriving abstract]
type train_status = {
  identifier: string,
  full: bool
};

[@bs.deriving abstract]
type travel_request = {
  originName: string,
  destinationName: string,
  date: string,
  trains: list(train_status)
};

type result('a) = 
  | ResultError(exn)
  | ResultContent('a);

Js.log("Hello, BuckleScript and Reason!");

let get_homepage = cb =>
  request(
    "https://simulateur.tgvmax.fr/VSC/", 
    (err, _, body) => switch(Js.Nullable.toOption(err)) {
      | Some(e) => cb(ResultError(e))
      | None => cb(ResultContent(body))
    });

exception TokenNotFound;
let extract_token = (http_res) => {
  switch(http_res) {
    | ResultError(err) => ResultError(err)
    | ResultContent(body) => {
      let res = body
        ->cheerio_load(. "#hiddenToken")
        ->cheerio_attr("value");

      switch(Js.Nullable.toOption(res)) {
        | Some(token) => ResultContent(token)
        | None => ResultError(TokenNotFound)
      }
    }
  }
}

let sncf_api = (validity_token, endpoint, cb) => 
  switch(validity_token) {
    | ResultError(err) => cb(ResultError(err))
    | ResultContent(vtoken) => request2(
        params(~uri="https://sncf-simulateur-api-prod.azurewebsites.net/api" ++ endpoint, ~headers={"ValidityToken": vtoken}, ()), 
        (err,_, body) => switch(Js.Nullable.toOption(err)) {
          | Some(e) => cb(ResultError(e))
          | None => cb(ResultContent(body))
        })
};

//let get_trains = sncf_inited_api => sncf_inited_api("/RailAvailability/Search/RENNES/LYON%20(gares%20intramuros)/2019-10-16T00:00:00/2019-10-16T23:59:59")
[@bs.module] external requested_travels : list(travel_request) = "../request_travels.json";
Js.log(List.hd(requested_travels)->destinationNameGet)

let with_sncf_api = cb => get_homepage(v => v->extract_token->sncf_api->cb);
let get_train_availability = (sapi, origName, destName, date, cb) => sapi(
  "/RailAvailability/Search/" ++ origName ++ "/" ++ destName ++ "/" ++ date ++ "T00:00:00/" ++ date ++ "T23:59:59",
  v => switch(v) {
    | ResultContent(json) => cb(ResultContent(json->parseIntoTravelList))
    | ResultError(err) => cb(ResultError(err))
  });

with_sncf_api(sapi => {
  get_train_availability(sapi, "RENNES", "LYON%20(gares%20intramuros)", "2019-10-23", v => switch(v) {
      | ResultContent(parsed) => Js.Console.log(parsed)
      | ResultError(err) => Js.Console.error(err)
  });
})

/*
//let sms_conf = {"user": "00000000", "pass": "xxxxxxxxx", "msg": "plop ddd22é"}
[@bs.module] external sms_conf : 'a = "../sms.json";

request2(
  params(~uri="https://smsapi.free-mobile.fr/sendmsg", ~qs=sms_conf, ()),
  (err,res,body) => Js.log3(err,res##statusCode,body)
);
*/