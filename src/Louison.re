[@bs.module] external request : 
  string => 
  (Js.Nullable.t(exn) => Js.Nullable.t('res) => string => unit) 
  => unit 
  = "request";

let message = "hello";
print_endline(message); /* Prints "hello" */
let message = "bye";
print_endline(message); /* Prints "bye" */


