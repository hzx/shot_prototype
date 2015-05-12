#include "Options.h"
#include "Filer.h"
#include "utils.h"
#include "AppHandler.h"


namespace shot {


AppTemplate::AppTemplate(string title, string js) {
  // add noscript redirect
  // inline css, js
  head <<
R"(<noscript><meta http-equiv="refresh" content="0;url=/noscript"/></noscript>
<title>)" << title << "</title>" << 
R"(<style type="text/css">
html, body {
  position: absolute;
  left: 0;
  top: 0;
  right: 0;
  bottom: 0;
  overflow: hidden;
  margin: 0;
  padding: 0;
  border: 0;
}
html,
button,
input,
select,
textarea {
  font-family: sans-serif;
}
img {
  border: 0; /* 1 */
  -ms-interpolation-mode: bicubic; /* 2 */
}
table {
  border-collapse: collapse;
  border-spacing: 0;
}
input {
  outline-style: none;
  margin: 0;
  padding: 0;
}
</style>
<script type="text/javascript">)" << js << "</script>";
}


void AppHandler::appGet(string title, string jsfilename) {
  /* auto cssabs = shot::Options::instance().directory + cssfilename; */
  auto jsabs = shot::Options::instance().directory + jsfilename;

  if (!shot::pathExists(jsabs.c_str())) {
    response.content << "Что-то не нашел на сервере.";
    return;
  }

  /* auto css = shot::Filer::instance().loadCached(cssabs.c_str()); */
  auto js = shot::Filer::instance().loadCached(jsabs.c_str());

  AppTemplate templ(title, js);

  response.setHtmlHeader();
  response.content << templ.toString();
}


} /* namespace shot */
