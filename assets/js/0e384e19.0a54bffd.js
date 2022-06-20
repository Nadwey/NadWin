"use strict";(self.webpackChunknadwin_website=self.webpackChunknadwin_website||[]).push([[671],{3905:function(e,n,t){t.d(n,{Zo:function(){return d},kt:function(){return m}});var o=t(7294);function i(e,n,t){return n in e?Object.defineProperty(e,n,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[n]=t,e}function a(e,n){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);n&&(o=o.filter((function(n){return Object.getOwnPropertyDescriptor(e,n).enumerable}))),t.push.apply(t,o)}return t}function r(e){for(var n=1;n<arguments.length;n++){var t=null!=arguments[n]?arguments[n]:{};n%2?a(Object(t),!0).forEach((function(n){i(e,n,t[n])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):a(Object(t)).forEach((function(n){Object.defineProperty(e,n,Object.getOwnPropertyDescriptor(t,n))}))}return e}function l(e,n){if(null==e)return{};var t,o,i=function(e,n){if(null==e)return{};var t,o,i={},a=Object.keys(e);for(o=0;o<a.length;o++)t=a[o],n.indexOf(t)>=0||(i[t]=e[t]);return i}(e,n);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);for(o=0;o<a.length;o++)t=a[o],n.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(i[t]=e[t])}return i}var s=o.createContext({}),p=function(e){var n=o.useContext(s),t=n;return e&&(t="function"==typeof e?e(n):r(r({},n),e)),t},d=function(e){var n=p(e.components);return o.createElement(s.Provider,{value:n},e.children)},u={inlineCode:"code",wrapper:function(e){var n=e.children;return o.createElement(o.Fragment,{},n)}},c=o.forwardRef((function(e,n){var t=e.components,i=e.mdxType,a=e.originalType,s=e.parentName,d=l(e,["components","mdxType","originalType","parentName"]),c=p(t),m=i,w=c["".concat(s,".").concat(m)]||c[m]||u[m]||a;return t?o.createElement(w,r(r({ref:n},d),{},{components:t})):o.createElement(w,r({ref:n},d))}));function m(e,n){var t=arguments,i=n&&n.mdxType;if("string"==typeof e||i){var a=t.length,r=new Array(a);r[0]=c;var l={};for(var s in n)hasOwnProperty.call(n,s)&&(l[s]=n[s]);l.originalType=e,l.mdxType="string"==typeof e?e:i,r[1]=l;for(var p=2;p<a;p++)r[p]=t[p];return o.createElement.apply(null,r)}return o.createElement.apply(null,t)}c.displayName="MDXCreateElement"},9881:function(e,n,t){t.r(n),t.d(n,{assets:function(){return d},contentTitle:function(){return s},default:function(){return m},frontMatter:function(){return l},metadata:function(){return p},toc:function(){return u}});var o=t(7462),i=t(3366),a=(t(7294),t(3905)),r=["components"],l={slug:"./",title:"Intro",sidebar_position:1},s=void 0,p={unversionedId:"intro",id:"intro",title:"Intro",description:"Contents",source:"@site/docs/intro.md",sourceDirName:".",slug:"/",permalink:"/NadWin/docs/",draft:!1,tags:[],version:"current",sidebarPosition:1,frontMatter:{slug:"./",title:"Intro",sidebar_position:1},sidebar:"tutorialSidebar"},d={},u=[{value:"Contents",id:"contents",level:2},{value:"Installation",id:"installation",level:2},{value:"Examples",id:"examples",level:2},{value:"Simple window",id:"simple-window",level:3},{value:"Simple window with close button handle",id:"simple-window-with-close-button-handle",level:3},{value:"Simple window with button",id:"simple-window-with-button",level:3},{value:"My app is very ugly and looks like it was made on Windows 95",id:"my-app-is-very-ugly-and-looks-like-it-was-made-on-windows-95",level:3},{value:"I don&#39;t use MSVC, help.",id:"i-dont-use-msvc-help",level:4},{value:"Destroying",id:"destroying",level:3}],c={toc:u};function m(e){var n=e.components,t=(0,i.Z)(e,r);return(0,a.kt)("wrapper",(0,o.Z)({},c,t,{components:n,mdxType:"MDXLayout"}),(0,a.kt)("h2",{id:"contents"},"Contents"),(0,a.kt)("ul",null,(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#contents"},"Contents")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#installation"},"Installation")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#examples"},"Examples"),(0,a.kt)("ul",{parentName:"li"},(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#simple-window"},"Simple window")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#simple-window-with-close-button-handle"},"Simple window with close button handle")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#simple-window-with-button"},"Simple window with button")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#my-app-is-very-ugly-and-looks-like-it-was-made-on-windows-95"},"My app is very ugly and looks like it was made on Windows 95"),(0,a.kt)("ul",{parentName:"li"},(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#i-dont-use-msvc-help"},"I don't use MSVC, help.")))),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"#destroying"},"Destroying"))))),(0,a.kt)("h2",{id:"installation"},"Installation"),(0,a.kt)("p",null,"Git clone the repo or download ",(0,a.kt)("inlineCode",{parentName:"p"},"NadWin.hpp")," and ",(0,a.kt)("inlineCode",{parentName:"p"},"NadWin.cpp")," and place them next to each other.",(0,a.kt)("br",{parentName:"p"}),"\n","Then just include ",(0,a.kt)("inlineCode",{parentName:"p"},"NadWin.hpp")," and it should work."),(0,a.kt)("h2",{id:"examples"},"Examples"),(0,a.kt)("h3",{id:"simple-window"},"Simple window"),(0,a.kt)("p",null,"Really simple window, without close button handle."),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp",metastring:"showLineNumbers",showLineNumbers:!0},'#include "NadWin/src/NadWin.hpp"\n\n#pragma comment(lib, "Comctl32.lib")\n\nint main()\n{\n    NW::App app(L"Really simple app"); // your app name\n    NW::Window window(L"Really simple window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);\n\n    window.Show();\n    return app.MessageLoop();\n}\n')),(0,a.kt)("h3",{id:"simple-window-with-close-button-handle"},"Simple window with close button handle"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp",metastring:"showLineNumbers",showLineNumbers:!0},'#include "NadWin/src/NadWin.hpp"\n\n#pragma comment(lib, "Comctl32.lib")\n\nint main()\n{\n    NW::App app(L"app name");\n    NW::Window window(L"your beautiful window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);\n\n    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {\n        switch (eventType)\n        {\n        case NW::WindowEventTypes::Destroy:\n            PostQuitMessage(0);\n            eventInfo->OverrideProcResult(0);\n            break;\n        }\n    };\n\n    window.Show();\n    return app.MessageLoop();\n}\n')),(0,a.kt)("h3",{id:"simple-window-with-button"},"Simple window with button"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp",metastring:"showLineNumbers",showLineNumbers:!0},'#include "NadWin/src/NadWin.hpp"\n\n#pragma comment(lib, "Comctl32.lib")\n\nint main()\n{\n    NW::App app(L"app");\n    NW::Window window(L"window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);\n\n    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {\n        switch (eventType)\n        {\n        case NW::WindowEventTypes::Destroy:\n            PostQuitMessage(0);\n            eventInfo->OverrideProcResult(0);\n            break;\n        }\n    };\n\n    NW::Button button(&window, NW::Position(5, 5, 200, 25), L"Button");\n    button.EventHandler = [&](NW::ControlEventTypes eventType, NW::ControlEventInfo* eventInfo) {\n        switch (eventType)\n        {\n        case NW::ControlEventTypes::FromParent_Command:\n            MessageBoxW(window.Hwnd(), L"You clicked the button!", L"NadWin demo", MB_OK | MB_ICONINFORMATION);\n            break;\n        }\n    };\n\n    window.Show();\n    return app.MessageLoop();\n}\n')),(0,a.kt)("h3",{id:"my-app-is-very-ugly-and-looks-like-it-was-made-on-windows-95"},"My app is very ugly and looks like it was made on Windows 95"),(0,a.kt)("p",null,"Well, that is because you have probably some old version of Windows Common Controls\xae.",(0,a.kt)("br",{parentName:"p"}),"\n",'You "update" them by adding this at the top of the file:'),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp",metastring:"showLineNumbers",showLineNumbers:!0},"#if defined _M_IX86\n#pragma comment(linker,\"/manifestdependency:\\\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\\\"\")\n#elif defined _M_IA64\n#pragma comment(linker,\"/manifestdependency:\\\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\\\"\")\n#elif defined _M_X64\n#pragma comment(linker,\"/manifestdependency:\\\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\\\"\")\n#else\n#pragma comment(linker,\"/manifestdependency:\\\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\\\"\")\n#endif\n")),(0,a.kt)("h4",{id:"i-dont-use-msvc-help"},"I don't use MSVC, help."),(0,a.kt)("p",null,"Idk, I didn't test it on other compilers but it probably doesn't work.",(0,a.kt)("br",{parentName:"p"}),"\n","Anyways good luck."),(0,a.kt)("h3",{id:"destroying"},"Destroying"),(0,a.kt)("p",null,"NadWin uses high tech C++ destructors to destroy your controls, so you don't(or do) have to worry about memory leaks.",(0,a.kt)("br",{parentName:"p"}),"\n","This is an example of how it's working:"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp",metastring:"showLineNumbers",showLineNumbers:!0},'#include "NadWin/src/NadWin.hpp"\n\n#pragma comment(lib, "Comctl32.lib")\n\nint main()\n{\n    NW::App app(L"destroying");\n    NW::Window window(L"destroying demo", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);\n\n    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {\n        switch (eventType)\n        {\n        case NW::WindowEventTypes::Destroy:\n            PostQuitMessage(0);\n            eventInfo->OverrideProcResult(0);\n            break;\n        }\n    };\n\n    NW::Static label1(&window, NW::Position(5, 5, 200, 25), L"This label will show");\n    {\n        NW::Static label2(&window, NW::Position(5, 35, 200, 25), L"This label won\'t show");\n        // end of the scope, gets destroyed\n    }\n    NW::Static label3(&window, NW::Position(210, 35, 200, 25), L"<-- label2 should be here");\n\n    window.Show();\n    return app.MessageLoop();\n}\n')))}m.isMDXComponent=!0}}]);