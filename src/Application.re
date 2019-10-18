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
  availableSeatsCount: int,
  axe: string
};

[@bs.scope "JSON"] [@bs.val]
external parseIntoTravelList : string => array(travel) = "parse";

[@bs.deriving abstract]
type train_status = {
  identifier: string,
  mutable full: bool
};

[@bs.deriving abstract]
type travel_request = {
  originName: string,
  destinationName: string,
  date: string,
  trains: array(train_status)
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

[@bs.module] external requested_travels : array(travel_request) = "../request_travels.json";

[@bs.deriving abstract]
type sms_conf_fmt = {
  user: string,
  pass: string
};
[@bs.module] external sms_conf : sms_conf_fmt = "../sms.json";

let with_sncf_api = cb => get_homepage(v => v->extract_token->sncf_api->cb);
let get_train_availability = (sapi, origName, destName, date, cb) => sapi(
  "/RailAvailability/Search/" ++ origName ++ "/" ++ destName ++ "/" ++ date ++ "T00:00:00/" ++ date ++ "T23:59:59",
  v => switch(v) {
    | ResultContent(json) => cb(ResultContent(json->parseIntoTravelList))
    | ResultError(err) => cb(ResultError(err))
  });

let travel_logic = (requested, from_api) => {
  let en_tete = "Train " ++ requested->identifierGet ++ " du " ++ from_api->departureDateTimeGet ++ " : ";
  switch(requested->fullGet, from_api->availableSeatsCountGet) {
    | (true, 0) => Js.Console.log(en_tete ++ "toujours pas de place disponible :(")
    | (false, 0) => {
      requested->fullSet(true);
      let msg = en_tete ++ "plus de place disponible. Trop tard :(";
      Js.Console.log(msg);
      request2(
        params(~uri="https://smsapi.free-mobile.fr/sendmsg", ~qs={"user": sms_conf->userGet, "pass": sms_conf->passGet, "msg": msg}, ()),
        (err,res,body) => switch(Js.Nullable.toOption(err), Js.Nullable.toOption(res)) {
          | (Some(e), _) => Js.Console.error2("Unable to send SMS", e)
          | (_, None) => Js.Console.error("Unable to send SMS, emptry res")
          | (None, Some(r)) => Js.Console.log("API answered " ++ r##statusCode ++ " with body " ++ body)
        });
    }
    | (true, _) => {
      requested->fullSet(false);
      let msg = en_tete ++ from_api->availableSeatsCountGet->string_of_int ++ " places disponibles. Réservez vite !";
      Js.Console.log(msg);
      request2(
        params(~uri="https://smsapi.free-mobile.fr/sendmsg", ~qs={"user": sms_conf->userGet, "pass": sms_conf->passGet, "msg": msg}, ()),
        (err,res,body) => switch(Js.Nullable.toOption(err), Js.Nullable.toOption(res)) {
          | (Some(e), _) => Js.Console.error2("Unable to send SMS", e)
          | (_, None) => Js.Console.error("Unable to send SMS, emptry res")
          | (None, Some(r)) => Js.Console.log("API answered " ++ r##statusCode ++ " with body " ++ body)
        });
    }
    | (false, _) => Js.Console.log(en_tete ++ "toujours " ++ from_api->availableSeatsCountGet->string_of_int ++ " places disponibles.") 
  }
}

let iter_train = (req_trav, api_travels) => 
  Js.Array.forEach(req_train => {
    switch (Js.Array.find(api_travel => api_travel->trainGet == req_train->identifierGet, api_travels)) {
      | Some(api_travel) => travel_logic(req_train, api_travel)
      | None => Js.Console.error("Train " ++ req_train->identifierGet ++ " has not been found for date " ++ req_trav->dateGet) 
    }
  }, req_trav->trainsGet)

let iter_date = (sapi, req_trav) => {
  get_train_availability(sapi, req_trav->originNameGet, req_trav->destinationNameGet, req_trav->dateGet, v => switch(v) {
      | ResultContent(parsed) => iter_train(req_trav, parsed)
      | ResultError(err) => Js.Console.error(err)
  });
}

type timerId;
[@bs.val] external setInterval: ((. unit) => unit, int) => timerId = "setInterval";

let main_loop = () => {
  Js.Console.log("New probe...")
  with_sncf_api(sapi => {
    Js.Array.forEach(iter_date(sapi), requested_travels)
  })
}

main_loop()
setInterval((.) => main_loop(), 30000)

/*
//let sms_conf = {"user": "00000000", "pass": "xxxxxxxxx", "msg": "plop ddd22é"}
[@bs.module] external sms_conf : 'a = "../sms.json";

request2(
  params(~uri="https://smsapi.free-mobile.fr/sendmsg", ~qs=sms_conf, ()),
  (err,res,body) => Js.log3(err,res##statusCode,body)
);
*/