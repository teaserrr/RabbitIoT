#ifndef HtmlDefines_h
#define HtmlDefines_h

const char RABBIT_HTTP_HEADER[] PROGMEM = 
"<!DOCTYPE html>" \
"<html lang=\"en\">" \
"  <head>" \
"    <meta charset=\"UTF-8\">" \
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>" \
"    <title>{device}</title>";
const char RABBIT_HTTP_STYLE[] PROGMEM = 
"   <style>" \
"   body { font-family: \"Helvetica Neue\", Helvetica, Arial; font-size: 14px; line-height: 20px; font-weight: 400; color: #f6f6f6; background: #2b2b2b; }" \
"   .wrapper { margin: auto; padding: 40px; }" \
"   .table { margin: 0 0 20px 0; color: #3b3b3b; width: 50%; box-shadow: 0 1px 3px rgba(0, 0, 0, 0.2); display: table; }" \
"   .row { display: table-row; background: #f6f6f6; }" \
"   .row:nth-of-type(odd) { background: #e9e9e9; }" \
"   .row.header { font-weight: 900; color: #ffffff; background: #ea6153; }" \
"   .row.green { background: #27ae60; }" \
"   .row.blue { background: #2980b9; }" \
"   .cell { padding: 6px 12px; display: table-cell; }" \
"   .button { width: 50%; box-shadow: 0 1px 3px rgba(0, 0, 0, 0.2); display: table; margin: 0 0 10px 0; font-weight: 900; color: #ffffff; background: #ea6153; padding: 6px 0px; text-decoration: none; text-align: center; border: none; }" \
"   .button.green { background: #27ae60; }" \
"   .button.blue { background: #2980b9; }" \
"   .sidebyside { width: 50%;  }" \
"   .left { width: 48%; float: left; }" \
"   .right { width: 48%; margin-left: 52%; }" \
"   input[type=submit] { cursor: pointer; }" \
"   @media screen and (max-width: 650px) { .table {	width: 100%; } .sidebyside { width: 100%; } }" \
"   @media screen and (max-width: 450px) { .button {width: 100%;  margin-left: initial; } }" \
"   </style>";
const char RABBIT_HTTP_BODY[] PROGMEM = 
"  </head>" \
"  <body>" \
"    <h1>{device}</h1>" \
"    <div class=\"wrapper\">";
const char RABBIT_HTTP_FORM[] PROGMEM = 
"    <form action=\"save\" method=\"post\">";
const char RABBIT_HTTP_TABLE[] PROGMEM =
"      <div class=\"table\">" \
"        <div class=\"row header\">" \
"          <div class=\"cell\">{tableHeader}</div>" \
"          <div class=\"cell\"></div>" \
"        </div>";
const char RABBIT_HTTP_ITEM[] PROGMEM = 
"        <div class=\"row\">" \
"          <div class=\"cell\">{key}:</div>" \
"          <div class=\"cell\">{value}{unit}</div>" \
"        </div>";
const char RABBIT_HTTP_CONFIG_ITEM[] PROGMEM = 
"        <div class=\"row\">" \
"          <div class=\"cell\">{desc}:</div>" \
"          <div class=\"cell\">" \
"            <input type=\"text\" name=\"{name}\" placeholder=\"{value}\"/>" \
"          </div>" \
"        </div>";
const char RABBIT_HTTP_TABLE_END[] PROGMEM =
"      </div>";
const char RABBIT_HTTP_BUTTON_ROW[] PROGMEM =
"      <div class=\"sidebyside\">";
const char RABBIT_HTTP_CONFIG_BUTTON[] PROGMEM =
"      <a class=\"button\" href=\"configuration\">Configuration</a>";
const char RABBIT_HTTP_CANCEL_BUTTON[] PROGMEM =
"      <a class=\"button left\" href=\"/\">Cancel</a>";
const char RABBIT_HTTP_SAVE_BUTTON[] PROGMEM =
"      <input class=\"button right\" type=\"submit\" value=\"Save\"/>";
const char RABBIT_HTTP_FORM_END[] PROGMEM = 
"      </div>" \
"    </form>";
const char RABBIT_HTTP_END[] PROGMEM = 
"    </div>" \
"  </body>" \
"</html>";

#endif