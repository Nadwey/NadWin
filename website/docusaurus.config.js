const lightCodeTheme = require("prism-react-renderer/themes/github");
const darkCodeTheme = require("prism-react-renderer/themes/dracula");

/** @type {import('@docusaurus/types').Config} */
const config = {
    title: "NadWin Website",
    url: "https://Nadwey.github.io/",
    baseUrl: "/NadWin/",
    onBrokenLinks: "throw",
    onBrokenMarkdownLinks: "warn",

    organizationName: "Nadwey",
    projectName: "NadWin",

    i18n: {
        defaultLocale: "en",
        locales: ["en"],
    },

    themes: [[require.resolve("@easyops-cn/docusaurus-search-local"), {}]],

    presets: [
        [
            "classic",
            /** @type {import('@docusaurus/preset-classic').Options} */
            ({
                docs: {
                    sidebarPath: require.resolve("./sidebars.js"),
                },
                theme: {
                    customCss: require.resolve("./src/css/custom.css"),
                },
            }),
        ],
    ],

    themeConfig:
        /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
        ({
            navbar: {
                title: "NadWin",
                items: [
                    {
                        type: "doc",
                        docId: "intro",
                        position: "left",
                        label: "Documentation",
                    },
                    {
                        href: "https://github.com/Nadwey/NadWin",
                        position: "right",
                        className: "header-github-link",
                    },
                ],
            },
            prism: {
                theme: lightCodeTheme,
                darkTheme: darkCodeTheme,
            },
            announcementBar: {
                id: "no_docs_note",
                content: "Documentation is not in progress, see <a href=\"https://github.com/Nadwey/NadWin/issues/1\">#1</a>",
                isCloseable: true
            }
        }),
};

module.exports = config;
